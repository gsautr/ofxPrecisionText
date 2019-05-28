#pragma once
#include "ofxHersheyFont.h"

class ofxPrecisionText {
private:
    
    bool samplesChanged;
    int cacheCharLimit;
    float hersheyStroke;
    float hersheyBaseline;
    ofColor strokeColor;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    bool shouldRedraw;
    
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo *> fboCache;
    
    string getFboKey(string text); /*-- Get unique hash for FBO cache --*/
    
    string defineFont(); /*-- Loads TTF to cache and returns font cache key --*/

    ofRectangle getBounds(string text, int x, int y); /*-- Get bounds for single text string --*/
    
    void getParagraph(string text, ofRectangle boundingBox, vector<int> & lineBreaks, int & totalHeight, int & totalWidth, int & iterateChars); /*-- Produces total width + height, and line breaks for a paragraph string --*/
    
    vector<int> formatParagraphs(string & text, ofRectangle & boundingBox); /*-- Format a text string into paragraphs and line breaks, and update bounding box to reflect this --*/
    
    ofRectangle drawFbo(string text, ofRectangle boundingBox); /*-- Draw text string with line breaks and formatting --*/
    
public:
    vector<string> fontList;

    int numSamples;
    float fontSize;
    int fontIndex;
    float lineHeight;
    int horizontalAlign, verticalAlign;
    
    void flagRedraw(); /*-- Force redraw on FBO cache --*/
    
    void setLineHeight(float lHeight);
    
    void setup(string fontLocation = "");  /*-- Setup + optionally load a folder of TTFs --*/
    
    void setFboSamples(int fboSamples = 8);  /*-- Set number of antialiasing passes --*/
    
    void clearFboCache();
    
    void setFont(int index); /*-- Set font via index --*/
    
    void setColor(ofColor color);
    
    void setStroke(float stroke);

    ofRectangle draw(string text, glm::vec3 originPoint, float fSize, int horzAlign = -1, int vertAlign = 0);
    ofRectangle draw(string text, ofPoint originPoint, float fSize, int horzAlign = -1, int vertAlign = 0);
    ofRectangle draw(string text, ofRectangle boundingBox, float fSize, int horzAlign = -1, int vertAlign = 0);
    
    
    
};
