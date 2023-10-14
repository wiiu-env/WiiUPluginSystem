[Issue Tracker](https://github.com/wiiu-env/WiiUPluginSystem/issues) | [Discord](https://discord.gg/bZ2rep2)
 
# What is the Wii U Plugin System?

The Wii U Plugin System is a library to create plugins for the [WiiUPluginLoaderBackend](https://github.com/wiiu-env/WiiUPluginLoaderBackend).
See the repository of the Backend for more information.

## How create plugins
See the [example plugin](https://github.com/wiiu-env/WiiUPluginSystem/tree/master/plugins/example_plugin) for more information about creating a plugin.

## Compile and install the WUPS lib
For building you need:
- [wut](https://github.com/devkitPro/wut)

Install them (in this order) according to their README's. Don't forget the dependencies of the libs itself.

Then you can compile and install this lib via `make install`.

## Use this lib in Dockerfiles.
A prebuilt version of this lib can found on dockerhub. To use it for your projects, add this to your Dockerfile.
```
[...]
COPY --from=ghcr.io/wiiu-env/wiiupluginsystem:[tag] /artifacts $DEVKITPRO
[...]
```
Replace [tag] with a tag you want to use, a list of tags can be found [here](https://github.com/wiiu-env/WiiUPluginSystem/pkgs/container/wiiupluginsystem/versions).
It's highly recommended to pin the version to the **latest date** instead of using `latest`.

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./include ./libraries ./plugins/example_plugin/src  ./plugins/storage_test_plugin/src --exclude ./plugins/storage_test_plugin/src/catch2 -i`
