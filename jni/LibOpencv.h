#ifndef MAIN_H
#define MAIN_H

#include <jni.h>
#include <Python.h>
#include <pyconfig.h>
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "myfb.h"
#include "mybmp.h"

#define MAIN_PYTHON_FILE "/data/data/org.modrana.PySideExample/files/main.py"

#define PYTHON_HOME "/data/data/org.modrana.PySideExample/files/python/"

#define PYTHON_PATH "/data/data/org.modrana.PySideExample/files/python/lib/python2.7/lib-dynload:/data/data/org.modrana.PySideExample/files/python/lib/python2.7/:/data/data/org.modrana.PySideExample/files/python/lib/python2.7/site-packages:/data/data/org.modrana.PySideExample/files/python/lib"

#define LD_LIBRARY_PATH "/data/data/org.modrana.PySideExample/files/python/lib:/data/data/org.modrana.PySideExample/files/python/lib/python2.7/lib-dynload:/data/data/org.kde.necessitas.ministro/files/qt/lib/"

#define PATH "/data/data/org.modrana.PySideExample/files/python/bin:$PATH"

#define THEME_PATH "/data/data/org.modrana.PySideExample/files/python/themes/"

#define QML_IMPORT_PATH "/data/data/org.modrana.PySideExample/files/python/imports/"

#define PYSIDE_APPLICATION_FOLDER "/data/data/org.modrana.PySideExample/"

#define LOG(x) __android_log_write(ANDROID_LOG_INFO, "ZHUJIANG", (x))
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"ZHUJIANG",__VA_ARGS__)

#endif // MAIN_H
