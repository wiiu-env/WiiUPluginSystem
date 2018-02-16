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
 
#ifndef WUPS_H_
#define WUPS_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WUPS_SECTION(x) __attribute__((__section__ (".wups." x)))
        
#define DECL_FUNCTION(res, name, ...) \
        res (* real_ ## name)(__VA_ARGS__) __attribute__((section(".data"))); \
        res my_ ## name(__VA_ARGS__)

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
} wups_loader_library_type_t;

typedef enum wups_loader_hook_type_t {
    WUPS_LOADER_HOOK_INIT_FUNCTION
} wups_loader_hook_type_t;

typedef struct wups_loader_hook_t {
    wups_loader_hook_type_t type;
    const void *target;                         /*Address of our own, new function (my_XXX)*/
} wups_loader_hook_t;



#define WUPS_HOOK_INIT(original_func) \
    extern const wups_loader_hook_t wups_hooks_init_ ## original_func \
        WUPS_SECTION("hooks"); \
    const wups_loader_hook_t wups_hooks_init_ ## original_func = { \
        .type = WUPS_LOADER_HOOK_INIT_FUNCTION, \
        .target = (const void*)&(original_func) \
    }

#define WUPS_SDUSB_MOUNTED_NONE          0
#define WUPS_SDUSB_MOUNTED_FAKE          (1<<0)
#define WUPS_SDUSB_MOUNTED_OS_SD         (1<<1)
#define WUPS_SDUSB_LIBIOSU_LOADED        (1<<2)
#define WUPS_SD_MOUNTED_LIBFAT           (1<<3)
#define WUPS_USB_MOUNTED_LIBFAT          (1<<4)
#define WUPS_USB_MOUNTED_LIBNTFS         (1<<5)

#define WUPS_SD_MOUNTED                  (WUPS_SDUSB_MOUNTED_OS_SD | WUPS_SD_MOUNTED_LIBFAT)
#define WUPS_USB_MOUNTED                 (WUPS_USB_MOUNTED_LIBFAT)
    
typedef struct wups_loader_init_args_t {
    int device_mounted;
    struct {		        
		const void * open_repl;
		const void * close_repl;
		const void * write_repl;
		const void * read_repl;
		const void * lseek_repl;
		const void * stat_repl;
		const void * fstat_repl;
		const void * opendir_repl;
		const void * closedir_repl;
		const void * readdir_repl;
	} fs_wrapper;
} wups_loader_init_args_t;
    
#ifdef __cplusplus
    #define EXTERN_C_START      extern "C" {
    #define EXTERN_C_END        }
#else
    #define EXTERN_C_START
    #define EXTERN_C_END
#endif

#define INITIALIZE(my_args) \
    void init(wups_loader_init_args_t*);\
    void myInit(wups_loader_init_args_t*);\
    WUPS_HOOK_INIT(init); \
    void init(wups_loader_init_args_t* args){ \
        if(args != NULL){\
        }\
        myInit(args);\
    } \
    void myInit(wups_loader_init_args_t* my_args)
    
typedef enum wups_loader_entry_type_t {
    WUPS_LOADER_ENTRY_FUNCTION,
    WUPS_LOADER_ENTRY_FUNCTION_MANDATORY,
    WUPS_LOADER_ENTRY_EXPORT
} wups_loader_entry_type_t;

typedef struct wups_loader_entry_t {
    wups_loader_entry_type_t type;
	struct {
		const char *name;                           /* Name of the function that will be replaced */
		const wups_loader_library_type_t library;   /**/            
		const char *my_function_name;               /* Function name of your own, new function (my_XXX) */     
		const void *call_addr;                      /* Function name of function, to call the real function.(real_XXX) */            
		const void *target;                         /*Address of our own, new function (my_XXX)*/
	} _function;
} wups_loader_entry_t;

#define WUPS_MUST_REPLACE(x, lib, function_name) WUPS_MUST_REPLACE_EX(real_ ## x, lib, my_ ## x,  function_name);

#define WUPS_MUST_REPLACE_EX(original_func, rpl_type, replace_func, replace_function_name) \
    extern const wups_loader_entry_t wups_load_ ## replace_func \
        WUPS_SECTION("load"); \
    const wups_loader_entry_t wups_load_ ## replace_func = { \
        .type = WUPS_LOADER_ENTRY_FUNCTION_MANDATORY, \
        ._function = { \
                .name = #replace_function_name, \
                .library = rpl_type, \
                .my_function_name = #replace_func, \
                .call_addr = (const void*)&(original_func), \
                .target = (const void*)&(replace_func) \
            } \
    }

#define WUPS_META(id, value) \
    extern const char wups_meta_ ## id [] WUPS_SECTION("meta"); \
    const char wups_meta_ ## id [] = #id "=" value

#define WUPS_MODULE_NAME(x)    WUPS_META(name, x); WUPS_META(wups, "0.1")
#define WUPS_MODULE_AUTHOR(x)  WUPS_META(author, x)
#define WUPS_MODULE_VERSION(x) WUPS_META(version, x)
#define WUPS_MODULE_LICENSE(x) WUPS_META(license, x)

void WUPS_InitFS(wups_loader_init_args_t* args);

#ifdef __cplusplus
}
#endif

#endif /* WUPS_WUPS_H_ */