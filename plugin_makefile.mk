ASFLAGS	:= -mregnames

# -T: use the linker script specified
# -wrap: wrap function
# -q: Leave relocation sections and contents in fully linked executables
 
LDFLAGS  +=     -T $(WUPSDIR)/wups.ld \
                -Wl,-wrap,open,-wrap,close,-wrap,write,-wrap,read,-wrap,lseek,-wrap,stat,-wrap,fstat,-wrap,opendir,-wrap,closedir,-wrap,readdir,-wrap,mkdir \
                -Wl,-q