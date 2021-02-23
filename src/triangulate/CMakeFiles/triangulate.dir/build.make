# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /data/data/com.termux/files/usr/bin/sh

# The CMake executable.
CMAKE_COMMAND = /data/data/com.termux/files/usr/bin/cmake

# The command to remove a file.
RM = /data/data/com.termux/files/usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/data/com.termux/files/home/hagen-engine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/data/com.termux/files/home/hagen-engine

# Include any dependencies generated for this target.
include src/triangulate/CMakeFiles/triangulate.dir/depend.make

# Include the progress variables for this target.
include src/triangulate/CMakeFiles/triangulate.dir/progress.make

# Include the compile flags for this target's objects.
include src/triangulate/CMakeFiles/triangulate.dir/flags.make

src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.o: src/triangulate/CMakeFiles/triangulate.dir/flags.make
src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.o: src/triangulate/triangulate.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/data/com.termux/files/home/hagen-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.o"
	cd /data/data/com.termux/files/home/hagen-engine/src/triangulate && /data/data/com.termux/files/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/triangulate.dir/triangulate.cpp.o -c /data/data/com.termux/files/home/hagen-engine/src/triangulate/triangulate.cpp

src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/triangulate.dir/triangulate.cpp.i"
	cd /data/data/com.termux/files/home/hagen-engine/src/triangulate && /data/data/com.termux/files/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/data/com.termux/files/home/hagen-engine/src/triangulate/triangulate.cpp > CMakeFiles/triangulate.dir/triangulate.cpp.i

src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/triangulate.dir/triangulate.cpp.s"
	cd /data/data/com.termux/files/home/hagen-engine/src/triangulate && /data/data/com.termux/files/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/data/com.termux/files/home/hagen-engine/src/triangulate/triangulate.cpp -o CMakeFiles/triangulate.dir/triangulate.cpp.s

# Object files for target triangulate
triangulate_OBJECTS = \
"CMakeFiles/triangulate.dir/triangulate.cpp.o"

# External object files for target triangulate
triangulate_EXTERNAL_OBJECTS =

bin/libtriangulate.so: src/triangulate/CMakeFiles/triangulate.dir/triangulate.cpp.o
bin/libtriangulate.so: src/triangulate/CMakeFiles/triangulate.dir/build.make
bin/libtriangulate.so: src/triangulate/CMakeFiles/triangulate.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/data/com.termux/files/home/hagen-engine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../bin/libtriangulate.so"
	cd /data/data/com.termux/files/home/hagen-engine/src/triangulate && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/triangulate.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/triangulate/CMakeFiles/triangulate.dir/build: bin/libtriangulate.so

.PHONY : src/triangulate/CMakeFiles/triangulate.dir/build

src/triangulate/CMakeFiles/triangulate.dir/clean:
	cd /data/data/com.termux/files/home/hagen-engine/src/triangulate && $(CMAKE_COMMAND) -P CMakeFiles/triangulate.dir/cmake_clean.cmake
.PHONY : src/triangulate/CMakeFiles/triangulate.dir/clean

src/triangulate/CMakeFiles/triangulate.dir/depend:
	cd /data/data/com.termux/files/home/hagen-engine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/data/com.termux/files/home/hagen-engine /data/data/com.termux/files/home/hagen-engine/src/triangulate /data/data/com.termux/files/home/hagen-engine /data/data/com.termux/files/home/hagen-engine/src/triangulate /data/data/com.termux/files/home/hagen-engine/src/triangulate/CMakeFiles/triangulate.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/triangulate/CMakeFiles/triangulate.dir/depend

