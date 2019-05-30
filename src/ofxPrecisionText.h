#pragma once
#include "ofxHersheyFont.h"
#define STRINGIFY(A) string("#version 150\n")+#A

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
    bool isNewline;
    ofRectangle bounds;
};

struct ofxPrecisionTextStructure {
    vector<int> spaceIndexes;
    vector<int> lineBreakIndexes;
    vector<int> boldIndexes;
    vector<int> italicIndexes;
    vector<int> newLines;
    ofRectangle bounds;
    vector<ofxPrecisionTextChar> chars;
    vector<ofxPrecisionTextHyperlink> hyperLinks;
};

struct ofxPrecisionTextRegex {
    int start;
    int size;
    string match;
};

class ofxPrecisionText {
private:
    
    int fboType;
    
    ofShader italicShader;
    vector<string> splitString(int fromChar, string text, vector<int>);
    bool samplesChanged;
    int cacheCharLimit;
    float hersheyStroke;
    float hersheyBaseline;
    ofColor strokeColor;
    ofColor linkColor;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    bool allocateFbo(string fboKey, string text, ofRectangle boundingBox) {
        
        auto it = fboCache.find(fboKey);
        if (it == fboCache.end()) {
            ofFbo * fbo = new ofFbo();
            structCache[fboKey] = drawFbo(text, boundingBox, true);;
            fbo->allocate(structCache[fboKey].bounds.width, structCache[fboKey].bounds.height, fboType, numSamples);
            samplesChanged = false;
            ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << numSamples << " samples, " << structCache[fboKey].bounds.width << " x " << structCache[fboKey].bounds.height;
            fboCache[fboKey] = fbo;
            return true;
        } else {
            return false;
        }
    }
    
    vector<int> findInternalIndices(string t, int start, vector<int> search);
    bool shouldRedraw;
    vector<int> regexReplace(string & text, string reg);
    vector<ofxPrecisionTextRegex>  getMatchedStrings (string subject, string reg);
    
    std::map<string, ofxPrecisionTextStructure> structCache;
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo *> fboCache;
    
    string getFboKey(string text); /*-- Get unique hash for FBO cache --*/
    
    string defineFont(); /*-- Loads TTF to cache and returns font cache key --*/

    ofRectangle getBounds(string text, float x, float y); /*-- Get bounds for single text string --*/
    bool hasLink(vector<ofxPrecisionTextHyperlink> links, int i, ofxPrecisionTextHyperlink & link);
    bool hasIndex(vector<int> indexes, int i);
    void drawString(string fontKey, string text, float xx, float yy);
    
    
    ofxPrecisionTextStructure drawFbo(string text, ofRectangle boundingBox, bool dontDraw = false, bool useHorizontal = false, bool isPoint = false); /*-- Draw text string with line breaks and formatting --*/
    
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
    
    void setFont(int index); /*-- Set font via index --*/
    
    void setColor(ofColor color);
    
    void setStroke(float stroke);

    ofxPrecisionTextStructure draw(string text, glm::vec3 originPoint, float fSize, int horzAlign = -1, int vertAlign = 0);
    ofxPrecisionTextStructure draw(string text, ofPoint originPoint, float fSize, int horzAlign = -1, int vertAlign = 0);
    ofxPrecisionTextStructure draw(string text, ofRectangle boundingBox, float fSize, int horzAlign = -1, int vertAlign = 0, bool isPoint = false);
    
    
    
};
