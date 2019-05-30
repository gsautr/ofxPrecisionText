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
    bool isBold;
    bool isItalic;
    bool isLink;
    int isHeading;
    bool isNewline;
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

class ofxPrecisionText {
private:
    
    float dpi;
    int fboType;
    
    ofShader italicShader;
    vector<string> splitString(int fromChar, string text, vector<int>);
    bool samplesChanged;
    int cacheCharLimit;
    float headingScale;
    float hersheyStroke;
    float hersheyBaseline;
    ofColor strokeColor;
    ofColor linkColor;
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

    bool pixelAligned;
    int numSamples;
    float fontSize;
    int fontIndex;
    float lineHeight;
    float letterSpacing;
    int horizontalAlign, verticalAlign;
    
    void flagRedraw(); /*-- Force redraw on FBO cache --*/
    
    void setLineHeight(float lHeight);
    
    void setup(string fontLocation = "");  /*-- Setup + optionally load a folder of TTFs --*/
    
    void setFboSamples(int fboSamples = 8);  /*-- Set number of antialiasing passes --*/
    
    void clearFboCache();
    
    void setHeadingScale(float hScale);
    
    void setFont(int index); /*-- Set font via index --*/
    
    void setColor(ofColor color);
    
    void setStroke(float stroke);
    
    void setFontSize(float fSize);

    ofxPrecisionTextStructure draw(string text, glm::vec3 originPoint, int horzAlign = -1, int vertAlign = 0);
    ofxPrecisionTextStructure draw(string text, ofPoint originPoint, int horzAlign = -1, int vertAlign = 0);
    ofxPrecisionTextStructure draw(string text, ofRectangle boundingBox, int horzAlign = -1, int vertAlign = 0, bool isPoint = false);
    
    
    
};
