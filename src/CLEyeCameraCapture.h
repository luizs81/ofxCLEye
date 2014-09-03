#pragma once

#include "global.h"

// If you want, you can easily use threads uncommenting some parts
class PS3EyeCamera //: public ofThread 
{
	GUID _cameraGUID;
	CLEyeCameraInstance _cam;
	CLEyeCameraColorMode _mode;
	CLEyeCameraResolution _resolution;
	float _fps;
	PBYTE pCapBuffer;

public:
	IplImage *pCapImage;

	void start(GUID cameraGUID, CLEyeCameraColorMode mode, CLEyeCameraResolution resolution, float fps)
	{
		_cameraGUID = cameraGUID;
		_mode = mode;
		_resolution = resolution;
		_fps = fps;

		int w, h;
		pCapBuffer = NULL;

		// Create camera instance
		_cam = CLEyeCreateCamera(_cameraGUID, _mode, _resolution, _fps);

		if(_cam == NULL) return;

		// Get camera frame dimensions
		CLEyeCameraGetFrameDimensions(_cam, w, h);

		// Depending on color mode chosen, create the appropriate OpenCV image
		if(_mode == CLEYE_COLOR_PROCESSED || _mode == CLEYE_COLOR_RAW)
			pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 4);
		else
			pCapImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

		// Set some camera parameters
		CLEyeSetCameraParameter(_cam, CLEYE_GAIN, 0);
		CLEyeSetCameraParameter(_cam, CLEYE_EXPOSURE, 511);

		// Start capturing
		CLEyeCameraStart(_cam);
		//cvGetImageRawData(pCapImage, &pCapBuffer);
		cvGetRawData(pCapImage, &pCapBuffer);

		//startThread(true, false);
	}

	void stop()
	{
		// Stop camera capture
		CLEyeCameraStop(_cam);
		// Destroy camera object
		CLEyeDestroyCamera(_cam);
		// Destroy the allocated OpenCV image
		cvReleaseImage(&pCapImage);
		_cam = NULL;

		//stopThread();
	}

	void setCameraParameter(int param, int value)
	{
		if(!_cam)	return;
		printf("CLEyeGetCameraParameter %d\n", CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param));
		CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, value);
	}

	void IncrementCameraParameter(int param)
	{
		if(!_cam)	return;
		printf("CLEyeGetCameraParameter %d\n", CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param));
		CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)+10);
	}

	void DecrementCameraParameter(int param)
	{
		if(!_cam)	return;
		printf("CLEyeGetCameraParameter %d\n", CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param));
		CLEyeSetCameraParameter(_cam, (CLEyeCameraParameter)param, CLEyeGetCameraParameter(_cam, (CLEyeCameraParameter)param)-10);
	}

	void updateImage(){
		CLEyeCameraGetFrame(_cam, pCapBuffer);
	}

	// On the CLEye SDK, this function is the "Run"
	//void threadedFunction()
	//{
	//	while(isThreadRunning()) 
	//	{
	//		// Lock access to the resource (camera)
	//		if(lock())
	//		{
	//			//CLEyeCameraGetFrame(_cam, pCapBuffer);
	//			// Done with the resource
	//			unlock();
	//		}
	//	}
	//}

	// just a example about call functions from outside
	void draw()
	{
		//if(lock())
		//{
		//	image1 = pCapImage;
		//	unlock();
		//}
		//else
		//{
		//	printf("can't lock!\neither an error\nor the thread has stopped");
		//}
		//
		//image1.draw(0, 0);
	}
};