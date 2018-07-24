---
title: How to use official system functions
tags: 
keywords: system functions, dynamic_libs, dynamic libs, wut
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_using_system_functions.html
folder: dev
topnav: topnav
---

## Overview
On the Wii U all system libraries are linked dynamically  (with a few exceptions), not statically. This means the libraries are provides as extra files (.rpl) and loaded + linked when needed.

There are two ways to load and use `.rpl` files.
1. The loader resolves any usages of libraries, resulting in a transparent dynamic linking. (recommended)
2. Using system functions to explicitly load and use libraries via `OSDynLoad_Acquire `OSDynLoad_FindExport` on run time.

## Option 1: Let the loader resolve library usages. (recommended)
For the system libraries (`.rpl` files in the [OSv10 title](http://wiiubrew.org/wiki/Cafe_OS#Libraries)) you can link against stub libraries provided by [wut](https://github.com/decaf-emu/wut).  
wut also provides [header files](https://github.com/decaf-emu/wut/tree/master/include) for most of the libraries which can be used.  
The WUPS loader will resolve the usage of the functions automatically.

To use for example the `OSScreenInit` function from `coreinit.rpl` you can simply use the following code:
```
// Include the coreinit screen function header
#include <coreinit/screen.h>

int main(){
    OSScreenInit();
    OSScreenShutdown();
}
```
Because we used functions from the `coreinit.rpl` we need to link with `libcoreinit` which is located in `$(WUT_ROOT)/lib`. 
When using the [example Makefile](https://github.com/Maschell/WiiUPluginSystem/blob/master/plugins/example_plugin/makefile.mk), 
you simply would have add `-lcoreinit` to the `LIBS` variable, and make sure `$(WUT_ROOT)` was added to `LIBDIRS`.

The following snippet from the example Makefile allows to use functions from the `coreinit.rpl` and `nsysnet.rpl`.
```
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS             := $(WUPSDIR) $(WUT_ROOT)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS := -lwups -lutilswut -lcoreinit -lnsysnet
```
### How do I know which functions I can use? 
A list of all function exports from all system libraries can be found on the [wut repository](https://github.com/decaf-emu/wut/tree/master/cafe). 
All of these functions are defined, but not all of them are declared in the wut headers.
For the most common functions you should find declarations in [wut headers](https://github.com/decaf-emu/wut/tree/master/include). 
If you have any declarations that are missing, feel free to add them to wut via an pull request!

### How to use functions that are missing in the wut headers?

Example:
```
// Let's pretends OSScreenInit and OSScreenshutdown are NOT declared in the wut headers.
// We would have to declare them ourselves.

// When we are compiling it as C++, we have to make sure to declare them
// in a C context to avoid function name mangling. 
#ifdef __cplusplus
extern "C" {
#endif
    void OSScreenInit();
    void OSScreenShutdown();
#ifdef __cplusplus
}
#endif

int main(){
    OSScreenInit();
    OSScreenShutdown();
}
```
Now the file can be simply linked against `libcoreinit`.

### How do I know the return/argument types of a function?
Reverse engineering.  
All system libraries can be dumped from your console (TODO: add path) or downloaded from the NUS (title id 00050010-1000400A) with any NUS Downloader.  
Downloading via [JNUSTool](https://gbatemp.net/threads/jnustool-nusgrabber-and-cdecrypt-combined.413179/): `java -jar JNUSTool.jar  000500101000400A  -file .*`, bring your own common key.  
  
The `.rpl` files can be converted to standard elf with [rplf2elf](https://github.com/Relys/rpl2elf).  
  
There are also rpx/rpl plugin for IDA, these allow you to use them without converting to standard elf:  
* [Version 6](https://gbatemp.net/threads/ida-pro-wii-u-loader.382433/)
* [Version 7+](https://github.com/decaf-emu/ida_rpl_loader)

## Option 2: Using OSDynLoad_Acquire and OSDynLoad_FindExport (not recommended))
**For most cases it's enough to let the loader resolve any library usages**, but you may want to load any extra (optional?) libraries during runtime.  

The following example code shows how load a system library ("gx2.rpl", the library used for graphics).

```C++
OSDynLoad_Module gx2ModuleHandle;
OSDynLoad_Error error = OSDynLoad_Acquire("gx2.rpl", &gx2ModuleHandle);
if(error != OS_DYNLOAD_OK){
   // Error while loading
}
```
The `OSDynLoad_Acquire` will try to find a `gx2.rpl`. The __system library path__ and the __code folder of the currently running application__ are checked.
On success, the library will be loaded into memory and the pointer module handler (`gx2ModuleHandle`) will be filled.  
This handle can be used to get functions pointers (from the `.export` section) of library.

```
void (* GX2Init)();
OSDynLoad_FindExport(coreinit_handle, 0, "GX2Init", &GX2Init);
```
Now the function `GX2Init` can be used. 

Notice: This was a really oversimplified example, you might take a look at the [dynamic_libs](https://github.com/Maschell/dynamic_libs/).  
For more information checkout the [dynload functions](https://github.com/decaf-emu/wut/blob/master/include/coreinit/dynload.h)

{% include links.html %}
