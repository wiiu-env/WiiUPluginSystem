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
export PATH			:=	$(DEVKITPPC)/bin:$(PORTLIBS)/bin:$(PATH):$(DEVKITPPC)/tools/bin
export PORTLIBS		:=	$(DEVKITPRO)/portlibs/ppc

PREFIX	:=	powerpc-eabi-

export AS	:=	$(PREFIX)as
export CC	:=	$(PREFIX)gcc
export CXX	:=	$(PREFIX)g++
export AR	:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy

print-%  : ; @echo $* = $($*)

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET    :=  wiiupluginloader
BUILD     :=  build
BUILD_DBG	:=  $(TARGET)_dbg
SOURCES   :=  src/common \
              src/libelf \
              src/menu/content \
              src/menu \
              src/mymemory \
              src/mykernel \
              src/myutils \
              src/patcher \
              src/plugin \
              src/resources \
              src/settings \
              src/

DATA		:=  data/images \
                data/sounds \
                data/fonts \

INCLUDES  :=  src/libelf \
              src/

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	:=  -std=gnu11 -mrvl -mcpu=750 -meabi -mhard-float -ffast-math \
		    -O0 -D__wiiu__ -Wall -Wextra -Wno-unused-parameter -Wno-strict-aliasing -D_GNU_SOURCE $(INCLUDE)
CXXFLAGS := -std=gnu++11 -mrvl -mcpu=750 -meabi -mhard-float -ffast-math \
		    -O0 -D__wiiu__ -Wall -Wextra -Wno-unused-parameter -Wno-strict-aliasing -D_GNU_SOURCE $(INCLUDE)

ifeq ($(DO_LOGGING), 1)
   CFLAGS += -D__LOGGING__
   CXXFLAGS += -D__LOGGING__
endif

ASFLAGS	:= -mregnames
LDFLAGS	:= -nostartfiles -Wl,-Map,$(notdir $@).map,-wrap,malloc,-wrap,free,-wrap,memalign,-wrap,calloc,-wrap,realloc,-wrap,malloc_usable_size,-wrap,_malloc_r,-wrap,_free_r,-wrap,_realloc_r,-wrap,_calloc_r,-wrap,_memalign_r,-wrap,_malloc_usable_size_r,-wrap,valloc,-wrap,_valloc_r,-wrap,_pvalloc_r,--gc-sections

#---------------------------------------------------------------------------------
Q := @
MAKEFLAGS += --no-print-directory
#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lgui -lm -lgcc -lfat -lntfs -liosuhax -lutils -ldynamiclibs  -lfreetype -lgd -lpng -ljpeg -lz  -lmad -lvorbisidec

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(CURDIR)	\
			$(DEVKITPPC)/lib  \
			$(DEVKITPRO)/wups  \
			$(DEVKITPPC)/lib/gcc/powerpc-eabi/4.8.2


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
FILELIST	:=	$(shell bash ./filelist.sh)
LANGUAGES	:=	$(shell bash ./updatelang.sh)
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
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o) \
					$(PNGFILES:.png=.png.o) $(addsuffix .o,$(BINFILES))

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                    -I$(PORTLIBS)/include -I$(CURDIR)/$(BUILD) \
					-I$(PORTLIBS)/include/libutils \
                    -I$(PORTLIBS)/include/freetype2 -I$(PORTLIBS)/include/libgui


#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					-L$(PORTLIBS)/lib

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean install

#---------------------------------------------------------------------------------
$(BUILD): $(CURDIR)/src/mocha/ios_kernel/ios_kernel.bin.h
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

$(CURDIR)/src/mocha/ios_kernel/ios_kernel.bin.h: $(CURDIR)/src/mocha/ios_usb/ios_usb.bin.h $(CURDIR)/src/mocha/ios_mcp/ios_mcp.bin.h $(CURDIR)/src/mocha/ios_fs/ios_fs.bin.h $(CURDIR)/src/mocha/ios_bsp/ios_bsp.bin.h $(CURDIR)/src/mocha/ios_acp/ios_acp.bin.h
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_kernel -f  $(CURDIR)/src/mocha/ios_kernel/Makefile

$(CURDIR)/src/mocha/ios_usb/ios_usb.bin.h:
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_usb -f  $(CURDIR)/src/mocha/ios_usb/Makefile

$(CURDIR)/src/mocha/ios_fs/ios_fs.bin.h:
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_fs -f  $(CURDIR)/src/mocha/ios_fs/Makefile

$(CURDIR)/src/mocha/ios_bsp/ios_bsp.bin.h:
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_bsp -f  $(CURDIR)/src/mocha/ios_bsp/Makefile

$(CURDIR)/src/mocha/ios_mcp/ios_mcp.bin.h:
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_mcp -f  $(CURDIR)/src/mocha/ios_mcp/Makefile

$(CURDIR)/src/mocha/ios_acp/ios_acp.bin.h:
	@$(MAKE) -j1 --no-print-directory -C $(CURDIR)/src/mocha/ios_acp -f  $(CURDIR)/src/mocha/ios_acp/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).bin $(BUILD_DBG).elf
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_kernel -f  $(CURDIR)/src/mocha/ios_kernel/Makefile clean
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_usb -f  $(CURDIR)/src/mocha/ios_usb/Makefile clean
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_fs -f  $(CURDIR)/src/mocha/ios_fs/Makefile clean
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_bsp -f  $(CURDIR)/src/mocha/ios_bsp/Makefile clean
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_mcp -f  $(CURDIR)/src/mocha/ios_mcp/Makefile clean
	@$(MAKE) --no-print-directory -C $(CURDIR)/src/mocha/ios_acp -f $(CURDIR)/src/mocha/ios_acp/Makefile clean

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).elf:  $(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .jpg extension
#---------------------------------------------------------------------------------
%.elf: link.ld $(OFILES)
	@echo "linking ... $(TARGET).elf"
	$(Q)$(LD) -n -T $^ $(LDFLAGS) -o ../$(BUILD_DBG).elf  $(LIBPATHS) $(LIBS)
	$(Q)$(OBJCOPY) -S -R .comment -R .gnu.attributes ../$(BUILD_DBG).elf $@

#---------------------------------------------------------------------------------
%.a:
#---------------------------------------------------------------------------------
	@echo $(notdir $@)
	@rm -f $@
	@$(AR) -rc $@ $^

#---------------------------------------------------------------------------------
%.o: %.cpp
	@echo $(notdir $<)
	@$(CXX) -MMD -MP -MF $(DEPSDIR)/$*.d $(CXXFLAGS) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.c
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d $(CFLAGS) -c $< -o $@ $(ERROR_FILTER)

#---------------------------------------------------------------------------------
%.o: %.S
	@echo $(notdir $<)
	@$(CC) -MMD -MP -MF $(DEPSDIR)/$*.d -x assembler-with-cpp $(ASFLAGS) -c $< -o $@ $(ERROR_FILTER)
	
#---------------------------------------------------------------------------------
%.o: %.s
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

#---------------------------------------------------------------------------------
%.tga.o : %.tga
	@echo $(notdir $<)
	@bin2s -a 32 $< | $(AS) -o $(@)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
