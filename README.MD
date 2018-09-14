[Nightly builds](https://github.com/Maschell/WiiUPluginSystem/releases) | [Issue Tracker](https://github.com/Maschell/WiiUPluginSystem/issues) | [Discussion](https://gbatemp.net/threads/wii-u-plugin-system.496659/).  | [Discord](https://discord.gg/bZ2rep2) | [Wiki](https://maschell.github.io/WiiUPluginSystem/dev_overview.html)
# Plugin system for the Wii U. (WIP) [![Build Status](https://api.travis-ci.org/Maschell/WiiUPluginSystem.svg?branch=master)](https://travis-ci.org/Maschell/WiiUPluginSystem)

WARNING: THIS PROJECT HAS JUST STARTED AND IS FOR DEVS AND PEOPLE WHO WANT TO PLAY AROUND WITH NEW THINGS. IT MAY BE BUGGY, FEATURE COULD BREAK AT ANY TIME, INTERFACES CAN CHANGE, AND MANY BUGS CAN OCCUR. THIS APPLICATION COMES WITH NO WARRANTY-

# What is the Wii U Plugin System?

The Wii U Plugin allows you to load multiple plugins on your Wii U which enhance your experience. 
For example you can mod your games, use USB controller, swap your gamepad and TV screen and much more.

# Features

The project is still in it's very early days, but it already has basic functions to play with.

- The plugins will be loaded, even when you swap the running game.
- Support for up to 32 plugins at the same time.
- Each plugin can override up to 100 different existing system functions.
- Multiple plugins can override the same system functions.
- Plugins can register for certain hook (for example whenever an application was started)
- Plugins inherit the SD/USB access from the loader. All plugins have global SD and USB (FAT32 only) access.
- Plugins can be configured at run-time. Press L, DPAD down, and minus on the gamepad at the same time to open the configuration menu.

# How to build

A detailed instruction can be found in the Wiki:

- [Installing the required libraries for creating plugins](https://maschell.github.io/WiiUPluginSystem/dev_required_libraries.html)  
- [How to compile the loader](https://maschell.github.io/WiiUPluginSystem/dev_compile_loader.html)  

### Plugins

Last but not least you need to build the plugins that should be loaded.
Plugins depend on the "WUPS library" installed in build step one, other dependencies are optional.

This is a (maybe incomplete) list of currently available plugins:

- [HID to VPAD](https://github.com/Maschell/hid_to_vpad/tree/wups):  
Port of [HID to VPAD](https://github.com/Maschell/hid_to_vpad).
No configuration is possible yet, all controllers map automatically to the game pad, pro controller mapping not support. Network client is working.
- [Screenshot plugin](https://github.com/Maschell/ScreenshotWUPS):  
Simple plugin that take a screenshot by presing L+R+ZL+ZR on the gamepad. Button combo can be changed.
- [Streaming plugin](https://github.com/Maschell/StreamingPluginWiiU):  
Gamepad streaming tool. Open http://<ip of your wii u>:8000 on a browser in the same network.
- [SD Cafiine](https://github.com/Maschell/SDCafiine/tree/wups):  
Port of the orignal SDCafiine (https://github.com/Maschell/SDCafiine). No NTFS support yet.
- [SwipSwapMe](https://github.com/Maschell/SwipSwapMe):  
Swaps the TV and gamepad screen via a buttons combo.The button combo can be changed.
- [Diibugger](https://github.com/Maschell/DiiBuggerWUPS):  
Port of the DiiBugger.
- [WUPSPluginPlayground (Various other demo plugins)](https://github.com/Maschell/WUPSPluginPlayground):  
This repository is supposed to hold serveral small testing plugins for the Wii U Plugin System.
Most of the plugin are either really small (Padcon, NNU_Patcher) or are just for testing / gathering information.

# Configuration of the plugins
Plugins can register to the configuration menu.
This configuration menu can be opened while running any application (e.g. while gameplay).  

Just press **L, DPAD down and minus** on the gamepad.  

This does only work when the game is allowed to open the home menu.  
Settings made using the configuration menu will be saved to the SD Card and reloaded the next time the plugin will be used.

# Logging
For logging (for example of the loader) you need to start the UdpDebugReader on a computer in the same network. 
This has been created by @dimok789 and can be found in the tools folder.

# Load a plugin via network
While the loader is running, it's possible to load a single plugin via [wiiload](http://wiibrew.org/wiki/Wiiload).  
When using this feature, this and all plugins in `sd:/wiiu/plugins/temp` will be loaded. The plugin will copied to the SDCard, this mean a SDCard is required.
A windows executable can be found in `tools/wiiload.exe`  
More information about wiiload and alternatives can be found here: http://wiibrew.org/wiki/Wiiload

# Future and contribution
On the Discord you can more information about open tasks and how to contribute: https://discord.gg/bZ2rep2

# Credits
Some files are based on brainslug by Chadderz:  
https://github.com/Chadderz121/brainslug-wii  
Much stuff also wouldn't be possible without dimok789. He made many great tools and homebrew this stuff in based on (Makefiles, Mocha, homebrew channel, udp logger, dynamic_libs etc.)  
Also thanks to everyone who made actual exploits.  
Thanks to dhewg for wiiload:  
http://wiibrew.org/wiki/Wiiload