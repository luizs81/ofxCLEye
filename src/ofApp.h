#pragma once

#include "ofMain.h"
#include "CLEyeCameraCapture.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		/*ofApp();
		~ofApp() {};
*/
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		void exposureChanged(int &exposureValue);

	private:
		PS3EyeCamera cam;
		ofImage oImage;
		cv::Mat bgra;
		cv::Mat bgr;
		ofxPanel gui;
		ofParameter<int> exposure;
};
