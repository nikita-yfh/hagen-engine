LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := XML

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -fexceptions -O3

LOCAL_SRC_FILES := xmlParser.cpp

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

include $(BUILD_STATIC_LIBRARY)
