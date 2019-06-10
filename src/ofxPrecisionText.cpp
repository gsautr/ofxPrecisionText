
#pragma once
#include "ofxPrecisionText.h"
#include <regex>




string ofxPrecisionText::getFboKey(string text) {
    
    /*-- Get unique hash for FBO cache --*/
    
    string key = fontList[s.fontIndex];
    key += ofToString(s.fontSize);
    key += ofToString(s.strokeColor.r);
    key += ofToString(s.strokeColor.g);
    key += ofToString(s.strokeColor.b);
    key += ofToString(s.strokeColor.a);
    key += ofToString(s.numSamples);
    key += text;
    key += ofToString(s.horizontalAlign);
    key += ofToString(s.lineHeight);
    key += ofToString(s.pixelAligned);
    key += ofToString(s.letterSpacing);
    key += ofToString(s.dpi);
    if (s.fontIndex < hershey.getNumFonts()) key += ofToString(s.strokeWidth);
    return key;
}

string ofxPrecisionText::defineFont(float fSize) {
    
    /*-- Loads TTF to cache and returns font cache key --*/
    
    string fontName = fontList[s.fontIndex];
    if (s.fontIndex < hershey.getNumFonts()) {
        hershey.setFont(s.fontIndex);
        return fontName;
    }
    
    string fontKey = fontName + ofToString( fSize );
    auto it = fontCache.find(fontKey);
    if (it == fontCache.end()) {
        ofTrueTypeFont font;
        font.load(fontName, s.fontSize, true, true);
        font.setLineHeight(1.0f);
        font.setLetterSpacing(1.037);
        fontCache[fontKey] = font;
    }
    return fontKey;
}



vector<int> replaceAndGenerateIndexes(string & text, string find) {
    vector<int> indexes;
    
    int i = 0;
    for (auto & split : ofSplitString(text, find)) {
        i += split.size();
        indexes.push_back(i);
    }
    
    string t = text;
    size_t pos = t.find(find);
    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        t.replace(pos, find.size(), "");
        // Get the next occurrence from the current position
        pos = t.find(find, pos);
    }
    text = t;
    
    return indexes;
}


vector<int> generateIndexes(string text, string find) {
    
    vector<int> indexes;
    string::size_type pos = 0;
    while ( (pos = text.find(find, pos)) != string::npos) {
        indexes.push_back(pos);
        text.replace(pos, find.size(), "");
        pos += find.size();
    }
    
    return indexes;
}



ofRectangle ofxPrecisionText::getBounds(string text, float fSize, float x, float y) {
    
    /*-- Get bounds for single text string --*/
    
    if (text == " ") text = "_";
    
    ofRectangle b;
    
    
    if (s.fontIndex < hershey.getNumFonts()) {
        float scale = ( (1.0 / 31.0) * fSize );
        b = hershey.getBounds(text, x, y);
        b.width *= scale;
        b.height *= scale;
    } else {
        string fontKey = defineFont(fSize);
        b = fontCache[fontKey].getStringBoundingBox(text, x, y);
        b.y = y + (fSize * 1);
    }
    
    return b;
}
void ofxPrecisionText::drawString(string text, float fSize, float xx, float yy) {
    
    if (s.pixelAligned) {
        xx = (int)xx;
        yy = (int)yy;
    }
    
    if (s.fontIndex < hershey.getNumFonts()) {
//        hershey.setScale( ( 1.0 / 31.0 ) * (fSize) );
//        hershey.draw(text, xx, yy + getBounds(text, fSize, 0,0).height);
    } else {
        string fontKey = defineFont(fSize);
        fontCache[fontKey].drawString(text, xx, yy );
    }
}



bool ofxPrecisionText::hasLink(vector<ofxPrecisionTextHyperlink> links, int i, ofxPrecisionTextHyperlink & link) {
    for (auto & l : links) {
        if (i >= l.start && i < l.end) {
            link = l;
            return true;
        }
    }
    return false;
}


