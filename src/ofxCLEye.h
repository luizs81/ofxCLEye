/*=============================================================================

    Copyright (c) 2014 Luiz Gustavo Moreira Sampaio

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    ----------------------------------------------------------------------------

    This project uses the CL Eye Platform SDK copyrighted by Code Laboratories
    Inc. All rights reserved.

    See https://codelaboratories.com/products/eye/sdk/ for documentation.

	Implemented and tested with OpenFrameworks v0.8.3_vs_release

=============================================================================*/

#pragma once

#ifndef _OFX_CLEYE
	#define _OFX_CLEYE


 #include  "ofMain.h"
 #include "CLEyeMulticam.h"

	const std::string OFX_CLEYE_MODULE_NAME = "ofxCLEye";
	const std::string OFX_CLEYE_SET_PROPERTY_ERROR = " Attempt to set property failed.";
	const std::string OFX_CLEYE_GET_PROPERTY_ERROR = " Attempt to get property failed.";
	const std::string OFX_CLEYE_GRABBER_ERROR = " Grabber not initialized.";
	const std::string OFX_CLEYE_INVALID_VALUE = " Value out of range.";

	class ofxCLEye : protected ofBaseVideoGrabber,
		protected ofThread {

		public:

			ofxCLEye();
			~ofxCLEye();

			vector <ofVideoDevice> listDevices();

			static int  getDeviceCount();
			static int  getDeviceID(const GUID & guid);
			static GUID getDeviceGUID(int deviceID);

			void setDeviceID(int deviceID);
			void setDeviceGUID(GUID deviceGUID);
			void setUseTexture(bool useTexture);
			void setUseGrayscale(bool useGrayscale);
			void setUseThread(bool useThread);
			void setDesiredFrameRate(int frameRate);
			int getFrameRate();

			bool initGrabber(int w, int h);
			bool initGrabber(int w, int h, int deviceID, int frameRate, bool useTexture, bool useGrayscale, bool useThread);

			// CLEYE_AUTO_GAIN [false, true]
			// Automatically determines best gain value.
			void setAutoGain(bool autoGain);
			bool getAutoGain();

			// CLEYE_GAIN [0, 79]
			// Controls the camera signal gain (when auto gain is false).
			void setGain(int gain);
			int getGain();

			// CLEYE_AUTO_EXPOSURE [false, true]
			// Automatically determines best exposure value.
			void setAutoExposure(bool autoExposure);
			bool getAutoExposure();

			// CLEYE_EXPOSURE [0, 511]
			// Controls the sensor exposure time (when auto exposure is false).
			void setExposure(int exposure);
			int getExposure();

			// CLEYE_AUTO_WHITEBALANCE [false, true]
			// Automatically determines best color settings.
			void setAutoWhiteBalance(bool autoWhiteBalance);
			bool getAutoWhiteBalance();

			// CLEYE_WHITEBALANCE_RED [0, 255]
			// Controls the red color gain (when auto white balance is false).
			void setWhiteBalanceRed(int red);
			int getWhiteBalanceRed();

			// CLEYE_WHITEBALANCE_GREEN [0, 255]
			// Controls the green color gain (when auto white balance is false).
			void setWhiteBalanceGreen(int green);
			int getWhiteBalanceGreen();

			// CLEYE_WHITEBALANCE_BLUE [0, 255]
			// Controls the blue color gain (when auto white balance is false).
			void setWhiteBalanceBlue(int blue);
			int getWhiteBalanceBlue();

			// CLEYE_VFLIP [false, true]
			// Flip video vertically.
			void setVerticalFlip(bool vflip);
			bool getVerticalFlip();

			// CLEYE_HFLIP [false, true]
			// Flip video horizontally.
			void setHorizontalFlip(bool hflip);
			bool getHorizontalFlip();

			// CLEYE_VKEYSTONE [-500, 500]
			// Adjust vertical video keystone.
			void setVerticalKeystone(int vKeysone);
			int getVerticalKeystone();

			// CLEYE_HKEYSTONE [-500, 500]
			// Adjust horizontal video keystone.
			void setHorizontalKeystone(int hKeysone);
			int getHorizontalKeystone();

			// CLEYE_XOFFSET [-500, 500]
			// Translate video on X axis by an offset.
			void setXOffset(int xOffset);
			int getXOffset();

			// CLEYE_YOFFSET [-500, 500]
			// Translate video on Y axis by an offset.
			void setYOffset(int yOffset);
			int getYOffset();

			// CLEYE_ROTATION [-500, 500]
			// Rotate video image around center point.
			void setRotation(int rotation);
			int getRotation();

			// CLEYE_ZOOM [-500, 500]
			// Scale video to simulate digital zoom.
			void setZoom(int zoom);
			int getZoom();

			// CLEYE_LENSCORRECTION1 [-500, 500]
			// Correction transforms to correct lens distortion.
			void setLensCorrection1(int correction);
			int getLensCorrection1();
			
			// CLEYE_LENSCORRECTION2 [-500, 500]
			// Correction transforms to correct lens distortion.
			void setLensCorrection2(int correction);
			int getLensCorrection2();
			
			// CLEYE_LENSCORRECTION3 [-500, 500]
			// Correction transforms to correct lens distortion.
			void setLensCorrection3(int correction);
			int getLensCorrection3();

			// CLEYE_LENSBRIGHTNESS [-500, 500]
			// Correction of image brightness due to lens distortion.
			void setLensBrightness(int brigthness);
			int getLensBrightness();

			float getWidth();
			float getHeight();

			unsigned char * getPixels();
			ofPixelsRef getPixelsRef();
			ofTexture & getTextureReference();

			bool isFrameNew();
			bool isInitialized();

			void update();
			void close();
			void threadedFunction();

			bool setPixelFormat(ofPixelFormat pixelFormat);
			ofPixelFormat getPixelFormat();

		private:

			bool usingThread;
			bool usingTexture;
			bool initialized;
			bool newFrame;

			CLEyeCameraInstance cam;
			CLEyeCameraResolution resolution;
			CLEyeCameraColorMode colorMode;

			int requestedDeviceID;
			int desiredFrameRate;
			int width;
			int height;

			ofPixelFormat pixelFormat;
			ofPixels pixels;
			ofTexture texture;
			unsigned char * viPixels;
	};

#endif