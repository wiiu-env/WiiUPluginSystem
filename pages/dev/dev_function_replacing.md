---
title: Function replacing
tags: [features]
keywords: Function, replacing
last_updated: July 24, 2018
sidebar: dev_sidebar
permalink: dev_function_replacing.html
folder: dev
topnav: topnav
---

The plugin system allows plugins to replace (dynamically) loaded system functions.  
**Keep in mind that other plugin also could override function and may manipulate the arguments or return values!**
## Declare function replacement
The function replacement itself is done by a macro:
```C
DECL_FUNCTION(  RETURN_TYPE, ARBITRARY_NAME_OF_FUNCTION , ARGS_SEPERATED_BY_COMMA){
    //Your code goes here.
}
```
* `RETURN_TYPE` is the return type of the function. For example `void` or `int`
* `ARBITRARY_NAME_OF_FUNCTION` is the name of function replacement. This name can be choosen freely.
* `ARGS_SEPERATED_BY_COMMA` is parameter list, each argument seperated by a comma. Example `int, int, char`.
* Inside macro is the code of the function replacement. The return value corresponds to `RETURN_TYPE`.
* Inside the macro the function pointer `my_ARBITRARY_NAME_OF_FUNCTION` and `real_ARBITRARY_NAME_OF_FUNCTION` are avaiable. Calling `real_XXX` will call the original, replaced function.


### Example

To replace the function 
```C
int FSOpenFile(FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error)
```
The following macro could be created, which logs all FSOpenFile call via `log_printf`:
```C
DECL_FUNCTION(int, FSOpenFile, FSClient *pClient, FSCmdBlock *pCmd, const char *path, const char *mode, int *handle, int error) {
    int result = real_FSOpenFile(pClient, pCmd, path, mode, handle, error);
    
    log_printf("FSOpenFile called for folder %s! Result %d\n",path,result);
    return result;
}
```
In this example `FSOpenFile` was taken as an function name, but anything would've been also fine. In the beginning of the macro, the original function is called via `real_FSOpenFile`. 
After that, the result and path argument will be logged and the function returns with the original result.

## Replace function with a declared function replacement
After defining a function replacement, the plugin needs to tell the plugin system which function of which library should be replaced. This is done by the following macro:
```
WUPS_MUST_REPLACE(FUNCTION_NAME_IN_THIS_FILE,   NAME_OF_LIB_WHICH_CONTAINS_THIS_FUNCTION, NAME_OF_FUNCTION_TO_OVERRIDE)
```
* `FUNCTION_NAME_IN_THIS_FILE` corresponds to the function that was set in the declared function replacement (`ARBITRARY_NAME_OF_FUNCTION`).
* `NAME_OF_LIB_WHICH_CONTAINS_THIS_FUNCTION` is a value of the `wups_loader_library_type_t` enum. Each value corresponds to a system .rpl file.
* `NAME_OF_FUNCTION_TO_OVERRIDE` need to be set to the name of function that should be replaced. This needs to match the name exported in the corresponding .rpl.

Currently it's only possible to override functions that are loaded from .rpl files of OSv10 (00050010-1000400A). A full list of supported `.rpl` with their corresponding enum value can be found here:

