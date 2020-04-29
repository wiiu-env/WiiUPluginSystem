extern "C" void OSFatal(const char *msg);

extern "C" void __wups_start(){
    OSFatal("This file needs to be run with the Wii U Plugin System.");
}
    