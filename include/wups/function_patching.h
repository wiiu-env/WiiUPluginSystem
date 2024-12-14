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

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wups_loader_library_type_t {
    WUPS_LOADER_LIBRARY_AVM,
    WUPS_LOADER_LIBRARY_CAMERA,
    WUPS_LOADER_LIBRARY_COREINIT,
    WUPS_LOADER_LIBRARY_DC,
    WUPS_LOADER_LIBRARY_DMAE,
    WUPS_LOADER_LIBRARY_DRMAPP,
    WUPS_LOADER_LIBRARY_ERREULA,
    WUPS_LOADER_LIBRARY_GX2,
    WUPS_LOADER_LIBRARY_H264,
    WUPS_LOADER_LIBRARY_LZMA920,
    WUPS_LOADER_LIBRARY_MIC,
    WUPS_LOADER_LIBRARY_NFC,
    WUPS_LOADER_LIBRARY_NIO_PROF,
    WUPS_LOADER_LIBRARY_NLIBCURL,
    WUPS_LOADER_LIBRARY_NLIBNSS,
    WUPS_LOADER_LIBRARY_NLIBNSS2,
    WUPS_LOADER_LIBRARY_NN_AC,
    WUPS_LOADER_LIBRARY_NN_ACP,
    WUPS_LOADER_LIBRARY_NN_ACT,
    WUPS_LOADER_LIBRARY_NN_AOC,
    WUPS_LOADER_LIBRARY_NN_BOSS,
    WUPS_LOADER_LIBRARY_NN_CCR,
    WUPS_LOADER_LIBRARY_NN_CMPT,
    WUPS_LOADER_LIBRARY_NN_DLP,
    WUPS_LOADER_LIBRARY_NN_EC,
    WUPS_LOADER_LIBRARY_NN_FP,
    WUPS_LOADER_LIBRARY_NN_HAI,
    WUPS_LOADER_LIBRARY_NN_HPAD,
    WUPS_LOADER_LIBRARY_NN_IDBE,
    WUPS_LOADER_LIBRARY_NN_NDM,
    WUPS_LOADER_LIBRARY_NN_NETS2,
    WUPS_LOADER_LIBRARY_NN_NFP,
    WUPS_LOADER_LIBRARY_NN_NIM,
    WUPS_LOADER_LIBRARY_NN_OLV,
    WUPS_LOADER_LIBRARY_NN_PDM,
    WUPS_LOADER_LIBRARY_NN_SAVE,
    WUPS_LOADER_LIBRARY_NN_SL,
    WUPS_LOADER_LIBRARY_NN_SPM,
    WUPS_LOADER_LIBRARY_NN_TEMP,
    WUPS_LOADER_LIBRARY_NN_UDS,
    WUPS_LOADER_LIBRARY_NN_VCTL,
    WUPS_LOADER_LIBRARY_NSYSCCR,
    WUPS_LOADER_LIBRARY_NSYSHID,
    WUPS_LOADER_LIBRARY_NSYSKBD,
    WUPS_LOADER_LIBRARY_NSYSNET,
    WUPS_LOADER_LIBRARY_NSYSUHS,
    WUPS_LOADER_LIBRARY_NSYSUVD,
    WUPS_LOADER_LIBRARY_NTAG,
    WUPS_LOADER_LIBRARY_PADSCORE,
    WUPS_LOADER_LIBRARY_PROC_UI,
    WUPS_LOADER_LIBRARY_SND_CORE,
    WUPS_LOADER_LIBRARY_SND_USER,
    WUPS_LOADER_LIBRARY_SNDCORE2,
    WUPS_LOADER_LIBRARY_SNDUSER2,
    WUPS_LOADER_LIBRARY_SWKBD,
    WUPS_LOADER_LIBRARY_SYSAPP,
    WUPS_LOADER_LIBRARY_TCL,
    WUPS_LOADER_LIBRARY_TVE,
    WUPS_LOADER_LIBRARY_UAC,
    WUPS_LOADER_LIBRARY_UAC_RPL,
    WUPS_LOADER_LIBRARY_USB_MIC,
    WUPS_LOADER_LIBRARY_UVC,
    WUPS_LOADER_LIBRARY_UVD,
    WUPS_LOADER_LIBRARY_VPAD,
    WUPS_LOADER_LIBRARY_VPADBASE,
    WUPS_LOADER_LIBRARY_ZLIB125,
    WUPS_LOADER_LIBRARY_OTHER,
} wups_loader_library_type_t;

