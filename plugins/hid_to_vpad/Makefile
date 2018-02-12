DO_LOGGING := 1

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
export PATH			:=	$(DEVKITPPC)/bin:$(PORTLIBS)/bin:$(PATH)
export PORTLIBS :=	$(DEVKITPRO)/portlibs/ppc
export WUPSDIR  := $(DEVKITPRO)/wups
export GCC_VER  := $(shell $(DEVKITPPC)/bin/powerpc-eabi-gcc -dumpversion)

PREFIX	:=	powerpc-eabi-

export AS		:=	$(PREFIX)as
export CC		:=	$(PREFIX)gcc
export CXX		:=	$(PREFIX)g++
export LD		:=	$(PREFIX)ld
export AR		:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR)).mod
BUILD		:= 	build
SOURCES		:=	src \
              src/common \
              src/utils

DATA		:=	

INCLUDES	:=  src

MAP ?= $(TARGET:.mod=.map)

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

# -Os: optimise size
# -Wall: generate lots of warnings
# -DGEKKO_U: define the symbol GEKKO (used in some headers)
# -D__wiiu__: define the symbol __wii__ (used in some headers)
# -mrvl: enable wii/gamecube compilation
# -mcpu=750: enable processor specific compilation
# -meabi: enable eabi specific compilation
# -mhard-float: enable hardware floating point instructions
# -fshort-wchar: use 16 bit whcar_t type in keeping with Wii executables
# -fno-common: stop common variables which the loader can't understand
# -msdata-none: do not use r2 or r13 as small data areas
# -memb: enable embedded application specific compilation
# -ffunction-sections: split up functions so linker can garbage collect
# -fdata-sections: split up data so linker can garbage collect
COMMON_CFLAGS	+= -Os -Wall -DGEKKO_U -D__wiiu__ -mrvl -mcpu=750 -meabi -mhard-float -fshort-wchar -fno-common -msdata=none -memb -ffunction-sections -fdata-sections


# -x c: compile as c code
# -std=c11: use the c11 standard
CFLAGS	  		+= $(COMMON_CFLAGS) -x c -std=c11

# -x c: compile as c++ code
# -std=gnu++11: use the c++11 standard
CXXFLAGS  		+= $(COMMON_CFLAGS) -x c++ -std=gnu++11

ifeq ($(DO_LOGGING), 1)
   CFLAGS += -D__LOGGING__
   CXXFLAGS += -D__LOGGING__
endif
			
ASFLAGS	:= -mregnames
# --relocatable: make sure ld doesn't remove relocations wups will need
# -s: strip local symbols to speed linking
# -u: keep certain sections
# -wrap: wrap function
# --gc-sections: remove unneeded symbols
# -T: use the linker script specified (to force certain wups sections together)
# -Map: generate a map file

LDFLAG_COMMON += -u wups_load -u wups_meta -u wups_hooks -T $(WUPSDIR)/wups.ld \
				 -Wl,-Map,$(notdir $@).map,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size,-wrap,_malloc_r,-wrap,_free_r,-wrap,_realloc_r,-wrap,_calloc_r,-wrap,_memalign_r,-wrap,_malloc_usable_size_r,--gc-sections

LDFLAGS_MOD  +=  	$(LDFLAG_COMMON),--relocatable
LDFLAGS_ELF  +=  	--relocatable -s -T $(WUPSDIR)/wups_elf.ld

#---------------------------------------------------------------------------------
Q := @
MAKEFLAGS += --no-print-directory
#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lcontrollerpatcher -lutils -ldynamiclibs
#  

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(DEVKITPPC)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export PROJECTDIR := $(CURDIR)
export OUTPUT	:=	$(CURDIR)/$(TARGETDIR)/$(TARGET)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
TTFFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.ttf)))
PNGFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.png)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD_MOD	:=	$(CC)
else
	export LD_MOD	:=	$(CXX)
endif

export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o) \
					$(PNGFILES:.png=.png.o) $(addsuffix .o,$(BINFILES))

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD) -I$(PORTLIBS)/include \
					-I$(PORTLIBS)/include/libutils -I$(WUPSDIR)/include

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                    -L$(PORTLIBS)/lib 

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean install

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
	
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).bin $(BUILD_DBG).elf	$(OUTPUT)

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

###############################################################################
# Rule to make everything.
PHONY += all

all : $(OUTPUT)
###############################################################################
# Special build rules

# Rule to make the module file.
$(OUTPUT) : output.elf
	@echo "checking for missing symbols ..."
	@$(LD_MOD) ../$(BUILD)/output.elf $(LDFLAG_COMMON) $(LIBS) $(LIBPATHS) -o check_linking.elf	
	@echo "linking ..." $@
	@$(LD_MOD) ../$(BUILD)/output.elf $(LDFLAGS_MOD) $(LIBS) $(LIBPATHS) -o $@ 

# Rule to make the module file.
output.elf : $(OFILES)
	@echo "linking ... output.elf"
	
	
	@$(LD) $(OFILES) $(LDFLAGS_ELF)  $(LIBS) $(LIBPATHS) -o $@ 
  
###############################################################################
# Standard build rules
#---------------------------------------------------------------------------------
%.a:
#---------------------------------------------------------------------------------
	@echo $(notdir $@)
	@rm -f $@
	@$(AR) -rc $@ $^

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(INCLUDE)  -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) $(INCLUDE)  -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.S
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.png.o : %.png
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.jpg.o : %.jpg
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.ttf.o : %.ttf
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.bin.o : %.bin
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.wav.o : %.wav
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.mp3.o : %.mp3
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

#---------------------------------------------------------------------------------
%.ogg.o : %.ogg
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

###############################################################################
# Assembly listing rules

# Rule to make assembly listing.
PHONY += list
list  : $(LIST)

# Rule to make the listing file.
%.list : $(TARGET)
	$(LOG)
	-$Qmkdir -p $(dir $@)
	$Q$(OBJDUMP) -d $< > $@

###############################################################################
# Clean rule

# Rule to clean files.
PHONY += clean
clean : 
	$Qrm -rf $(wildcard $(BUILD) $(BIN))

###############################################################################
# Phony targets

.PHONY : $(PHONY)
  
-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
