---
title: Setting up an IDE
tags: [ide]
keywords: ide, codeblocks
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_ide_setup.html
folder: dev
topnav: topnav
---

## Code::Blocks
This page is about setting up CodeBlocks for developing Wii U Homebrew using the DevkitPPC toolchain.  
If you haven't already set up the toolchain, [follow this tutorial](dev_toolchain_setup).  
You also should've installed the required libraries by now. If not check out: [Installing the required libraries](dev_required_libraries).  

Code::Blocks is free and can be downloaded [here](http://www.codeblocks.org/).  
Download and install it according to the Code::Blocks homepage.

### Setting up the compiler
The Wii U is using custom Makefiles, but Code::Blocks still needs to have an valid compiler/toolchain set. Using this tutorial we are overriding an existing compiler setting. 
If you are using it for non-WiiU projects, make sure to make an backup / copy.  
Once you have opened Code::Blocks, you can access the compiler options via `Settings->Compiler...` on the menu bar.
1. Select the `GNU GCC Compiler for PowerPC (HighTec)` in the dropdown menu.
2. Press `Set as default`
3. Click on the `Toolchain executables` tab.
4. Set `Compiler's installation directory` to the bin folder of your DEVKITPPC installation (on windows probably `C:\devkitPro\devkitPPC\bin`)
5. Set `C compiler` to `powerpc-eabi-gcc` (on windows `powerpc-eabi-gcc.exe`)
6. Set `C++ compiler` to `powerpc-eabi-g++` (on windows `powerpc-eabi-g++.exe`)
7. Set `Linker for dynamic libs:` to `powerpc-eabi-g++` (on windows `powerpc-eabi-g++.exe`)
8. Set `Linker for static libs:` to `powerpc-eabi-ar` (on windows `powerpc-eabi-ar.exe`)
9. Set `Make program` to `make` (on windows `make.exe`)
10. Save the settings by clicking on `Ok`

An example configuration can be found here:  
![Example configuration of the compiler in Code::Blocks](https://i.imgur.com/cnfkmHR.gif)

### Next steps
Now you can turn any existing project into an Code::Blocks project by following [this](https://github.com/Maschell/WiiUPluginSystem/wiki/Creating-a-Code::Blocks-project-from-the-scratch) tutorial.  

Or you can use the the Code::Blocks template to create a new plugin project from the scratch. A tutroial can be found [here](https://github.com/Maschell/WiiUPluginSystem/wiki/Using-project-templates-in-Code::Blocks).

{% include links.html %}
