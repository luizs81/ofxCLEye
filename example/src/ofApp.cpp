#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);

	// Camera
	int cameras = camera.getDeviceCount();
	if(cameras > 0){
		camera.initGrabber(640, 480, 0, 45, true, false, false);
	}

	// GUI
	autoGain.addListener(this, & ofApp::autoGainChanged);
	gain.addListener(this, & ofApp::gainChanged);

	autoExposure.addListener(this, & ofApp::autoExposureChanged);
	exposure.addListener(this, & ofApp::exposureChanged);
	autoWhiteBalance.addListener(this, & ofApp::autoWhiteBalanceChanged);
	whiteBalanceRed.addListener(this, & ofApp::whiteBalanceRedChanged);
	whiteBalanceGreen.addListener(this, & ofApp::whiteBalanceGreenChanged);
	whiteBalanceBlue.addListener(this, & ofApp::whiteBalanceBlueChanged);
	verticalFlip.addListener(this, & ofApp::verticalFlipChanged);
	horizontalFlip.addListener(this, & ofApp::horizontalFlipChanged);
	verticalKeystone.addListener(this, & ofApp::verticalKeystoneChanged);
	horizontalKeystone.addListener(this, & ofApp::horizontalKeystoneChanged);
	xOffset.addListener(this, & ofApp::xOffsetChanged);
	yOffset.addListener(this, & ofApp::yOffsetChanged);
	rotation.addListener(this, & ofApp::rotationChanged);
	zoom.addListener(this, & ofApp::zoomChanged);
	lensCorrection1.addListener(this, & ofApp::lensCorrection1Changed);
	lensCorrection2.addListener(this, & ofApp::lensCorrection2Changed);
	lensCorrection3.addListener(this, & ofApp::lensCorrection3Changed);
	lensBrightness.addListener(this, & ofApp::lensBrightnessChanged);

	gui.setup("PlayStation Eye controls");
	gui.setShape(640, 0, 300, 20);
	gui.setDefaultHeight(20);
	gui.setDefaultWidth(300);
	gui.add(autoGain.setup("Auto Gain", false));
	gui.add(gain.set("Gain", 0, 0, 79));
	gui.add(autoExposure.setup("Auto Exposure", false));
	gui.add(exposure.set("Exposure", 250, 0, 511));
	gui.add(autoWhiteBalance.setup("Auto White Balance", true));
	gui.add(whiteBalanceRed.set("White Balance (Red)", 127, 0, 255));
	gui.add(whiteBalanceGreen.set("White Balance (Green)", 127, 0, 255));
	gui.add(whiteBalanceBlue.set("White Balance (Blue)", 127, 0, 255));
	gui.add(verticalFlip.setup("Vertical Flip", false));
	gui.add(horizontalFlip.setup("Horizontal Flip", false));
	gui.add(verticalKeystone.set("Vertical Keystone", 0, -500, 500));
	gui.add(horizontalKeystone.set("Horizontal Keystone", 0, -500, 500));
	gui.add(xOffset.set("X OFFSET", 0, -500, 500));
	gui.add(yOffset.set("Y OFFSET", 0, -500, 500));
	gui.add(rotation.set("Rotation", 0, -500, 500));
	gui.add(zoom.set("Zoom", 0, -500, 500));
	gui.add(lensCorrection1.set("Lens Correction 1", 0, -500, 500));
	gui.add(lensCorrection2.set("Lens Correction 2", 0, -500, 500));
	gui.add(lensCorrection3.set("Lens Correction 3", 0, -500, 500));
	gui.add(lensBrightness.set("Lens Brightness", 0, -500, 500));
}

//--------------------------------------------------------------
void ofApp::update(){
	camera.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	image.setFromPixels(camera.getPixelsRef());
	image.draw(0,0);

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);

	//GUI
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::autoGainChanged(bool & _autoGain){
	camera.setAutoGain(_autoGain);
}

//--------------------------------------------------------------
void ofApp::gainChanged(int & _gain){
	camera.setGain(_gain);
}

//--------------------------------------------------------------
void ofApp::autoExposureChanged(bool & _autoExposure){
	camera.setAutoExposure(_autoExposure);
}

//--------------------------------------------------------------
void ofApp::exposureChanged(int & _exposure){
	camera.setExposure(_exposure);
}

//--------------------------------------------------------------
void ofApp::autoWhiteBalanceChanged(bool & _autoWhiteBalance){
	camera.setAutoWhiteBalance(_autoWhiteBalance);
}

//--------------------------------------------------------------
void ofApp::whiteBalanceRedChanged(int & _red){
	camera.setWhiteBalanceRed(_red);
}

//--------------------------------------------------------------
void ofApp::whiteBalanceGreenChanged(int & _green){
	camera.setWhiteBalanceGreen(_green);
}

//--------------------------------------------------------------
void ofApp::whiteBalanceBlueChanged(int & _blue){
	camera.setWhiteBalanceBlue(_blue);
}

//--------------------------------------------------------------
void ofApp::verticalFlipChanged(bool & _vflip){
	camera.setVerticalFlip(_vflip);
}

//--------------------------------------------------------------
void ofApp::horizontalFlipChanged(bool & _hflip){
	camera.setHorizontalFlip(_hflip);
}

//--------------------------------------------------------------
void ofApp::verticalKeystoneChanged(int & _vKeystone){
	camera.setVerticalKeystone(_vKeystone);
}

//--------------------------------------------------------------
void ofApp::horizontalKeystoneChanged(int & _hKeystone){
	camera.setHorizontalKeystone(_hKeystone);
}

//--------------------------------------------------------------
void ofApp::xOffsetChanged(int & _xOffset){
	camera.setXOffset(_xOffset);
}

//--------------------------------------------------------------
void ofApp::yOffsetChanged(int & _yOffset){
	camera.setYOffset(_yOffset);
}

//--------------------------------------------------------------
void ofApp::rotationChanged(int & _rotation){
	camera.setRotation(_rotation);
}

//--------------------------------------------------------------
void ofApp::zoomChanged(int & _zoom){
	camera.setZoom(_zoom);
}

//--------------------------------------------------------------
void ofApp::lensCorrection1Changed(int & _correction){
	camera.setLensCorrection1(_correction);
}

//--------------------------------------------------------------
void ofApp::lensCorrection2Changed(int & _correction){
	camera.setLensCorrection2(_correction);
}

//--------------------------------------------------------------
void ofApp::lensCorrection3Changed(int & _correction){
	camera.setLensCorrection3(_correction);
}

//--------------------------------------------------------------
void ofApp::lensBrightnessChanged(int & _brightness){
	camera.setLensBrightness(_brightness);
}





