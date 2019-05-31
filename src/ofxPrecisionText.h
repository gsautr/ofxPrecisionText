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

struct ofxPrecisionTextStructure {

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


struct ofxPrecisionTextSettings {
    
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
    
    ofxPrecisionTextSettings() {
        
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
    
    float dpi;
    int fboType;
    
    ofShader italicShader;
    vector<string> splitString(int fromChar, string text, vector<int>);
    bool samplesChanged;
    int cacheCharLimit;
    
    ofxPrecisionTextSettings s;
    
    
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    bool allocateFbo(string fboKey, string text, ofRectangle boundingBox, bool isPoint);
    
    vector<int> findInternalIndices(string t, int start, vector<int> search);
    bool shouldRedraw;
    vector<int> regexReplace(string & text, string reg);
    vector<ofxPrecisionTextRegex>  getMatchedStrings (string subject, string reg);
    
    std::map<string, ofxPrecisionTextStructure> structCache;
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo *> fboCache;
    
    string getFboKey(string text); /*-- Get unique hash for FBO cache --*/
    
    string defineFont(float fSize); /*-- Loads TTF to cache and returns font cache key --*/

    ofRectangle getBounds(string text, float fSize, float x, float y); /*-- Get bounds for single text string --*/
    bool hasLink(vector<ofxPrecisionTextHyperlink> links, int i, ofxPrecisionTextHyperlink & link);
    bool hasIndex(vector<int> indexes, int i);
    void drawString(string text, float fSize, float xx, float yy);
    
    
    ofxPrecisionTextStructure drawFbo(string text, ofRectangle boundingBox, bool dontDraw = false, bool isPoint = false); /*-- Draw text string with line breaks and formatting --*/
    
public:
    vector<string> fontList;

    
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
    
    
    
    void flagRedraw(); /*-- Force redraw on FBO cache --*/
    
    void setup(string fontLocation = "");  /*-- Setup + optionally load a folder of TTFs --*/
    
    void clearFboCache();
    

    ofxPrecisionTextStructure draw(string text, glm::vec3 originPoint, ofxPrecisionTextSettings settings);
    ofxPrecisionTextStructure draw(string text, ofPoint originPoint, ofxPrecisionTextSettings settings);
    ofxPrecisionTextStructure draw(string text, int x, int y, int width, int height, ofxPrecisionTextSettings settings);
    ofxPrecisionTextStructure draw(string text, ofRectangle boundingBox, ofxPrecisionTextSettings settings, bool isPoint = false);
    
    void set(ofxPrecisionTextSettings settings);
    
};
