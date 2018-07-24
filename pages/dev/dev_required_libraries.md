---
title: Installing the required libraries for creating plugins
tags: [libraries]
keywords: libraries, libutils, libutilswut, wups, libwups, wut
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_required_libraries.html
folder: dev
topnav: topnav
---

## WUPS library
To be able to create own plugins, you need the Wii U Plugin System library.
Make sure you have set up the [toolchain](dev_toolchain_setup).

### Download the lib
Clone the WiiUPluginSystem repository.

```Bash
git clone --recursive https://github.com/Maschell/WiiUPluginSystem.git
```

### Compiling and installing
Then call the following command in the root directory.
```Makefile
make
make install
```
The "make" builds the library, the "make install" copies it into your `DEVKITPRO` folder.

## WUT
[wut](https://github.com/decaf-emu/wut) is a Wii U Toolchain / SDK for creating rpx/rpl, which provides header for most system 
rpl files (like coreinit, gx2 etc.). It provides stub libraries for these rpl's, including exports for all functions.  
WUPS plugins support using system functions by linking against the wut libraries.  
  
Instructions on how to install wut can be found on the [official repository](https://github.com/decaf-emu/wut).

**Make sure to set the `WUT_ROOT` environment variable to the place where you installed wut.**

## libutils (wut edition)
[libutils](https://github.com/Maschell/libutils/tree/wut) (wut branch) is a library which provides common functions to the user. 
This includes logging, file access and many more. See the repository for instruction on how install and use it.  

**Note:** You may have already installed the non-wut version to compile the plugin loader. 
Plugins will most likely need the **wut** branch/version. 

{% include links.html %}
