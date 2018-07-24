---
title: Makefile usage
tags: [makefile]
keywords: makefile, building, linking
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_plugin_makefile.html
folder: dev
topnav: topnav
---

The Makefile for a plugin is splitted into multiple parts.
* `$(WUPS_DIR)/plugin_makefile.mk` - provides some general linking flags
* `$(PLUGIN_DIR)/Makefile` - A generic Makefile that should fit for all plugin
* `$(PLUGIN_DIR)/makefile.mk` - The part that is plugin specific

The `plugin_makefile.mk` will be installed automatically into the _WUPS_DIR_ inside the _DEVKITPRO_ when installing the 
[WUPS library](https://github.com/Maschell/WiiUPluginSystem/wiki/Installing-the-required-libraries#wups-library).  

The `Makefile` and `makefile.mk` can be found inside the [example plugin](https://github.com/Maschell/WiiUPluginSystem/tree/master/plugins/example_plugin). 
When using the [Code::Blocks template](https://github.com/Maschell/WiiUPluginSystem/wiki/Using-project-templates-in-Code::Blocks), these will be provided.

# makefile.mk
The `makefile.mk` should be self explanatory, but common changes will be explained here.

## Defined variables that can be used
Inside the `makefile.mk` multiple variables are already set, here is a short overview.

| Varibale | Value |
| --- | --- |
| $(DEVKITPRO) | Refers to the devkitPro directory |
| $(DEVKITPPC) | Refers to the devkitPPC directory |
| $(WUPSDIR) | Refers to the directory of the installed WUPS libary |
| $(WUT_ROOT) | Refers to the directory of the installed wut |
| $(PORTLIBS) | Refers to the directory of the installed portlibs |
| $(GCC_VER) | Contains the installed/used GCC version |

## Enable logging
```Makefile
# Compiling the projects with libutils logging code?
DO_LOGGING := 1
```
This is a [libutils](https://github.com/Maschell/libutils) specific option. When it's set to 0, no logging related code will be compiled into the binary.

## WUT specific options
It's possible to use some wut speficic options, these require wut to be installed.

```
# Links against the wut implementation of newlib, this is useful for using any function
# from the C standard library
WUT_ENABLE_NEWLIB   := 0

# Links against the wut implementation of stdcpp, this is useful for using any function
# from the C++ standard library. This will enable WUT_ENABLE_NEWLIB if you have not already done so.
WUT_ENABLE_CPP      := 0

# By default newlib will allocate 90% of the default heap for use with sbrk & malloc, 
# if this is unacceptable to you then you should use this as it replaces the newlib 
# malloc functions which ones which redirect to the CafeOS default heap functions 
# such as MEMAllocFromDefaultHeap.
WUT_DEFAULT_MALLOC  := 1
```
In most cases you want to enable `WUT_DEFAULT_MALLOC` to able to use `malloc`, `free` etc.
## Input / Output
```Makefile
# Target filename
TARGET              := $(notdir $(CURDIR)).mod

# Source directories
SOURCES             := src

# Data directories
DATA                :=	

# Include directories
INCLUDES            := src
```

* `TARGET` defines the output filename. `$(notdir $(CURDIR)).mod` is the default value which result in DIR_NAME.mod. 
The file extenstion needs to be `.mod` to be recognized by the plugin loader.  
* `SOURCES` defines a _list_ of directories that will be scanned for C,C++ and .s files. Multiple folder are seperated by a space.  
* `DATA` defines a _list_ of directories that will be scanned for binary files (Like fonts, sounds, images). Multiple folder are seperated by a space.  
* `INCLUDES` defines a _list_ of directories that will be scanned for head files (.h and .hpp). Multiple folder are seperated by a space. 

## Compiler and linking flags
```Makefile
#---------------------------------------------------------------------------------
# options for code generation and linking
#---------------------------------------------------------------------------------
# Extra C AND C++ compiler flags
COMMON_CFLAGS       := 
# Extra C compiler flags
CFLAGS              :=
# Extra C++ compiler flags
CXXFLAGS            :=
# Extra linking flags for all linking steps
LDFLAGS             := 
```
* Set `COMMON_CFLAGS` to specify extra flags for the C and C++ compiler..  
* Set `CFLAG` to specify extra only `C compiler` flags.  
* Set `CXXFLAG` to specify extra only `C++ compiler` flags. 
* Set `LD_FLAGS` to add custom linking flags.

## Linking with libraries
```Makefile

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS             := $(WUPSDIR) $(WUT_ROOT)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS                := -lwups -lutilswut -lcoreinit -lnsysnet
#---------------------------------------------------------------------------------
# Will be added to the final lib paths
# example: 
# -L$C:/library1/lib
#---------------------------------------------------------------------------------
EXTERNAL_LIBPATHS   := 
                        
#---------------------------------------------------------------------------------
# Will be added to the final include paths
# -IC:/library1/include
#---------------------------------------------------------------------------------
EXTERNAL_INCLUDE    :=  -I$(WUT_ROOT)/include/libutilswut
```
Libraries that will be linked in, need to defined in `LIBS`, seperated by a space. `libwups` will be needed by all plugin, all other are optional. 
Most likely you will link against utility libraries like [libutils](https://github.com/Maschell/libutils/tree/wut) and wut libraries to 
[use system functions](dev_using_system_functions).

The path which contains the according binaries is defined in `EXTERNAL_LIBPATHS` (with `-L` as prefix), the path to the header in `EXTERNAL_INCLUDE` 
(with `-I` as prefix), seperated by a space. `EXTERNAL_INCLUDE` is also used to define other external include files.  
`LIBDIRS` can be used as a shortcut. It refers to directory containing the binaries of the libary in a subfolder called `lib`, and the header in a subfolder called `include`.  

```
LIBDIRS             := $(WUPSDIR)
```
is equivalent to 
```
EXTERNAL_LIBPATHS   :=  -L$(WUPSDIR)/lib
EXTERNAL_INCLUDE    :=  -I$(WUPSDIR)/include
```


{% include links.html %}