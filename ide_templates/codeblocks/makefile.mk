# Target filename
TARGET              := $(notdir $(CURDIR)).mod

# Source directories
SOURCES             := src

# Data directories
DATA                :=	

# Include directories
INCLUDES            := src

#---------------------------------------------------------------------------------
# options for code generation and linking
#---------------------------------------------------------------------------------
# Extra C AND C++ compiler flags
COMMON_CFLAGS       := 
# Extra C compiler flags
CFLAGS              :=
# Extra C++ compiler flags
CXXFLAGS            :=
# Extra linking flags for all linking steps
LDFLAGS             := 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS             := $(WUPSDIR) $(WUT_ROOT) $(PORTLIBS)

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS                := -lwups -lwut 

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
EXTERNAL_INCLUDE    := 
