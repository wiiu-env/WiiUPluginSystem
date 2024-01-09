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
/**
 * @defgroup meta
 * @addtogroup meta 
 *  @{
*/

/**
 * @brief The WUPS release version
 * 
 */
#define WUPS_VERSION_STR "0.7.1"
/**
 * @brief Sets the plugin name
 * 
 * @param __plugin_name The name of your plugin as a string
 */
#define WUPS_PLUGIN_NAME(__plugin_name)                                                                             \
    WUPS_META(name, __plugin_name);                                                                                 \
    WUPS_META(wups, WUPS_VERSION_STR);                                                                              \
    WUPS_USE_WUT_MALLOC();                                                                                          \
    WUPS_USE_WUT_SOCKETS();                                                                                         \
    WUPS_USE_WUT_NEWLIB();                                                                                          \
    WUPS_USE_WUT_STDCPP();                                                                                          \
    WUPS___INIT_WRAPPER();                                                                                          \
    WUPS___FINI_WRAPPER();                                                                                          \
    __EXTERN_C_MACRO void abort();                                                                                  \
    __EXTERN_C_MACRO void OSFatal(const char *msg);                                                                 \
    void abort() {                                                                                                  \
        OSFatal(__plugin_name ": abort() called. Uncaught exception?");                                             \
        while (1)                                                                                                   \
            ;                                                                                                       \
    }                                                                                                               \
    WUPS_META(buildtimestamp, __DATE__ " " __TIME__);                                                               \
    extern const char wups_meta_info_dump[] WUPS_SECTION("meta");                                                   \
    const char wups_meta_info_dump[] = "(plugin: " __plugin_name ";"                                                \
                                       "wups " WUPS_VERSION_STR ";"                                                 \
                                       "buildtime: " __DATE__ " " __TIME__ ")";                                     \
    extern const char wups_meta_info_linking_order[] WUPS_SECTION("meta");                                          \
    const char wups_meta_info_linking_order[] = "Loading \"" __plugin_name "\" failed.\n"                           \
                                                "Function \"wut_get_thread_specific\" returned unexpected value.\n" \
                                                "Please check linking order (expected \"-lwups -lwut\")";
/**
 * @brief Sets the plugin author
 * 
 * @param x The author or authors of the plugin as a string
 */
#define WUPS_PLUGIN_AUTHOR(x)          WUPS_META(author, x)
/**
 * @brief Sets the plugin release version
 * 
 * Sets the revision of the plugin as it is seen in the config. 
 * 
 * @param x The revision of the plugin as a string
 */
#define WUPS_PLUGIN_VERSION(x)         WUPS_META(version, x)
/**
 * @brief Sets the plugin license
 * 
 * Sets the license that the plugin is licensed under.
 * 
 * @param x The license of the plugin as a string
 */
#define WUPS_PLUGIN_LICENSE(x)         WUPS_META(license, x)
/**
 * @brief Sets the description of the plugin
 * 
 * @param x The description of the plugin as a string
 */
#define WUPS_PLUGIN_DESCRIPTION(x)     WUPS_META(description, x)
#define WUPS_PLUGIN_CONFIG_REVISION(x) WUPS_META(config_revision, #x)

/**
 * @}
 */
#ifdef __cplusplus
}
#endif
