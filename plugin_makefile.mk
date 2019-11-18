ASFLAGS	:= -mregnames

# -T: use the linker script specified
# -wrap: wrap function
# -q: Leave relocation sections and contents in fully linked executables
 
LDFLAGS  +=     -T $(WUPSDIR)/wups.ld -Wl,-q