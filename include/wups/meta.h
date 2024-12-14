/* based on blsug.h
 *   by Alex Chadwick
 *
 * Copyright (C) 2014, Alex Chadwick
 * Modified by Maschell, 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "common.h"
#include "hooks.h"

#ifdef __cplusplus
#define __EXTERN_C_MACRO extern "C"
#else
#define __EXTERN_C_MACRO
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define WUPS_VERSION_STR "0.8.1"
#define WUPS_PLUGIN_NAME(__plugin_name)                                                                             \
    WUPS_META(name, __plugin_name);                                                                                 \
    WUPS_META(wups, WUPS_VERSION_STR);                                                                              \
    WUPS_USE_WUT_MALLOC();                                                                                          \
    WUPS_USE_WUT_SOCKETS();                                                                                         \
    WUPS_USE_WUT_NEWLIB();                                                                                          \
    WUPS_USE_WUT_STDCPP();                                                                                          \
    WUPS___INIT_WRAPPER();                                                                                          \
    WUPS___FINI_WRAPPER();                                                                                          \
    WUPS_INIT_CONFIG_FUNCTIONS();                                                                                   \
    WUPS_META(buildtimestamp, __DATE__ " " __TIME__);                                                               \
    WUPS_SECTION("meta") const char wups_meta_plugin_name[] = __plugin_name;                                        \
    WUPS_SECTION("meta") const char wups_meta_info_dump[] = "(plugin: " __plugin_name ";"                           \
                                       "wups " WUPS_VERSION_STR ";"                                                 \
                                       "buildtime: " __DATE__ " " __TIME__ ")";                                     \
    WUPS_SECTION("meta") const char wups_meta_info_linking_order[] = "Loading \"" __plugin_name "\" failed.\n"      \
                                                "Function \"wut_get_thread_specific\" returned unexpected value.\n" \
                                                "Please check linking order (expected \"-lwups -lwut\")";

#define WUPS_PLUGIN_AUTHOR(x)          WUPS_META(author, x)
#define WUPS_PLUGIN_VERSION(x)         WUPS_META(version, x)
#define WUPS_PLUGIN_LICENSE(x)         WUPS_META(license, x)
#define WUPS_PLUGIN_DESCRIPTION(x)     WUPS_META(description, x)
#define WUPS_PLUGIN_CONFIG_REVISION(x) WUPS_META(config_revision, #x)

#ifdef __cplusplus
}
#endif
