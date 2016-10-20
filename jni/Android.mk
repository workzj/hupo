LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ../../OpenCV-android-sdk/sdk/native/jni/OpenCV.mk

MY_ANDROID_SOURCE := E:\androdi-source
LOCAL_PRELINK_MODULE := false
ANDROID4_SYSTEM_LIB := $(LOCAL_PATH)/andSource4_4lib/

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp) \
				$(wildcard $(LOCAL_PATH)/*.c)

LOCAL_MODULE := OpencvHeader
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
LOCAL_CFLAGS := -I $(LOCAL_PATH)/include/
LOCAL_LDFLAGS += -L $(LOCAL_PATH)/../libs/armeabi/

LOCAL_C_INCLUDES += $(LOCAL_PATH) \
					$(MY_ANDROID_SOURCE)/frameworks/native/include \
					$(MY_ANDROID_SOURCE)/system/core/include \
					$(MY_ANDROID_SOURCE)/hardware/libhardware/include \
					F:\android-ndk-r10d\platforms\android-19\arch-arm\usr\include
					#E:\androdi-source\prebuilts\ndk\9\platforms\android-18\arch-arm\usr\include \

LOCAL_CERTIFICATE := platform
LOCAL_MODULE_TAGS := optional
#LOCAL_CFLAGS += -DHAVE_SYS_UIO_H

LOCAL_LDLIBS := -L${ANDROID4_SYSTEM_LIB} 
LOCAL_LDLIBS += -llog -ldl
#LOCAL_LDLIBS += -lGLESv2 -lEGL -landroid  -lui -landroid_runtime  -ljnigraphics -lcutils
LOCAL_LDLIBS += -lcutils -lutils -lgui -lbinder

LOCAL_SHARED_LIBRARIES += python2.7

include $(BUILD_EXECUTABLE)
#include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/python/Android.mk

