---
title: Setting up wut
tags: [setup]
keywords: wut, wsl, libraries, wut setup
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_wut_setup.html
folder: dev
topnav: topnav
---

## Overview

This wiki page is providing some notes regarding the [wut](https://github.com/decaf-emu/wut) usage with plugins. 
See also: [How to use system functions](dev_using_system_functions).

To use wut you can either grab a [pre-compiled version](https://github.com/decaf-emu/wut/releases) version, or [compile it yourself](https://github.com/decaf-emu/wut#building).

## Unix / OSX
Using it wut on unix is pretty straight forward. 
* When using the pre-compiled version, simply extract the archive and set `WUT_ROOT` environment variable. See [this](https://github.com/decaf-emu/wut#install)
* See the [official guide](https://github.com/decaf-emu/wut#building-on-linux--macos).

## Windows
On Windows it's gets a bit more complicated. It is **not** possible to compile wut directly with Windows. 
While it is still **possible to use wut libraries** on windows, while using (unix) wut executables is not possible 
(without WSL([Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10))).  

### Precompiled libaries on Windows
The libraries built on Unix can also be used on Windows, this does **not** include the wut binaries like (elf2rpl) 
(Which are not need for the Wii U Plugin System). Grab the latest pre-compiled libraires / header from the 
[wut release page](https://github.com/decaf-emu/wut/releases) (linux 64 version). Extract it somewhere on your disk and set the `WUT_ROOT` envrionment variable to this folder.

**Note:**
You may also need other wut libraries (like [libutilswut](https://github.com/Maschell/libutils/tree/wut)), 
which also can't be compiled on windows. These may **not** provide a pre-compiled version. See the readme of the libraries for more information.

### Compiling wut under Windows using WSL.
To be able to compile wut under Windows, you need to do it via WSL ([Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10)).  
It has been tested successfully using the `Ubuntu 18.04` distribution, which can be downloaded via the Windows Store. 
Make sure to actually download the version with "18.04" in the name, the one called "Ubuntu" does **not** work.  
When inside the WSL bash you can simply follow the [Unix instructions](https://github.com/decaf-emu/wut#building-on-linux--macos).  

Here a few tips that will simplify your workflow regarding the plugin development.
* Install wut in a directory that's accessible from Windows (example `/mnt/c/devkitpro/wut`). 
This allows you to use Windows for the actual plugin development. Only for updating/compiling wut (or compile any wut library) you'll still need WSL.

{% include links.html %}
