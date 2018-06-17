#include <wups.h>
#include <whb/log.h>
#include <whb/log_udp.h>
#include <coreinit/filesystem.h>
/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("Example plugin");
WUPS_PLUGIN_DESCRIPTION("This is just an example plugin and will log the FSOpenFile function.");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

/**
    Add this to one of your projects file to have access to SD/USB.
**/
WUPS_FS_ACCESS()
/**
    Add this to one of your projects file to be able to create overlays.
**/
WUPS_ALLOW_OVERLAY()

/**
    All of this defines can be used in ANY file.
    It's possible to split it up into multiple files.

**/

/**
    Get's called ONCE when the loader exits, but BEFORE the ON_APPLICATION_START gets called or functions are overridden.
**/
INITIALIZE_PLUGIN(){
    WHBLogUdpInit();
    WHBLogPrintf("INITIALIZE_PLUGIN");
    WHBLogUdpDeinit();
}


/**
    Gets called when the plugin loader is re-entered => when the plugin is unloaded. 
	The overridden functions are restored before this is getting called.
**/
DEINITIALIZE_PLUGIN(){
    WHBLogPrintf("DEINITIALIZE_PLUGIN");
    WHBLogUdpDeinit();
}

/**
    Gets called when an application starts.
	This is called BEFORE the functions are overridden.
	Make sure to initialize all functions you're using in the overridden functions!
**/
ON_APPLICATION_START(){
    WHBLogUdpInit();
    WHBLogPrintf("ON_APPLICATION_START of example_plugin!");
    
} 

/**
    Gets called when an application ends. A good place for freeing memory.
**/
ON_APPLICATION_ENDING(){
    WHBLogPrintf("ON_APPLICATION_ENDING of example_plugin!");
    WHBLogUdpDeinit();
}

/**
    Gets called on each frame.
**/
ON_VYSNC(){
    WHBLogPrintf("ON_VYSNC of example_plugin!\n");
}

/**
Gets called whenever the application status changes.

Possible values of "status":
    WUPS_APP_STATUS_FOREGROUND,         App is now running in foreground
    WUPS_APP_STATUS_BACKGROUND          App is now running in background
    WUPS_APP_STATUS_CLOSED              App is going to be closed
**/
ON_APP_STATUS_CHANGED(status){
	if(status == WUPS_APP_STATUS_FOREGROUND){
		WHBLogPrintf("ON_APP_STATUS_CHANGED of example_plugin! App is now in foreground");
	} else if(status == WUPS_APP_STATUS_BACKGROUND){
		WHBLogPrintf("ON_APP_STATUS_CHANGED of example_plugin! App is now in background");
	} else if(status == WUPS_APP_STATUS_CLOSED){
		WHBLogPrintf("ON_APP_STATUS_CHANGED of example_plugin! App is now going to be closed");
	}
}

/**
    This defines a function replacement.
    It allows to replace the system function with an own function. 
    So whenever a game / application calles an overridden function, your function gets called instead.
    
    Currently it's only possible to override functions that are loaded from .rpl files of OSv10 (00050010-1000400A).
    
    Signature of this macro:
    DECL_FUNCTION(  RETURN_TYPE, ARBITRARY_NAME_OF_FUNCTION , ARGS_SEPERATED_BY_COMMA){
        //Your code goes here.
    }
    
    Within this macro, two more function get declare you can use.
    my_ARBITRARY_NAME_OF_FUNCTION and real_FSOpenFile
    
    RETURN_TYPE my_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is just name of the function that gets declared in this macro. 
        It has the same effect as calling the overridden function directly.
    
    RETURN_TYPE real_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
        is the name of the function, that leads to function that was overridden.
        Use this to call the original function that will be overridden.
        CAUTION: Other plugins may already have manipulated the the return value or arguments.
        
    
    Use this macro for each function you want to override

**/
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    
    WHBLogPrintf("FSOpenFile called for folder %s! Result %d",path,result);
    return result;
}

/**
This tells the loader which functions from which library (.rpl) should be replaced with which function from this file.
The list of possible libraries can be found in the wiki. (In general it's WUPS_LOADER_LIBRARY_ + the name of the RPL in caps lock)

WUPS_MUST_REPLACE(FUNCTION_NAME_IN_THIS_FILE,   NAME_OF_LIB_WHICH_CONTAINS_THIS_FUNCTION,   NAME_OF_FUNCTION_TO_OVERRIDE)

Define this for each function you want to override.
**/
WUPS_MUST_REPLACE(FSOpenFile,                   WUPS_LOADER_LIBRARY_COREINIT,               FSOpenFile);
