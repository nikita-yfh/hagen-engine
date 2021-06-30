CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
set(ENGINE engine)
set(LEVEL_EDITOR level-editor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_definitions(-DFC_USE_SDL_GPU -DLUABRIDGE_DISABLE_EXCEPTIONS)
set(CMAKE_CXX_STANDART 11)

option(EXT_LUAJIT "External LuaJit" OFF)

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
        src/XMLParser/xmlParser.cpp

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
        src/XMLParser/xmlParser.h
)

set(ENGINE_SOURCES
        src/engine/input.cpp
        src/engine/shader.cpp
        src/engine/effect.cpp
        src/engine/dialog.cpp
        src/engine/main.cpp
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
        src/engine/interface.cpp
        src/SDL_FontCache/SDL_FontCache.c
        src/XMLParser/xmlParser.cpp
        src/box2d/collision/b2_broad_phase.cpp
        src/box2d/collision/b2_chain_shape.cpp
        src/box2d/collision/b2_circle_shape.cpp
        src/box2d/collision/b2_collide_circle.cpp
        src/box2d/collision/b2_collide_edge.cpp
        src/box2d/collision/b2_collide_polygon.cpp
        src/box2d/collision/b2_collision.cpp
        src/box2d/collision/b2_distance.cpp
        src/box2d/collision/b2_dynamic_tree.cpp
        src/box2d/collision/b2_edge_shape.cpp
        src/box2d/collision/b2_polygon_shape.cpp
        src/box2d/collision/b2_time_of_impact.cpp
        src/box2d/common/b2_block_allocator.cpp
        src/box2d/common/b2_draw.cpp
        src/box2d/common/b2_math.cpp
        src/box2d/common/b2_settings.cpp
        src/box2d/common/b2_stack_allocator.cpp
        src/box2d/common/b2_timer.cpp
        src/box2d/dynamics/b2_body.cpp
        src/box2d/dynamics/b2_chain_circle_contact.cpp
        src/box2d/dynamics/b2_chain_circle_contact.h
        src/box2d/dynamics/b2_chain_polygon_contact.cpp
        src/box2d/dynamics/b2_chain_polygon_contact.h
        src/box2d/dynamics/b2_circle_contact.cpp
        src/box2d/dynamics/b2_circle_contact.h
        src/box2d/dynamics/b2_contact.cpp
        src/box2d/dynamics/b2_contact_manager.cpp
        src/box2d/dynamics/b2_contact_solver.cpp
        src/box2d/dynamics/b2_contact_solver.h
        src/box2d/dynamics/b2_distance_joint.cpp
        src/box2d/dynamics/b2_edge_circle_contact.cpp
        src/box2d/dynamics/b2_edge_circle_contact.h
        src/box2d/dynamics/b2_edge_polygon_contact.cpp
        src/box2d/dynamics/b2_edge_polygon_contact.h
        src/box2d/dynamics/b2_fixture.cpp
        src/box2d/dynamics/b2_friction_joint.cpp
        src/box2d/dynamics/b2_gear_joint.cpp
        src/box2d/dynamics/b2_island.cpp
        src/box2d/dynamics/b2_island.h
        src/box2d/dynamics/b2_joint.cpp
        src/box2d/dynamics/b2_motor_joint.cpp
        src/box2d/dynamics/b2_mouse_joint.cpp
        src/box2d/dynamics/b2_polygon_circle_contact.cpp
        src/box2d/dynamics/b2_polygon_circle_contact.h
        src/box2d/dynamics/b2_polygon_contact.cpp
        src/box2d/dynamics/b2_polygon_contact.h
        src/box2d/dynamics/b2_prismatic_joint.cpp
        src/box2d/dynamics/b2_pulley_joint.cpp
        src/box2d/dynamics/b2_revolute_joint.cpp
        src/box2d/dynamics/b2_weld_joint.cpp
        src/box2d/dynamics/b2_wheel_joint.cpp
        src/box2d/dynamics/b2_world.cpp
        src/box2d/dynamics/b2_world_callbacks.cpp
        src/box2d/rope/b2_rope.cpp
)
set(ENGINE_HEADERS
        src/engine/input.hpp
        src/engine/shader.hpp
        src/engine/effect.hpp
        src/engine/main.hpp
        src/engine/dialog.hpp
        src/engine/level.hpp
        src/engine/render.hpp
        src/engine/utility.hpp
        src/engine/entity.hpp
        src/engine/lua.hpp
        src/engine/physic.hpp
        src/engine/sdl.hpp
        src/engine/camera.hpp
        src/engine/text.hpp
        src/engine/weapon.hpp
        src/engine/triangulate.hpp
        src/engine/interface.hpp
        src/SDL_FontCache/SDL_FontCache.h
        src/XMLParser/xmlParser.h
		src/box2d/b2_api.h
        src/box2d/b2_block_allocator.h
        src/box2d/b2_body.h
        src/box2d/b2_broad_phase.h
        src/box2d/b2_chain_shape.h
        src/box2d/b2_circle_shape.h
        src/box2d/b2_collision.h
        src/box2d/b2_common.h
        src/box2d/b2_contact.h
        src/box2d/b2_contact_manager.h
        src/box2d/b2_distance.h
        src/box2d/b2_distance_joint.h
        src/box2d/b2_draw.h
        src/box2d/b2_dynamic_tree.h
        src/box2d/b2_edge_shape.h
        src/box2d/b2_fixture.h
        src/box2d/b2_friction_joint.h
        src/box2d/b2_gear_joint.h
        src/box2d/b2_growable_stack.h
        src/box2d/b2_joint.h
        src/box2d/b2_math.h
        src/box2d/b2_motor_joint.h
        src/box2d/b2_mouse_joint.h
        src/box2d/b2_polygon_shape.h
        src/box2d/b2_prismatic_joint.h
        src/box2d/b2_pulley_joint.h
        src/box2d/b2_revolute_joint.h
        src/box2d/b2_rope.h
        src/box2d/b2_settings.h
        src/box2d/b2_shape.h
        src/box2d/b2_stack_allocator.h
        src/box2d/b2_time_of_impact.h
        src/box2d/b2_timer.h
        src/box2d/b2_time_step.h
        src/box2d/b2_types.h
        src/box2d/b2_weld_joint.h
        src/box2d/b2_wheel_joint.h
        src/box2d/b2_world.h
        src/box2d/b2_world_callbacks.h
        src/box2d/box2d.h
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED sdl2 SDL2_ttf SDL2_mixer)
pkg_check_modules(GTK REQUIRED gtk+-2.0)
if(EXT_LUAJIT)
	pkg_check_modules(LUA REQUIRED luajit)
	include_directories(${LUA_INCLUDE_DIRS})
