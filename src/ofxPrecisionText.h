#pragma once
#include "ofxHersheyFont.h"

class ofxPrecisionText {
private:
    
    bool samplesChanged;
    int cacheCharLimit;
    int fontIndex;
    float lineHeight;
    ofColor backgroundColor;
    float hersheyStroke;
    float hersheyColor;
    ofColor strokeColor;
    ofxHersheyFont hershey;
    ofFbo fbo;
    
    std::map<string, ofTrueTypeFont> fontCache;
    std::map<string, ofFbo> fboCache;
    
    string defineFont(int index, float fontSize) {
        
        if (index == 0) return "Hershey";
        string fontName = fontList[index];
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
    
    
    ofRectangle getBounds(string text, ofRectangle boundingBox, float fontSize, int horizontalAlign = -1, int verticalAlign = 0) {
        ofRectangle b;
        if (fontIndex == 0) {
            b = hershey.getBounds(text, 0, 0);
        } else {
            string fontKey = defineFont(fontIndex, fontSize);
            b = fontCache[fontKey].getStringBoundingBox(text, 0, 0);
        }
        
        b.x = boundingBox.x;
        b.y = boundingBox.y;
        
        if (horizontalAlign == 1) b.x += boundingBox.width - b.width;
        if (horizontalAlign == 0) b.x += (boundingBox.width/2) - (b.width/2);
        
        if (verticalAlign == 1) b.y += b.height;
        if (verticalAlign == 0) b.y += (boundingBox.height/2) + (b.height/2);
        if (verticalAlign == -1) b.y += boundingBox.height;
        
        return b;
    }
    ofRectangle drawFbo(string text, ofRectangle boundingBox, float fontSize, int horizontalAlign = -1, int verticalAlign = 0) {
        
        ofRectangle b = getBounds(text, boundingBox, fontSize, horizontalAlign, verticalAlign);
        
        hershey.setScale(fontSize *  (1.0 / 21.0));
        
        
        int totalHeight = 0;
        int iterateWidth = 0;
        int iterateChars = 0;
        vector<int> lineBreaks = {0};
        
        if (b.width > boundingBox.width) {
            for (auto & word : ofSplitString(text, " ")) {
                
                ofRectangle b;
                
                if (fontIndex == 0) {
                    b = hershey.getBounds(word + "_", 0, 0);
                } else {
                    string fontKey = defineFont(fontIndex, fontSize);
                    b = fontCache[fontKey].getStringBoundingBox(word + "_", 0, 0);
                }
                
                
                iterateWidth += b.width;
                if (iterateWidth > boundingBox.width) {
                    iterateWidth = b.width;
                    totalHeight += fontSize * lineHeight;
                    if (totalHeight + (fontSize * lineHeight) >= boundingBox.height) {
                        text = text.substr(0, iterateChars - 3) + "...";
                        totalHeight = boundingBox.height;
                        break;
                    }
                    lineBreaks.push_back(iterateChars);
                }
                iterateChars += word.size() + 1;
            }
            
        }
        
        
        ofFill();
        ofSetColor(strokeColor);
        string fontKey = defineFont(fontIndex, fontSize);
        
        
        if (b.width > boundingBox.width) {
            
            b.x = boundingBox.x;
            b.width = boundingBox.width;
            b.y = boundingBox.y;
            b.height = (fontSize * lineHeight) * (lineBreaks.size());
            if (verticalAlign == -1)  b.y += boundingBox.height - b.height;
            if (verticalAlign == 0)  b.y += (boundingBox.height - b.height)/2;
            
            for (int i = 0; i < lineBreaks.size(); i++) {
                int yy =  b.y + ( (lineHeight * fontSize) * i );
                string line = text.substr(lineBreaks[i], lineBreaks[i + 1] - lineBreaks[i]);
                
                if (line.substr(line.size() - 1, line.size()) == " ") line = line.substr(0, line.size() - 1);
                
                ofRectangle rect(boundingBox.x, yy, boundingBox.width, fontSize * lineHeight);
                rect = getBounds(line, rect, fontSize, horizontalAlign, 0);
                
                
                if (fontIndex == 0) {
                    hershey.draw(line, (int)rect.x, (int)rect.y);
                } else {
                    fontCache[fontKey].drawString(line, (int)rect.x, (int)rect.y);
                }
                
            }
        } else {
            if (fontIndex == 0) {
                hershey.draw(text, (int)b.x, (int)b.y);
                b.y -= hershey.getHeight();
            } else {
                fontCache[fontKey].drawString(text, (int)b.x, (int)b.y);
                b.y -= fontSize * lineHeight  + fontCache[fontKey].getDescenderHeight();
                b.height = fontSize * lineHeight; //fontCache[fontKey].getAscenderHeight() + fontCache[fontKey].getDescenderHeight();
            }
            
//            b.y -= fontSize;
        }
        
//        ofNoFill();
//        ofSetColor(255,255,0);
//        ofDrawRectangle(b);
        
        
        return b;
    }
    
public:
    vector<string> fontList;
//    ofEvent<float> characterDrawn;
    int numSamples;
    bool isCachingFbo;
    bool isBackgroundEnabled;
    void setup(string fontLocation = "") {
    
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
    
    ofRectangle draw(string text, ofRectangle boundingBox, float fontSize, int horizontalAlign = -1, int verticalAlign = 0) {
        
        if (!isCachingFbo) return drawFbo(text, boundingBox, fontSize, horizontalAlign, verticalAlign);
        
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
        if ((fbo.getWidth() != boundingBox.width) || (fbo.getHeight() != boundingBox.height) || samplesChanged) {
            fbo.allocate(boundingBox.width, boundingBox.height, type, numSamples);
            samplesChanged = false;
            ofLogNotice("[ofxPrecisionText]") << "Setting FBO with " << numSamples << " samples";
        }
        
        fbo.begin();
        ofClear(0, 0, 0, 0);
        if (isBackgroundEnabled) ofBackground(backgroundColor);
        ofPushMatrix();
        ofTranslate(-boundingBox.x, -boundingBox.y);
        ofRectangle returnedRectangle = drawFbo(text, boundingBox, fontSize, horizontalAlign, verticalAlign);
        ofPopMatrix();
        fbo.end();
        
        fbo.draw(boundingBox.x, boundingBox.y);
        
        return returnedRectangle;
        
    }
    
    
    
};
