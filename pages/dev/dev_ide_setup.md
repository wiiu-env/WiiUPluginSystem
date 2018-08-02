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
This page is about setting up `Code::Blocks` for developing Wii U Homebrew using the `devkitPPC` toolchain.  

### Preparations
If you didn't set up `devkitPPC` already, you may want to follow [this](dev_toolchain_setup) tutorial. You should've installed the required libraries for plugin development by now as well. If not, check out [this](dev_required_libraries) page.  

### Installation
`Code::Blocks` is a free IDE and can be downloaded [here](http://www.codeblocks.org/). Install it according to the `Code::Blocks` homepage.

### Setting up the compiler
For Wii U homebrew we generally use custom makefiles but `Code::Blocks` still needs to have a valid compiler/toolchain set. Using this tutorial we are overriding an existing compiler setting. 
If you are using it for non Wii U projects, make sure to make a backup/copy of the settings.  
Once you have opened Code::Blocks, you can access the compiler options menu via `Settings -> Compiler...` on the menu bar.
1. Select the `GNU GCC Compiler for PowerPC (HighTec)` in the dropdown menu.
2. Press `Set as default`
3. Click on the `Toolchain executables` tab.
4. Set `Compiler's installation directory` to the bin folder of your `devkitPPC` installation (on Windows this usually is `C:\devkitPro\devkitPPC\bin`)
5. Set `C compiler` to `powerpc-eabi-gcc` (on Windows `powerpc-eabi-gcc.exe`)
6. Set `C++ compiler` to `powerpc-eabi-g++` (on Windows `powerpc-eabi-g++.exe`)
7. Set `Linker for dynamic libs:` to `powerpc-eabi-g++` (on Windows `powerpc-eabi-g++.exe`)
8. Set `Linker for static libs:` to `powerpc-eabi-ar` (on Windows `powerpc-eabi-ar.exe`)
9. Set `Make program` to `make` (on Windows `make.exe`)
10. Save the settings by clicking on `OK`

An example configuration can be found here:  
![Example configuration of the compiler in Code::Blocks](https://i.imgur.com/cnfkmHR.gif)

### Next steps
Now you can turn any existing project into a `Code::Blocks` project by following [this](dev_codeblocks_project_from_scratch.html) tutorial.  

Alternatively, you can use the `Code::Blocks` template to create a new plugin project from the scratch. A tutorial for this can be found [here](https://github.com/Maschell/WiiUPluginSystem/wiki/Using-project-templates-in-Code::Blocks).

{% include links.html %}
