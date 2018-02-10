/****************************************************************************
 * Copyright (C) 2016-2018 Maschell
 * With code from chadderz and dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <wups.h>

#include "function_patcher.h"
#include <utils/logger.h>
#include <kernel/kernel_defs.h>
#include <kernel/syscalls.h>

#define DEBUG_LOG_DYN                                   1

rpl_handling rpl_handles[] __attribute__((section(".data"))) = {
    {WUPS_LOADER_LIBRARY_AVM,       "avm.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_CAMERA,    "camera.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_COREINIT,  "coreinit.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_DC,        "dc.rpl"        ,0},
    {WUPS_LOADER_LIBRARY_DMAE,      "dmae.rpl"      ,0},
    {WUPS_LOADER_LIBRARY_DRMAPP,    "drmapp.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_ERREULA,   "erreula.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_GX2,       "gx2.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_H264,      "h264.rpl"      ,0},
    {WUPS_LOADER_LIBRARY_LZMA920,   "lzma920.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_MIC,       "mic.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_NFC,       "nfc.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_NIO_PROF,  "nio_prof.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_NLIBCURL,  "nlibcurl.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_NLIBNSS,   "nlibnss.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NLIBNSS2,  "nlibnss2.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_NN_AC,     "nn_ac.rpl"     ,0},
    {WUPS_LOADER_LIBRARY_NN_ACP,    "nn_acp.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_ACT,    "nn_act.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_AOC,    "nn_aoc.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_BOSS,   "nn_boss.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_CCR,    "nn_ccr.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_CMPT,   "nn_cmpt.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_DLP,    "nn_dlp.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_EC,     "nn_ec.rpl"     ,0},
    {WUPS_LOADER_LIBRARY_NN_FP,     "nn_fp.rpl"     ,0},
    {WUPS_LOADER_LIBRARY_NN_HAI,    "nn_hai.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_HPAD,   "nn_hpad.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_IDBE,   "nn_idbe.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_NDM,    "nn_ndm.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_NETS2,  "nn_nets2.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_NN_NFP,    "nn_nfp.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_NIM,    "nn_nim.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_OLV,    "nn_olv.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_PDM,    "nn_pdm.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_SAVE,   "nn_save.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_SL,     "nn_sl.rpl"     ,0},
    {WUPS_LOADER_LIBRARY_NN_SPM,    "nn_spm.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_TEMP,   "nn_temp.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NN_UDS,    "nn_uds.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_NN_VCTL,   "nn_vctl.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSCCR,   "nsysccr.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSHID,   "nsyshid.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSKBD,   "nsyskbd.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSNET,   "nsysnet.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSUHS,   "nsysuhs.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NSYSUVD,   "nsysuvd.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_NTAG,      "ntag.rpl"      ,0},
    {WUPS_LOADER_LIBRARY_PADSCORE,  "padscore.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_PROC_UI,   "proc_ui.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_SNDCORE2,  "sndcore2.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_SNDUSER2,  "snduser2.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_SND_CORE,  "snd_core.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_SND_USER,  "snd_user.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_SWKBD,     "swkbd.rpl"     ,0},
    {WUPS_LOADER_LIBRARY_SYSAPP,    "sysapp.rpl"    ,0},
    {WUPS_LOADER_LIBRARY_TCL,       "tcl.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_TVE,       "tve.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_UAC,       "uac.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_UAC_RPL,   "uac_rpl.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_USB_MIC,   "usb_mic.rpl"   ,0},
    {WUPS_LOADER_LIBRARY_UVC,       "uvc.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_UVD,       "uvd.rpl"       ,0},
    {WUPS_LOADER_LIBRARY_VPAD,      "vpad.rpl"      ,0},
    {WUPS_LOADER_LIBRARY_VPADBASE,  "vpadbase.rpl"  ,0},
    {WUPS_LOADER_LIBRARY_ZLIB125,   "zlib125.rpl"   ,0}
};

void new_PatchInvidualMethodHooks(replacement_data_module_t * module_data){
    InitAcquireOS();
    new_resetLibs();

    DEBUG_FUNCTION_LINE("Patching %d given functions\n",module_data->number_used_functions);

    s32 method_hooks_count = module_data->number_used_functions;

    u32 skip_instr = 1;
    u32 my_instr_len = 6;
    u32 instr_len = my_instr_len + skip_instr;
    u32 flush_len = 4*instr_len;
    for(s32 i = 0; i < method_hooks_count; i++){
        replacement_data_function_t * function_data = &module_data->functions[i];
        /* Patch branches to it.  */
        volatile u32 *space = function_data->replace_data;

        DEBUG_FUNCTION_LINE("Patching %s ...\n",function_data->function_name);
        if(function_data->functionType == STATIC_FUNCTION && function_data->alreadyPatched == 1){
            if(new_isDynamicFunction((u32)OSEffectiveToPhysical((void*)function_data->realAddr))){
                log_printf("The function %s is a dynamic function. Please fix that <3\n", function_data->function_name);
                function_data->functionType = DYNAMIC_FUNCTION;
            }else{
                log_printf("Skipping %s, its already patched\n", function_data->function_name);
                continue;
            }
        }

        u32 physical = 0;
        u32 repl_addr = (u32)function_data->replaceAddr;
        u32 call_addr = (u32)function_data->replaceCall;

        u32 real_addr = new_GetAddressOfFunction(function_data->function_name,function_data->library);

        if(!real_addr){
            log_printf("\n");
            DEBUG_FUNCTION_LINE("OSDynLoad_FindExport failed for %s\n", function_data->function_name);
            continue;
        }

        if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("%s is located at %08X!\n", function_data->function_name,real_addr);}

        physical = (u32)OSEffectiveToPhysical((void*)real_addr);
        if(!physical){
             log_printf("Error. Something is wrong with the physical address\n");
             continue;
        }

        if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("%s physical is located at %08X!\n", function_data->function_name,physical);}

        *(volatile u32 *)(call_addr) = (u32)(space);


        SC0x25_KernelCopyData((u32)space, physical, 4);
        space++;

        //Only works if skip_instr == 1
        if(skip_instr == 1){
            // fill the restore instruction section
            function_data->realAddr = real_addr;
            function_data->restoreInstruction = *(space-1);
             if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("function_data->realAddr = %08X!\n", function_data->realAddr);}
             if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("function_data->restoreInstruction = %08X!\n",function_data->restoreInstruction) ;}
        }
        else{
            log_printf("Error. Can't save %s for restoring!\n", function_data->function_name);
        }

        //adding jump to real function thx @ dimok for the assembler code
        /*
            90 61 ff e0     stw     r3,-32(r1)
            3c 60 12 34     lis     r3,4660
            60 63 56 78     ori     r3,r3,22136
            7c 69 03 a6     mtctr   r3
            80 61 ff e0     lwz     r3,-32(r1)
            4e 80 04 20     bctr*/
        *space = 0x9061FFE0;
        space++;
        *space = 0x3C600000 | (((real_addr + (skip_instr * 4)) >> 16) & 0x0000FFFF); // lis r3, real_addr@h
        space++;
        *space = 0x60630000 |  ((real_addr + (skip_instr * 4)) & 0x0000ffff); // ori r3, r3, real_addr@l
        space++;
        *space = 0x7C6903A6; // mtctr   r3
        space++;
        *space = 0x8061FFE0; // lwz     r3,-32(r1)
        space++;
        *space = 0x4E800420; // bctr
        space++;
        DCFlushRange((void*)(space - instr_len), flush_len);
        ICInvalidateRange((unsigned char*)(space - instr_len), flush_len);

        //setting jump back
        u32 replace_instr = 0x48000002 | (repl_addr & 0x03fffffc);
        ICInvalidateRange(&replace_instr, 4);
        DCFlushRange(&replace_instr, 4);

        SC0x25_KernelCopyData(physical, (u32)OSEffectiveToPhysical(&replace_instr), 4);
        ICInvalidateRange((void*)(real_addr), 4);
        DCFlushRange((void*)(real_addr), 4);

        function_data->alreadyPatched = 1;
        DEBUG_FUNCTION_LINE("done with patching %s!\n", function_data->function_name);

    }
    DEBUG_FUNCTION_LINE("Done with patching given functions!\n");
}

