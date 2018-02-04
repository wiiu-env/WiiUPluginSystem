#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif
ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPRO")
endif

WUPSDIR := $(DEVKITPRO)/wups

# Rule to install wups.
PHONY += install
install :  wups.ld  wups_elf.ld
	$(addprefix $Qrm -rf ,$(wildcard $(WUPSDIR)))
	$Qmkdir $(WUPSDIR)
	$Qcp -r wups_include $(WUPSDIR)/include
	$Qcp -r wups.ld $(WUPSDIR)
	$Qcp -r wups_elf.ld $(WUPSDIR)
  
# Rule to install wups.
PHONY += uninstall
uninstall :
	$(addprefix $Qrm -rf ,$(wildcard $(WUPSDIR)))