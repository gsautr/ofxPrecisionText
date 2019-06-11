/*
 *  ofxHersheyFont.cpp
 *
 *  Branched by Gilbert Sinnott, www.autr.tv, June 2019
 *
 *  + parser for .dhf files (see dhf-parser)
 *  + simplex Hershey font sets (inc. cyrillic, greek, japanese)
 *  + integration w. ofxPrecisionText
 *
 *  ofxHersheyFont originally created by Tobias Zimmer, August 2016.
 *  www.tobiaszimmer.net
 *
 *  Fonts originally developed by Dr. Allen V. Hershey in 1967.
 *  Font vectors made available by Paul Bourke.
 *  paulbourke.net/dataformats/hershey
 *
 */

#pragma once
#include "ofMain.h"

class ofxHersheyFont {
	public:
		ofxHersheyFont();
    
        ofPath getPath(int i);
        void setFont(int i);
        int getNumFonts();
        void setFontList(vector<string> list);
        vector<string> getFontList();
    
        void setItalic(bool b, float slant = 3);
        float getSimplex(int a, int b, int aa = -1, int bb = -1);

        void draw(string stringValue, float x, float y);
        void draw(string stringValue, float x, float y, float str);
        void draw(string stringValue, float x, float y, float str, ofColor col);
        void draw(string stringValue, ofRectangle rectangle, int horizontalAlign, int verticalAlign, float str, ofColor col);
    
        ofRectangle getBounds(string stringValue, float fSize, int x, int y);
		float getWidth(string stringValue);
		float getHeight();
		ofPath getPath(string stringValue, float scale);
		ofPath getPath(string stringValue, float xPos, float yPos, float scale);

		ofColor color;
        float scale;
        float italic;
        float stroke;
        float angle;
        bool centered;

	private:
        map<string, map<int, ofPath>> charCache;
        map<string, int> heightCache;
        map<string, map<int, int>> widthCache;
        vector<vector<int>> simplex;
        map<string, vector<vector<int>>> fonts;
        vector<string> names;
        int fontIndex;
		void drawChar(int asciiValue, float stroke);
};


