#pragma once


struct ofxPrecisionTextHyperlink {
    int start;
    int end;
    string url;
};


struct ofxPrecisionTextChar {
    ofColor color;
    string letter;
    float fontSize;
    bool isLineEnd;
    bool isBold;
    bool isItalic;
    bool isLink;
    int isHeading;
    ofRectangle bounds;
    int index;
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
    float dpi;
    bool cache;
    
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
    float boldWidth;
    
    int horizontalAlign;
    int verticalAlign;
    
    ofxPrecisionSettings() {
        
        markdown = true;
        dpi = 1.0;
        cache = true;
        
        fontIndex = 0;
        headingScale = 2;
        
        strokeColor = ofColor(255);
        linkColor = ofColor(0,255,255);
        
        pixelAligned = true;
#ifdef TARGET_RASPBERRY_PI
        numSamples = 0;
#else
        numSamples = 8;
#endif
        
        fontSize = 14;
        strokeWidth = 1.2;
        boldWidth = 1.5;
        lineHeight = 1.6;
        letterSpacing = 0.1;
        
        horizontalAlign = -1;
        verticalAlign = 1;
    }
};


