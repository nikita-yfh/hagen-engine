# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikita/c++/hagen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikita/c++/hagen

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/nikita/c++/hagen/CMakeFiles /home/nikita/c++/hagen//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/nikita/c++/hagen/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named level-editor

# Build rule for target.
level-editor: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 level-editor
.PHONY : level-editor

# fast build rule for target.
level-editor/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/build
.PHONY : level-editor/fast

#=============================================================================
# Target rules for targets named engine

# Build rule for target.
engine: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 engine
.PHONY : engine

# fast build rule for target.
engine/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/build
.PHONY : engine/fast

#=============================================================================
# Target rules for targets named box2d

# Build rule for target.
box2d: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 box2d
.PHONY : box2d

# fast build rule for target.
box2d/fast:
	$(MAKE) $(MAKESILENT) -f src/box2d/CMakeFiles/box2d.dir/build.make src/box2d/CMakeFiles/box2d.dir/build
.PHONY : box2d/fast

#=============================================================================
# Target rules for targets named fontcache

# Build rule for target.
fontcache: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 fontcache
.PHONY : fontcache

# fast build rule for target.
fontcache/fast:
	$(MAKE) $(MAKESILENT) -f src/SDL_FontCache/CMakeFiles/fontcache.dir/build.make src/SDL_FontCache/CMakeFiles/fontcache.dir/build
.PHONY : fontcache/fast

#=============================================================================
# Target rules for targets named xml

# Build rule for target.
xml: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 xml
.PHONY : xml

# fast build rule for target.
xml/fast:
	$(MAKE) $(MAKESILENT) -f src/XMLParser/CMakeFiles/xml.dir/build.make src/XMLParser/CMakeFiles/xml.dir/build
.PHONY : xml/fast

#=============================================================================
# Target rules for targets named gfx

# Build rule for target.
gfx: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 gfx
.PHONY : gfx

# fast build rule for target.
gfx/fast:
	$(MAKE) $(MAKESILENT) -f src/SDL_GFX/CMakeFiles/gfx.dir/build.make src/SDL_GFX/CMakeFiles/gfx.dir/build
.PHONY : gfx/fast

#=============================================================================
# Target rules for targets named triangulate

# Build rule for target.
triangulate: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 triangulate
.PHONY : triangulate

# fast build rule for target.
triangulate/fast:
	$(MAKE) $(MAKESILENT) -f src/triangulate/CMakeFiles/triangulate.dir/build.make src/triangulate/CMakeFiles/triangulate.dir/build
.PHONY : triangulate/fast

#=============================================================================
# Target rules for targets named SDL_gpu_shared

# Build rule for target.
SDL_gpu_shared: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 SDL_gpu_shared
.PHONY : SDL_gpu_shared

# fast build rule for target.
SDL_gpu_shared/fast:
	$(MAKE) $(MAKESILENT) -f src/sdl-gpu/src/CMakeFiles/SDL_gpu_shared.dir/build.make src/sdl-gpu/src/CMakeFiles/SDL_gpu_shared.dir/build
.PHONY : SDL_gpu_shared/fast

src/engine/camera.o: src/engine/camera.cpp.o

.PHONY : src/engine/camera.o

# target to build an object file
src/engine/camera.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/camera.cpp.o
.PHONY : src/engine/camera.cpp.o

src/engine/camera.i: src/engine/camera.cpp.i

.PHONY : src/engine/camera.i

# target to preprocess a source file
src/engine/camera.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/camera.cpp.i
.PHONY : src/engine/camera.cpp.i

src/engine/camera.s: src/engine/camera.cpp.s

.PHONY : src/engine/camera.s

# target to generate assembly for a file
src/engine/camera.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/camera.cpp.s
.PHONY : src/engine/camera.cpp.s

src/engine/entity.o: src/engine/entity.cpp.o

.PHONY : src/engine/entity.o

# target to build an object file
src/engine/entity.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/entity.cpp.o
.PHONY : src/engine/entity.cpp.o

src/engine/entity.i: src/engine/entity.cpp.i

.PHONY : src/engine/entity.i

# target to preprocess a source file
src/engine/entity.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/entity.cpp.i
.PHONY : src/engine/entity.cpp.i

src/engine/entity.s: src/engine/entity.cpp.s

.PHONY : src/engine/entity.s

