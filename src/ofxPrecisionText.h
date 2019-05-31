#pragma once
#include "ofxHersheyFont.h"


struct ofxPrecisionTextHyperlink {
    int start;
    int end;
    string url;
};


struct ofxPrecisionTextChar {
    string letter;
    float fontSize;
    bool isLineEnd;
    bool isBold;
    bool isItalic;
    bool isLink;
    int isHeading;
    ofRectangle bounds;
};

struct ofxPrecisionStructure {
    
    vector<int> h1;
    vector<int> h2;
    vector<int> h3;
    vector<int> italic;
    vector<int> bold;
    vector<ofxPrecisionTextHyperlink> links;
    
    string text;
    int inSize;
    int outSize;
    int removed;
    
    ofRectangle bounds;
    vector<ofxPrecisionTextChar> chars;
};

struct ofxPrecisionTextRegex {
    string toBeReplaced;
    int originalStart;
    int size;
    int start;
    int end;
    string match;
    int type;
};


#include "parseMarkdown.h"


struct ofxPrecisionSettings {
    
    bool markdown;
    
    int fontIndex;
    float headingScale;
    
    ofColor strokeColor;
    ofColor linkColor;
    
    bool pixelAligned;
    int numSamples;
    
    float fontSize;
    float lineHeight;
    float letterSpacing;
    float strokeWidth;
    
    int horizontalAlign;
    int verticalAlign;
    
    ofxPrecisionSettings() {
        
        markdown = true;
        
        fontIndex = 0;
        headingScale = 2;
        
        strokeColor = ofColor(255);
        linkColor = ofColor(0,255,255);
        
        pixelAligned = false;
#ifdef TARGET_RASPBERRY_PI
        numSamples = 0;
#else
        numSamples = 8;
#endif
        
        fontSize = 14;
        strokeWidth = 1.2;
        lineHeight = 1;
        letterSpacing = 0;
        
        horizontalAlign = -1;
        verticalAlign = 1;
    }
};


class ofxPrecisionText {
private:
    
    ofxPrecisionSettings s;
    float dpi;
    int fboType;
    bool samplesChanged;
    bool shouldRedraw;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    vector<string> splitString(int fromChar, string text, vector<int>);
    
    bool allocateFbo(string fboKey, string text, ofRectangle boundingBox, bool isPoint);
    
    bool hasLink(vector<ofxPrecisionTextHyperlink> links, int i, ofxPrecisionTextHyperlink & link);
    bool hasIndex(vector<int> indexes, int i);
    vector<int> regexReplace(string & text, string reg);
    
    std::map<string, ofxPrecisionStructure> structCache;
    std::map<string, ofFbo *> fboCache;
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, string> markdownCache;
    
    string defineFont(float fSize);
    string getFboKey(string text);
    
    ofRectangle getBounds(string text, float fSize, float x, float y);
    void drawString(string text, float fSize, float xx, float yy);
    
    ofxPrecisionStructure drawFbo(string text, ofRectangle boundingBox, bool dontDraw = false, bool isPoint = false);
    ofxPrecisionStructure generateStructure(string text, ofRectangle boundingBox, bool dontDraw = false, bool isPoint = false);
    
public:
    vector<string> fontList;
    
    /*-- Force redraw on FBO cache --*/
    
    void flagRedraw();
    void clearFboCache();
    
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
