#include <algorithm>
#include <string>
#include <vector>
#include <string.h>
#include <zlib.h>
#include <libgen.h>

#include "common/common.h"
#include "TcpReceiver.h"
#include <dynamic_libs/os_functions.h>
#include <dynamic_libs/socket_functions.h>
#include <fs/CFile.hpp>
#include <fs/FSUtils.h>
#include <utils/logger.h>
#include <utils/StringTools.h>
#include <utils/net.h>
#include "Application.h"
#include "plugin/PluginLoader.h"
#include "plugin/PluginInformation.h"

TcpReceiver::TcpReceiver(int port)
    : CThread(CThread::eAttributeAffCore0 | CThread::eAttributePinnedAff)
    , exitRequested(false)
    , serverPort(port)
    , serverSocket(-1) {

    resumeThread();
}

TcpReceiver::~TcpReceiver() {
    exitRequested = true;

    if(serverSocket >= 0) {
        shutdown(serverSocket, SHUT_RDWR);
    }
}

void TcpReceiver::executeThread() {
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (serverSocket < 0)
        return;

    u32 enable = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    struct sockaddr_in bindAddress;
    memset(&bindAddress, 0, sizeof(bindAddress));
    bindAddress.sin_family = AF_INET;
    bindAddress.sin_port = serverPort;
    bindAddress.sin_addr.s_addr = INADDR_ANY;

    s32 ret;
    if ((ret = bind(serverSocket, (struct sockaddr *)&bindAddress, sizeof(bindAddress))) < 0) {
        socketclose(serverSocket);
        return;
    }

    if ((ret = listen(serverSocket, 3)) < 0) {
        socketclose(serverSocket);
        return;
    }

    struct sockaddr_in clientAddr;
    s32 addrlen = sizeof(struct sockaddr);

    while(!exitRequested) {
        s32 clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrlen);
        if(clientSocket >= 0) {
            u32 ipAddress = clientAddr.sin_addr.s_addr;
            //serverReceiveStart(this, ipAddress);
            int result = loadToMemory(clientSocket, ipAddress);
            //serverReceiveFinished(this, ipAddress, result);
            socketclose(clientSocket);

            if(result > 0)
                break;
        } else {
            os_usleep(100000);
        }
    }

    socketclose(serverSocket);
}

int TcpReceiver::loadToMemory(s32 clientSocket, u32 ipAddress) {
    DEBUG_FUNCTION_LINE("Loading file from ip %08X\n", ipAddress);

    u32 fileSize = 0;
    u32 fileSizeUnc = 0;
    unsigned char haxx[8];
    memset(haxx, 0, sizeof(haxx));
    //skip haxx
    recvwait(clientSocket, haxx, sizeof(haxx));
    recvwait(clientSocket, (unsigned char*)&fileSize, sizeof(fileSize));

    if (haxx[4] > 0 || haxx[5] > 4) {
        recvwait(clientSocket, (unsigned char*)&fileSizeUnc, sizeof(fileSizeUnc)); // Compressed protocol, read another 4 bytes
    }

    u32 bytesRead = 0;
    struct in_addr in;
    in.s_addr = ipAddress;

    DEBUG_FUNCTION_LINE("transfer start\n");

    unsigned char* loadAddress = (unsigned char*)memalign(0x40, fileSize);
    if(!loadAddress) {
        os_sleep(1);
        return NOT_ENOUGH_MEMORY;
    }

    // Copy rpl in memory
    while(bytesRead < fileSize) {

        u32 blockSize = 0x1000;
        if(blockSize > (fileSize - bytesRead))
            blockSize = fileSize - bytesRead;

        int ret = recv(clientSocket, loadAddress + bytesRead, blockSize, 0);
        if(ret <= 0) {
            DEBUG_FUNCTION_LINE("Failure on reading file\n");
            break;
        }

        bytesRead += ret;
    }

    if(bytesRead != fileSize) {
        free(loadAddress);
        DEBUG_FUNCTION_LINE("File loading not finished, %i of %i bytes received\n", bytesRead, fileSize);
        os_sleep(1);
        return FILE_READ_ERROR;
    }

    bool res = false;

    // Do we need to unzip this thing?
    if (haxx[4] > 0 || haxx[5] > 4) {
        unsigned char* inflatedData = NULL;

        // We need to unzip...
        if (loadAddress[0] == 'P' && loadAddress[1] == 'K' && loadAddress[2] == 0x03 && loadAddress[3] == 0x04) {
            //! TODO:
            //! mhmm this is incorrect, it has to parse the zip

            // Section is compressed, inflate
            inflatedData = (unsigned char*)malloc(fileSizeUnc);
            if(!inflatedData) {
                free(loadAddress);
                os_sleep(1);
                return NOT_ENOUGH_MEMORY;
            }

            int ret = 0;
            z_stream s;
            memset(&s, 0, sizeof(s));

            s.zalloc = Z_NULL;
            s.zfree = Z_NULL;
            s.opaque = Z_NULL;

            ret = inflateInit(&s);
            if (ret != Z_OK) {
                free(loadAddress);
                free(inflatedData);
                os_sleep(1);
                return FILE_READ_ERROR;
            }

            s.avail_in = fileSize;
            s.next_in = (Bytef *)(&loadAddress[0]);

            s.avail_out = fileSizeUnc;
            s.next_out = (Bytef *)&inflatedData[0];

            ret = inflate(&s, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                free(loadAddress);
                free(inflatedData);
                os_sleep(1);
                return FILE_READ_ERROR;
            }

            inflateEnd(&s);
            fileSize = fileSizeUnc;
        } else {
            // Section is compressed, inflate
            inflatedData = (unsigned char*)malloc(fileSizeUnc);
            if(!inflatedData) {
                free(loadAddress);
                os_sleep(1);
                return NOT_ENOUGH_MEMORY;
            }

            uLongf f = fileSizeUnc;
            int result = uncompress((Bytef*)&inflatedData[0], &f, (Bytef*)loadAddress, fileSize);
            if(result != Z_OK) {
                DEBUG_FUNCTION_LINE("uncompress failed %i\n", result);
                os_sleep(1);
                return FILE_READ_ERROR;
            }

            fileSizeUnc = f;
            fileSize = fileSizeUnc;
        }

        FSUtils::CreateSubfolder(WUPS_TEMP_PLUGIN_PATH);
        res = FSUtils::saveBufferToFile(WUPS_TEMP_PLUGIN_FILE,inflatedData, fileSize);
        free(inflatedData);
    } else {

        FSUtils::CreateSubfolder(WUPS_TEMP_PLUGIN_PATH);
        res = FSUtils::saveBufferToFile(WUPS_TEMP_PLUGIN_FILE,loadAddress, fileSize);
        free(loadAddress);
    }

    if(!res) {
        os_sleep(1);
        return NOT_ENOUGH_MEMORY;
    }

    PluginInformation * plugin = PluginInformation::loadPluginInformation(WUPS_TEMP_PLUGIN_FILE);
    if(plugin == NULL) {
        return NOT_A_VALID_PLUGIN;
    }
    PluginLoader * pluginLoader = PluginLoader::getInstance();
    pluginLoader->resetPluginLoader();
    std::vector<PluginInformation* > pluginList = pluginLoader->getPluginInformation(WUPS_TEMP_PLUGIN_PATH);
    if(pluginList.size() == 0 || !pluginLoader->loadAndLinkPlugins(pluginList)) {
        return NOT_A_VALID_PLUGIN;
    }
    Application::instance()->quit(APPLICATION_CLOSE_APPLY);

    return fileSize;
}