typedef enum wups_loader_entry_type_t {
    WUPS_LOADER_ENTRY_FUNCTION,
    WUPS_LOADER_ENTRY_FUNCTION_MANDATORY,
    WUPS_LOADER_ENTRY_EXPORT
} wups_loader_entry_type_t;

typedef enum WUPSFPTargetProcess {
    WUPS_FP_TARGET_PROCESS_ALL              = 0xFF,
    WUPS_FP_TARGET_PROCESS_ROOT_RPX         = 1,
    WUPS_FP_TARGET_PROCESS_WII_U_MENU       = 2,
    WUPS_FP_TARGET_PROCESS_TVII             = 3,
    WUPS_FP_TARGET_PROCESS_E_MANUAL         = 4,
    WUPS_FP_TARGET_PROCESS_HOME_MENU        = 5,
    WUPS_FP_TARGET_PROCESS_ERROR_DISPLAY    = 6,
    WUPS_FP_TARGET_PROCESS_MINI_MIIVERSE    = 7,
    WUPS_FP_TARGET_PROCESS_BROWSER          = 8,
    WUPS_FP_TARGET_PROCESS_MIIVERSE         = 9,
    WUPS_FP_TARGET_PROCESS_ESHOP            = 10,
    WUPS_FP_TARGET_PROCESS_PFID_11          = 11,
    WUPS_FP_TARGET_PROCESS_DOWNLOAD_MANAGER = 12,
    WUPS_FP_TARGET_PROCESS_PFID_13          = 13,
    WUPS_FP_TARGET_PROCESS_PFID_14          = 14,
    WUPS_FP_TARGET_PROCESS_GAME             = 15,
    WUPS_FP_TARGET_PROCESS_GAME_AND_MENU    = 16,
} WUPSFPTargetProcess;

typedef struct wups_loader_entry_t {
    wups_loader_entry_type_t type;
    struct {
        const void *physical_address;             /* (optional) Physical Address. If set, the name and lib will be ignored */
        const void *virtual_address;              /* (optional) Physical Address. If set, the name and lib will be ignored */
        const char *name;                         /* Name of the function that will be replaced */
        const wups_loader_library_type_t library; /**/
        const char *my_function_name;             /* Function name of your own, new function (my_XXX) */
        const void *target;                       /* Address of our own, new function (my_XXX)*/
        const void *call_addr;                    /* Address for calling the real function.(real_XXX) */
        const WUPSFPTargetProcess targetProcess;  /* Target process*/
    } _function;
} wups_loader_entry_t;

#define WUPS_MUST_REPLACE_PHYSICAL(x, physical_address, virtual_address)                            WUPS_MUST_REPLACE_PHYSICAL_FOR_PROCESS(x, physical_address, virtual_address, WUPS_FP_TARGET_PROCESS_GAME_AND_MENU)
#define WUPS_MUST_REPLACE_PHYSICAL_FOR_PROCESS(x, physical_address, virtual_address, targetProcess) WUPS_MUST_REPLACE_EX(physical_address, virtual_address, real_##x, WUPS_LOADER_LIBRARY_OTHER, my_##x, x, targetProcess)

#define WUPS_MUST_REPLACE(x, lib, function_name)                                                    WUPS_MUST_REPLACE_FOR_PROCESS(x, lib, function_name, WUPS_FP_TARGET_PROCESS_GAME_AND_MENU)
#define WUPS_MUST_REPLACE_FOR_PROCESS(x, lib, function_name, targetProcess)                         WUPS_MUST_REPLACE_EX(NULL, NULL, real_##x, lib, my_##x, function_name, targetProcess)

#define WUPS_MUST_REPLACE_EX(pAddress, vAddress, original_func, rpl_type, replace_func, replace_function_name, process) \
    WUPS_SECTION("load")                                                                                                \
    const wups_loader_entry_t wups_load_##replace_func = {                                                              \
            .type      = WUPS_LOADER_ENTRY_FUNCTION_MANDATORY,                                                          \
            ._function = {                                                                                              \
                    .physical_address = (const void *) pAddress,                                                        \
                    .virtual_address  = (const void *) vAddress,                                                        \
                    .name             = #replace_function_name,                                                         \
                    .library          = rpl_type,                                                                       \
                    .my_function_name = #replace_func,                                                                  \
                    .target           = (const void *) &(replace_func),                                                 \
                    .call_addr        = (const void *) &(original_func),                                                \
                    .targetProcess    = process}}

#define DECL_FUNCTION(res, name, ...)                                  \
    res (*real_##name)(__VA_ARGS__) __attribute__((section(".data"))); \
    res my_##name(__VA_ARGS__)

#ifdef __cplusplus
}
#endif