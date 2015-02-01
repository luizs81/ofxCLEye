#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCLEye.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// GUI functions
		void autoGainChanged(bool & _autoGain);
		void gainChanged(int & _gain);
		void autoExposureChanged(bool & _autoExposure);
		void exposureChanged(int & _exposure);
		void autoWhiteBalanceChanged(bool & _autoWhiteBalance);
		void whiteBalanceRedChanged(int & _red);
		void whiteBalanceGreenChanged(int & _green);
		void whiteBalanceBlueChanged(int & _red);
		void verticalFlipChanged(bool & _vFlip);
		void horizontalFlipChanged(bool & _hFlip);
		void verticalKeystoneChanged(int & _vKeystone);
		void horizontalKeystoneChanged(int & _hKeystone);
		void xOffsetChanged(int & vKeystone);
		void yOffsetChanged(int & _hKeystone);
		void rotationChanged(int & _rotation);
		void zoomChanged(int & _zoom);
		void lensCorrection1Changed(int & _correction);
		void lensCorrection2Changed(int & _correction);
		void lensCorrection3Changed(int & _correction);
		void lensBrightnessChanged(int & _brightness);

		// GUI controls
		ofxPanel gui;
		ofxToggle autoGain;
		ofParameter <int> gain;
		ofxToggle autoExposure;
		ofParameter <int> exposure;
		ofxToggle autoWhiteBalance;
		ofParameter <int> whiteBalanceRed;
		ofParameter <int> whiteBalanceGreen;
		ofParameter <int> whiteBalanceBlue;
		ofxToggle verticalFlip;
		ofxToggle horizontalFlip;
		ofParameter <int> verticalKeystone;
		ofParameter <int> horizontalKeystone;
		ofParameter <int> xOffset;
		ofParameter <int> yOffset;
		ofParameter <int> rotation;
		ofParameter <int> zoom;
		ofParameter <int> lensCorrection1;
		ofParameter <int> lensCorrection2;
		ofParameter <int> lensCorrection3;
		ofParameter <int> lensBrightness;

		ofxCLEye camera;
		ofImage image;
};