| Enum value| .rpl file | Description |
| --- | --- | --- |
| WUPS_LOADER_LIBRARY_AVM|        avm.rpl|       | 
| WUPS_LOADER_LIBRARY_CAMERA|     camera.rpl|    | 
| WUPS_LOADER_LIBRARY_COREINIT|   coreinit.rpl|  | 
| WUPS_LOADER_LIBRARY_DC|         dc.rpl|        | 
| WUPS_LOADER_LIBRARY_DMAE|       dmae.rpl|      | 
| WUPS_LOADER_LIBRARY_DRMAPP|     drmapp.rpl|    | 
| WUPS_LOADER_LIBRARY_ERREULA|    erreula.rpl|   | 
| WUPS_LOADER_LIBRARY_GX2|        gx2.rpl|       | 
| WUPS_LOADER_LIBRARY_H264|       h264.rpl|      | 
| WUPS_LOADER_LIBRARY_LZMA920|    lzma920.rpl|   | 
| WUPS_LOADER_LIBRARY_MIC|        mic.rpl|       | 
| WUPS_LOADER_LIBRARY_NFC|        nfc.rpl|       | 
| WUPS_LOADER_LIBRARY_NIO_PROF|   nio_prof.rpl|  | 
| WUPS_LOADER_LIBRARY_NLIBCURL|   nlibcurl.rpl|  | 
| WUPS_LOADER_LIBRARY_NLIBNSS|    nlibnss.rpl|   | 
| WUPS_LOADER_LIBRARY_NLIBNSS2|   nlibnss2.rpl|  | 
| WUPS_LOADER_LIBRARY_NN_AC|      nn_ac.rpl|     | 
| WUPS_LOADER_LIBRARY_NN_ACP|     nn_acp.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_ACT|     nn_act.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_AOC|     nn_aoc.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_BOSS|    nn_boss.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_CCR|     nn_ccr.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_CMPT|    nn_cmpt.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_DLP|     nn_dlp.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_EC|      nn_ec.rpl|     | 
| WUPS_LOADER_LIBRARY_NN_FP|      nn_fp.rpl|     | 
| WUPS_LOADER_LIBRARY_NN_HAI|     nn_hai.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_HPAD|    nn_hpad.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_IDBE|    nn_idbe.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_NDM|     nn_ndm.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_NETS2|   nn_nets2.rpl|  | 
| WUPS_LOADER_LIBRARY_NN_NFP|     nn_nfp.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_NIM|     nn_nim.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_OLV|     nn_olv.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_PDM|     nn_pdm.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_SAVE|    nn_save.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_SL|      nn_sl.rpl|     | 
| WUPS_LOADER_LIBRARY_NN_SPM|     nn_spm.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_TEMP|    nn_temp.rpl|   | 
| WUPS_LOADER_LIBRARY_NN_UDS|     nn_uds.rpl|    | 
| WUPS_LOADER_LIBRARY_NN_VCTL|    nn_vctl.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSCCR|    nsysccr.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSHID|    nsyshid.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSKBD|    nsyskbd.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSNET|    nsysnet.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSUHS|    nsysuhs.rpl|   | 
| WUPS_LOADER_LIBRARY_NSYSUVD|    nsysuvd.rpl|   | 
| WUPS_LOADER_LIBRARY_NTAG|       ntag.rpl|      | 
| WUPS_LOADER_LIBRARY_PADSCORE|   padscore.rpl|  | 
| WUPS_LOADER_LIBRARY_PROC_UI|    proc_ui.rpl|   | 
| WUPS_LOADER_LIBRARY_SNDCORE2|   sndcore2.rpl|  | 
| WUPS_LOADER_LIBRARY_SNDUSER2|   snduser2.rpl|  | 
| WUPS_LOADER_LIBRARY_SND_CORE|   snd_core.rpl|  | 
| WUPS_LOADER_LIBRARY_SND_USER|   snd_user.rpl|  | 
| WUPS_LOADER_LIBRARY_SWKBD|      swkbd.rpl|     | 
| WUPS_LOADER_LIBRARY_SYSAPP|     sysapp.rpl|    | 
| WUPS_LOADER_LIBRARY_TCL|        tcl.rpl|       | 
| WUPS_LOADER_LIBRARY_TVE|        tve.rpl|       | 
| WUPS_LOADER_LIBRARY_UAC|        uac.rpl|       | 
| WUPS_LOADER_LIBRARY_UAC_RPL|    uac_rpl.rpl|   | 
| WUPS_LOADER_LIBRARY_USB_MIC|    usb_mic.rpl|   | 
| WUPS_LOADER_LIBRARY_UVC|        uvc.rpl|       | 
| WUPS_LOADER_LIBRARY_UVD|        uvd.rpl|       | 
| WUPS_LOADER_LIBRARY_VPAD|       vpad.rpl|      | 
| WUPS_LOADER_LIBRARY_VPADBASE|   vpadbase.rpl|  | 
| WUPS_LOADER_LIBRARY_ZLIB125|    zlib125.rpl|   |

### Example
To tell the plugin system to replace the FSOpenFile function replacement, the following macro could be used:
```
WUPS_MUST_REPLACE(FSOpenFile, WUPS_LOADER_LIBRARY_COREINIT, FSOpenFile);
```
This has to be defined in the same file where the function replacement was declared.

## Technical information

Within the DECL_FUCTION macro, two more function get declare you can use. `my_ARBITRARY_NAME_OF_FUNCTION` and `real_ARBITRARY_NAME_OF_FUNCTION`.
```
RETURN_TYPE my_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
    is just name of the function that gets declared in this macro. 
    It has the same effect as calling the overridden function directly.

RETURN_TYPE real_ARBITRARY_NAME_OF_FUNCTION(ARGS_SEPERATED_BY_COMMA):
    is the name of the function, that leads to function that was overridden.
    Use this to call the original function that will be overridden.
    CAUTION: Other plugins may already have manipulated the the return value or arguments.
```

{% include links.html %}
