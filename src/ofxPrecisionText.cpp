
#pragma once
#include "ofxPrecisionText.h"
#include <regex>

#include "ofxTimeMeasurements.h"



string ofxPrecisionText::getStructureKey(string text) {
    
    /*-- Get unique hash for Texture cache --*/
    
    string key = text;
    key += fontList[s.fontIndex];
    key += ofToString(s.fontSize);
    if (s.bounds.width != 0 && s.bounds.height != 0) {
        key += ofToString(s.verticalAlign);
        key += ofToString(s.horizontalAlign);
        key += ofToString(s.bounds.width);
        key += ofToString(s.bounds.height);
    }
    key += ofToString(s.lineHeight);
    key += ofToString(s.letterSpacing);
    key += ofToString(s.dpi);
    key += ofToString(s.markdown);
    key += ofToString(s.strokeColor.r);
    key += ofToString(s.strokeColor.g);
    key += ofToString(s.strokeColor.b);
    key += ofToString(s.strokeColor.a);
    key += ofToString(s.strokeWidth);
    if (s.fontIndex < hershey.getNumFonts()) {
        key += ofToString(s.strokeWidth);
        key += ofToString(s.boldWidth);
    }
    return key;
}

string ofxPrecisionText::getTextureKey(string text) {
    
    /*-- Get unique hash for Texture cache --*/
    
    string key = getStructureKey(text);
    key += ofToString(s.numSamples);
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
        font.load(fontName, s.fontSize, true, false, true);
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
    
    string fontKey = defineFont(fSize);
    if (text == " ") text = "_";
    
    ofRectangle b;
    
    
    if (s.fontIndex < hershey.getNumFonts()) {
        b = hershey.getBounds(text, fSize, x, y);
    } else {
        vector<ofPath> pp = fontCache[fontKey].getStringAsPoints(text, true, true);
        ofRectangle rect(0, 0, 0, 0);
        for (auto & path : pp ) {
            path.setPolyWindingMode(OF_POLY_WINDING_ODD);
            for (auto & poly : path.getOutline()) {
                rect.growToInclude(poly.getBoundingBox());
            }
        }
        b = rect;
        b.x = x;
        b.y = y;
    }
    
    return b;
}
void ofxPrecisionText::drawString(string text, float fSize, float xx, float yy) {
    
    string fontKey = defineFont(fSize);
    if (s.pixelAligned) {
        xx = (int)xx;
        yy = (int)yy;
    }
    
    if (s.fontIndex < hershey.getNumFonts()) {
//        hershey.setScale( ( 1.0 / 31.0 ) * (fSize) );
//        hershey.draw(text, xx, yy + getBounds(text, fSize, 0,0).height);
    } else {
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
    
    if (text.size() == 0) return structure;
    
    vector<ofxPrecisionTextChar> chars;
    string outputString = "";
    
    
    /*-- Check for cached markdown --*/
    
    if (s.markdown) {
        auto it = markdownCache.find(text);
        if (it == markdownCache.end()) {
            ofLog() << "[ofxPrecisionText] Setting Markdown";
            markdownCache[text] = parseMarkdown(text, false);
        }
        structure = markdownCache[text];
        text = structure.text;
    } else {
        structure.text = text;
    }
    
    structure.outerBox = boundingBox;
    structure.innerBox = boundingBox;
    structure.innerBox.x += s.margin;
    structure.innerBox.y += s.margin;
    structure.innerBox.width -= s.margin*2;
    structure.innerBox.height -= s.margin*2;
    
    boundingBox = structure.innerBox;
    
    
    /* -- Generate chars -- */
    
    bool isBold = false;
    bool isItalic = false;
    bool isH1 = false;
    bool isH2 = false;
    bool isH3 = false;
    
    
    for (int i = 0; i < text.size(); i++) {
        
        if (hasIndex(structure.bold, i)) {
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
        ch.strokeWidth = (ch.isBold) ? s.strokeWidth * s.boldWidth : s.strokeWidth ;
        
        ch.color = (ch.isLink) ? s.linkColor : s.strokeColor;
        
        string l = ofToString(text[i]);
        ch.letter = l;
        ch.bounds = getBounds(l, ch.fontSize * s.dpi, 0,0);
        chars.push_back(ch);
        outputString += l;
        
    }
    
    /*-- Define positions and new lines --*/
    
    float linePix = (int)(s.fontSize * s.lineHeight * s.dpi);
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
                linePix = (int)(prev.fontSize * s.lineHeight * s.dpi );
            }
            ch.bounds = getBounds(ch.letter, ch.fontSize * s.dpi, iX, iY);
            ch.bounds.height = ch.fontSize * s.lineHeight * s.dpi;
            
            /*-- if new line --*/
            
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
        
        /*-- Hack to go back and offset last chars --*/
        
        if (iX + 1 > boundingBox.width && ww < boundingBox.width) {
            
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
    
    
    vector<vector<ofxPrecisionTextChar >> asLines;
    vector<ofxPrecisionTextChar> l;
    asLines.push_back(l);
    
    float lastY = chars[0].bounds.y;
    float lastX = chars[0].bounds.x;
    int iLine = 0;
    int lineIdx = 0;
    float lastH = 0;
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
        if (ch.bounds.y != lastY) {
            lineIdx += 1;
            vector<ofxPrecisionTextChar> l;
            asLines.push_back(l);
        }
        ch.line = lineIdx;
        ch.index = i;
        asLines.back().push_back(ch);
        
        lastY = ch.bounds.y;
        lastX = ch.bounds.x + ch.bounds.width;
        
    }
    
    
    structure.lines = asLines;
    structure.bounds.width = maxWidth;
    float descender = s.fontSize * 0.5; // roughly 1/4 of the font size, usually
    structure.bounds.height = iY + linePix + descender;
    structure.chars = chars;
    
    
    ofRectangle r = structure.bounds;
    structure.bounds.x = boundingBox.x;
    structure.bounds.y = boundingBox.y;
    
    if (isPoint) {

        if (s.horizontalAlign == 0) structure.bounds.x -= (r.width )/2;
        if (s.horizontalAlign == -1) structure.bounds.x -= (r.width);
        if (s.verticalAlign == 0) structure.bounds.y -= (r.height) / 2;
        if (s.verticalAlign == 1) structure.bounds.y -= (r.height);

    } else {

        if (s.horizontalAlign == 0) structure.bounds.x += (boundingBox.width - r.width)/2;
        if (s.horizontalAlign == 1) structure.bounds.x += (boundingBox.width - r.width);
        if (s.verticalAlign == 0) structure.bounds.y += (boundingBox.height - r.height) / 2;
        if (s.verticalAlign == -1) structure.bounds.y += (boundingBox.height - r.height);

    }
    
    return structure;
}

ofxPrecisionStructure ofxPrecisionText::drawStructure(ofxPrecisionStructure structure) {
    
    
    TS_START("defineFont");
    
    string fontKey = defineFont(s.fontSize * s.dpi);
    
    TS_STOP("defineFont");
    
    TS_START("drawChars");

    for (auto & ch : structure.chars) {
        
        if ( int(ch.letter[0]) != 10 && int(ch.letter[0]) != 0 ) {
            
            drawChar(ch);
            
        }
    }
    TS_STOP("drawChars");

    
    
    
    return structure;
}

string ofxPrecisionText::getCharKey(ofxPrecisionTextChar & ch) {
    string k = ch.letter;
    k += ofToString( s.fontIndex );
    k += ofToString( ch.color );
    k += ofToString( ch.fontSize );
    k += ofToString( ch.strokeWidth );
    return k;
}

void ofxPrecisionText::drawCharPath( ofxPrecisionTextChar & ch ) {
    
    
    float x = ch.bounds.x;
    float y = ch.bounds.y;
    float w = ch.bounds.width;
    float h = ch.bounds.height;
    string fontKey = defineFont(ch.fontSize);
    
    /*-- scaling --*/
    
    bool isHershey = s.fontIndex < hershey.getNumFonts();
    float sc = ( 1.0 / 31.0 ) * (ch.fontSize * s.dpi);
    if (!isHershey) sc = 1 * s.dpi;
    
    /*-- getpath --*/
    
    
    ofPath * p;
    
    if (isHershey) {
        p = &hershey.getPath(ch.letter[0]);
        p->setStrokeColor(ch.color);
        p->setStrokeWidth( ch.strokeWidth * s.dpi);
    } else {
        p = &fontCache[fontKey].getStringAsPoints(ch.letter, true, true)[0];
        p->setStrokeColor(ch.color);
        p->setFillColor(ch.color);
        float bold = (ch.strokeWidth - s.strokeWidth) * s.dpi;
        p->setStrokeWidth(bold);
    }
    
    ofPushMatrix();
    ofTranslate(0, h * 0.75);
    ofScale(sc, -sc);
    ofNotifyEvent(charBegin, ch);
    p->draw(0,0);
    ofNotifyEvent(charEnd, ch);
    ofPopMatrix();
}

void ofxPrecisionText::drawChar(ofxPrecisionTextChar & ch) {
    
    string key = getCharKey(ch);
    float x = ch.bounds.x;
    float y = ch.bounds.y;
    float w = ch.bounds.width;
    float h = ch.bounds.height;
    
    if (!s.cache) {
        auto it = charTexCache.find(key);
        if (it == charTexCache.end()) {
            
            ofLogNotice("[ofxPrecisionText]") << "Add Char Texture";
            ofFbo * fbo = new ofFbo();
            fbo->allocate(w, h + (h * 0.5), fboType, s.numSamples);
            fbo->begin();
            
            ofClear(255,0);
            ofSetColor(255);
            drawCharPath(ch);
            
            fbo->end();
            charTexCache[key] = fbo->getTexture();
            
            delete fbo;
        }
        
    }
    
    ofPushMatrix();
    ofTranslate( x, y);
    ofSetColor(255);
    if (!s.cache) charTexCache[key].draw(0,0);
    if (s.cache) drawCharPath(ch);
    ofPopMatrix();
    if (s.debug) {
        ofNoFill();
        ofSetColor(255,0,0,255);
        ofDrawRectangle(ch.bounds);
    }
    
//
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
    texCache.clear();
    structCache.clear();
    fontCache.clear();
    markdownCache.clear();
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
    return ofxPrecisionText::draw(text, r, settings, false);
    
}

ofxPrecisionStructure ofxPrecisionText::draw(string text, ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint) {
    
    if (text.size() == 0) {
        ofxPrecisionStructure temp;
        return temp;
    }
    s = settings;
    s.bounds = boundingBox;
    if (isPoint) {
        s.bounds.width = 0;
        s.bounds.height = 0;
    }
    
    /*-- Generate unique key from settings --*/
    
//    TS_START("getKeys");
    string key = getStructureKey(text);
    string tKey = getTextureKey(text);
    
//    TS_STOP("getKeys");
    
    auto itt = structCache.find(key);
    if (itt == structCache.end()) {
        ofLogNotice("[ofxPrecisionText]") << "Setting Structure";
        structCache[key] = generateStructure(text, boundingBox, true, isPoint);
        
    }
    
    ofRectangle drawn = structCache[key].bounds;
    
    if (isPoint) {
        ofRectangle bb = boundingBox;
        ofRectangle sb = structCache[key].bounds;
        if (s.horizontalAlign == 0) {
            drawn.x = bb.x - (sb.width/2);
        } else if (s.horizontalAlign == -1) {
            drawn.x = bb.x - sb.width;
        } else {
            drawn.x = bb.x;
        }
        if (s.verticalAlign == 0) {
            drawn.y = bb.y - (sb.height/2);
        } else if (s.verticalAlign == 1) {
            drawn.y = bb.y - sb.height;
        } else {
            drawn.y = bb.y;
        }
    }
    
    if (s.cache) {
        
        auto it = texCache.find(tKey);
        if (it == texCache.end()) {
            
            ofLogNotice("[ofxPrecisionText]") << "Setting Texture";
            
            ofFbo * fbo = new ofFbo();
            fbo->allocate(structCache[key].bounds.width, structCache[key].bounds.height, fboType, 8);
            
            fbo->begin();
            ofClear(255,0);
            ofSetColor(255);
            ofPushMatrix();
            drawStructure(structCache[key]);
            ofPopMatrix();
            fbo->end();
            shouldRedraw = false;
            
            texCache[tKey] = fbo->getTexture();
            
            delete fbo;
            
        }
        
        ofSetColor(255);
        texCache[tKey].draw( drawn );
        if (s.debug) {
            ofSetColor(255,0,255);
            ofSetLineWidth(1 * s.dpi);
            ofNoFill();
            ofDrawRectangle( drawn );
        }
        
    } else {
        
        ofPushMatrix();
        ofTranslate( drawn.x, drawn.y);
        drawStructure(structCache[key]);
        ofPopMatrix();
    }
    
    if (s.debug) {
        ofNoFill();
        
        ofSetColor(0,255,255,255);
        ofDrawRectangle(structCache[key].outerBox);
        (isActive) ? ofSetColor(255, 0, 0) : ofSetColor(255,0,255,255);
        ofDrawRectangle(structCache[key].innerBox);
    }
        
    return structCache[key];
    
}

