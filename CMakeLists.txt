CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
set(ENGINE engine)
set(LEVEL_EDITOR level-editor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(CMAKE_CXX_STANDART 11)

set(LEVEL_EDITOR_SOURCES
        src/level-editor/main.cpp
        src/level-editor/object.cpp
        src/level-editor/editor.cpp
        src/level-editor/events.cpp
        src/level-editor/level.cpp
        src/level-editor/utils.cpp
        src/level-editor/tree_view.cpp
        src/level-editor/shape.cpp
        src/level-editor/joint.cpp
        src/level-editor/body.cpp
)
set(LEVEL_EDIROR_HEADERS
        src/level-editor/main.hpp
        src/level-editor/object.hpp
        src/level-editor/editor.hpp
        src/level-editor/events.hpp
        src/level-editor/level.hpp
        src/level-editor/utils.hpp
        src/level-editor/tree_view.hpp
        src/level-editor/shape.hpp
        src/level-editor/joint.hpp
        src/level-editor/body.hpp
)

set(ENGINE_SOURCES
        src/engine/game.cpp
        src/engine/dialog.cpp
        src/engine/main.cpp
        src/engine/player.cpp
        src/engine/utility.cpp
        src/engine/entity.cpp
        src/engine/lua.cpp
        src/engine/physic.cpp
        src/engine/level.cpp
        src/engine/render.cpp
        src/engine/sdl.cpp
        src/engine/camera.cpp
        src/engine/text.cpp
        src/engine/weapon.cpp
        src/engine/triangulate.cpp
        src/engine/console.cpp
        src/SDL_FontCache/SDL_FontCache.c
        src/XMLParser/xmlParser.cpp
)
set(ENGINE_HEADERS
        src/engine/main.hpp
        src/engine/dialog.hpp
        src/engine/level.hpp
        src/engine/render.hpp
        src/engine/game.hpp
        src/engine/player.hpp
        src/engine/utility.hpp
        src/engine/entity.hpp
        src/engine/lua.hpp
        src/engine/physic.hpp
        src/engine/sdl.hpp
        src/engine/camera.hpp
        src/engine/text.hpp
        src/engine/weapon.hpp
        src/engine/triangulate.hpp
        src/engine/console.hpp
        src/SDL_FontCache/SDL_FontCache.h
        src/XMLParser/xmlParser.h
        src/Nuklear/nuklear.h
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED sdl2 SDL2_ttf)
pkg_check_modules(GTK REQUIRED gtk+-2.0)
pkg_check_modules(LUA REQUIRED lua53)
include_directories(
        ${SDL_INCLUDE_DIRS}
        ${GTK_INCLUDE_DIRS}
        ${LUA_INCLUDE_DIRS}
        "src/engine"
        "src/level-editor"
        "src/box2d/"
        "src/LuaBridge/Source"
        "src/LuaBridge/Source/LuaBridge"
        "src/SDL_FontCache"
        "src/XMLParser"
        "src/sdl-gpu/include"
        "src/Nuklear"
)

add_executable(${ENGINE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_executable(${LEVEL_EDITOR} ${LEVEL_EDITOR_SOURCES} ${LEVEL_EDITOR_HEADERS})
add_subdirectory("src/box2d")
add_subdirectory("src/LuaBridge")
add_subdirectory("src/XMLParser")
add_subdirectory("src/sdl-gpu")


target_link_libraries (${ENGINE}
        ${SDL_LIBRARIES}
        ${LUA_LIBRARIES}
        box2d
        xml
        SDL_gpu_shared
)
if(UNIX)
        target_link_libraries (${ENGINE} ${GTK_LIBRARIES})
endif()
target_link_libraries (${LEVEL_EDITOR}
        ${GTK_LIBRARIES}
        xml
)