ofxPrecisionStructure ofxPrecisionText::generateStructure(string text, ofRectangle boundingBox, bool dontDraw, bool isPoint) {
    
    ofxPrecisionStructure structure;
    vector<ofxPrecisionTextChar> chars;
    string outputString = "";
    
    /*-- Check for cached markdown --*/
    
    if (s.markdown) {
        auto it = markdownCache.find(text);
        if (it == markdownCache.end()) {
            ofLog() << "[ofxPrecisionText] Parsing markdown";
            structure = parseMarkdown(text, false);
            markdownCache[text] = structure.text;
        }
        
        ofLog() << "[ofxPrecisionText] Returning markdown";
        text = markdownCache[text];
    }
    
    
    /* -- Generate chars -- */
    
    bool isBold = false;
    bool isItalic = false;
    bool isH1 = false;
    bool isH2 = false;
    bool isH3 = false;
    
    int bC = 0;
    
    for (int i = 0; i < text.size(); i++) {
        
        if (hasIndex(structure.bold, i)) {
            bC += 1;
            isBold = !isBold;
            
        }
        if (hasIndex(structure.italic, i)) isItalic = !isItalic;
        if (hasIndex(structure.h1, i)) isH1 = !isH1;
        if (hasIndex(structure.h2, i)) isH2 = !isH2;
        if (hasIndex(structure.h3, i)) isH3 = !isH3;
        
        ofxPrecisionTextChar ch;
        ch.isHeading = 0;
        ch.fontSize = s.fontSize;
        ch.isItalic =  false;
        ch.isLink = false;
        ch.isLineEnd = false;
        ch.isBold = isBold;
        ch.isItalic = isItalic;
        ch.index = i;
        
        if (isH1) ch.isHeading += 1;
        if (isH2) ch.isHeading += 2;
        if (isH3) ch.isHeading += 3;
        if (ch.isHeading > 0) {
            float extra = (s.fontSize * s.headingScale) - s.fontSize;
            extra /= (float)ch.isHeading;
            ch.fontSize += extra;
        }
        
        
        ofxPrecisionTextHyperlink link;
        if (hasLink(structure.links, i, link)) ch.isLink = true;
        ch.color = (ch.isLink) ? s.linkColor : s.strokeColor;
        if (ch.isLink) {
            ofLog() << "COLOR" << s.linkColor << ch.color;
        }
        ch.strokeWidth = (ch.isBold) ? s.strokeWidth * s.boldWidth * s.dpi : s.strokeWidth * s.dpi;
        
        string l = ofToString(text[i]);
        ch.letter = l;
        ch.bounds = getBounds(l, ch.fontSize * s.dpi, 0,0);
        chars.push_back(ch);
        outputString += l;
        
    }
    
    /*-- Define positions and new lines --*/
    
    int linePix = (int)(s.fontSize * s.lineHeight * s.dpi);
    float iX = 0;
    float iY = 0;
    float maxWidth = 0;
    int iChars = 0;
    bool nl = false;
    
    vector<int> spaces = parseIndexes(outputString, "[\n# $&:\n\t]");
    vector<string> words = parseWithIndexes(0, outputString, spaces);
    
    for (auto & w : words) {
        int ww = 0;
        int xx = iX;
        for (auto & l : w) {
            
            ofxPrecisionTextChar & ch = chars[iChars];
            if (hasIndex(structure.h1, iChars)||hasIndex(structure.h2, iChars)||hasIndex(structure.h3, iChars)) {
                int ii = (iChars > 0) ? iChars - 1 : 0;
                ofxPrecisionTextChar prev = chars[ii];
                hershey.setScale( (1.0 / 31.0) * prev.fontSize);
                linePix = (int)(prev.fontSize * s.lineHeight * s.dpi );
            }
            ch.bounds = getBounds(ch.letter, ch.fontSize * s.dpi, iX, iY);
            ch.bounds.height = ch.fontSize * s.lineHeight * s.dpi;
            if (ch.letter == "\n") {
                iY += linePix;
                iX = 0;
            } else {
                iX += ch.bounds.width + s.letterSpacing;
                ww += ch.bounds.width + s.letterSpacing;
            }
            if (iX > maxWidth) maxWidth = iX;
            iChars += 1;
        }
        
        if (iX + 1 > boundingBox.width && ww < boundingBox.width) {
            
            /*-- Hack to go back and offset last chars --*/
            
            for (int i = iChars - w.size(); i < iChars; i++) {
                ofxPrecisionTextChar & ch = chars[i];
                linePix = (int)(ch.fontSize * s.lineHeight * s.dpi);
                ch.bounds.x -= xx + getBounds("_", ch.fontSize * s.dpi, 0, 0).width + s.letterSpacing;
                ch.bounds.y += linePix;
            }
            
            /*-- Iterate height and width --*/
            
            iY += linePix;
            iX = ww;
            maxWidth = boundingBox.width;
            
            
        }
        
    }
    
    
    
    /*--- Hacky horizontal  --*/
    
    
    
    float lastY = chars[0].bounds.y;
    float lastX = chars[0].bounds.x;
    int iLine = 0;
    int lastH = 0;
    for (int i = 0; i < chars.size(); i++) {
        ofxPrecisionTextChar & ch = chars[i];
        
        /*-- Hacky, fix whitespace position --*/
        
        if (ch.bounds.y != lastY && ch.letter == " ") {
            ch.bounds.x = lastX;
            ch.bounds.y = lastY;
        }
        
        if (ch.bounds.y != lastY) chars[i-1].isLineEnd = true;
        if (i == chars.size() - 1) chars[i].isLineEnd = true;
        
        
        if (s.horizontalAlign != -1 && !isPoint) {
            
            /*-- Set horizontal position --*/
            
            if (ch.bounds.y != lastY || i+1 == chars.size()) {
                float tt = maxWidth;
                string ss ="";
                for (int ii = iLine; ii < i; ii++) {
                    if (chars[ii].letter[0] != '\n' && !chars[ii].isLineEnd) {
                        tt  -= chars[ii].bounds.width + s.letterSpacing;
                        ss += chars[ii].letter;
                    }
                }
                
                tt -= s.letterSpacing;
                
                ss = "";
                
                if (s.horizontalAlign == 0) tt /= 2;
                
                for (int ii = iLine; ii <= i; ii++) {
                    ofxPrecisionTextChar & cc = chars[ii];
                    if ( ii == iLine) {
                        cc.bounds.x = chars[ii+1].bounds.x - cc.bounds.width;
                    }
                    ss += cc.letter;
                    cc.bounds.x += tt;
                }
                
                iLine = i;
            }
        }
        
        lastY = ch.bounds.y;
        lastX = ch.bounds.x + ch.bounds.width;
        
    }
    structure.bounds.width = maxWidth;
    float descender = s.fontSize * 0.25;
    structure.bounds.height = iY + linePix;
    structure.chars = chars;
    
    return structure;
}

