/*
 *  ofxPrecisionText
 *
 *  Created by Gilbert Sinnott, www.autr.tv, June 2019
 *
 */

#pragma once
#include "ofxHersheyFont.h"
#include "ofxPrecisionDefinitions.h"

class ofxPrecisionText {
private:
    
    ofxPrecisionSettings s;
    int fboType;
    bool samplesChanged;
    bool shouldRedraw;
    ofFbo fbo;
    
    vector<string> splitString(int fromChar, string text, vector<int>);
    
    bool hasLink(vector<ofxPrecisionTextHyperlink> links, int i, ofxPrecisionTextHyperlink & link);
    bool hasIndex(vector<int> indexes, int i);
    vector<int> regexReplace(string & text, string reg);
    
    std::map<string, ofxPrecisionStructure> structCache;
    std::map<string, ofTexture> texCache;
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, string> markdownCache;
    
    string defineFont(float fSize);
    string getTextureKey(string text);
    string getMarkdownKey(string text);
    string getStructureKey(string text);
    
    ofRectangle getBounds(string text, float fSize, float x, float y);
    void drawString(string text, float fSize, float xx, float yy);
    
    ofxPrecisionStructure drawStructure(ofxPrecisionStructure structure, ofRectangle boundingBox, bool dontDraw = false, bool isPoint = false);
    ofxPrecisionStructure generateStructure(string text, ofRectangle boundingBox, bool dontDraw = false, bool isPoint = false);
    
public:
    ofxHersheyFont hershey;
    ofEvent<ofxPrecisionTextChar &> charBegin;
    ofEvent<ofxPrecisionTextChar &> charEnd;
    vector<string> fontList;
    
    /*-- Force redraw on FBO cache --*/
    
    void flagRedraw();
    void clearCache();
    
    /*-- Setup w. optional TTFs --*/
    
    void setup(string fontLocation = "");
    
    /*-- ofxPrecisionSettings --*/
    
    void set(ofxPrecisionSettings settings);
    
    /*-- Draw from ofPoint --*/
    
    ofxPrecisionStructure draw(string text, glm::vec3 originPoint, ofxPrecisionSettings settings);
    ofxPrecisionStructure draw(string text, ofPoint originPoint, ofxPrecisionSettings settings);
    
    /*-- Draw from ofRectangle --*/
    
    ofxPrecisionStructure draw(string text, int x, int y, int width, int height, ofxPrecisionSettings settings);
    ofxPrecisionStructure draw(string text, ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint = false);
    
    
};
