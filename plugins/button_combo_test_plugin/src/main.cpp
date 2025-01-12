#include "utils/logger.h"
#include <catch2/catch_amalgamated.hpp>
#include <wups.h>

WUPS_PLUGIN_NAME("Button_Combo_Test_Plugin");
WUPS_PLUGIN_DESCRIPTION("This plugin tests the button combo api");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Maschell");
WUPS_PLUGIN_LICENSE("BSD");

WUPS_USE_WUT_DEVOPTAB(); // Use the wut devoptabs

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