else()
	add_subdirectory("src/LuaJIT-cmake")
endif()
include_directories(
        ${SDL_INCLUDE_DIRS}
        ${GTK_INCLUDE_DIRS}
        ${IMGUI_INCLUDE_DIRS}
        "src/engine"
        "src/level-editor"
        "src/box2d/"
        "src/LuaBridge"
        "src/SDL_FontCache"
        "src/XMLParser"
        "src/sdl-gpu/include"
		"src/imgui/"
		"src/imgui/examples"
        "src"
)

add_executable(${ENGINE} ${ENGINE_SOURCES} ${ENGINE_HEADERS})
add_executable(${LEVEL_EDITOR} ${LEVEL_EDITOR_SOURCES} ${LEVEL_EDITOR_HEADERS})
add_subdirectory("src/sdl-gpu")
add_subdirectory("src/imgui")


target_link_libraries (${ENGINE}
	${SDL_LIBRARIES}
	${IMGUI_LIBRARIES}
	SDL_gpu_shared
	imgui_sdl2
)
if(EXT_LUAJIT)
	target_link_libraries(${ENGINE} ${LUA_LIBRARIES})
else()
	target_link_libraries(${ENGINE} liblua-static)
endif()
if(UNIX)
	target_link_libraries (${ENGINE} ${GTK_LIBRARIES})
endif()
target_link_libraries (${LEVEL_EDITOR}
	${GTK_LIBRARIES}
)
