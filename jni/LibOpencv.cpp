#include "LibOpencv.h"

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <condition_variable>
#include <mutex>

#include <binder/ProcessState.h>

#define private public
#include <gui/SurfaceComposerClient.h>
#undef private

#include <gui/ISurfaceComposer.h>
#include <ui/DisplayInfo.h>

#include <ui/PixelFormat.h>
#include <ui/Rect.h>
#include <private/gui/ComposerService.h>


using namespace std;
using namespace cv;
using namespace android;
static uint32_t DEFAULT_DISPLAY_ID = ISurfaceComposer::eDisplayIdMain;

#ifdef __cplusplus
extern "C" {
#endif

static const char*
	error_name(int32_t err) {
		switch (err) {
		case android::NO_ERROR: // also android::OK
			return "NO_ERROR";
		case android::UNKNOWN_ERROR:
			return "UNKNOWN_ERROR";
		case android::NO_MEMORY:
			return "NO_MEMORY";
		case android::INVALID_OPERATION:
			return "INVALID_OPERATION";
		case android::BAD_VALUE:
			return "BAD_VALUE";
		case android::BAD_TYPE:
			return "BAD_TYPE";
		case android::NAME_NOT_FOUND:
			return "NAME_NOT_FOUND";
		case android::PERMISSION_DENIED:
			return "PERMISSION_DENIED";
		case android::NO_INIT:
			return "NO_INIT";
		case android::ALREADY_EXISTS:
			return "ALREADY_EXISTS";
		case android::DEAD_OBJECT: // also android::JPARKS_BROKE_IT
			return "DEAD_OBJECT";
		case android::FAILED_TRANSACTION:
			return "FAILED_TRANSACTION";
		case android::BAD_INDEX:
			return "BAD_INDEX";
		case android::NOT_ENOUGH_DATA:
			return "NOT_ENOUGH_DATA";
		case android::WOULD_BLOCK:
			return "WOULD_BLOCK";
		case android::TIMED_OUT:
			return "TIMED_OUT";
		case android::UNKNOWN_TRANSACTION:
			return "UNKNOWN_TRANSACTION";
		case android::FDS_NOT_ALLOWED:
			return "FDS_NOT_ALLOWED";
		default:
			return "UNMAPPED_ERROR";
		}
}


struct FrameAvailableListener1 {
	virtual
		~FrameAvailableListener1() {}

	virtual void
		onFrameAvailable() = 0;
};

class FrameWaiter: public FrameAvailableListener1 {
public:
	FrameWaiter()
		: mPendingFrames(0),
		mTimeout(chrono::milliseconds(100)),
		mStopped(false) {
	}

	int
		waitForFrame() {
			unique_lock<mutex> lock(mMutex);

			while (!mStopped) {
				if (mCondition.wait_for(lock, mTimeout, [this]{return mPendingFrames > 0;})) {
					return mPendingFrames--;
				}
			}

			return 0;
	}

	void
		reportExtraConsumption(int count) {
			unique_lock<mutex> lock(mMutex);
			mPendingFrames -= count;
	}

	void
		onFrameAvailable() {
			LOG("onFrameAvailable");
			unique_lock<mutex> lock(mMutex);
			mPendingFrames += 1;
			mCondition.notify_one();
	}

	void
		stop() {
			mStopped = true;
	}

	bool
		isStopped() {
			return mStopped;
	}

private:
	mutex mMutex;
	condition_variable mCondition;
	chrono::milliseconds mTimeout;
	int mPendingFrames;
	bool mStopped;
};

class FrameProxy: public ConsumerBase::FrameAvailableListener {
public:
	FrameProxy(FrameAvailableListener1* listener): mUserListener(listener) {
	}

	virtual void onFrameAvailable() {
			mUserListener->onFrameAvailable();
	}

private:
	FrameAvailableListener1* mUserListener;
};

static FrameWaiter gWaiter;

/*
* Class: com_android_libjni_LibJavaHeader
* Method: Add
* Signature: (II)I
*/
JNIEXPORT jint JNICALL Java_com_hupo_opencv_LibOpencvHeader_Add(JNIEnv *jPE, jobject jo, jint jiX, jint jiY)
{
	LOG("HUPO is begin running ... ");
	double t = (double)getTickCount();
	Py_Initialize();
	LOG("Py_Initialize ... u can do something ... success ");
	Py_Finalize();
	/*
	int result = system("su");
	if (result == -1)
	{
		return -1;
	}*/

	ProcessState::self()->startThreadPool();

	ScreenshotClient mScreenshotClient;
	
	sp<android::CpuConsumer> mConsumer;
	sp<android::IBinder> mVirtualDisplay;
	CpuConsumer::LockedBuffer mBuffer;
	sp<FrameProxy> mFrameProxy;
	status_t err;

	DisplayInfo dinfo;
	/*
	sp<android::IBinder> dpy = SurfaceComposerClient::getBuiltInDisplay(DEFAULT_DISPLAY_ID);
	err = SurfaceComposerClient::getDisplayInfo(dpy, &dinfo);

	if (err != NO_ERROR) {
		LOGD("SurfaceComposerClient::getDisplayInfo() failed: %s (%d)\n", error_name(err), err);
		return err;
	}*/

	//test code
	sp<ISurfaceComposer> sm(ComposerService::getComposerService());
	if (sm != 0) {
		sp<ISurfaceComposerClient> conn = sm->createConnection();
		if (conn != 0) {
			LOG("init success ... ");
			return 8;
		}
		LOG("conn == 0");
		return -1;
	}

	return -1;

	// Create a Surface for the virtual display to write to.
	LOG("Creating SurfaceComposerClient");
	android::sp<android::SurfaceComposerClient> sc = new android::SurfaceComposerClient();

	LOG("Performing SurfaceComposerClient init check");
	if ((err = sc->initCheck()) != android::NO_ERROR) {
		LOGD("Unable to initialize SurfaceComposerClient failed: %s (%d)", error_name(err), err);
		return err;
	}

	// Create virtual display.
	LOG("Creating virtual display");
	mVirtualDisplay = SurfaceComposerClient::createDisplay( String8("hupocap"),false);
	
	LOG("Creating buffer queue");
	//sp<android::BufferQueue> mBufferQueue = new BufferQueue();

	mScreenshotClient.getCpuConsumer();
	sp<android::BufferQueue> mBufferQueue = mScreenshotClient.mBufferQueue;

	LOGD("the buffer size w:%d;h:%d",dinfo.w,dinfo.h);

	mConsumer = new CpuConsumer(mBufferQueue, 3, false);
	mConsumer->setName(String8("hupocap"));
	mConsumer->setDefaultBufferSize(dinfo.w,dinfo.h);
	mConsumer->setDefaultBufferFormat(PIXEL_FORMAT_RGBA_8888);

	LOG("Creating frame waiter");
	mFrameProxy = new FrameProxy((FrameAvailableListener1*)&gWaiter);
	mConsumer->setFrameAvailableListener(mFrameProxy);

	LOG("Publishing virtual display2");
	SurfaceComposerClient::openGlobalTransaction();
	SurfaceComposerClient::setDisplaySurface(mVirtualDisplay, mBufferQueue);
	SurfaceComposerClient::setDisplayProjection(mVirtualDisplay,DISPLAY_ORIENTATION_0, android::Rect(dinfo.w,dinfo.h), android::Rect(dinfo.w,dinfo.h));
	SurfaceComposerClient::setDisplayLayerStack(mVirtualDisplay, 0); // default stack
	SurfaceComposerClient::closeGlobalTransaction();

	LOG("Wait for frame ... ");
	
	if (!gWaiter.waitForFrame()) {
		LOG("Unable to wait for frame");
		return 4;
	}

	if ((err = mConsumer->lockNextBuffer(&mBuffer)) != NO_ERROR) {
		if (err == -EINTR) {
			LOGD("lockNextBuffer failed: %s (%d)\n", error_name(err), err);
			return err;
		}
		else {
			LOGD("Unable to lock next buffer %s (%d)", error_name(err), err);
			return err;
		}
	}

	LOG("look like it is sucess ... ");


	return 3;
}

int main(int argc, char *argv[])
{

}

#ifdef __cplusplus
}
#endif
