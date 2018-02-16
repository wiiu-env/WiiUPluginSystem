# SDCafiine Plugin for the Wii U Plugin System

See https://github.com/Maschell/SDCafiine for more information.

# Currently supported

- Modpack selector
- FAT32 support for SD and USB

Whats NOT working:

- NTFS support

## Building

For building you need: 
- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.
- [libutils](https://github.com/Maschell/libutils) for common functions.
- [libfswrapper](https://github.com/Maschell/fs_wrapper) to replace the fs functions.


Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.