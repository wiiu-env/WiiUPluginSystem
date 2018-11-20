---
title: "Wii U Plugin System"
keywords: Overview
tags: [overview]
sidebar: home_sidebar
permalink: index.html
---

{% include note.html content="THIS PROJECT HAS JUST STARTED AND IS FOR DEVS AND PEOPLE WHO WANT TO PLAY AROUND WITH NEW THINGS. IT MAY BE BUGGY, FEATURE COULD BREAK AT ANY TIME, INTERFACES CAN CHANGE, AND MANY BUGS CAN OCCUR. THIS APPLICATION COMES WITH NO WARRANTY" %}

The Wii U Plugin System allows you to load multiple plugins on your Wii U which enhance your experience. 
For example you can mod your games, use USB controller, swap your gamepad and TV screen and much more.

# Features

The project is still in it's very early days, but it already has basic functions to play with.

- The plugins will be loaded, even when you swap the running game.
- Support for up to 32 plugins at the same time.
- Each plugin can override up to 100 different existing system functions.
- Multiple plugins can override the same system functions.
- Plugins can register for certain hook (for example whenever an application was started)
- Plugins inherit the SD/USB access from the loader. All plugins have global SD and USB (FAT32 only) access.
- Plugins can be configured at run-time. Press **L, DPAD down, and minus** on the gamepad at the same time to open the configuration menu.

# Usage

The "Wii U Plugin Loader" is built to be loaded through the homebrew launcher, which can be either loaded with the browser exploit or haxchi.  
Plugins needs to be placed into the folder:

```
sd:/wiiu/plugins
```

# Configuration of the plugins
Plugins can register to the configuration menu.
This configuration menu can be opened while running any application (e.g. while gameplay).  

Just press **L, DPAD down and minus** on the gamepad.  

This does only work when the game is allowed to open the home menu.  
Settings made using the configuration menu will be saved to the SD Card and reloaded the next time the plugin will be used.

# Load plugin via network
While the loader is running, it's possible to load a single plugin via wiiload.  
When using this feature, this and all plugins in `sd:/wiiu/plugins/temp` will be loaded. The plugin will copied to the SDCard, this mean a SDCard is required.
A windows executable can be found in `tools/wiiload.exe`  
More information about wiiload and alternatives can be found here: http://wiibrew.org/wiki/Wiiload

# Download
The plugin loader can be downloaded here:  
Nightlies: [Download](https://github.com/Maschell/WiiUPluginLoader/releases)  
Source: [Here](https://github.com/Maschell/WiiUPluginLoader)  
Load the file with the homebrew launcher.

{% include links.html %}
