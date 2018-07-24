---
title: Inherit features from the loader
tags: [features]
keywords: features, filesystem, fat32, access, overlay, kernel, 
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_inherit_features.html
folder: dev
topnav: topnav
---

To use extra functions on the plugins, you can add macros. The following list give a short overiew about all existing macros:

| macro | Description |
|-------|-------------|
|WUPS_FS_ACCESS() | Allows plugin access the sd card |
|WUPS_ALLOW_KERNEL() | Used to request access to kernel functions |
|WUPS_ALLOW_OVERLAY() | Needed to able to open an overlay |

--------

## WUPS_FS_ACCESS()
**Description:**  
This macro will inherit the fs functions from the loader (open, close, read, opendir etc.). This allows the plugin to access the sd card (`sd:/`) or fat32 usb device (`usb:/`) if connected.
The [ON_APPLICATION_START](https://github.com/Maschell/WiiUPluginSystem/wiki/Using-hooks#on_application_startmyargs) macro holds information if a sdcard or usb device was mounted and can be used.  
Example:
```C++
WUPS_FS_ACCESS()
// Called whenever an application was started.
ON_APPLICATION_START(args){
    socket_lib_init();
    log_init();
    DEBUG_FUNCTION_LINE("SD mounted? %d\n",args.sd_mounted);
    DEBUG_FUNCTION_LINE("USB mounted? %d\n",args.usb_mounted);
}
```

--------

## WUPS_ALLOW_KERNEL()
--------
**Description:**
This macro requests the access to function PPC kernel functions. The loader may restrict the access.  
The [ON_APPLICATION_START](https://github.com/Maschell/WiiUPluginSystem/wiki/Using-hooks#on_application_startmyargs) macro holds information if the kernel function are actually usable.  
Example:
```C++
WUPS_ALLOW_KERNEL()

// Called whenever an application was started.
ON_APPLICATION_START(args){
    if(!args.kernel_access){
        OSFatal("The diibugger plugin needs kernel access!");
    }
    // Normal code
}
```
When the plugin has access, the following functios can be used:
```C++
/**
    Reads a 32bit value from a given address with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
uint32_t WUPS_KernelRead(const void *addr);

/**
    Write a 32bit value from a given address with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
void WUPS_KernelWrite(void *addr, uint32_t value);

/**
    Copies data from a source address to a destination address for a given lenght with kernel rights.
    This function only has an effect if the plugin has the "WUPS_ALLOW_KERNEL" hook and the loader is NOT blocking the kernel access.
    The argument of the ON_APPLICATION_START hook provides the information if the plugin has kernel access which should be checked before using/relying on this function.
**/
void WUPS_KernelCopyDataFunction(uint32_t addr, uint32_t src, uint32_t len);

```
If the plugin doesn't have the required access, calling the functions has no effect.


## WUPS_ALLOW_OVERLAY()
**Description:**  
This macro allows you to use the overlay functions. It's possible to open a simple OSScreen whenever the home menu is allowed.  
WUPS provide to a convinient way to open and use such a menu using the following functions:
```C++
// Callback used when opening the overlay.
// The first wups_overlay_options_type_t tell your which screen are actually used.
// Use this as input for the other overlay function.
//
// The void* can used for user data.
typedef void (*overlay_callback)(wups_overlay_options_type_t, void*);

// Open an overlay on a given screen (see the enum below for possible values)
void WUPS_OpenOverlay(wups_overlay_options_type_t screen, overlay_callback callback, void* args);

// Clears the screen for a given sceentype
void WUPS_Overlay_OSScreenClear(wups_overlay_options_type_t screen);

// Prints text on a given sceentype
void WUPS_Overlay_PrintTextOnScreen(wups_overlay_options_type_t screen, int x,int y, const char * msg, ...);

// Flips the buffer for a given screentype.
void WUPS_Overlay_FlipBuffers(wups_overlay_options_type_t screen);
```

Possible screentype options that can be passed to `WUPS_OpenOverlay`:
```C++
typedef enum wups_overlay_options_type_t {
    WUPS_OVERLAY_NONE,
    WUPS_OVERLAY_DRC_ONLY,                          /* Tries to display only on gamepad screen */
    WUPS_OVERLAY_TV_ONLY,                           /* Tries to display only on tv screen */
    WUPS_OVERLAY_DRC_AND_TV,                        /* Tries to display on both screens. Prioritizes the TV screen if memory is low. */
    WUPS_OVERLAY_DRC_AND_TV_WITH_DRC_PRIO           /* Tries to display on both screens. But if memory is low, prioritize the DRC screen.*/
}
wups_overlay_options_type_t;
```
Short usage example:
```C++
WUPS_ALLOW_OVERLAY()

void SplashScreen_callback(wups_overlay_options_type_t screen, void * args){
    int32_t i=0;
    while(i<100){
        WUPS_Overlay_OSScreenClear(screen);
        WUPS_Overlay_PrintTextOnScreen(screen, 0,0,"This could be something cool.");
        WUPS_Overlay_FlipBuffers(screen);
        i++;
    } 
}

INITIALIZE_PLUGIN(){
    WUPS_OpenOverlay(WUPS_OVERLAY_DRC_AND_TV ,SplashScreen_callback, NULL);
}
```
A full usage example can be found here:  
https://github.com/Maschell/WUPSPluginPlayground/blob/master/overlay_test

{% include links.html %}
