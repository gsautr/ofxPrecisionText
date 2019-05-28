#pragma once
#include "ofxHersheyFont.h"

struct ofxPrecisionTextHyperlink {
    int start;
    int end;
    string url;
};

struct ofxPrecisionTextStructure {
    vector<int> lineBreakIndexes;
    vector<int> boldIndexes;
    vector<int> italicIndexes;
    vector<int> newLines;
    ofRectangle boundingBox;
    vector<ofxPrecisionTextHyperlink> hyperLinks;
};

struct ofxPrecisionTextRegex {
    int start;
    int size;
    string match;
};

class ofxPrecisionText {
private:
    
    bool isBold, isItalic, isHyperlink;
    bool samplesChanged;
    int cacheCharLimit;
    float hersheyStroke;
    float hersheyBaseline;
    ofColor strokeColor;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    bool shouldRedraw;
    vector<int> regexReplace(string & text, string reg);
    vector<ofxPrecisionTextRegex>  getMatchedStrings (string subject, string reg);
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo *> fboCache;
    
    string getFboKey(string text); /*-- Get unique hash for FBO cache --*/
    
    string defineFont(); /*-- Loads TTF to cache and returns font cache key --*/

    ofRectangle getBounds(string text, int x, int y); /*-- Get bounds for single text string --*/
    
    void drawString(string fontKey, string text, int xx, int yy);
    
    void formatParagraphs(string & text, ofRectangle & boundingBox); /*-- Format a text string into paragraphs and line breaks, and update bounding box to reflect this --*/
    
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
