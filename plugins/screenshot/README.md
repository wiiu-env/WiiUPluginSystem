# Screenshot tool

This is just a simple plugin that takes screenshot of the TV and DRC screen. The screenshot will saved on the sd card.

## Building

For building you need: 
- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.
- [libutils](https://github.com/Maschell/libutils) for common functions.

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

You also need some other, external libraries.

All needed dependencies are in the "libs" folder of this plugin. Extract the "portlibs.zip" archive into your devkitPro directory.
The archive includes:

- gd
- libjpeg