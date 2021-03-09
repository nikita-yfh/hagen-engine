CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
set(ENGINE engine)
set(LEVEL_EDITOR level-editor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ENGINE_INCLUDE_DIR src/engine)
set(LEVEL_EDITOR_INCLUDE_DIR src/level-editor)

set(CMAKE_CXX_STANDART 11)

set(LEVEL_EDITOR_SOURCES 
	${LEVEL_EDITOR_INCLUDE_DIR}/main.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/object.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/editor.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/events.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/level.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/utils.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/tree_view.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/shape.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/joint.cpp
	${LEVEL_EDITOR_INCLUDE_DIR}/body.cpp
)
set(LEVEL_EDIROR_HEADERS
	${LEVEL_EDITOR_INCLUDE_DIR}/defines.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/object.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/editor.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/events.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/level.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/utils.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/tree_view.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/shape.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/joint.hpp
	${LEVEL_EDITOR_INCLUDE_DIR}/body.hpp
)

set(ENGINE_SOURCES
	${ENGINE_INCLUDE_DIR}/game.cpp
	${ENGINE_INCLUDE_DIR}/main.cpp
	${ENGINE_INCLUDE_DIR}/player.cpp
	${ENGINE_INCLUDE_DIR}/utility.cpp
	${ENGINE_INCLUDE_DIR}/entity.cpp
	${ENGINE_INCLUDE_DIR}/lua.cpp
	${ENGINE_INCLUDE_DIR}/physic.cpp
	${ENGINE_INCLUDE_DIR}/level.cpp
	${ENGINE_INCLUDE_DIR}/render.cpp
	${ENGINE_INCLUDE_DIR}/sdl.cpp
	${ENGINE_INCLUDE_DIR}/camera.cpp
)
set(ENGINE_HEADERS
	${ENGINE_INCLUDE_DIR}/level.hpp
	${ENGINE_INCLUDE_DIR}/render.hpp
	${ENGINE_INCLUDE_DIR}/game.hpp 
	${ENGINE_INCLUDE_DIR}/player.hpp
	${ENGINE_INCLUDE_DIR}/utility.hpp
	${ENGINE_INCLUDE_DIR}/entity.hpp
	${ENGINE_INCLUDE_DIR}/lua.hpp
	${ENGINE_INCLUDE_DIR}/physic.hpp
	${ENGINE_INCLUDE_DIR}/sdl.hpp
	${ENGINE_INCLUDE_DIR}/camera.hpp
)
find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image SDL2_ttf SDL2_mixer)
pkg_check_modules(LUA REQUIRED lua53)
pkg_check_modules(GTK REQUIRED gtk+-2.0)
add_definitions(${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})

include_directories(
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIRS}
	${SDL2MIXER_INCLUDE_DIRS}
	${ENGINE_INCLUDE_DIR}
	${GTK2_INCLUDE_DIRS}
	${LUA_INCLUDE_DIRS}
	"src/box2d"
	"src/LuaBridge/Source"
	"src/LuaBridge/Source/LuaBridge"
	"src/SDL_FontCache"
	"src/XMLParser"
	"src/triangulate"
	"src/sdl-gpu/include"
)

add_executable(${ENGINE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_executable(${LEVEL_EDITOR} ${LEVEL_EDITOR_SOURCES} ${LEVEL_EDITOR_HEADERS})
add_subdirectory("src/box2d")
add_subdirectory("src/LuaBridge")
add_subdirectory("src/SDL_FontCache")
add_subdirectory("src/XMLParser")
add_subdirectory("src/triangulate")
add_subdirectory("src/sdl-gpu")


target_link_libraries (${ENGINE}
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	${SDL2TTF_LIBRARIES}
	${SDL2MIXER_LIBRARIES}
	${LUA_LIBRARIES}
	box2d
	triangulate
	xml
	fontcache
	SDL_gpu_shared
)
target_link_libraries (${LEVEL_EDITOR}
	${GTK_LIBRARIES}
	xml
)
