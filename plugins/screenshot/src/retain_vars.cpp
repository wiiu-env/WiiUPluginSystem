#include "retain_vars.hpp"
wups_loader_app_status_t gAppStatus __attribute__((section(".data"))) = WUPS_APP_STATUS_UNKNOWN;
u32 gButtonCombo __attribute__((section(".data"))) = 0;
