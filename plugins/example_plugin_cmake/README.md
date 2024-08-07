# Example plugin

This is just a simple example plugin which can be used as a template.
The plugin logs the FSOpenFile calls via UDP (**Only when CMake is passed `-DDEBUG=1`**).

The logging can be enabled/disabled via the WUPS Config menu (press L, DPAD Down and Minus on the GamePad, Pro Controller or Classic Controller).

## Installation

(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Copy the file `ExamplePlugin.wps` into `sd:/wiiu/environments/[ENVIRONMENT]/plugins`.
2. Requires the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.

Start the environment (e.g Aroma) and the backend should load the plugin.

## Building

For building you need:

- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [wut](https://github.com/devkitpro/wut)

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

Then you should be able to configure via `${DEVKITPRO}/portlibs/wiiu/bin/powerpc-eabi-cmake -S . -B build` (with no logging) or `${DEVKITPRO}/portlibs/wiiu/bin/powerpc-eabi-cmake -S . -B build -DDEBUG=1` (with logging), and build with `cd build && make`.

## Buildflags

### Logging

Building only logs errors by default (via OSReport). To enable logging via the [LoggingModule](https://github.com/wiiu-env/LoggingModule) set `DEBUG` to `1` or `VERBOSE`.

`DEBUG=1` Enables information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).  
`DEBUG=VERBOSE` Enables verbose information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).

If the [LoggingModule](https://github.com/wiiu-env/LoggingModule) is not present, it'll fallback to UDP (Port 4405) and [CafeOS](https://github.com/wiiu-env/USBSerialLoggingModule) logging.

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t example-plugin-builder

# Configure + Build 
docker run -it --rm -v ${PWD}:/project example-plugin-builder /bin/bash -c "/opt/devkitpro/portlibs/wiiu/bin/powerpc-eabi-cmake -S . -B build && cd build && make"
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./src -i`
