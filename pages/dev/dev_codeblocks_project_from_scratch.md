---
title: Creating a new project for an existing plugin
tags: [features]
keywords: codeblocks, ide, template, plugin, example, existing
last_updated: November 20, 2018
sidebar: dev_sidebar
permalink: dev_codeblocks_project_from_scratch.html
folder: dev
topnav: topnav
---

## Creating a new project for an existing plugin.
**This requires the [IDE](dev_ide_setup), [toolchain](dev_toolchain_setup) and [libraries](dev_required_libraries) already to installed and set up.**

**You may also want to take a look at the Code::Blocks templates, a tutorial can be found [here](dev_project_template_in_code_blocks).**

Existing source files, can be turned into a Code::Blocks project. This requires an existing Makefile that will be used.
One can be found for example in the [example_plugin](https://github.com/Maschell/WiiUPluginSystem/tree/master/plugins/example_plugin).

**Creating the project files**

1. Click on `File -> New -> Project` on the menu bar.
2. Choose `Empty project` and press `Go`
3. Click on `Next`
4. Set a project title
5. Fill in the path of the (existing) project
6. Make sure the `resulting filename` is correct (removing on directory)
7. Press on `Next`
8. Choose the `GNU GCC Compiler for PowerPC (HighTec)`
9. Press `Finish`

**Configure the project - setting custom makefile**
1. Right click on the project and choose `Properties`
2. Tick the checkbox at `This is a custom Makefile`, to a custom Makefile.
3. Save the settings by clicking on `Ok`
4. Right click on the project and choose `Build options`
5. Go to the tab `"Make" commands`
6. Set `Build project/target` to `make -j8`
7. Set `Clean project/target` to `make clean`
8. Save the setting by pressing `Ok`

**Configure the project - adding include paths**  
To have the best experience with the auto-completion, you need to add the header of all linked static libs.  
This can be done with the following steps:  
1. Right click on the project and choose `Properties`
2. Open the C/C++ parser options` tab
3. Add all missing include path using the `Add` button.
4. Save the settings by pressing `Ok`.

Common include paths would be:
```
<path-to-installed-devkitpro>\wups\include
<path-to-installed-wut>\include
<path-to-installed-wut>\include\libutils\wut
```

**Adding the source files**  
To add the source files:  
1. Right click on the project and choose `Add files recursively...`
2. Choose the project folder (should be default)
3. Click on `Ok`
4. Click again on `Ok`
5. Click one last time on `Ok`  

Now you should see your source files and can modify them. Try to build the project with `CTRL + F9`  

![Creating a new project from the scratch](assets/img/SpU4lbj.gif)
{% include links.html %}