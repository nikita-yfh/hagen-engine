LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2-2.0.7

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include

LOCAL_SRC_FILES := camera.cpp dialog.cpp effect.cpp entity.cpp interface.cpp level.cpp lua_draw.cpp lua.cpp main.cpp physic.cpp render.cpp sdl.cpp shader.cpp text.cpp triangulate.cpp utility.cpp weapon.cpp input.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_gpu luajit

LOCAL_STATIC_LIBRARIES := box2d imgui XML LuaBridge SDL2_mixer SDL2_FC

LOCAL_CFLAGS := -std=c++11 -fexceptions -DFC_USE_SDL_GPU -O3 -s -ffunction-sections

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
