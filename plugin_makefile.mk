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
                    -Wl,-Map,$(notdir $@).map,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size,-wrap,_malloc_r,-wrap,_free_r,-wrap,_realloc_r,-wrap,_calloc_r,-wrap,_memalign_r,-wrap,_malloc_usable_size_r,--gc-sections

LDFLAGS_MOD     :=  $(LDFLAG_COMMON),--relocatable
LDFLAGS_ELF     :=  --relocatable -s -T $(WUPSDIR)/wups_elf.ld