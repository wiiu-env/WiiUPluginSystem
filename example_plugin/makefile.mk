###############################################################################
# Source files

# The source files to compile.
SRC      := src/main.c 
            
# Include directories
INC_DIRS := src
# Library directories
LIB_DIRS :=
# The names of libraries to use.
LIBS     :=
# The output directory for compiled results.
BIN      := bin
# The name of the output file to generate.
TARGET 	 := $(BIN)/$(notdir $(CURDIR)).mod
# C compiler flags
CFLAGS   :=
