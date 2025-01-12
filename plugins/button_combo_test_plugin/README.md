# ButtonComboAPI test plugin

This plugin implements several tests of ButtonComboAPI. Results are be printed to the OSConsole ([USBSerialLoggingModule](https://github.com/wiiu-env/USBSerialLoggingModule))

## Installation

(`[ENVIRONMENT]` is a placeholder for the actual environment name.)

1. Requires the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend) in `sd:/wiiu/environments/[ENVIRONMENT]/modules`.

It's recommended to [wiiload](https://github.com/wiiu-env/wiiload_plugin) this plugin whenever you want to run the test.

This plugin uses [Catch2](https://github.com/catchorg/Catch2) to run tests.

## Building

For building you need:

- [wups](https://github.com/Maschell/WiiUPluginSystem)
- [wut](https://github.com/devkitpro/wut)

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

Then you should be able to compile via `make` (with no logging) or `make DEBUG=1` (with logging).

## Buildflags

### Logging

Building via `make` only logs errors (via OSReport). To enable logging via the [LoggingModule](https://github.com/wiiu-env/LoggingModule) set `DEBUG` to `1` or `VERBOSE`.

`make` Logs errors only (via OSReport).  
`make DEBUG=1` Enables information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).  
`make DEBUG=VERBOSE` Enables verbose information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).

If the [LoggingModule](https://github.com/wiiu-env/LoggingModule) isn't present, it will fall back to UDP (port 4405) and [CafeOS](https://github.com/wiiu-env/USBSerialLoggingModule) logging.

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t button-combo-test-plugin-builder

# make 
docker run -it --rm -v ${PWD}:/project button-combo-test-plugin-builder make DEBUG=1

# make clean
docker run -it --rm -v ${PWD}:/project button-combo-test-plugin-builder make clean
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./src -i`