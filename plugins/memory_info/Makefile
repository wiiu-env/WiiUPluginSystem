#   You probably never need to adjust this Makefile.
#   All changes can be done in the makefile.mk

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
ifeq ($(strip $(WUT_ROOT)),)
$(error "Please set WUT_ROOT in your environment. export WUT_ROOT=<path to>wut)
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

ifeq ($(notdir $(CURDIR)),$(BUILD))
include ../makefile.mk
else
include makefile.mk
endif

include $(WUPSDIR)/plugin_makefile.mk

#MAP ?= $(TARGET:.mod=.map)

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
# -fno-common: stop common variables which the loader can't understand
# -msdata-none: do not use r2 or r13 as small data areas
# -memb: enable embedded application specific compilation
# -ffunction-sections: split up functions so linker can garbage collect
# -fdata-sections: split up data so linker can garbage collect
COMMON_CFLAGS	+= -Os -Wall -DGEKKO_U -D__wiiu__ -mrvl -mcpu=750 -meabi -mhard-float -fno-common -msdata=none -memb -ffunction-sections -fdata-sections


# -x c: compile as c code
# -std=c11: use the c11 standard
CFLAGS	  		+= $(COMMON_CFLAGS) -x c -std=gnu11

# -x c: compile as c++ code
# -std=gnu++11: use the c++11 standard
CXXFLAGS  		+= $(COMMON_CFLAGS) -x c++ -std=gnu++11

ifeq ($(DO_LOGGING), 1)
   CFLAGS += -D__LOGGING__
   CXXFLAGS += -D__LOGGING__
endif
			
ASFLAGS	+= 

LDFLAG_COMMON       += 

ifeq ($(WRAP_MALLOC), 1)
   LDFLAG_COMMON += -Wl,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size,-wrap,_malloc_r,-wrap,_free_r,-wrap,_realloc_r,-wrap,_calloc_r,-wrap,_memalign_r,-wrap,_malloc_usable_size_r
endif

LDFLAGS_MOD     += $(LD_FLAGS_MOD)
LDFLAGS_ELF     += $(LD_FLAGS_ELF)

#---------------------------------------------------------------------------------
Q := @
MAKEFLAGS += --no-print-directory
#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
ALL_LIBS    := $(LIBS)
#  

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	+=  $(WUT_ROOT)

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
export INCLUDE_FULL	+=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                    $(EXTERNAL_INCLUDE)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS_FULL +=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                        $(EXTERNAL_LIBPATHS)
                    

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
	@$(LD_MOD) ../$(BUILD)/output.elf $(LDFLAG_COMMON) $(LD_FLAGS_MOD)  $(ALL_LIBS) $(LIBPATHS_FULL) -o check_linking.elf	
	@echo "linking ..." $@
	@$(LD_MOD) ../$(BUILD)/output.elf $(LDFLAG_COMMON) $(LDFLAGS_MOD) $(ALL_LIBS) $(LIBPATHS_FULL) -o $@ 

# Rule to make the module file.
output.elf : $(OFILES)
	@echo "linking ... output.elf"
	@$(LD) $(OFILES) $(LDFLAGS_ELF)  $(ALL_LIBS) $(LIBPATHS_FULL) -o $@ 
  
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
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) $(INCLUDE_FULL)  -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) $(INCLUDE_FULL)  -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.S
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(INCLUDE_FULL) -c $< -o $@ $(ERROR_FILTER)

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
