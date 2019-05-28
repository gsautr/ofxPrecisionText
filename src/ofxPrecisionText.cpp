
#pragma once
#include "ofxPrecisionText.h"

    
string ofxPrecisionText::getFboKey(string text) {

    /*-- Get unique hash for FBO cache --*/

    string key = fontList[fontIndex] + ofToString(fontSize) + ofToString(strokeColor.r) + ofToString(strokeColor.g) + ofToString(strokeColor.b) + ofToString(strokeColor.a) + ofToString(numSamples) + text + ofToString(horizontalAlign) + ofToString(lineHeight);
    if (fontIndex == 0 ) key += ofToString(hersheyStroke);
    return key;
}

string ofxPrecisionText::defineFont() {
    
    /*-- Loads TTF to cache and returns font cache key --*/

    if (fontIndex == 0) return "Hershey";
    string fontName = fontList[fontIndex];
    string fontKey = fontName + ofToString( fontSize );
    auto it = fontCache.find(fontKey);
    if (it == fontCache.end()) {
        ofTrueTypeFont font;
        font.load(fontName, fontSize, true, true);
        font.setLineHeight(1.0f);
        font.setLetterSpacing(1.037);
        fontCache[fontKey] = font;
    }
    return fontKey;
}

ofRectangle ofxPrecisionText::getBounds(string text, int x, int y) {

    /*-- Get bounds for single text string --*/

    ofRectangle b;
    
    if (fontIndex == 0) {
        b = hershey.getBounds(text, x, y);
    } else {
        string fontKey = defineFont();
        b = fontCache[fontKey].getStringBoundingBox(text, x, y);
    }
    
    return b;
}

void ofxPrecisionText::getParagraph(string text, ofRectangle boundingBox, vector<int> & lineBreaks, int & totalHeight, int & totalWidth, int & iterateChars) {
    
    /*-- Produces total width + height, and line breaks for a paragraph string --*/
    
    int iterateWidth = 0;
    totalHeight += (int)(fontSize * lineHeight);
    
    for (auto word : ofSplitString(text, " ")) {
        
        
        ofRectangle b = getBounds(word + "_", 0, 0);
        
        
        iterateWidth += b.width;
        if (iterateWidth > totalWidth) {
            totalWidth = iterateWidth;
        }
        if (iterateWidth > boundingBox.width) {
            iterateWidth = b.width;
            totalHeight += (int)(fontSize * lineHeight);
            lineBreaks.push_back(iterateChars);
        }
        iterateChars += word.size() + 1;
    }
}

vector<int> ofxPrecisionText::formatParagraphs(string & text, ofRectangle & boundingBox) {
    
    /*-- Format a text string into paragraphs and line breaks, and update bounding box to reflect this --*/

    int totalHeight = 0;
    int totalWidth = 0;
    int iterateChars = 0;
    vector<int> lineBreaks = {0};
    
    string originalText = text;
    
    std::string::size_type i = 0u;
    while((i = text.find("\n")) != std::string::npos){
        text = text.replace(i, 1, " ");
    }
    
        
    vector<string> paragraphs  = ofSplitString(originalText, "\n");
    int iterPara = 0;
    for (auto paragraph : paragraphs) {
        getParagraph(paragraph, boundingBox, lineBreaks, totalHeight, totalWidth, iterateChars);
        if (paragraph != paragraphs.back()) {
            iterPara += paragraph.size() + 1;
            lineBreaks.push_back(iterPara);
        }
    }
    if (lineBreaks.size() > 1) totalWidth = boundingBox.width;
    boundingBox.width = totalWidth;
    
    float extra = 0;
    if (fontIndex != 0) {
        extra = (fontSize * 0.25) + 1;
    } else {
        extra = fontSize * 0.5;
    }
    boundingBox.height = totalHeight + (int)extra;
    return lineBreaks;
}

