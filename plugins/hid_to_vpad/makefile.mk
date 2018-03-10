# Compiling the projects with libutils logging code?
DO_LOGGING          := 1

# Target filename
TARGET              := $(notdir $(CURDIR)).mod

# Source directories
SOURCES             := src

# Data directories
DATA                :=	

# Include directories
INCLUDES            := src

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
# Extra C compiler flags
CFLAGS              := 
# Extra C++ compiler flags
CXXFLAGS            := 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS             := $(WUPSDIR) $(PORTLIBS)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS                := -lwups -lcontrollerpatcher -lutils -ldynamiclibs

#---------------------------------------------------------------------------------
# Will be added to the final lib paths
# example: 
# -L$C:/library1/lib
#---------------------------------------------------------------------------------
EXTERNAL_LIBPATHS   := 
                        
#---------------------------------------------------------------------------------
# Will be added to the final include paths
# -IC:/library1/include
#---------------------------------------------------------------------------------
EXTERNAL_INCLUDE    :=  -I$(PORTLIBS)/include/libutils \
                        -I$(WUPSDIR)/include
