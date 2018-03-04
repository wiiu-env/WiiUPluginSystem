#ifndef TCP_RECEIVER_H_
#define TCP_RECEIVER_H_

#include <vector>
#include <string>
#include <dynamic_libs/os_types.h>

#include <system/CThread.h>
#include <gui/sigslot.h>
#include <gui/Gui.h>

class TcpReceiver : public CThread
{
public:
    enum eLoadResults
    {
        SUCCESS = 0,
        INVALID_INPUT = -1,
        FILE_OPEN_FAILURE = -2,
        FILE_READ_ERROR = -3,
        NOT_ENOUGH_MEMORY = -4,
        NOT_A_VALID_PLUGIN = -5,
    };

    TcpReceiver(int port);
    ~TcpReceiver();

    sigslot::signal2<GuiElement *, u32> serverReceiveStart;
    sigslot::signal3<GuiElement *, u32, int> serverReceiveFinished;

private:

    void executeThread();
    int loadToMemory(s32 clientSocket, u32 ipAddress);
    bool saveFileToSDCard(const char * path, void * buffer,u32 size);

    bool exitRequested;
    s32 serverPort;
    s32 serverSocket;
};


#endif
