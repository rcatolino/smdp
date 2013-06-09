LOCAL_PATH := $(call my-dir)
SMDP_PATH := ../../src

include $(CLEAR_VARS)

LOCAL_MODULE    := smdp
LOCAL_SRC_FILES := $(SMDP_PATH)/smdp.c smdp-jni.c
LOCAL_EXPORT_C_INCLUDE := $(LOCAL_PATH)/$(SMDP_PATH)
LOCAL_SHARED_LIBRARY := libc
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
