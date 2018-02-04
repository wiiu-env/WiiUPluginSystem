/* wups.h
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
 
#ifndef WUPS_H_
#define WUPS_H_

#include <stddef.h>
#include <stdint.h>

#define WUPS_SECTION(x) __attribute__((__section__ (".wups." x)))

typedef enum wups_loader_entry_type_t {
    WUPS_LOADER_ENTRY_FUNCTION,
    WUPS_LOADER_ENTRY_FUNCTION_MANDATORY,
    WUPS_LOADER_ENTRY_EXPORT
} wups_loader_entry_type_t;


typedef enum wups_loader_library_type_t {
    WUPS_LOADER_LIBRARY_COREINIT,
    WUPS_LOADER_LIBRARY_GX2
} wups_loader_library_type_t;

typedef struct wups_loader_entry_t {
    wups_loader_entry_type_t type;
    union {
        struct {
            const char *name;
            const wups_loader_library_type_t library;
            const void *target;
        } _function;
        struct {
            const char *name;
            const void *target;
        } _export;
    } data;
} wups_loader_entry_t;

#define WUPS_REPLACE(original_func, rpl_type, replace_func) \
    extern const wups_loader_entry_t wups_load_ ## original_func \
        WUPS_SECTION("load"); \
    const wups_loader_entry_t wups_load_ ## original_func = { \
        .type = WUPS_LOADER_ENTRY_FUNCTION, \
        .data = { \
            ._function = { \
                .name = #original_func, \
                .library = rpl_type, \
                .target = &(replace_func) \
            } \
        } \
    }
#define WUPS_MUST_REPLACE(original_func, rpl_type, replace_func) \
    extern const wups_loader_entry_t wups_load_ ## original_func \
        WUPS_SECTION("load"); \
    const wups_loader_entry_t wups_load_ ## original_func = { \
        .type = WUPS_LOADER_ENTRY_FUNCTION_MANDATORY, \
        .data = { \
            ._function = { \
                .name = #original_func, \
                .library = rpl_type, \
                .target = &(replace_func) \
            } \
        } \
    }
#define WUPS_EXPORT(symbol) \
    extern const wups_loader_entry_t wups_export_ ## symbol \
        WUPS_SECTION("load"); \
    const wups_loader_entry_t wups_export_ ## symbol = { \
        .type = WUPS_LOADER_ENTRY_EXPORT, \
        .data = { \
            ._export = { \
                .name = #symbol, \
                .target = &(symbol) \
            } \
        } \
    }

#define WUPS_META(id, value) \
    extern const char wups_meta_ ## id [] WUPS_SECTION("meta"); \
    const char wups_meta_ ## id [] = #id "=" value

#define WUPS_MODULE_NAME(x)    WUPS_META(name, x); WUPS_META(wups, "0.1")
#define WUPS_MODULE_AUTHOR(x)  WUPS_META(author, x)
#define WUPS_MODULE_VERSION(x) WUPS_META(version, x)
#define WUPS_MODULE_LICENSE(x) WUPS_META(license, x)

/* wups_game_start - first address that is part of the game's executable.
 * wups_game_end   - address after the end of the game's executable.
 * Added in WUPS_LIB_VERSION 0.1.1 */
extern uint8_t wups_game_start[];
extern uint8_t wups_game_end[];

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */