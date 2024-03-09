# Makefile: Used for compiling the program automatically 
# ***********************************************************
# v1 - First implementation of a Makefile to compile my code. Done following the guide in https://stackoverflow.com/questions/30573481/how-to-write-a-makefile-with-separate-source-and-header-directories.
# v2 - Second implementation, added compatibility with C++.
#
# TODO: Understand the execution process of Makefiles.

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
EXE := $(BIN_DIR)/abc # Executable is named abc.exe
SRC := $(wildcard $(SRC_DIR)/*.cpp) # Via /* I get every .c file from the SRC_DIR.

# In the following code, I list every .c file inside of the src directory and replace them with the
# corresponding object file. % is a wildcard that serves as a placeholder. Essentialy: 
# - Search for PATTERN: $(SRC_DIR)/%.c inside of TEXT: $(SRC)
# - Replace PATTERN in TEXT for REPLACEMENT: $(OBJ_DIR)/%.o
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) 

# C Pre-processor flags:
# -I : Specifies directories where the preprocessor should look for header files. In this case,
# it's gonna search for header files inside of the include directory.
# -MMD : Generates the header dependency file (.d) for the Makefile.
# -MP : Generates .Phony targets for each header file. (?)
CPPFLAGS := -Iinclude -MMD -MP # C Pre-processor flags
CXXFLAGS := -Wall -Wextra
CXX := g++
LDFLAGS := -Llib # -L : Linker flag
LDLIBS := #-lm # Since I need no libraries, this is left empty.

.PHONY: all
all: $(EXE) # Default target on Make depends on the existence of my executable.

# ------------------- LINKING PHASE ---------------------
# I'll be linking every .o file (and .lib file) in order to create my executable.
#
# Note $(BIN_DIR) is an order only pre-requisite. Meaning it'll be created in case it doesn't
# exist, and it WON'T recompile our file if it's modified.
# $@ is the name of the target being generated -> $(EXE)
# $< the first prerequisite -> $(OBJ)
$(EXE): $(OBJ) | $(BIN_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# ------------------- COMPILATION PHASE ---------------------
# Here I'll be compiling the .o files given the .c files.
# 
# Order only pre-requisite: $(OBJ_DIR)
# In this case, via the /%.o and /%.cpp inclusions I essentially state that every .o file
# requires its corresponding .cpp file.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# This is used whenever both directories required aren't present. It creates them.
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# ------------------- CLEAN-UP PHASE ---------------------
# Here we define a clean command that removes files inside of our bin and obj directories.
# Note $(RM) is a make implicit variable which becomes: rm -f
.PHONY: clean
clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
