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

#ifndef WUPS_META_DEF_H_
#define WUPS_META_DEF_H_

#include "common.h"
#include "hooks.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WUPS_PLUGIN_NAME(x)    WUPS_META(name, x); WUPS_META(wups, "0.4"); WUPS_USE_WUT_MALLOC() WUPS_USE_WUT_NEWLIB() WUPS_USE_WUT_STDCPP() WUPS_META(buildtimestamp, __DATE__ " " __TIME__); 
#define WUPS_PLUGIN_AUTHOR(x)  WUPS_META(author, x)
#define WUPS_PLUGIN_VERSION(x) WUPS_META(version, x)
#define WUPS_PLUGIN_LICENSE(x) WUPS_META(license, x)
#define WUPS_PLUGIN_DESCRIPTION(x) WUPS_META(description, x)
#define WUPS_PLUGIN_ID(x)      WUPS_META(id, x)
#define WUPS_PLUGIN_CONFIG_REVISION(x) WUPS_META(config_revision, #x)

#ifdef __cplusplus
}
#endif

#endif /* WUPS_COMMON_DEF_H_ */