ofxPrecisionStructure ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox, bool dontDraw, bool isPoint) {
    
    
    
    string fontKey = defineFont(s.fontSize * s.dpi);
    ofxPrecisionStructure & structure = structCache[getFboKey(text)];
    
    
    if (!dontDraw) {
        
        for (auto & ch : structure.chars) {
            
            if ( int(ch.letter[0]) != 10 && int(ch.letter[0]) != 0 ) {
                
                
                
                
                
                float  scale = ( 1.0 / 31.0 ) * (ch.fontSize * s.dpi);
                int xPos = ch.bounds.x;
                int yPos = ch.bounds.y;
                int width = ch.bounds.getWidth();
                int height = ch.bounds.getHeight();
                yPos += ( height / 2 ) + ( (21 * scale) / 2 );
                ofPushMatrix();
                ofTranslate(xPos, yPos );
                ofScale(scale, -scale);
//                ofLog() << "A" << ch.color;
                
//                ofNotifyEvent(charBegin, ch);
                
                ofPath path = hershey.getPath(ch.letter[0]);
                ofSetColor(255);
                ofColor c = ch.color;
                float b = ch.strokeWidth;
                path.setColor( c );
                path.setStrokeColor( c );
                path.setFillColor( c);
                path.setFilled(false);
//                ofSetFillColor(c);
                path.setStrokeWidth(b);
                ofNoFill();
//                hershey.setItalic( ch.isItalic , 4 );
                if (ch.isLink) {
                    ofLog() << "L" << ch.letter << ch.strokeWidth << path.getStrokeColor();
                } else {
                    ofLog() << "T" << ch.letter << ch.strokeWidth << path.getStrokeColor();
                }
                path.draw(0,0);
                
//                ofNotifyEvent(charEnd, ch);
                
                ofPopMatrix();
                ofPopMatrix();
                
//                ofNoFill();
//                ofDrawRectangle(ch.bounds);
                
                
//                drawString(ch.letter, ch.fontSize * s.dpi, ch.bounds.x, ch.bounds.y);
                ofNoFill();
                ofSetLineWidth(1);
                
            }
        }
    }
    
    
    return structure;
}