/* ****************************************************************** */
/*                  RESTORE ORIGINAL INSTRUCTIONS                     */
/* ****************************************************************** */
void new_RestoreInvidualInstructions(replacement_data_module_t * module_data){
    InitAcquireOS();
    new_resetLibs();
    DEBUG_FUNCTION_LINE("Restoring given functions!\n");

    s32 method_hooks_count = module_data->number_used_functions;

    for(s32 i = 0; i < method_hooks_count; i++){
        replacement_data_function_t * function_data = &module_data->functions[i];

        DEBUG_FUNCTION_LINE("Restoring %s... ",function_data->function_name);
        if(function_data->restoreInstruction == 0 || function_data->realAddr == 0){
            log_printf("I dont have the information for the restore =( skip\n");
            continue;
        }

        u32 real_addr = new_GetAddressOfFunction(function_data->function_name,function_data->library);

        if(!real_addr){
            log_printf("OSDynLoad_FindExport failed for %s\n", function_data->function_name);
            continue;
        }

        u32 physical = (u32)OSEffectiveToPhysical((void*)real_addr);
        if(!physical){
            log_printf("Something is wrong with the physical address\n");
            continue;
        }

        if(new_isDynamicFunction(physical)){
             log_printf("Its a dynamic function. We don't need to restore it!\n",function_data->function_name);
        } else {
            physical = (u32)OSEffectiveToPhysical((void*)function_data->realAddr); //When its an static function, we need to use the old location
            if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("Restoring %08X to %08X\n",(u32)function_data->restoreInstruction,physical);}
            SC0x25_KernelCopyData(physical,(u32)&function_data->restoreInstruction , 4);
            if(DEBUG_LOG_DYN){DEBUG_FUNCTION_LINE("ICInvalidateRange %08X\n",(void*)function_data->realAddr);}
            ICInvalidateRange((void*)function_data->realAddr, 4);
            DCFlushRange((void*)function_data->realAddr, 4);
            log_printf("done\n");
        }
        function_data->alreadyPatched = 0; // In case a
    }

    DEBUG_FUNCTION_LINE("Done with restoring given functions!\n");
}