ofRectangle ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox) {

    /*-- Draw text string with line breaks and formatting --*/
    
    ofLogNotice("[ofxPrecisionText]") << "Drawing fbo, " << boundingBox.width << "x" << boundingBox.height;
    
    int initialWidth = getBounds(text, 0, 0).width;
    int initialHeight = getBounds("ITPBgyq", 0, 0).height;
    ofRectangle b = boundingBox;
    ofRectangle fboBox = boundingBox;
    
    
    ofFill();
    ofSetColor(strokeColor);
    string fontKey = defineFont();
    
    vector<int> lineBreaks = formatParagraphs(text, fboBox);
    
    if (lineBreaks.size() == 1) lineBreaks.push_back(text.size());
    
    for (int i = 0; i < lineBreaks.size(); i++) {
        
        int breakA = lineBreaks[i];
        int breakB = lineBreaks[i + 1] - lineBreaks[i];
        
        string line = text.substr(breakA, breakB);
        
        if (line != "") {
            
            if (line.substr(line.size() - 1, line.size()) == " ") {
                line = line.substr(0, line.size() - 1);
            }
            
            int linePix = (int)(fontSize * lineHeight);
            int extra = 0;
            
            if (fontIndex != 0) {
                extra = initialHeight * 1.75;
            } else {
                extra = initialHeight + 1;
            }
            
            ofRectangle rect = getBounds(line, 0, (linePix * i) + extra);
            
            if (horizontalAlign == 0) rect.x = (boundingBox.width - rect.width) /2;
            if (horizontalAlign == 1) rect.x = (boundingBox.width - rect.width) ;
            
            ofSetColor(strokeColor);
            if (fontIndex == 0) {
                hershey.draw(line, (int)rect.x, (int)rect.y);
            } else {
                fontCache[fontKey].drawString(line, (int)rect.x, (int)rect.y);
            }
        }
        
    }
    
    
    return fboBox;
}

void ofxPrecisionText::flagRedraw() {

    /*-- Force redraw on FBO cache --*/

    shouldRedraw = true;
}

void ofxPrecisionText::setLineHeight(float lHeight) {

    lineHeight = lHeight;
}

void ofxPrecisionText::setup(string fontLocation) {

    /*-- Setup + optionally load a folder of TTFs --*/

    shouldRedraw = false;
    samplesChanged = false;
    numSamples = 8;
    cacheCharLimit = 200;
    fontIndex = 0;
    lineHeight = 1.4;
    ofDirectory dir(fontLocation);
    dir.allowExt("ttf");
    hersheyBaseline = 31.0;

    ofTrueTypeFont::setGlobalDpi(72);
    
    dir.listDir();
    fontList.push_back("Hershey");
    for (auto & f: dir.getFiles()) {
        ofLogNotice("[ofxPrecisionText]") << "Adding TTF " << f.path();
        fontList.push_back(f.path());
    }
    if (fontList.size() == 0) ofLogNotice("[ofxPrecisionText]") << "No TTF fonts loaded";
}

void ofxPrecisionText::setFboSamples(int fboSamples) {
    
    /*-- Set number of antialiasing passes --*/

    if (fboSamples >= 0 && fboSamples <= 8) {
        numSamples = fboSamples;
        samplesChanged = true;
    }
}

void ofxPrecisionText::clearFboCache() {
    fboCache.clear();
}



void ofxPrecisionText::setFont(int index) {

    /*-- Set font via index --*/

    if (index >= 0 && index < fontList.size()) fontIndex = index;
}

void ofxPrecisionText::setColor(ofColor color) {
    strokeColor = color;
    hershey.setColor(color);
}

void ofxPrecisionText::setStroke(float stroke) {
    hersheyStroke = stroke;
    hershey.setStroke(stroke);
}
ofRectangle ofxPrecisionText::draw(string text, glm::vec3 originPoint, float fSize, int horzAlign, int vertAlign) {
    ofPoint p(originPoint.x, originPoint.y);
    ofxPrecisionText::draw(text, p, fSize, horzAlign, vertAlign);
}

