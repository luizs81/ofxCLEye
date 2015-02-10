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

#include "ofxCLEye.h"

//--------------------------------------------------------------
ofxCLEye::ofxCLEye(){
	usingTexture = false;
	usingThread = true;
	initialized = false;
	requestedDeviceID = -1;
	desiredFrameRate = 60;
	width = 640;
	height = 480;

	colorMode = CLEYE_COLOR_PROCESSED;
	pixelFormat = OF_PIXELS_RGBA;
}

//--------------------------------------------------------------
ofxCLEye::~ofxCLEye(){
	close();
}

//--------------------------------------------------------------
vector <ofVideoDevice> ofxCLEye::listDevices(){
	int numCams = getDeviceCount();

	if(numCams == 0){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "No PS Eye cameras found!";
	}

	ofLogVerbose(OFX_CLEYE_MODULE_NAME) << numCams << " PS Eye camera(s) found";
	vector <ofVideoDevice> devices;

	for(int i = 0; i < numCams; i++){
		//GUID guidCamera;
		//guidCamera = CLEyeGetCameraUUID(i);
		// TODO: properly fullfil the ofVideoDevice with the specs from the SDK.
		// See https://codelaboratories.com/research/view/cl-eye-muticamera-api
	}


	ofLogWarning(OFX_CLEYE_MODULE_NAME) << "Function not implemented yet";
	return devices;
}

//--------------------------------------------------------------
int ofxCLEye::getDeviceCount(){
	return CLEyeGetCameraCount();
}

//--------------------------------------------------------------
int ofxCLEye::getDeviceID(const GUID & guid){
	int id = -1;
	int deviceCount = getDeviceCount();

	for(int i = 0; i < deviceCount; i++){
		if(CLEyeGetCameraUUID(i) == guid){
			id = i;
		}
	}

	return id;
}

//--------------------------------------------------------------
GUID ofxCLEye::getDeviceGUID(int deviceID){
	return CLEyeGetCameraUUID(deviceID);
}

//--------------------------------------------------------------
void ofxCLEye::setDeviceID(int deviceID){
	if(deviceID > getDeviceCount() || deviceID < 0){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setDeviceID(): can't find a device with this ID.";
		return;
	}

	if(initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setDeviceID(): can't set device while grabber is running.";
		return;
	}

	requestedDeviceID = deviceID;
}

//--------------------------------------------------------------
void ofxCLEye::setDeviceGUID(GUID deviceGUID){
	int id = getDeviceID(deviceGUID);

	if(id < 0){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setdeviceGUID(): can't find device with this GUID.";
		return;
	}

	if(initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setdeviceGUID(): can't set device while grabber is running.";
		return;
	}

	requestedDeviceID = id;
}

//--------------------------------------------------------------
void ofxCLEye::setUseTexture(bool useTexture){
	usingTexture = useTexture;
}

//--------------------------------------------------------------
void ofxCLEye::setUseGrayscale(bool useGrayscale){
	colorMode = useGrayscale ? CLEYE_MONO_PROCESSED : CLEYE_COLOR_PROCESSED;
}

//--------------------------------------------------------------
void ofxCLEye::setUseThread(bool useThread){
	usingThread = useThread;
}

//--------------------------------------------------------------
void ofxCLEye::setDesiredFrameRate(int frameRate){
	desiredFrameRate = frameRate;
}

//--------------------------------------------------------------
int ofxCLEye::getFrameRate(){
	return desiredFrameRate;
}

//--------------------------------------------------------------
bool ofxCLEye::initGrabber(int w, int h){
	bool usingGrayscale = (colorMode == CLEYE_COLOR_PROCESSED);
	return initGrabber(w, h, requestedDeviceID, desiredFrameRate, usingTexture, usingGrayscale, usingThread);
}

