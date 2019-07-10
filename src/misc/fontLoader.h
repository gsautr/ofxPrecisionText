/*
 *  ofxPrecisionFont
 *  Gilbert Sinnott
 *  www.autr.tv, June 2019
 *
 *  ofxHersheyFont originally created by Tobias Zimmer, August 2016.
 *  www.tobiaszimmer.net
 *
 *  Fonts originally developed by Dr. Allen V. Hershey in 1967.
 *  Font vectors made available by Paul Bourke.
 *  paulbourke.net/dataformats/hershe
 *
 */

#pragma once
#include "ofMain.h"

struct ofxPrecisionFontChar {
    int width;
    int height;
    ofPath path;
    ofPolyline polyline;
};

struct ofxPrecisionFontStructure {
    int maxCharWidth;
    int maxCharHeight;
    map<int, ofxPrecisionFontChar> chars;
    map<int, ofPath> paths;
    map<int, ofPolyline> polylines;
};


class ofxPrecisionFont {
	public:
		ofxPrecisionFont();
    
        ofPath & getPath(int i);
        void setFont(int i);
        int getNumFonts();
        void setFontList(vector<string> list);
        vector<string> getFontList();
    
        void setItalic(bool b, float slant = 3);
        float getSimplex(int a, int b, int aa = -1, int bb = -1);

        ofRectangle getBounds(string str, float fSize, int x, int y);
		float getWidth(string str);
		float getHeight();

		ofColor color;
        float scale;
        float italic;
        float stroke;
        float angle;
        bool centered;

	private:
        map<string, ofxPrecisionFontStructure> fontCache;
        vector<vector<int>> simplex;
        map<string, vector<vector<int>>> hersheyFonts;
        vector<string> names;
        int fontIndex;
};