ofRectangle ofxPrecisionText::draw(string text, ofPoint originPoint, float fSize, int horzAlign, int vertAlign) {
    
#ifdef TARGET_OPENGLES
    int type = GL_RGBA;
#else
    int type = GL_RGBA32F_ARB;
#endif
    
#ifdef TARGET_RASPBERRY_PI
    int samples = 0;
#else
    int sample = numSamples;
#endif
    
    ofRectangle b(0,0, 999999, 999999);
    fontSize = fSize;
    horizontalAlign = horzAlign;
    verticalAlign = vertAlign;
    hershey.setScale(fontSize *  (1.0 / hersheyBaseline));
    
    
    ofFbo * fbo;
    string fboKey = getFboKey(text);
    auto it = fboCache.find(fboKey);
    if (it == fboCache.end()) {
        fbo = new ofFbo();
        string textCopy = text;
        formatParagraphs(textCopy, b);
        ofLog() << " Allocate " << b.width << " " << b.height;
        fbo->allocate(b.width, b.height, type, numSamples);
        samplesChanged = false;
        ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << numSamples << " samples, " << originPoint.x << " x " << originPoint.y;
        fboCache[fboKey] = fbo;
        
        
    } else {
        fbo = fboCache[fboKey];
    }
    
    int xx = originPoint.x;
    int yy = originPoint.y;
    
    if (verticalAlign == 0) yy -= fbo->getHeight() / 2;
    if (verticalAlign == 1) yy -= fbo->getHeight();

    if (horizontalAlign == 0) xx -= fbo->getWidth() / 2;
    if (horizontalAlign == 1) xx -= fbo->getWidth();
    
    ofRectangle fboBox(xx,yy,fbo->getWidth(), fbo->getHeight());
    
    
    if (it == fboCache.end() || shouldRedraw) {
        
        fbo->begin();
        ofClear(255,0);
        ofSetColor(255);
        ofPushMatrix();
        string textCopy = text;
        drawFbo(textCopy, b);
        ofPopMatrix();
        fbo->end();
        shouldRedraw = false;
    }
    
    ofSetColor(255);
    fbo->draw(xx, yy);
    return fboBox;
}

ofRectangle ofxPrecisionText::draw(string text, ofRectangle boundingBox, float fSize, int horzAlign, int vertAlign) {
    
#ifdef TARGET_OPENGLES
    int type = GL_RGBA;
#else
    int type = GL_RGBA32F_ARB;
#endif
    
#ifdef TARGET_RASPBERRY_PI
    int samples = 0;
#else
    int sample = numSamples;
#endif
    
    fontSize = fSize;
    horizontalAlign = horzAlign;
    verticalAlign = vertAlign;
    hershey.setScale(fontSize *  (1.0 / hersheyBaseline));
    
    ofFbo * fbo;
    string fboKey = getFboKey(text);
    auto it = fboCache.find(fboKey);
    if (it == fboCache.end()) {
        fbo = new ofFbo();
        ofRectangle b = boundingBox;
        string textCopy = text;
        formatParagraphs(textCopy, b);
        fbo->allocate(b.width, b.height, type, numSamples);
        samplesChanged = false;
        ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << numSamples << " samples, " << b.width << " x " << b.height;
        fboCache[fboKey] = fbo;
        
        
    } else {
        fbo = fboCache[fboKey];
    }
    
    int xx = boundingBox.x;
    int yy = boundingBox.y;
    if (verticalAlign == 0) yy += (boundingBox.height - fbo->getHeight()) / 2;
    if (verticalAlign == -1) yy += (boundingBox.height - fbo->getHeight());
    
    ofRectangle fboBox(xx,yy,fbo->getWidth(), fbo->getHeight());

    ofSetColor(255);
    
    if (it == fboCache.end() || shouldRedraw) {
    
        fbo->begin();
        ofClear(255,0);
        ofSetColor(255);
        ofPushMatrix();
        string textCopy = text;
        drawFbo(textCopy, boundingBox);
        ofPopMatrix();
        fbo->end();
        shouldRedraw = false;
    }
    
    ofSetColor(255);
    fbo->draw(xx, yy);
    return fboBox;
    
}
    
