# HIDtoVPAD Plugin for the Wii U Plugin System

See https://github.com/Maschell/hid_to_vpad for more information.

# Currently supported

- Support for Controller with built in support.
- Network Client support
- All attached devices are mapped automatically to the Gamepad
- Loading configuration files (to extend controller support or change configs)

Whats NOT working:

- Mapping the controllers individually (No GUI for this)

## Building

For building you need: 
- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [dynamic_libs](https://github.com/Maschell/dynamic_libs/tree/lib) for access to the functions.
- [libutils](https://github.com/Maschell/libutils) for common functions.
- [controller_patcher](https://github.com/Maschell/controller_patcher) to emulate the controllers.


Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.