#APP_CPPFLAGS := -frtti -fexceptions
APP_ABI := armeabi-v7a
APP_PLATFORM := android-19

# Get C++11 working
APP_CPPFLAGS += -std=c++11 -fexceptions
APP_STL := gnustl_static
