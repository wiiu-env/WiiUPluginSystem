ASFLAGS	:= -mregnames

# --relocatable: make sure ld doesn't remove relocations wups will need
# -s: strip local symbols to speed linking
# -u: keep certain sections
# -wrap: wrap function
# --gc-sections: remove unneeded symbols
# -T: use the linker script specified (to force certain wups sections together)
# -Map: generate a map file
 
LDFLAG_COMMON   :=  -u wups_load -u wups_meta -u wups_hooks -T $(WUPSDIR)/wups.ld \
                    -Wl,-wrap,open,-wrap,close,-wrap,write,-wrap,read,-wrap,lseek,-wrap,stat,-wrap,fstat,-wrap,opendir,-wrap,closedir,-wrap,readdir,-wrap,mkdir \
                    -Wl,-Map,$(notdir $@).map,--gc-sections

LDFLAGS_MOD     :=  -Wl,--relocatable
LDFLAGS_ELF     :=  --relocatable -s -T $(WUPSDIR)/wups_elf.ld