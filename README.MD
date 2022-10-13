[Nightly builds](https://github.com/Maschell/WiiUPluginSystem/releases) | [Issue Tracker](https://github.com/Maschell/WiiUPluginSystem/issues) | [Discussion](https://gbatemp.net/threads/wii-u-plugin-system.496659/).  | [Discord](https://discord.gg/bZ2rep2) | [Wiki](https://maschell.github.io/WiiUPluginSystem/dev_overview.html)

[![Build Status](https://api.travis-ci.org/Maschell/WiiUPluginSystem.svg?branch=master)](https://travis-ci.org/Maschell/WiiUPluginSystem)

# Plugin system for the Wii U

WARNING: THIS PROJECT IS FOR DEVS AND PEOPLE WHO WANT TO PLAY AROUND WITH NEW THINGS. IT MAY BE BUGGY, FEATURES COULD BREAK AT ANY TIME, INTERFACES CAN CHANGE, AND MANY BUGS CAN OCCUR. THIS APPLICATION COMES WITH NO WARRANTY.

# What is the Wii U Plugin System?

The Wii U Plugin allows you to load multiple plugins on your Wii U which enhance your experience. 
For example you can mod your games, use USB controller, swap your gamepad and TV screen and much more.
# How to build

Detailed instructions can be found in the Wiki:
- [Installing the required libraries for creating plugins](https://wiiu-env.github.io/WiiUPluginSystem/dev_required_libraries.html)  

# Plugin loader
To load the plugins, you need the WiiUPluginLoaderGUI, you can find it [here](https://github.com/wiiu-env/WiiUPluginLoaderGUI).

# Plugins

Last but not least you need to build the plugins that should be loaded.
Plugins depend on the "WUPS library" installed in build step one, other dependencies are optional.

This is a (maybe incomplete) list of currently available plugins:

- [Homebrew on Menu](https://github.com/wiiu-env/homebrew_on_menu_plugin):  
Allows you to boot your .rpx/.wuhb homebrew directly from your Wii U's menu
- [Region Free Plugin](https://github.com/wiiu-env/region_free_plugin):  
Allows you to launch games from other regions
- [DRC Region Free Plugin](https://github.com/wiiu-env/drc_region_free_plugin):  
Allows you to pair gamepads from other regions
- [Wiiload Plugin](https://github.com/wiiu-env/wiiload_plugin):  
Load a .rpx/.wuhb/.wps directly from e.g. your PC via the network
- [FTPiiU Plugin](https://github.com/wiiu-env/ftpiiu_plugin):  
Runs a FTP server in the background so that you can connect to it and manage the Wii U's filesystem
- [UTag v2.0.0 – RiiTag-Plugin](https://github.com/RiiConnect24/UTag):  
A Wii U plugin which sends the titles you play to your RiiTag
- [DRC Sensorbar](https://github.com/WiiDatabase/DRC-Sensorbar):  
Enables the sensor bar of the GamePad (DRC) permanently.
- [re_nfpii - Amiibo emulation](https://github.com/GaryOderNichts/re_nfpii):   
A more or less accurate reimplementation of the Wii U's `nn_nfp.rpl` library, with the goal of research and amiibo emulation.
- [SDCafiine](https://github.com/wiiu-env/sdcafiine_plugin):  
Allows you to mod games by redirecting file read accesses to files located on the SD card.

# Configuration of the plugins
Plugins can register to the configuration menu. This configuration menu can be opened while running any application (e.g. while gameplay).  

Just press **L, DPAD down and minus** on the gamepad.  

This does only work when the game is allowed to open the home menu.  
Settings made using the configuration menu will be saved to the SD Card and reloaded the next time the plugin will be used.

# Logging
For logging (for example of the loader) you need to start the [UDPDebugReader](https://github.com/BullyWiiPlaza/UDP-Debug-Reader) on a computer in the same network.

# Future and contribution
On [our Discord server](https://discord.gg/bZ2rep2
) you can find more information about open tasks and how to contribute.

## Format the code via docker

`docker run --rm -v ${PWD}:/src wiiuenv/clang-format:13.0.0-2 -r ./include ./libraries ./plugins/example_plugin/src -i`

# Credits
* Some files are based on brainslug by Chadderz: https://github.com/Chadderz121/brainslug-wii  
* Much stuff also wouldn't be possible without dimok789. He made many great tools and homebrew this stuff in based on (Makefiles, Mocha, Homebrew Launcher, UDPDebugLogger, dynamic_libs etc.)  
* Thanks to everyone who made actual exploits
* Thanks to dhewg for wiiload: http://wiibrew.org/wiki/Wiiload
