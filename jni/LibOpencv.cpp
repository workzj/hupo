#include <jni.h>
#include <Python.h>
#include <pyconfig.h>
#include "LibOpencv.h"
#include <android/log.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define LOG(x) __android_log_write(ANDROID_LOG_INFO, "ZHUJIANG", (x))

using namespace std;
using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif

/*
* Class: com_android_libjni_LibJavaHeader
* Method: Add
* Signature: (II)I
*/
JNIEXPORT jint JNICALL Java_com_hupo_opencv_LibOpencvHeader_Add(JNIEnv *jPE, jobject jo, jint jiX, jint jiY)
{
	LOG("begin for my dig // ");
	double t = (double)getTickCount();
	Mat img = imread("/sdcard/DCIM/Camera/IMG20151110120133.jpg");
	if( img.empty() ) // Check for invalid input
	{
		LOG("image is empty!");
		return 0;
	}
	LOG("success");
	Py_Initialize();
	LOG("onliy ... your ");
	Py_Finalize();
	
	return img.channels();
}

#ifdef __cplusplus
}
#endif