bool ofxPrecisionText::hasIndex(vector<int> indexes, int i) {
    
    if ( std::find(indexes.begin(), indexes.end(), i) != indexes.end()) return true;
    return false;
}

void ofxPrecisionText::flagRedraw() {
    
    shouldRedraw = true;
}

void ofxPrecisionText::setup(string fontLocation) {
    
    
    fontList = hershey.getFontList();
    shouldRedraw = false;
    
    ofxPrecisionSettings settings;
    s = settings;
    
#ifdef TARGET_OPENGLES
    fboType = GL_RGBA;
#else
    fboType = GL_RGBA32F_ARB;
#endif
    
    ofDirectory dir(fontLocation);
    dir.allowExt("ttf");
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    dir.listDir();
    for (auto & f: dir.getFiles()) {
        ofLogNotice("[ofxPrecisionText]") << "Adding TTF " << f.path();
        fontList.push_back(f.path());
    }
    if (fontList.size() == 0) ofLogNotice("[ofxPrecisionText]") << "No TTF fonts loaded";
}


void ofxPrecisionText::clearCache() {
//    for (auto & f : texCache) delete f.second;
    texCache.clear();
}

ofxPrecisionStructure ofxPrecisionText::draw(string text, glm::vec3 originPoint, ofxPrecisionSettings settings) {
    ofPoint p(originPoint.x, originPoint.y);
    return ofxPrecisionText::draw(text, p, settings);
}

ofxPrecisionStructure ofxPrecisionText::draw(string text, ofPoint originPoint, ofxPrecisionSettings settings) {
    
    ofRectangle r(originPoint.x, originPoint.y, 444, 444);
    return ofxPrecisionText::draw(text, r, settings, true);
    
}
ofxPrecisionStructure ofxPrecisionText::draw(string text, int x, int y, int width, int height, ofxPrecisionSettings settings) {
    
    ofRectangle r(x,y,width,height);
    return ofxPrecisionText::draw(text, r, settings);
    
}

ofxPrecisionStructure ofxPrecisionText::draw(string text, ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint) {
    
    s = settings;
    
    /*-- Generate unique key from settings --*/
    
    string fboKey = getFboKey(text);
    string key = getFboKey(text);
    
        
        
    auto it = texCache.find(fboKey);
    if (it == texCache.end()) {
        ofFbo * fbo = new ofFbo();
        
        structCache[fboKey] = generateStructure(text, boundingBox, true, isPoint);
        fbo->allocate(structCache[fboKey].bounds.width, structCache[fboKey].bounds.height, fboType, s.numSamples);
        ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << s.numSamples << " samples, " << structCache[fboKey].bounds.width << " x " << structCache[fboKey].bounds.height;
        
        fbo->begin();
        ofClear(255,0);
        ofSetColor(255);
        ofPushMatrix();
        drawFbo(text, boundingBox, false, isPoint);
        ofPopMatrix();
        fbo->end();
        shouldRedraw = false;
        
        texCache[fboKey] = fbo->getTexture();
        
        delete fbo;
        
    }
    
    
    ofxPrecisionStructure & structure = structCache[key];
    
    
    structCache[key].bounds.x = boundingBox.x;
    structCache[key].bounds.y = boundingBox.y;
    
    ofRectangle r = structCache[key].bounds;
    
    if (isPoint) {
        
        if (s.horizontalAlign == 0) structCache[key].bounds.x -= (r.width )/2;
        if (s.horizontalAlign == -1) structCache[key].bounds.x -= (r.width);
        if (s.verticalAlign == 0) structCache[key].bounds.y -= (r.height) / 2;
        if (s.verticalAlign == 1) structCache[key].bounds.y -= (r.height);
        
    } else {
        
        if (s.horizontalAlign == 0) structCache[key].bounds.x += (boundingBox.width - r.width)/2;
        if (s.horizontalAlign == 1) structCache[key].bounds.x += (boundingBox.width - r.width);
        if (s.verticalAlign == 0) structCache[key].bounds.y += (boundingBox.height - r.height) / 2;
        if (s.verticalAlign == -1) structCache[key].bounds.y += (boundingBox.height - r.height);
        
    }
    
    ofSetColor(255);
    texCache[key].draw(structCache[key].bounds);
    return structCache[key];
    
}

