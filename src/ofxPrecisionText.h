#pragma once
#include "ofxHersheyFont.h"

class ofxPrecisionText {
private:
    
    bool samplesChanged;
    int cacheCharLimit;
    float fontSize;
    int fontIndex;
    float lineHeight;
    int horizontalAlign, verticalAlign;
    ofColor backgroundColor;
    float hersheyStroke;
    ofColor strokeColor;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    bool shouldRedraw;
    
    
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo *> fboCache;
    
    
    void findAllOccurances(std::vector<size_t> & vec, std::string data, std::string toSearch)
    {
        // Get the first occurrence
        size_t pos = data.find(toSearch);
        
        // Repeat till end is reached
        while( pos != std::string::npos)
        {
            // Add position to the vector
            vec.push_back(pos);
            
            // Get the next occurrence from the current position
            pos =data.find(toSearch, pos + toSearch.size());
        }
    }
    
    string getFboKey(string text) {
        string key = fontList[fontIndex] + ofToString(fontSize) + ofToString(strokeColor.r) + ofToString(strokeColor.g) + ofToString(strokeColor.b) + ofToString(strokeColor.a) + ofToString(numSamples) + ofToString(isBackgroundEnabled) + text + ofToString(horizontalAlign) + ofToString(lineHeight);
        if (fontIndex == 0 ) key += ofToString(hersheyStroke);
        return key;
    }
    
    string defineFont() {
        
        if (fontIndex == 0) return "Hershey";
        string fontName = fontList[fontIndex];
        string fontKey = fontName + ofToString( fontSize );
        auto it = fontCache.find(fontKey);
        if (it == fontCache.end()) {
            ofTrueTypeFont font;
            font.load(fontName, fontSize, true, true);
            font.setLineHeight(18.0f);
            font.setLetterSpacing(1.037);
            fontCache[fontKey] = font;
        }
        return fontKey;
    }
    
    ofRectangle getBounds(string text, int x, int y) {
        ofRectangle b;
        
        if (fontIndex == 0) {
            b = hershey.getBounds(text, x, y);
        } else {
            string fontKey = defineFont();
            b = fontCache[fontKey].getStringBoundingBox(text, x, y);
        }
//        ofRectangle boundingBox, float fontSize, int horizontalAlign = -1, int verticalAlign = 0
//        b.x = boundingBox.x;
//        b.y = boundingBox.y;
//
//        if (horizontalAlign == 1) b.x += boundingBox.width - b.width;
//        if (horizontalAlign == 0) b.x += (boundingBox.width/2) - (b.width/2);
//
//        if (verticalAlign == 1) b.y += b.height;
//        if (verticalAlign == 0) b.y += (boundingBox.height/2) + (b.height/2);
//        if (verticalAlign == -1) b.y += boundingBox.height;
        
        return b;
    }
    
    void getParagraph(string text, ofRectangle boundingBox, vector<int> & lineBreaks, int & totalHeight, int & iterateChars) {
        
        /** Produces totalHeight and lineBreaks **/
        
        int iterateWidth = 0;
        totalHeight += (int)(fontSize * lineHeight);
        
        for (auto & word : ofSplitString(text, " ")) {
            
            
            ofRectangle b = getBounds(word + "_", 0, 0);
            iterateWidth += b.width;
            if (iterateWidth > boundingBox.width) {
                iterateWidth = b.width;
                totalHeight += (int)(fontSize * lineHeight);
                lineBreaks.push_back(iterateChars);
            }
            iterateChars += word.size() + 1;
        }
    }
    
    vector<int> formatParagraphs(string & text, ofRectangle & boundingBox) {
        
        int totalHeight = 0;
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
            getParagraph(paragraph, boundingBox, lineBreaks, totalHeight, iterateChars);
            if (paragraph != paragraphs.back()) {
                iterPara += paragraph.size() + 1;
                lineBreaks.push_back(iterPara);
            }
        }
        
        boundingBox.height = totalHeight;
        return lineBreaks;
    }
    
    ofRectangle drawFbo(string text, ofRectangle boundingBox) {
        
        
        int initialWidth = getBounds(text, 0, 0).width;
        ofRectangle b = boundingBox;
        ofRectangle fboBox = boundingBox;
        
        
        ofFill();
        ofSetColor(strokeColor);
        string fontKey = defineFont();
        
        
//        if (initialWidth > boundingBox.width) {
        
            
            vector<int> lineBreaks = formatParagraphs(text, fboBox);
            
            for (int i = 0; i < lineBreaks.size(); i++) {
                
                int breakA = lineBreaks[i];
                int breakB = lineBreaks[i + 1] - lineBreaks[i];
                
                string line = text.substr(breakA, breakB);
                
                if (line != "") {
                    
                    if (line.substr(line.size() - 1, line.size()) == " ") {
                        line = line.substr(0, line.size() - 1);
                    }
                    int linePix = (int)(fontSize * lineHeight);
                    ofRectangle rect(0, ( linePix * i ) + linePix, boundingBox.width, linePix);
                    rect = getBounds(line, rect.x, rect.y);
                    if (horizontalAlign == 0) rect.x = (boundingBox.width - rect.width) /2;
                    if (horizontalAlign == 1) rect.x = (boundingBox.width - rect.width) ;
                    
                    if (fontIndex == 0) {
                        hershey.draw(line, (int)rect.x, (int)rect.y);
                    } else {
                        fontCache[fontKey].drawString(line, (int)rect.x, (int)rect.y);
                    }
                }
                
            }
//        } else {
//            if (fontIndex == 0) {
//                hershey.draw(text, (int)b.x, (int)b.y);
//                b.y -= hershey.getHeight();
//            } else {
//                fontCache[fontKey].drawString(text, (int)b.x, (int)b.y);
//                b.y -= fontSize * lineHeight  + fontCache[fontKey].getDescenderHeight();
//                b.height = fontSize * lineHeight; //fontCache[fontKey].getAscenderHeight() + fontCache[fontKey].getDescenderHeight();
//            }
//        }
        
//        ofNoFill();
//        ofSetColor(255,255,0);
//        ofDrawRectangle(b);
        
        
        return fboBox;
    }
    