# target to generate assembly for a file
src/engine/entity.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/entity.cpp.s
.PHONY : src/engine/entity.cpp.s

src/engine/game.o: src/engine/game.cpp.o

.PHONY : src/engine/game.o

# target to build an object file
src/engine/game.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/game.cpp.o
.PHONY : src/engine/game.cpp.o

src/engine/game.i: src/engine/game.cpp.i

.PHONY : src/engine/game.i

# target to preprocess a source file
src/engine/game.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/game.cpp.i
.PHONY : src/engine/game.cpp.i

src/engine/game.s: src/engine/game.cpp.s

.PHONY : src/engine/game.s

# target to generate assembly for a file
src/engine/game.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/game.cpp.s
.PHONY : src/engine/game.cpp.s

src/engine/level.o: src/engine/level.cpp.o

.PHONY : src/engine/level.o

# target to build an object file
src/engine/level.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/level.cpp.o
.PHONY : src/engine/level.cpp.o

src/engine/level.i: src/engine/level.cpp.i

.PHONY : src/engine/level.i

# target to preprocess a source file
src/engine/level.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/level.cpp.i
.PHONY : src/engine/level.cpp.i

src/engine/level.s: src/engine/level.cpp.s

.PHONY : src/engine/level.s

# target to generate assembly for a file
src/engine/level.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/level.cpp.s
.PHONY : src/engine/level.cpp.s

src/engine/lua.o: src/engine/lua.cpp.o

.PHONY : src/engine/lua.o

# target to build an object file
src/engine/lua.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/lua.cpp.o
.PHONY : src/engine/lua.cpp.o

src/engine/lua.i: src/engine/lua.cpp.i

.PHONY : src/engine/lua.i

# target to preprocess a source file
src/engine/lua.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/lua.cpp.i
.PHONY : src/engine/lua.cpp.i

src/engine/lua.s: src/engine/lua.cpp.s

.PHONY : src/engine/lua.s

# target to generate assembly for a file
src/engine/lua.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/lua.cpp.s
.PHONY : src/engine/lua.cpp.s

src/engine/main.o: src/engine/main.cpp.o

.PHONY : src/engine/main.o

# target to build an object file
src/engine/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/main.cpp.o
.PHONY : src/engine/main.cpp.o

src/engine/main.i: src/engine/main.cpp.i

.PHONY : src/engine/main.i

# target to preprocess a source file
src/engine/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/main.cpp.i
.PHONY : src/engine/main.cpp.i

src/engine/main.s: src/engine/main.cpp.s

.PHONY : src/engine/main.s

# target to generate assembly for a file
src/engine/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/main.cpp.s
.PHONY : src/engine/main.cpp.s

src/engine/physic.o: src/engine/physic.cpp.o

.PHONY : src/engine/physic.o

# target to build an object file
src/engine/physic.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/physic.cpp.o
.PHONY : src/engine/physic.cpp.o

src/engine/physic.i: src/engine/physic.cpp.i

.PHONY : src/engine/physic.i

# target to preprocess a source file
src/engine/physic.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/physic.cpp.i
.PHONY : src/engine/physic.cpp.i

src/engine/physic.s: src/engine/physic.cpp.s

.PHONY : src/engine/physic.s

# target to generate assembly for a file
src/engine/physic.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/physic.cpp.s
.PHONY : src/engine/physic.cpp.s

src/engine/player.o: src/engine/player.cpp.o

.PHONY : src/engine/player.o

# target to build an object file
src/engine/player.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/player.cpp.o
.PHONY : src/engine/player.cpp.o

src/engine/player.i: src/engine/player.cpp.i

.PHONY : src/engine/player.i

# target to preprocess a source file
src/engine/player.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/player.cpp.i
.PHONY : src/engine/player.cpp.i

src/engine/player.s: src/engine/player.cpp.s

.PHONY : src/engine/player.s

# target to generate assembly for a file
src/engine/player.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/player.cpp.s
.PHONY : src/engine/player.cpp.s

src/engine/render.o: src/engine/render.cpp.o

.PHONY : src/engine/render.o

# target to build an object file
src/engine/render.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/render.cpp.o
.PHONY : src/engine/render.cpp.o

src/engine/render.i: src/engine/render.cpp.i

.PHONY : src/engine/render.i

# target to preprocess a source file
src/engine/render.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/render.cpp.i
.PHONY : src/engine/render.cpp.i

