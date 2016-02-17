LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ../../OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_MODULE := OpencvHeader
LOCAL_SRC_FILES := LibOpencv.cpp
LOCAL_CFLAGS := -I $(LOCAL_PATH)/include/
LOCAL_LDFLAGS += -L $(LOCAL_PATH)/../libs/armeabi/

LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_SHARED_LIBRARIES += python2.7
LOCAL_LDLIBS += -llog -ldl
include $(BUILD_SHARED_LIBRARY)


include $(LOCAL_PATH)/python/Android.mk