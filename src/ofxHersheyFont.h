/*
*  ofxHersheyFont.h
*
*  Created by Tobias Zimmer, August 2016.
*  www.tobiaszimmer.net
*
*  Font originally developed by Dr. Allen V. Hershey in 1967.
*  Font vectors made available by Paul Bourke.
*  paulbourke.net/dataformats/hershey/
*
*  A simple single line font for machines like CNC, Lasercutter, ...
*  Available characters: ASCII codes 32 to 126.
*
*/

#include "ofMain.h"

class ofxHersheyFont {
	public:
		ofxHersheyFont();

        void draw(string stringValue, float x, float y);
        void draw(string stringValue, float x, float y, float str);
        void draw(string stringValue, float x, float y, float str, ofColor col);
        void draw(string stringValue, ofRectangle rectangle, int horizontalAlign, int verticalAlign, float str, ofColor col);
		void setColor(ofColor c);
        void setScale(float s);
        void setStroke(float s);
        ofRectangle getBounds(string stringValue, int x, int y);
		float getWidth(string stringValue);
		float getHeight();
		ofPath getPath(string stringValue, float scale);
		ofPath getPath(string stringValue, float xPos, float yPos, float scale);

		ofColor color;
        float scale;
        float stroke;
        float angle;
        bool centered;

	private:
		void drawChar(int asciiValue, float stroke);
};