s32 new_isDynamicFunction(u32 physicalAddress){
    if((physicalAddress & 0x80000000) == 0x80000000){
        return 1;
    }
    return 0;
}

u32 new_GetAddressOfFunction(const char * functionName,wups_loader_library_type_t library){
    u32 real_addr = 0;

    if(strcmp(functionName, "OSDynLoad_Acquire") == 0)
    {
        memcpy(&real_addr, &OSDynLoad_Acquire, 4);
        return real_addr;
    }
    else if(strcmp(functionName, "LiWaitOneChunk") == 0)
    {
        real_addr = (u32)addr_LiWaitOneChunk;
        return real_addr;
    }
    else if(strcmp(functionName, "LiBounceOneChunk") == 0)
    {
        //! not required on firmwares above 3.1.0
        if(OS_FIRMWARE >= 400)
            return 0;

        u32 addr_LiBounceOneChunk = 0x010003A0;
        real_addr = (u32)addr_LiBounceOneChunk;
        return real_addr;
    }

    u32 rpl_handle = 0;

    int rpl_handles_size = sizeof rpl_handles / sizeof rpl_handles[0];

    for(int i = 0;i< rpl_handles_size; i++){
        if(rpl_handles[i].library == library){
            if(rpl_handles[i].handle == 0){
                    DEBUG_FUNCTION_LINE("Lets acquire handle for rpl: %s\n",rpl_handles[i].rplname);
                    OSDynLoad_Acquire(rpl_handles[i].rplname, &rpl_handles[i].handle);
            }
            if(rpl_handles[i].handle == 0){log_printf("%s failed to acquire\n",rpl_handles[i].rplname); return 0;}
            rpl_handle = rpl_handles[i].handle;
            break;
        }
    }

    if(!rpl_handle){
        DEBUG_FUNCTION_LINE("Failed to find the RPL handle for %s\n", functionName);
        return 0;
    }

    OSDynLoad_FindExport(rpl_handle, 0, functionName, &real_addr);

    if(!real_addr){
        DEBUG_FUNCTION_LINE("OSDynLoad_FindExport failed for %s\n", functionName);
        return 0;
    }

    if((library == WUPS_LOADER_LIBRARY_NN_ACP) && (u32)(*(volatile u32*)(real_addr) & 0x48000002) == 0x48000000){
        u32 address_diff = (u32)(*(volatile u32*)(real_addr) & 0x03FFFFFC);
        if((address_diff & 0x03000000) == 0x03000000) {
            address_diff |=  0xFC000000;
        }
        real_addr += (s32)address_diff;
        if((u32)(*(volatile u32*)(real_addr) & 0x48000002) == 0x48000000){
            return 0;
        }
    }

    return real_addr;
}

void new_resetLibs(){
    int rpl_handles_size = sizeof rpl_handles / sizeof rpl_handles[0];

    for(int i = 0;i< rpl_handles_size; i++){
        if(rpl_handles[i].handle != 0){
            DEBUG_FUNCTION_LINE("Resetting handle for rpl: %s\n",rpl_handles[i].rplname);
        }
        rpl_handles[i].handle = 0;
        // Release handle?
    }
}
