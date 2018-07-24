---
title: Using hooks
tags: [hooks]
keywords: hooks, start, vsync, onchange
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_plugin_hooks.html
folder: dev
topnav: topnav
---

The plugin system provides serveral hooks the plugin can use. An example plugin that is using all hooks can be found [here](https://github.com/Maschell/WiiUPluginSystem/blob/master/plugins/example_plugin/src/main.cpp). 

An overview of all hooks can be found here:

# ON_APP_STATUS_CHANGED(status)
**Arguments:**
- wups_loader_app_status_t: the current status of the application.  

**Gets called when:**  

This hook gets called whenever the status of the running application changes.  

**Notes:**  

A list of possible values for the arguement can be found here:

| Value | Description |
|---|---|
| WUPS_APP_STATUS_FOREGROUND | App is now running in foreground | 
| WUPS_APP_STATUS_BACKGROUND | App is now running in background  (e.g. the home menu has been called)|
| WUPS_APP_STATUS_CLOSED | App is going to be closed  |
| WUPS_APP_STATUS_UNKNOWN | Unknown status. |

------
# INITIALIZE_PLUGIN()
**Arguments:**
- none

**Gets called when:**  

The plugin was successfully loaded by the plugin loader and the loader is going to exit.

**Notes:**  

Everytime the loader gets opened, existing plugins get unloaded. This means it's possible that the hook is called more than once during the console runtime. 
Effectivly the hook is called when the loader exists and the plugin is active.

------
# DEINITIALIZE_PLUGIN()
**Arguments:**
- none

**Gets called when:**  

The plugin was active and the loader gets re-opened.

**Notes:**  

Everytime the loader gets opened, existing plugins get unloaded. Before the actual unloading, this hook gets called.

------
# ON_APPLICATION_START(myargs)
**Arguments:**
- wups_loader_app_started_args_t: arguments provided by the loader.

**Gets called when:**  

Gets called when a application was started, but **before** the function were patched. The given arguments provides some extra information. 
Check the notes for detailed information.

**Notes:**  
If you want to hook into the application start, but **after** the function were patched, use the _ON_FUNCTIONS_PATCHED_ hook. Both hook, 
together with the actual function patching will be done by the plugin loader. This means other plugin may have already patched functions which interfer function results.  

wups_loader_app_started_args_t is a struct with the following elements:  
``` C
typedef struct wups_loader_app_started_args_t {
     bool sd_mounted;    // is true when an sd card was mounted.
     bool usb_mounted;   // is true when an usb device was mounted.
     bool kernel_access; // is true when the kernel functions can be used.
} wups_loader_app_started_args_t;
```
* Make sure to use the `WUPS_FS_ACCESS()` macro to actually get access to the sd/usb device. 
* To request access to the kernel functions, you need add the `WUPS_ALLOW_KERNEL` macro to your plugin.  

Read the following page for detailed information in inherited functions:  
https://github.com/Maschell/WiiUPluginSystem/wiki/Inherit-functions-from-the-loader

------
# ON_FUNCTIONS_PATCHED()
**Arguments:**
- none

**Gets called when:**  

Gets called when a application was started, but **after** the function were patched.

**Notes:**  
If you want to hook into the application start, but **before** the function were patched, use the _ON_APPLICATION_START_ hook. Both hook, together with the actual 
function patching will be done by the plugin loader. This means other plugin may have already patched functions which interfer function results.  

------
# ON_APPLICATION_ENDING()
**Arguments:**
- none

**Gets called when:**  

Gets called when the application is ending.

**Notes:**  
This hook is a shortcut for when the _ON_APP_STATUS_CHANGED_ hook get's called with `WUPS_APP_STATUS_CLOSED `. For more information check the _ON_APP_STATUS_CHANGED_ hook.

------
# ON_VYSNC()
**Arguments:**
- none

**Gets called when:**  

Gets called when the console is waiting for VSync, which is on each (rendered) frame.

**Notes:**  
This hook gets called **before** the console actually calls _GX2WaitForVsync_.

{% include links.html %}
