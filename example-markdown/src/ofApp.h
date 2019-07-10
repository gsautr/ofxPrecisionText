#pragma once

#include "ofMain.h"
#include "ofxPrecisionDocument.h"
#include "ofxPrecisionInput.h"
#include "ofxTimeMeasurements.h"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(ofKeyEventArgs & e);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        ofxPrecisionDocument doc;
        ofxPrecisionInput input;
        ofxPrecisionSettings settings;
        ofEventListener inputUpdated;
        string markdownText;
};