public:
    vector<string> fontList;
//    ofEvent<float> characterDrawn;
    int numSamples;
    bool isCachingFbo;
    bool isBackgroundEnabled;
    
    void flagRedraw() {
        shouldRedraw = true;
    }
    
    void setLineHeight(float lHeight) {
        lineHeight = lHeight;
    }
    
    void setup(string fontLocation = "") {
    
        shouldRedraw = false;
        samplesChanged = false;
        isBackgroundEnabled = false;
        isCachingFbo = true;
        numSamples = 8;
        cacheCharLimit = 200;
        fontIndex = 0;
        lineHeight = 1.4;
        ofDirectory dir(fontLocation);
        dir.allowExt("ttf");

        ofTrueTypeFont::setGlobalDpi(72);
        
        dir.listDir();
        fontList.push_back("Hershey");
        for (auto & f: dir.getFiles()) {
            ofLogNotice("[ofxPrecisionText]") << "Adding TTF " << f.path();
            fontList.push_back(f.path());
        }
        if (fontList.size() == 0) ofLogNotice("[ofxPrecisionText]") << "No TTF fonts loaded";
    }
    
    void enableBackground(ofColor color) {
        backgroundColor = color;
        isBackgroundEnabled = true;
    }
    void disableBackground() {
        isBackgroundEnabled = false;
    }

    void enableCache(int fboSamples = 8) {
        isCachingFbo = true;
        if (fboSamples >= 0 && fboSamples <= 8) {
            numSamples = fboSamples;
            samplesChanged = true;
        }
    }
    
    void disableCache() {
        isCachingFbo = false;
    }
    
    
    void setFont(int index) {
        if (index >= 0 && index < fontList.size()) fontIndex = index;
    }
    
    void setColor(ofColor color) {
        strokeColor = color;
        hershey.setColor(color);
    }
    
    void setStroke(float stroke) {
        hersheyStroke = stroke;
        hershey.setStroke(stroke);
    }
    
    void clearFboCache() {
        fboCache.clear();
    }
    
    ofRectangle draw(string text, glm::vec3 originPoint, float fSize, int horzAlign = -1, int vertAlign = 0) {
        ofPoint p(originPoint.x, originPoint.y);
        ofxPrecisionText::draw(text, p, fSize, horzAlign, vertAlign);
    }
    
    ofRectangle draw(string text, ofPoint originPoint, float fSize, int horzAlign = -1, int vertAlign = 0) {
        
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
        hershey.setScale(fontSize *  (1.0 / 21.0));
        
        ofFbo * fbo;
        string fboKey = getFboKey(text);
        auto it = fboCache.find(fboKey);
        if (it == fboCache.end()) {
            fbo = new ofFbo();
            string textCopy = text;
            formatParagraphs(textCopy, b);
            fbo->allocate(b.width, b.height, type, numSamples);
            samplesChanged = false;
            ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << numSamples << " samples, " << originPoint.x << " x " << originPoint.y;
            fboCache[fboKey] = fbo;
            
            
        } else {
            fbo = fboCache[fboKey];
        }
        
        int xx = originPoint.x;
        int yy = originPoint.y;
        
        if (verticalAlign == 0) yy += fbo->getHeight() / 2;
        if (verticalAlign == -1) yy += fbo->getHeight();
        
        if (horizontalAlign == 0) xx += fbo->getWidth() / 2;
        if (horizontalAlign == 1) yy += fbo->getWidth();
        
        ofSetColor(0,0,255);
        ofRectangle fboBox(xx,yy,fbo->getWidth(), fbo->getHeight());
        ofDrawRectangle(fboBox);
        
        
        if (it == fboCache.end() || shouldRedraw) {
            
            fbo->begin();
            ofClear(255,0);
            ofSetColor(255);
            if (isBackgroundEnabled) ofBackground(backgroundColor);
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
    
    ofRectangle draw(string text, ofRectangle boundingBox, float fSize, int horzAlign = -1, int vertAlign = 0) {
        
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
        hershey.setScale(fontSize *  (1.0 / 21.0));
        
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
        
        ofSetColor(0,0,255);
        ofRectangle fboBox(xx,yy,fbo->getWidth(), fbo->getHeight());
        ofDrawRectangle(fboBox);

        ofSetColor(255);
        
        if (it == fboCache.end() || shouldRedraw) {
        
            fbo->begin();
            ofClear(255,0);
            ofSetColor(255);
            if (isBackgroundEnabled) ofBackground(backgroundColor);
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
    
    
    
};
