DO_LOGGING := 0

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC)
endif

export PATH			:=	$(DEVKITPPC)/bin:$(PORTLIBS)/bin:$(PATH)
export PORTLIBS		:=	$(DEVKITPRO)/portlibs/ppc

PREFIX	:=	powerpc-eabi-

export AS	:=	$(PREFIX)as
export CC	:=	$(PREFIX)gcc
export CXX	:=	$(PREFIX)g++
export AR	:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy

include $(DEVKITPPC)/base_rules

#---------------------------------------------------------------------------------
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary files
# LIBDIR is where the built library will be placed
# all directories are relative to this makefile
#---------------------------------------------------------------------------------
BUILD		?=	release
SOURCES		:=	src 

INCLUDES	:=	src \
				wups_include
DATA		:=
LIB		:=	lib

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	= -g -Os -Wall -D__wiiu__ -D_GNU_SOURCE $(MACHDEP) $(INCLUDE)
CXXFLAGS	=	$(CFLAGS)

ifeq ($(DO_LOGGING), 1)
   CFLAGS += -D__LOGGING__
   CXXFLAGS += -D__LOGGING__
endif

ASFLAGS	:=	-mregnames

export WIIUBIN	:=	$(LIB)/libwups.a

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=  

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=  

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export TOPDIR ?= $(CURDIR)/..
export DEPSDIR := $(CURDIR)/$(BUILD)

export INCLUDEDIR := $(PORTLIBS)/include/

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))


export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o) $(sFILES:.s=.o)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
            -I$(CURDIR)/$(BUILD) -I$(PORTLIBS)/include

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr debug release $(LIB) include

all: $(WIIUBIN)

WUPSDIR := $(DEVKITPRO)/wups

# Rule to install wups.
PHONY += install
install :  wups.ld
	$(addprefix $Qrm -rf ,$(wildcard $(WUPSDIR)))
	$Qmkdir $(WUPSDIR)
	$Qmkdir $(WUPSDIR)/lib/
	$Qcp -r wups_include $(WUPSDIR)/include
	$Qcp -r wups.ld $(WUPSDIR)
	$Qcp -r plugin_makefile.mk $(WUPSDIR)
	@cp $(BUILD)/lib/libwups.a $(WUPSDIR)/lib/

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(WIIUBIN)	:	$(OFILES) $(LIB)
	@rm -f "$(WIIUBIN)"
	@$(AR) rcs "$(WIIUBIN)" $(OFILES) 
	@echo built ... $(notdir $@)
	
$(LIB):
	mkdir $(LIB)

-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------

