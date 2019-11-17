OUTPUT_FORMAT("elf32-powerpc")
OUTPUT_ARCH(powerpc:common)

SECTIONS {
	.text : {
		*(.text*)
	}
	.sdata : {
		*(.sdata*)
	}
	.data : {
		*(.data*)
	}
	.rodata : {
		*(.rodata*)
	}
	.sbss : {
		*(.sbss*)
	}
	.bss : {
		*(.bss*)
	}    
    .got2 : {
		*(.got2*)
	}
    .gcc_except_table : {
        *(.gcc_except_table*)
    }
    
    .fimport_avm ALIGN(16) : {
        KEEP ( *(.fimport_avm) )
        *(.fimport_avm.*)
    }
    .fimport_camera ALIGN(16) : {
        KEEP ( *(.fimport_camera) )
        *(.fimport_camera.*)
    }
    .fimport_coreinit ALIGN(16) : {
        KEEP ( *(.fimport_coreinit) )
        *(.fimport_coreinit.*)
    }
    .fimport_dc ALIGN(16) : {
        KEEP ( *(.fimport_dc) )
        *(.fimport_dc.*)
    }
    .fimport_dmae ALIGN(16) : {
        KEEP ( *(.fimport_dmae) )
        *(.fimport_dmae.*)
    }
    .fimport_drmapp ALIGN(16) : {
        KEEP ( *(.fimport_drmapp) )
        *(.fimport_drmapp.*)
    }
    .fimport_erreula ALIGN(16) : {
        KEEP ( *(.fimport_erreula) )
        *(.fimport_erreula.*)
    }
    .fimport_gx2 ALIGN(16) : {
        KEEP ( *(.fimport_gx2) )
        *(.fimport_gx2.*)
    }
    .fimport_h264 ALIGN(16) : {
        KEEP ( *(.fimport_h264) )
        *(.fimport_h264.*)
    }
    .fimport_lzma920 ALIGN(16) : {
        KEEP ( *(.fimport_lzma920) )
        *(.fimport_lzma920.*)
    }
    .fimport_mic ALIGN(16) : {
        KEEP ( *(.fimport_mic) )
        *(.fimport_mic.*)
    }
    .fimport_nfc ALIGN(16) : {
        KEEP ( *(.fimport_nfc) )
        *(.fimport_nfc.*)
    }
    .fimport_nio_prof ALIGN(16) : {
        KEEP ( *(.fimport_nio_prof) )
        *(.fimport_nio_prof.*)
    }
    .fimport_nlibcurl ALIGN(16) : {
        KEEP ( *(.fimport_nlibcurl) )
        *(.fimport_nlibcurl.*)
    }
    .fimport_nlibnss2 ALIGN(16) : {
        KEEP ( *(.fimport_nlibnss2) )
        *(.fimport_nlibnss2.*)
    }
    .fimport_nlibnss ALIGN(16) : {
        KEEP ( *(.fimport_nlibnss) )
        *(.fimport_nlibnss.*)
    }
    .fimport_nn_ac ALIGN(16) : {
        KEEP ( *(.fimport_nn_ac) )
        *(.fimport_nn_ac.*)
    }
    .fimport_nn_acp ALIGN(16) : {
        KEEP ( *(.fimport_nn_acp) )
        *(.fimport_nn_acp.*)
    }
    .fimport_nn_act ALIGN(16) : {
        KEEP ( *(.fimport_nn_act) )
        *(.fimport_nn_act.*)
    }
    .fimport_nn_aoc ALIGN(16) : {
        KEEP ( *(.fimport_nn_aoc) )
        *(.fimport_nn_aoc.*)
    }
    .fimport_nn_boss ALIGN(16) : {
        KEEP ( *(.fimport_nn_boss) )
        *(.fimport_nn_boss.*)
    }
    .fimport_nn_ccr ALIGN(16) : {
        KEEP ( *(.fimport_nn_ccr) )
        *(.fimport_nn_ccr.*)
    }
    .fimport_nn_cmpt ALIGN(16) : {
        KEEP ( *(.fimport_nn_cmpt) )
        *(.fimport_nn_cmpt.*)
    }
    .fimport_nn_dlp ALIGN(16) : {
        KEEP ( *(.fimport_nn_dlp) )
        *(.fimport_nn_dlp.*)
    }
    .fimport_nn_ec ALIGN(16) : {
        KEEP ( *(.fimport_nn_ec) )
        *(.fimport_nn_ec.*)
    }
    .fimport_nn_fp ALIGN(16) : {
        KEEP ( *(.fimport_nn_fp) )
        *(.fimport_nn_fp.*)
    }
    .fimport_nn_hai ALIGN(16) : {
        KEEP ( *(.fimport_nn_hai) )
        *(.fimport_nn_hai.*)
    }
    .fimport_nn_hpad ALIGN(16) : {
        KEEP ( *(.fimport_nn_hpad) )
        *(.fimport_nn_hpad.*)
    }
    .fimport_nn_idbe ALIGN(16) : {
        KEEP ( *(.fimport_nn_idbe) )
        *(.fimport_nn_idbe.*)
    }
    .fimport_nn_ndm ALIGN(16) : {
        KEEP ( *(.fimport_nn_ndm) )
        *(.fimport_nn_ndm.*)
    }
    .fimport_nn_nets2 ALIGN(16) : {
        KEEP ( *(.fimport_nn_nets2) )
        *(.fimport_nn_nets2.*)
    }
    .fimport_nn_nfp ALIGN(16) : {
        KEEP ( *(.fimport_nn_nfp) )
        *(.fimport_nn_nfp.*)
    }
    .fimport_nn_nim ALIGN(16) : {
        KEEP ( *(.fimport_nn_nim) )
        *(.fimport_nn_nim.*)
    }
    .fimport_nn_olv ALIGN(16) : {
        KEEP ( *(.fimport_nn_olv) )
        *(.fimport_nn_olv.*)
    }
    .fimport_nn_pdm ALIGN(16) : {
        KEEP ( *(.fimport_nn_pdm) )
        *(.fimport_nn_pdm.*)
    }
    .fimport_nn_save ALIGN(16) : {
        KEEP ( *(.fimport_nn_save) )
        *(.fimport_nn_save.*)
    }
    .fimport_nn_sl ALIGN(16) : {
        KEEP ( *(.fimport_nn_sl) )
        *(.fimport_nn_sl.*)
    }
    .fimport_nn_spm ALIGN(16) : {
        KEEP ( *(.fimport_nn_spm) )
        *(.fimport_nn_spm.*)
    }
    .fimport_nn_temp ALIGN(16) : {
        KEEP ( *(.fimport_nn_temp) )
        *(.fimport_nn_temp.*)
    }
    .fimport_nn_uds ALIGN(16) : {
        KEEP ( *(.fimport_nn_uds) )
        *(.fimport_nn_uds.*)
    }
    .fimport_nn_vctl ALIGN(16) : {
        KEEP ( *(.fimport_nn_vctl) )
        *(.fimport_nn_vctl.*)
    }
    .fimport_nsysccr ALIGN(16) : {
        KEEP ( *(.fimport_nsysccr) )
        *(.fimport_nsysccr.*)
    }
    .fimport_nsyshid ALIGN(16) : {
        KEEP ( *(.fimport_nsyshid) )
        *(.fimport_nsyshid.*)
    }
    .fimport_nsyskbd ALIGN(16) : {
        KEEP ( *(.fimport_nsyskbd) )
        *(.fimport_nsyskbd.*)
    }
    .fimport_nsysnet ALIGN(16) : {
        KEEP ( *(.fimport_nsysnet) )
        *(.fimport_nsysnet.*)
    }
    .fimport_nsysuhs ALIGN(16) : {
        KEEP ( *(.fimport_nsysuhs) )
        *(.fimport_nsysuhs.*)
    }
    .fimport_nsysuvd ALIGN(16) : {
        KEEP ( *(.fimport_nsysuvd) )
        *(.fimport_nsysuvd.*)
    }
    .fimport_ntag ALIGN(16) : {
        KEEP ( *(.fimport_ntag) )
        *(.fimport_ntag.*)
    }
    .fimport_padscore ALIGN(16) : {
        KEEP ( *(.fimport_padscore) )
        *(.fimport_padscore.*)
    }
    .fimport_proc_ui ALIGN(16) : {
        KEEP ( *(.fimport_proc_ui) )
        *(.fimport_proc_ui.*)
    }
    .fimport_sndcore2 ALIGN(16) : {
        KEEP ( *(.fimport_sndcore2) )
        *(.fimport_sndcore2.*)
    }
    .fimport_snd_core ALIGN(16) : {
        KEEP ( *(.fimport_snd_core) )
        *(.fimport_snd_core.*)
    }
    .fimport_snduser2 ALIGN(16) : {
        KEEP ( *(.fimport_snduser2) )
        *(.fimport_snduser2.*)
    }
    .fimport_snd_user ALIGN(16) : {
        KEEP ( *(.fimport_snd_user) )
        *(.fimport_snd_user.*)
    }
    .fimport_swkbd ALIGN(16) : {
        KEEP ( *(.fimport_swkbd) )
        *(.fimport_swkbd.*)
    }
    .fimport_sysapp ALIGN(16) : {
        KEEP ( *(.fimport_sysapp) )
        *(.fimport_sysapp.*)
    }
    .fimport_tcl ALIGN(16) : {
        KEEP ( *(.fimport_tcl) )
        *(.fimport_tcl.*)
    }
    .fimport_tve ALIGN(16) : {
        KEEP ( *(.fimport_tve) )
        *(.fimport_tve.*)
    }
    .fimport_uac ALIGN(16) : {
        KEEP ( *(.fimport_uac) )
        *(.fimport_uac.*)
    }
    .fimport_uac_rpl ALIGN(16) : {
        KEEP ( *(.fimport_uac_rpl) )
        *(.fimport_uac_rpl.*)
    }
    .fimport_usb_mic ALIGN(16) : {
        KEEP ( *(.fimport_usb_mic) )
        *(.fimport_usb_mic.*)
    }
    .fimport_uvc ALIGN(16) : {
        KEEP ( *(.fimport_uvc) )
        *(.fimport_uvc.*)
    }
    .fimport_uvd ALIGN(16) : {
        KEEP ( *(.fimport_uvd) )
        *(.fimport_uvd.*)
    }
    .fimport_vpadbase ALIGN(16) : {
        KEEP ( *(.fimport_vpadbase) )
        *(.fimport_vpadbase.*)
    }
    .fimport_vpad ALIGN(16) : {
        KEEP ( *(.fimport_vpad) )
        *(.fimport_vpad.*)
    }
    .fimport_zlib125 ALIGN(16) : {
        KEEP ( *(.fimport_zlib125) )
        *(.fimport_zlib125.*)
    }

    .dimport_coreinit ALIGN(16) : {
        KEEP ( *(.dimport_coreinit) )
        *(.dimport_coreinit.*)
    }
    .dimport_nn_act ALIGN(16) : {
        KEEP ( *(.dimport_nn_act) )
        *(.dimport_nn_act.*)
    }
    .dimport_nn_boss ALIGN(16) : {
        KEEP ( *(.dimport_nn_boss) )
        *(.dimport_nn_boss.*)
    }
    .dimport_nn_ec ALIGN(16) : {
        KEEP ( *(.dimport_nn_ec) )
        *(.dimport_nn_ec.*)
    }
    .dimport_nn_nim ALIGN(16) : {
        KEEP ( *(.dimport_nn_nim) )
        *(.dimport_nn_nim.*)
    }
    .dimport_nn_sl ALIGN(16) : {
        KEEP ( *(.dimport_nn_sl) )
        *(.dimport_nn_sl.*)
    }
    .dimport_nn_uds ALIGN(16) : {
        KEEP ( *(.dimport_nn_uds) )
        *(.dimport_nn_uds.*)
    }

    .wups.meta : {
		*(.wups.meta*)
        KEEP(*(.wups.meta*))
	}
	.wups.load : {
		*(.wups.load*)
        KEEP(*(.wups.load*))
	}
    .wups.hooks : {
		*(.wups.hooks*)
        KEEP(*(.wups.hooks*))
	}
    
    /DISCARD/ : {
        *(.abs)
        *(.comment)
        *(.gnu.attributes)
        *(.gnu.version)
        *(.gnu.version_d)
        *(.gnu.version_r)
    }
}