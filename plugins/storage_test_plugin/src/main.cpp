#include "utils/logger.h"

#include <wups.h>

#include <catch2/catch_amalgamated.hpp>

WUPS_PLUGIN_NAME("Storage_Test_Plugin");
WUPS_PLUGIN_DESCRIPTION("This plugin tests the storage api");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

WUPS_USE_WUT_DEVOPTAB();              // Use the wut devoptabs
WUPS_USE_STORAGE("storage_api_test"); // Unique id for the storage api

INITIALIZE_PLUGIN() {
    initLogging();
    try {
        Catch::ConfigData config;
        //config.showSuccessfulTests = true;
        Catch::Session session = Catch::Session();
        session.useConfigData(config);
        session.run();
    } catch (std::exception &e) {
        DEBUG_FUNCTION_LINE_ERR("Exception happened: %s", e.what());
    }

    deinitLogging();
}