TOPDIR ?= $(CURDIR)
include $(TOPDIR)/share/wups_rules

export WUPS_MAJOR	:=	0
export WUPS_MINOR	:=	7
export WUPS_PATCH	:=	1

VERSION	:=	$(WUPS_MAJOR).$(WUPS_MINOR).$(WUPS_PATCH)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# DATA is a list of directories containing data files
# INCLUDES is a list of directories containing header files
#---------------------------------------------------------------------------------
TARGET		:=	wups
#BUILD		:=	build
SOURCES		:=	libraries/libwups/ \
				libraries/libwups/utils
DATA		:=	data
INCLUDES	:=	include

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	:=	-g -Wall -Werror -save-temps \
			-ffunction-sections -fdata-sections \
			-fno-exceptions -fno-rtti \
			$(MACHDEP) \
			$(BUILD_CFLAGS)

CFLAGS	+=	$(INCLUDE) -D__WIIU__ -D__WUT__ -D__WUPS__

CXXFLAGS	:= $(CFLAGS) -std=gnu++20

ASFLAGS	:=	-g $(MACHDEP)

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

export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
DEFFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.def)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(DEFFILES:.def=.o) $(SFILES:.s=.o) $(CFILES:.c=.o) $(CPPFILES:.cpp=.o)
export OFILES 	:=	$(OFILES_BIN) $(OFILES_SRC)
export HFILES	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I.

.PHONY: all dist-bin dist-src dist install clean

#---------------------------------------------------------------------------------
all: lib/libwups.a lib/libwupsd.a

dist-bin: all
	@tar --exclude=*~ -cjf wups-$(VERSION).tar.bz2 include lib share

dist-src:
	@tar --exclude=*~ -cjf wups-src-$(VERSION).tar.bz2 include libraries share Makefile

dist: dist-src dist-bin

install: dist-bin
	mkdir -p $(DESTDIR)$(DEVKITPRO)/wups
	bzip2 -cd wups-$(VERSION).tar.bz2 | tar -xf - -C $(DESTDIR)$(DEVKITPRO)/wups

lib:
	@[ -d $@ ] || mkdir -p $@

release:
	@[ -d $@ ] || mkdir -p $@

debug:
	@[ -d $@ ] || mkdir -p $@

lib/libwups.a : lib release $(SOURCES) $(INCLUDES)
	@$(MAKE) BUILD=release OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DNDEBUG=1 -O2" \
	DEPSDIR=$(CURDIR)/release \
	--no-print-directory -C release \
	-f $(CURDIR)/Makefile

lib/libwupsd.a : lib debug $(SOURCES) $(INCLUDES)
	@$(MAKE) BUILD=debug OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DDEBUG=1 -Og" \
	DEPSDIR=$(CURDIR)/debug \
	--no-print-directory -C debug \
	-f $(CURDIR)/Makefile

# Cancel .a rule from devkitPPC/base_rules
%.a:

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -rf release debug lib

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)

$(OFILES_SRC)	: $(HFILES)

#---------------------------------------------------------------------------------
%_bin.h %.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------