//--------------------------------------------------------------
bool ofxCLEye::initGrabber(int w, int h, int deviceID, int frameRate, bool useTexture,
						   bool useGrayscale, bool useThread){
	setDeviceID(deviceID);
	setDesiredFrameRate(frameRate);
	setUseThread(useThread);
	setUseGrayscale(useGrayscale);
	setUseTexture(useTexture);

	if(w == 640 && h == 480){
		resolution = CLEYE_VGA;
	}
	else if(w == 320 && h == 240){
		resolution = CLEYE_QVGA;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "initGrabber(): selected resolution " + ofToString(w) + "x"
			+ ofToString(h) + " is not available with ofxCLEye";
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "initGrabber(): using 640x480 instead";
		resolution = CLEYE_VGA;
	}

	if(desiredFrameRate < 0){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "initGrabber(): selected framerate" + ofToString(desiredFrameRate) + "is not available with ofxCLeye";
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "initGrabber(): using 60fps instead";
		desiredFrameRate = 60;
	}

	GUID guid = getDeviceGUID(requestedDeviceID);
	cam = CLEyeCreateCamera(guid, colorMode, resolution, desiredFrameRate);

	if(cam == NULL){
		ofLogError(OFX_CLEYE_MODULE_NAME) << "initGrabber(): error when creating instance of CLEyeCamera.";
		return false;
	}

	initialized = CLEyeCameraStart(cam);

	if(!initialized){
		ofLogError(OFX_CLEYE_MODULE_NAME) << "initGrabber(): can't start the CLEye camera.";
		return false;
	}

	CLEyeCameraGetFrameDimensions(cam, width, height);

	// oF code style says to not use ternary operators, but sometimes they are really convenient.
	// Native color image from camera is RGBA (4 channels)
	viPixels = new unsigned char[width * height * ((colorMode == CLEYE_MONO_PROCESSED) ? 1 : 4)];

	pixels.allocate(width, height, (colorMode == CLEYE_MONO_PROCESSED) ? 1 : 3);
	pixels.set(0);

	if(usingTexture){
		int glFormat = (colorMode == CLEYE_MONO_PROCESSED) ? GL_LUMINANCE : GL_RGB;
		texture.allocate(width, height, glFormat);
		texture.loadData((unsigned char *)pixels.getPixels(), width, height, glFormat);
	}

	if(usingThread){
		startThread(true);
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "initGrabber(): thread started.";
	}

	return true;
}

