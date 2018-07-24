---
title: How to use official system functions
tags: 
keywords: system functions, dynamic_libs, dynamic libs, wut
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_compile_loader.html
folder: dev
topnav: topnav
---

## Requirements
To be able to compile the loader, you need to already have installed the [toolchain](https://github.com/Maschell/WiiUPluginSystem/wiki/Setting-up-the-Toolchain) 
and [required libs](https://github.com/Maschell/WiiUPluginSystem/wiki/Installing-the-required-libraries).  
**Make sure to also install DevkitARM!**

## Compiling the loader
Before the loader can be compiled, dependencies need to be installed. This can be either can via an experimental bash script or manually.

### Installing libraries via bash script
The plugin system provides an experimental bash script that clones all needed git repositories and builds/installs these libraries.  
The script can be found here:  
https://github.com/Maschell/WiiUPluginSystem/blob/master/installupdateportlibs.sh

Before the bash script can be used, make sure you have the following tools installed and set to your `PATH` variable.
```
devkitPro
git
7z
```

This script creates a new directory `portlib_repos` where the repository will be stored. When running for the first time, the repositories will be cloned, 
built and installed. Running a second time will update the repositories and rebuild/reinstalled the libraries on updates.

### Installing libraries manually
The loader has some dependencies, a full list can be found here:  
- [libiosuhax](https://github.com/dimok789/libiosuhax) (Build WITHOUT the WUT flag set.)
- [libfat](https://github.com/Maschell/libfat/) (Build with `make wiiu-release && make wiiu-install`)
- [libntfs](https://github.com/Maschell/libntfs-wiiu) (Build with make wiiu-install)
- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.
- [libutils](https://github.com/Maschell/libutils) for common functions.  
- [libgui](https://github.com/Maschell/libgui) for the gui elements.

Install the according to their readmes. Don't forget to install their dependencies.

**Note: Don't forget to install the [WUPS library](dev_required_libraries#wups-library)**

#### Other dependencies

All needed dependencies are in the "loader/libs" folder of this repository. Extract the "portlibs.zip" archive into your devkitPro directory.
The archive includes:

- zlib

## Compiling

Then call the following command in the "loader" directory.

``` Bash
make
```

This should create an `wiiupluginloader.elf` which can be loaded with the Homebrew Launcher.

## Notes
- When you change code in the mocha files, this isn't detected by the Makefile. You need to do a `make clean` beforehand in this case.

{% include links.html %}