src/engine/render.s: src/engine/render.cpp.s

.PHONY : src/engine/render.s

# target to generate assembly for a file
src/engine/render.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/render.cpp.s
.PHONY : src/engine/render.cpp.s

src/engine/sdl.o: src/engine/sdl.cpp.o

.PHONY : src/engine/sdl.o

# target to build an object file
src/engine/sdl.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/sdl.cpp.o
.PHONY : src/engine/sdl.cpp.o

src/engine/sdl.i: src/engine/sdl.cpp.i

.PHONY : src/engine/sdl.i

# target to preprocess a source file
src/engine/sdl.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/sdl.cpp.i
.PHONY : src/engine/sdl.cpp.i

src/engine/sdl.s: src/engine/sdl.cpp.s

.PHONY : src/engine/sdl.s

# target to generate assembly for a file
src/engine/sdl.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/sdl.cpp.s
.PHONY : src/engine/sdl.cpp.s

src/engine/shapes.o: src/engine/shapes.cpp.o

.PHONY : src/engine/shapes.o

# target to build an object file
src/engine/shapes.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/shapes.cpp.o
.PHONY : src/engine/shapes.cpp.o

src/engine/shapes.i: src/engine/shapes.cpp.i

.PHONY : src/engine/shapes.i

# target to preprocess a source file
src/engine/shapes.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/shapes.cpp.i
.PHONY : src/engine/shapes.cpp.i

src/engine/shapes.s: src/engine/shapes.cpp.s

.PHONY : src/engine/shapes.s

# target to generate assembly for a file
src/engine/shapes.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/shapes.cpp.s
.PHONY : src/engine/shapes.cpp.s

src/engine/utility.o: src/engine/utility.cpp.o

.PHONY : src/engine/utility.o

# target to build an object file
src/engine/utility.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/utility.cpp.o
.PHONY : src/engine/utility.cpp.o

src/engine/utility.i: src/engine/utility.cpp.i

.PHONY : src/engine/utility.i

# target to preprocess a source file
src/engine/utility.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/utility.cpp.i
.PHONY : src/engine/utility.cpp.i

src/engine/utility.s: src/engine/utility.cpp.s

.PHONY : src/engine/utility.s

# target to generate assembly for a file
src/engine/utility.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/engine.dir/build.make CMakeFiles/engine.dir/src/engine/utility.cpp.s
.PHONY : src/engine/utility.cpp.s

src/level-editor/body.o: src/level-editor/body.cpp.o

.PHONY : src/level-editor/body.o

# target to build an object file
src/level-editor/body.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/body.cpp.o
.PHONY : src/level-editor/body.cpp.o

src/level-editor/body.i: src/level-editor/body.cpp.i

.PHONY : src/level-editor/body.i

# target to preprocess a source file
src/level-editor/body.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/body.cpp.i
.PHONY : src/level-editor/body.cpp.i

src/level-editor/body.s: src/level-editor/body.cpp.s

.PHONY : src/level-editor/body.s

# target to generate assembly for a file
src/level-editor/body.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/body.cpp.s
.PHONY : src/level-editor/body.cpp.s

src/level-editor/editor.o: src/level-editor/editor.cpp.o

.PHONY : src/level-editor/editor.o

# target to build an object file
src/level-editor/editor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/editor.cpp.o
.PHONY : src/level-editor/editor.cpp.o

src/level-editor/editor.i: src/level-editor/editor.cpp.i

.PHONY : src/level-editor/editor.i

# target to preprocess a source file
src/level-editor/editor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/editor.cpp.i
.PHONY : src/level-editor/editor.cpp.i

src/level-editor/editor.s: src/level-editor/editor.cpp.s

.PHONY : src/level-editor/editor.s

# target to generate assembly for a file
src/level-editor/editor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/editor.cpp.s
.PHONY : src/level-editor/editor.cpp.s

src/level-editor/events.o: src/level-editor/events.cpp.o

.PHONY : src/level-editor/events.o

# target to build an object file
src/level-editor/events.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/events.cpp.o
.PHONY : src/level-editor/events.cpp.o

src/level-editor/events.i: src/level-editor/events.cpp.i

.PHONY : src/level-editor/events.i

# target to preprocess a source file
src/level-editor/events.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/events.cpp.i
.PHONY : src/level-editor/events.cpp.i