//--------------------------------------------------------------
void ofxCLEye::setAutoGain(bool autoGain){
	if(!initialized){
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_AUTO_GAIN, (int)autoGain);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "AUTO GAIN: " << (autoGain ? "ON" : "OFF");
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setAutoGain(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
bool ofxCLEye::getAutoGain(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getAutoGain():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_AUTO_GAIN);
}

//--------------------------------------------------------------
void ofxCLEye::setGain(int gain){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setGain():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(gain < 0 || gain > 79){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setGain():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_GAIN, gain);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "GAIN: " << gain;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setGain():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getGain(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getGain():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_GAIN);
}

//--------------------------------------------------------------
void ofxCLEye::setAutoExposure(bool autoExposure){
	if(!initialized){
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_AUTO_EXPOSURE, (int)autoExposure);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "AUTO EXPOSURE: " << (autoExposure ? "ON" : "OFF");
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setAutoExposure():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
bool ofxCLEye::getAutoExposure(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getAutoExposure():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_AUTO_EXPOSURE);
}

//--------------------------------------------------------------
void ofxCLEye::setExposure(int exposure){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setExposure():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(exposure < 0 || exposure > 511){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setExposure():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_EXPOSURE, exposure);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "EXPOSURE: " << exposure;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setExposure():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getExposure(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getExposure():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_EXPOSURE);
}

//--------------------------------------------------------------
void ofxCLEye::setAutoWhiteBalance(bool autoWhiteBalance){
	if(!initialized){
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_AUTO_WHITEBALANCE, (int)autoWhiteBalance);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "AUTO WHITE BALANCE: " << (autoWhiteBalance ? "ON" : "OFF");
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setAutoWhiteBalance(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
bool ofxCLEye::getAutoWhiteBalance(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getAutoWhiteBalance():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_AUTO_WHITEBALANCE);
}

//--------------------------------------------------------------
void ofxCLEye::setWhiteBalanceRed(int red){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceRed():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(red < 0 || red > 255){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceRed():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_RED, red);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "WHITE BALANCE (RED): " << red;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceRed():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getWhiteBalanceRed(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getWhiteBalanceRed():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_WHITEBALANCE_RED);
}

//--------------------------------------------------------------
void ofxCLEye::setWhiteBalanceGreen(int green){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceGreen():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(green < 0 || green > 255){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceGreen():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_GREEN, green);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "WHITE BALANCE (GREEN): " << green;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceGreen():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getWhiteBalanceGreen(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getWhiteBalanceGreen():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_WHITEBALANCE_GREEN);
}

//--------------------------------------------------------------
void ofxCLEye::setWhiteBalanceBlue(int blue){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceBlue():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(blue < 0 || blue > 255){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceBlue():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_WHITEBALANCE_BLUE, blue);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "WHITE BALANCE (BLUE): " << blue;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setWhiteBalanceBlue():" << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getWhiteBalanceBlue(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getWhiteBalanceBlue():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_WHITEBALANCE_BLUE);
}

//--------------------------------------------------------------
void ofxCLEye::setVerticalFlip(bool vFlip){
	if(!initialized){
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_VFLIP, (int)vFlip);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "VERTICAL FLIP: " << (vFlip ? "ON" : "OFF");
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setVerticalFlip(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
bool ofxCLEye::getVerticalFlip(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getVerticalFlip():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_VFLIP);
}

//--------------------------------------------------------------
void ofxCLEye::setHorizontalFlip(bool hFlip){
	if(!initialized){
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_HFLIP, (int)hFlip);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "HORIZONTAL FLIP: " << (hFlip ? "ON" : "OFF");
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setHorizontalFlip(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
bool ofxCLEye::getHorizontalFlip(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getHorizontalFlip():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_HFLIP);
}

//--------------------------------------------------------------
void ofxCLEye::setHorizontalKeystone(int hKeystone){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setHorizontalKeystone():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(hKeystone < -500 || hKeystone > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setHorizontalKeystone():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_HKEYSTONE, hKeystone);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "HORIZONTAL KEYSTONE: " << hKeystone;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setHorizontalKeystone(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getHorizontalKeystone(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getHorizontalKeystone():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_HKEYSTONE);
}

//--------------------------------------------------------------
void ofxCLEye::setVerticalKeystone(int vKeystone){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setVerticalKeystone():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(vKeystone < -500 || vKeystone > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setVerticalKeystone():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_VKEYSTONE, vKeystone);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "VERTICAL KEYSTONE: " << vKeystone;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setVerticalKeystone(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getVerticalKeystone(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getVerticallKeystone():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_VKEYSTONE);
}




//--------------------------------------------------------------
void ofxCLEye::setXOffset(int xOffset){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setXOffset():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(xOffset < -500 || xOffset > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setXOffset():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_XOFFSET, xOffset);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "X OFFSET: " << xOffset;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setXOffset(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getXOffset(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getXOffset():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_XOFFSET);
}

//--------------------------------------------------------------
void ofxCLEye::setYOffset(int yOffset){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setYOffset():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(yOffset < -500 || yOffset > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setYOffset():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_YOFFSET, yOffset);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "Y OFFSET: " << yOffset;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setYOffset(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getYOffset(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getYOffset():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_YOFFSET);
}

//--------------------------------------------------------------
void ofxCLEye::setRotation(int rotation){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setRotation():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(rotation < -500 || rotation > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setRotation():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_ROTATION, rotation);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "ROTATION: " << rotation;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setRotation(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getRotation(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getRotation():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_ROTATION);
}

//--------------------------------------------------------------
void ofxCLEye::setZoom(int zoom){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setZoom():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(zoom < -500 || zoom > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setZoom():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_ZOOM, zoom);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "ZOOM: " << zoom;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setZoom(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getZoom(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getZoom():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_ZOOM);
}

//--------------------------------------------------------------
void ofxCLEye::setLensCorrection1(int correction){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection1():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(correction < -500 || correction > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION1, correction);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "LENS CORRECTION 1: " << correction;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection1(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getLensCorrection1(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getLensCorrection1():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_LENSCORRECTION1);
}

//--------------------------------------------------------------
void ofxCLEye::setLensCorrection2(int correction){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection2():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(correction < -500 || correction > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection2():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION2, correction);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "LENS CORRECTION 2: " << correction;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection2(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getLensCorrection2(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getLensCorrection2():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_LENSCORRECTION2);
}

//--------------------------------------------------------------
void ofxCLEye::setLensCorrection3(int correction){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection3():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(correction < -500 || correction > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection3():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_LENSCORRECTION3, correction);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "LENS CORRECTION 3: " << correction;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensCorrection3(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getLensCorrection3(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getLensCorrection3():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_LENSCORRECTION3);
}

//--------------------------------------------------------------
void ofxCLEye::setLensBrightness(int brightness){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensBrightness():" << OFX_CLEYE_GRABBER_ERROR;
		return;
	}

	if(brightness < -500 || brightness > 500){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensBrightness():" << OFX_CLEYE_INVALID_VALUE;
		return;
	}

	bool success = CLEyeSetCameraParameter(cam, CLEYE_LENSBRIGHTNESS, brightness);
	if(success){
		ofLogVerbose(OFX_CLEYE_MODULE_NAME) << "LENS BRIGHTNESS: " << brightness;
	}
	else{
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "setLensBrightness(): " << OFX_CLEYE_SET_PROPERTY_ERROR;
	}
}

//--------------------------------------------------------------
int ofxCLEye::getLensBrightness(){
	if(!initialized){
		ofLogWarning(OFX_CLEYE_MODULE_NAME) << "getLensBrightness():" << OFX_CLEYE_GET_PROPERTY_ERROR;
		return false;
	}

	return CLEyeGetCameraParameter(cam, CLEYE_LENSBRIGHTNESS);
}


//--------------------------------------------------------------
float ofxCLEye::getWidth(){
	return (float)width;
}

//--------------------------------------------------------------
float ofxCLEye::getHeight(){
	return (float)height;
}

//--------------------------------------------------------------
unsigned char * ofxCLEye::getPixels(){
	return pixels.getPixels();
}

//--------------------------------------------------------------
ofPixelsRef ofxCLEye::getPixelsRef(){
	return pixels;
}

//--------------------------------------------------------------
ofTexture & ofxCLEye::getTextureReference(){
	return texture;
}

//--------------------------------------------------------------
bool ofxCLEye::isFrameNew(){
	return newFrame;
}

//--------------------------------------------------------------
bool ofxCLEye::isInitialized(){
	return initialized;
}

//--------------------------------------------------------------
void ofxCLEye::update(){
	if(!initialized){
		return;
	}

	newFrame = false;
	bool success = false;

	if(usingThread){
		success = true;
	}
	else{
		success = CLEyeCameraGetFrame(cam, (PBYTE)viPixels);
	}

	if(success){
		newFrame = true;

		if(colorMode == CLEYE_MONO_PROCESSED){
			pixels.setFromPixels(viPixels, width, height, (colorMode == CLEYE_MONO_PROCESSED) ? 1 : 4);
		}
		else{
			for(int i = 0; i < width * height; i++){
				pixels[i * 3 + 0] = viPixels[i * 4 + 2];
				pixels[i * 3 + 1] = viPixels[i * 4 + 1];
				pixels[i * 3 + 2] = viPixels[i * 4 + 0];
			}
		}

		if(usingTexture){
			texture.loadData(pixels.getPixels(), width, height, (colorMode == CLEYE_MONO_PROCESSED) ? GL_LUMINANCE : GL_RGB);
		}

		if(usingThread){
			unlock();
		}
	}
}

//--------------------------------------------------------------
void ofxCLEye::close(){
	if(initialized){
		if(usingThread){
			stopThread();
		}

		CLEyeCameraStop(cam);
		CLEyeDestroyCamera(cam);
		cam = NULL;
		delete[] viPixels;
		pixels.clear();

		if(usingTexture){
			texture.clear();
		}

		initialized = false;
	}
}

//------------------------------------------------------------------
void ofxCLEye::threadedFunction(){
	while(isThreadRunning()){
		if(lock()){
			CLEyeCameraGetFrame(cam, (PBYTE)viPixels);
			unlock();
		}
	}
}

//-------------------------------------------------------------------
bool ofxCLEye::setPixelFormat(ofPixelFormat pixelFormat){
	return false;
}

//--------------------------------------------------------------
ofPixelFormat ofxCLEye::getPixelFormat(){
	return (colorMode == CLEYE_MONO_PROCESSED) ? OF_PIXELS_MONO : OF_PIXELS_RGB;
}






