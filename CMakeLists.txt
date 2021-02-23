CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
set(ENGINE engine)
set(LEVEL_EDITOR level-editor)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(ENGINE_INCLUDE_DIR src/engine)
set(LEVEL_EDITOR_INCLUDE_DIR src/level-editor)

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
	${LEVEL_EDITOR_INCLUDE_DIR}/defines.h
	${LEVEL_EDITOR_INCLUDE_DIR}/object.h
	${LEVEL_EDITOR_INCLUDE_DIR}/editor.h
	${LEVEL_EDITOR_INCLUDE_DIR}/events.h
	${LEVEL_EDITOR_INCLUDE_DIR}/level.h
	${LEVEL_EDITOR_INCLUDE_DIR}/utils.h
	${LEVEL_EDITOR_INCLUDE_DIR}/tree_view.h
	${LEVEL_EDITOR_INCLUDE_DIR}/shape.h
	${LEVEL_EDITOR_INCLUDE_DIR}/joint.h
	${LEVEL_EDITOR_INCLUDE_DIR}/body.h
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
	${ENGINE_INCLUDE_DIR}/shapes.cpp
	${ENGINE_INCLUDE_DIR}/sdl.cpp
	${ENGINE_INCLUDE_DIR}/camera.cpp
)
set(ENGINE_HEADERS
	${ENGINE_INCLUDE_DIR}/level.h
	${ENGINE_INCLUDE_DIR}/render.h
	${ENGINE_INCLUDE_DIR}/game.h 
	${ENGINE_INCLUDE_DIR}/player.h
	${ENGINE_INCLUDE_DIR}/utility.h
	${ENGINE_INCLUDE_DIR}/entity.h
	${ENGINE_INCLUDE_DIR}/lua.hpp
	${ENGINE_INCLUDE_DIR}/physic.h
	${ENGINE_INCLUDE_DIR}/sdl.h
	${ENGINE_INCLUDE_DIR}/camera.h
)
find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image SDL2_ttf SDL2_mixer)
pkg_check_modules(LUA REQUIRED lua53)
pkg_check_modules(GL REQUIRED opengl glu)
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
	${GL_INCLUDE_DIRS}
	"src/SDL_GFX"
	"src/box2d"
	"src/LuaBridge"
	"src/SDL_FontCache"
	"src/XMLParser"
	"src/triangulate"
)

add_executable(${ENGINE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_executable(${LEVEL_EDITOR} ${LEVEL_EDITOR_SOURCES} ${LEVEL_EDITOR_HEADERS})
add_subdirectory("src/box2d")
add_subdirectory("src/LuaBridge")
add_subdirectory("src/SDL_FontCache")
add_subdirectory("src/XMLParser")
add_subdirectory("src/SDL_GFX")
add_subdirectory("src/triangulate")


target_link_libraries (${ENGINE}
	${SDL2_LIBRARIES}
	${SDL2IMAGE_LIBRARIES}
	${SDL2TTF_LIBRARIES}
	${SDL2MIXER_LIBRARIES}
	${LUA_LIBRARIES}
	${GL_LIBRARIES}
	box2d
	triangulate
	gfx
	xml
	fontcache
)
target_link_libraries (${LEVEL_EDITOR}
	${GTK_LIBRARIES}
	xml
)