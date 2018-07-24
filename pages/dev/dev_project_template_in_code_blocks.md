---
title: Using project templates in Code::Blocks
tags: [features]
keywords: codeblocks, ide, template, plugin, example
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_project_template_in_code_blocks.html
folder: dev
topnav: topnav
---

Code::Blocks allows you to save projects as an template for other (new projects). This page is about setting up the example plugin template.
## Setting up a plugin template.
The Wii U Plugin also provides an example Code::Blocks project which can be used as a template.   
It can be found in the following directory of this repository:  
`ide_templates\codeblocks`  
The directory contains a project file (`windows_template.cbp`), a example source file (`src/main.cpp`) and a default Makefile (`Makefile`).
Once you've opened the project in Code::Blocks, make sure you can compile it. Press `Ctrl + F11` to call `make clean && make`. You can also do `right click -> Rebuild` on the project.  

If everything was setup correctly, you should see this in the console:
``` Bash
Running command: make -j8
main.cpp
linking ... output.elf
checking for missing symbols ...
linking ... /g/Programmieren/WiiUPluginSystem/ide_templates/codeblocks/codeblocks.mod
```

Now you are ready to save it as an user template. To do this, click on `File -> Save project as template` and enter an template name.  

![Adding project as template](https://i.imgur.com/nUYEsEC.gif)

## Creating a plugin using the template
1. Click on `File -> New -> From template` on the menu bar.
2. Choose the template you have just created
3. Press `Go`
4. Choose a destination folder
5. Choose a name for the project
6. Profit  

![Created a project from a user template](https://i.imgur.com/KKAbz4n.gif)

{% include links.html %}