src/level-editor/events.s: src/level-editor/events.cpp.s

.PHONY : src/level-editor/events.s

# target to generate assembly for a file
src/level-editor/events.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/events.cpp.s
.PHONY : src/level-editor/events.cpp.s

src/level-editor/joint.o: src/level-editor/joint.cpp.o

.PHONY : src/level-editor/joint.o

# target to build an object file
src/level-editor/joint.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/joint.cpp.o
.PHONY : src/level-editor/joint.cpp.o

src/level-editor/joint.i: src/level-editor/joint.cpp.i

.PHONY : src/level-editor/joint.i

# target to preprocess a source file
src/level-editor/joint.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/joint.cpp.i
.PHONY : src/level-editor/joint.cpp.i

src/level-editor/joint.s: src/level-editor/joint.cpp.s

.PHONY : src/level-editor/joint.s

# target to generate assembly for a file
src/level-editor/joint.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/joint.cpp.s
.PHONY : src/level-editor/joint.cpp.s

src/level-editor/level.o: src/level-editor/level.cpp.o

.PHONY : src/level-editor/level.o

# target to build an object file
src/level-editor/level.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/level.cpp.o
.PHONY : src/level-editor/level.cpp.o

src/level-editor/level.i: src/level-editor/level.cpp.i

.PHONY : src/level-editor/level.i

# target to preprocess a source file
src/level-editor/level.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/level.cpp.i
.PHONY : src/level-editor/level.cpp.i

src/level-editor/level.s: src/level-editor/level.cpp.s

.PHONY : src/level-editor/level.s

# target to generate assembly for a file
src/level-editor/level.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/level.cpp.s
.PHONY : src/level-editor/level.cpp.s

src/level-editor/main.o: src/level-editor/main.cpp.o

.PHONY : src/level-editor/main.o

# target to build an object file
src/level-editor/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/main.cpp.o
.PHONY : src/level-editor/main.cpp.o

src/level-editor/main.i: src/level-editor/main.cpp.i

.PHONY : src/level-editor/main.i

# target to preprocess a source file
src/level-editor/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/main.cpp.i
.PHONY : src/level-editor/main.cpp.i

src/level-editor/main.s: src/level-editor/main.cpp.s

.PHONY : src/level-editor/main.s

# target to generate assembly for a file
src/level-editor/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/main.cpp.s
.PHONY : src/level-editor/main.cpp.s

src/level-editor/object.o: src/level-editor/object.cpp.o

.PHONY : src/level-editor/object.o

# target to build an object file
src/level-editor/object.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/object.cpp.o
.PHONY : src/level-editor/object.cpp.o

src/level-editor/object.i: src/level-editor/object.cpp.i

.PHONY : src/level-editor/object.i

# target to preprocess a source file
src/level-editor/object.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/object.cpp.i
.PHONY : src/level-editor/object.cpp.i

src/level-editor/object.s: src/level-editor/object.cpp.s

.PHONY : src/level-editor/object.s

# target to generate assembly for a file
src/level-editor/object.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/object.cpp.s
.PHONY : src/level-editor/object.cpp.s

src/level-editor/shape.o: src/level-editor/shape.cpp.o

.PHONY : src/level-editor/shape.o

# target to build an object file
src/level-editor/shape.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/shape.cpp.o
.PHONY : src/level-editor/shape.cpp.o

src/level-editor/shape.i: src/level-editor/shape.cpp.i

.PHONY : src/level-editor/shape.i

# target to preprocess a source file
src/level-editor/shape.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/shape.cpp.i
.PHONY : src/level-editor/shape.cpp.i

src/level-editor/shape.s: src/level-editor/shape.cpp.s

.PHONY : src/level-editor/shape.s

# target to generate assembly for a file
src/level-editor/shape.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/shape.cpp.s
.PHONY : src/level-editor/shape.cpp.s

src/level-editor/tree_view.o: src/level-editor/tree_view.cpp.o

.PHONY : src/level-editor/tree_view.o

# target to build an object file
src/level-editor/tree_view.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/tree_view.cpp.o
.PHONY : src/level-editor/tree_view.cpp.o

src/level-editor/tree_view.i: src/level-editor/tree_view.cpp.i

.PHONY : src/level-editor/tree_view.i

# target to preprocess a source file
src/level-editor/tree_view.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/tree_view.cpp.i
.PHONY : src/level-editor/tree_view.cpp.i

