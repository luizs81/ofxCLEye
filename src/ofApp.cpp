#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	int numCams = CLEyeGetCameraCount();

	if(numCams == 0)
	{
		printf("No PS3Eye cameras detected\n");
		return;
	}

	char windowName[64];
	// Query unique camera uuid
	GUID guid = CLEyeGetCameraUUID(0);
	printf("Camera %d GUID: [%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x]\n", 
		0+1, guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	sprintf(windowName, "Camera Window %d", 0+1);
	// Create camera capture object
	// Randomize resolution and color mode
	cam.start(guid, CLEYE_COLOR_PROCESSED, CLEYE_VGA, 45);
	printf("Starting capture on camera %d\n", 0+1);
	//cam[i]->StartCapture();

	bgra = cv::Mat::zeros(cv::Size(640, 480), CV_8UC4);
	bgr = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
	oImage.allocate(640, 480, OF_IMAGE_COLOR);

	exposure.addListener(this, &ofApp::exposureChanged);
	gui.setup("Control Panel");
	gui.add(exposure.set("exposure", 500, 0, 511));

}

//--------------------------------------------------------------
void ofApp::update(){

	// If you want to use threads
	//cam.lock();

	cam.updateImage();
	bgra = cam.pCapImage;

	cv::cvtColor(bgra, bgr, CV_BGRA2BGR);
	oImage.setFromPixels(bgr.data, bgr.cols, bgr.rows, OF_IMAGE_COLOR, false);

	//cam.unlock();
}

//--------------------------------------------------------------
void ofApp::draw(){

	oImage.draw(0,0);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

	if(key == 'a'){
		cam.IncrementCameraParameter(CLEYE_EXPOSURE);
		std::cout << "Increment exposure" << std::endl;
	}

	if(key == 's'){
		cam.DecrementCameraParameter(CLEYE_EXPOSURE);
		std::cout << "Decrement exposure" << std::endl;
	}

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
void ofApp::exit(){

	cam.stop();
}


//--------------------------------------------------------------
void ofApp::exposureChanged(int & exposureValue){

	//std::cout << exposureValue << std::endl;
	cam.setCameraParameter(CLEYE_EXPOSURE, exposureValue);
}