src/level-editor/tree_view.s: src/level-editor/tree_view.cpp.s

.PHONY : src/level-editor/tree_view.s

# target to generate assembly for a file
src/level-editor/tree_view.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/tree_view.cpp.s
.PHONY : src/level-editor/tree_view.cpp.s

src/level-editor/utils.o: src/level-editor/utils.cpp.o

.PHONY : src/level-editor/utils.o

# target to build an object file
src/level-editor/utils.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/utils.cpp.o
.PHONY : src/level-editor/utils.cpp.o

src/level-editor/utils.i: src/level-editor/utils.cpp.i

.PHONY : src/level-editor/utils.i

# target to preprocess a source file
src/level-editor/utils.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/utils.cpp.i
.PHONY : src/level-editor/utils.cpp.i

src/level-editor/utils.s: src/level-editor/utils.cpp.s

.PHONY : src/level-editor/utils.s

# target to generate assembly for a file
src/level-editor/utils.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/level-editor.dir/build.make CMakeFiles/level-editor.dir/src/level-editor/utils.cpp.s
.PHONY : src/level-editor/utils.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... SDL_gpu_shared"
	@echo "... box2d"
	@echo "... engine"
	@echo "... fontcache"
	@echo "... gfx"
	@echo "... level-editor"
	@echo "... triangulate"
	@echo "... xml"
	@echo "... src/engine/camera.o"
	@echo "... src/engine/camera.i"
	@echo "... src/engine/camera.s"
	@echo "... src/engine/entity.o"
	@echo "... src/engine/entity.i"
	@echo "... src/engine/entity.s"
	@echo "... src/engine/game.o"
	@echo "... src/engine/game.i"
	@echo "... src/engine/game.s"
	@echo "... src/engine/level.o"
	@echo "... src/engine/level.i"
	@echo "... src/engine/level.s"
	@echo "... src/engine/lua.o"
	@echo "... src/engine/lua.i"
	@echo "... src/engine/lua.s"
	@echo "... src/engine/main.o"
	@echo "... src/engine/main.i"
	@echo "... src/engine/main.s"
	@echo "... src/engine/physic.o"
	@echo "... src/engine/physic.i"
	@echo "... src/engine/physic.s"
	@echo "... src/engine/player.o"
	@echo "... src/engine/player.i"
	@echo "... src/engine/player.s"
	@echo "... src/engine/render.o"
	@echo "... src/engine/render.i"
	@echo "... src/engine/render.s"
	@echo "... src/engine/sdl.o"
	@echo "... src/engine/sdl.i"
	@echo "... src/engine/sdl.s"
	@echo "... src/engine/shapes.o"
	@echo "... src/engine/shapes.i"
	@echo "... src/engine/shapes.s"
	@echo "... src/engine/utility.o"
	@echo "... src/engine/utility.i"
	@echo "... src/engine/utility.s"
	@echo "... src/level-editor/body.o"
	@echo "... src/level-editor/body.i"
	@echo "... src/level-editor/body.s"
	@echo "... src/level-editor/editor.o"
	@echo "... src/level-editor/editor.i"
	@echo "... src/level-editor/editor.s"
	@echo "... src/level-editor/events.o"
	@echo "... src/level-editor/events.i"
	@echo "... src/level-editor/events.s"
	@echo "... src/level-editor/joint.o"
	@echo "... src/level-editor/joint.i"
	@echo "... src/level-editor/joint.s"
	@echo "... src/level-editor/level.o"
	@echo "... src/level-editor/level.i"
	@echo "... src/level-editor/level.s"
	@echo "... src/level-editor/main.o"
	@echo "... src/level-editor/main.i"
	@echo "... src/level-editor/main.s"
	@echo "... src/level-editor/object.o"
	@echo "... src/level-editor/object.i"
	@echo "... src/level-editor/object.s"
	@echo "... src/level-editor/shape.o"
	@echo "... src/level-editor/shape.i"
	@echo "... src/level-editor/shape.s"
	@echo "... src/level-editor/tree_view.o"
	@echo "... src/level-editor/tree_view.i"
	@echo "... src/level-editor/tree_view.s"
	@echo "... src/level-editor/utils.o"
	@echo "... src/level-editor/utils.i"
	@echo "... src/level-editor/utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

