
#pragma once
#include "ofxPrecisionText.h"
#include <regex>



bool ofxPrecisionText::allocateFbo(string fboKey, string text, ofRectangle boundingBox, bool isPoint) {
    
    auto it = fboCache.find(fboKey);
    if (it == fboCache.end()) {
        ofFbo * fbo = new ofFbo();
        structCache[fboKey] = drawFbo(text, boundingBox, true, isPoint);;
        fbo->allocate(structCache[fboKey].bounds.width, structCache[fboKey].bounds.height, fboType, s.numSamples);
        ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << s.numSamples << " samples, " << structCache[fboKey].bounds.width << " x " << structCache[fboKey].bounds.height;
        fboCache[fboKey] = fbo;
        return true;
    } else {
        return false;
    }
}

string ofxPrecisionText::getFboKey(string text) {
    
    /*-- Get unique hash for FBO cache --*/
    
    string key = fontList[fontIndex] + ofToString(s.fontSize) + ofToString(s.strokeColor.r) + ofToString(s.strokeColor.g) + ofToString(s.strokeColor.b) + ofToString(s.strokeColor.a) + ofToString(s.numSamples) + text + ofToString(s.horizontalAlign) + ofToString(s.lineHeight) + ofToString(s.pixelAligned) + ofToString(s.letterSpacing);
    if (fontIndex == 0 ) key += ofToString(s.strokeWidth);
    return key;
}

string ofxPrecisionText::defineFont(float fSize) {
    
    /*-- Loads TTF to cache and returns font cache key --*/
    
    if (fontIndex == 0) return "Hershey";
    string fontName = fontList[fontIndex];
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


vector<int> ofxPrecisionText::regexReplace(string & text, string regex) {
    
    vector<int> indexes;
    int iter = 0;
    for (auto & reg : getMatchedStrings(text, regex)) {
        int start = reg.start;
        indexes.push_back(start);
    }
    
    return indexes;
}


ofRectangle ofxPrecisionText::getBounds(string text, float fSize, float x, float y) {
    
    /*-- Get bounds for single text string --*/
    
    if (text == " ") text = "_";
    
    ofRectangle b;
    
    
    if (fontIndex == 0) {
        hershey.setScale( (1.0 / 31.0) * fSize );
        b = hershey.getBounds(text, x, y);
    } else {
        string fontKey = defineFont(fSize);
        b = fontCache[fontKey].getStringBoundingBox(text, x, y);
        b.y = y + (s.fontSize * 1);
    }
    
    return b;
}
void ofxPrecisionText::drawString(string text, float fSize, float xx, float yy) {
    
    if (s.pixelAligned) {
        xx = (int)xx;
        yy = (int)yy;
    }
    
    //    float spc = ((1.0 - lineHeight) / 2) * s.fontSize;
    
    if (fontIndex == 0) {
        hershey.setScale( ( 1.0 / 31.0 ) * fSize );
        hershey.draw(text, xx, yy + getBounds(text, fSize, 0,0).height);
    } else {
        string fontKey = defineFont(fSize);
        fontCache[fontKey].drawString(text, xx, yy );
    }
}

bool compareInts(int a, int b) {
    return a < b;
}
vector<string> ofxPrecisionText::splitString(int fromChar, string text, vector<int> indices) {
    indices.push_back(fromChar);
    indices.push_back(fromChar + text.size());
    ofSort(indices, compareInts);
    vector<string> output;
    string t = text;
    for (int i = 0; i < indices.size() - 1; i++ ) {
        int iA = indices[i] - fromChar;
        int iB = indices[i+1] - fromChar - iA;
        if (iA < text.size()) {
            string sub = t.substr(iA, iB);
            output.push_back( sub );
        }
    }
    return output;
    
}

vector<int> ofxPrecisionText::findInternalIndices(string t, int start, vector<int> search) {
    
    vector<int> output;
    int end = start + t.size();
    for (auto & i : search) {
        if ((i >= start)&&(i <= end)) output.push_back(i);
        
    }
    return output;
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


vector<ofxPrecisionTextRegex> ofxPrecisionText::getMatchedStrings (string subject, string reg ){
    
    vector<ofxPrecisionTextRegex> results;
    try {
        regex re(reg);
        sregex_iterator next(subject.begin(), subject.end(), re);
        sregex_iterator end;
        while (next != end) {
            smatch match = *next;
            ofxPrecisionTextRegex m;
            m.start = next->position();
            m.size = std::distance(next, end);
            m.match = next->str();
            //            ofLog() << m.match;
            results.push_back(m);
            next++;
        }
    } catch (std::regex_error& e) {
        // Syntax error in the regular expression
        ofLogError("Error in regular expression");
    }
    return results;
}

ofxPrecisionTextStructure ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox, bool dontDraw, bool isPoint) {
    
    
    /*-- Draw text string with line breaks and formatting --*/
    
    string fontKey = defineFont(s.fontSize);
    
    ofxPrecisionTextStructure structure = parseMarkdown(text, false);
    text = structure.text;
    
    
    vector<ofxPrecisionTextChar> chars;
    string outputString = "";
    
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
            //            ofLog() << bC << isBold << i;
            
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
        
        //        setFontSize(ch.fontSize);
        
        string l = ofToString(text[i]);
        ch.letter = l;
        ch.bounds = getBounds(l, ch.fontSize, 0,0);
        chars.push_back(ch);
        outputString += l;
        
    }
    
    
    /*-- Define positions and new lines --*/
    
    int linePix = (int)(s.fontSize * s.lineHeight);
    float iX = 0;
    float iY = 0;
    float maxWidth = 0;
    int iChars = 0;
    bool nl = false;
    
    vector<int> spaces = regexReplace(outputString, "[\n# $&:\n\t]");
    vector<string> words = splitString(0, outputString, spaces);
    
    for (auto & w : words) {
        
        int ww = 0;
        int xx = iX;
        int i = 0;
        for (auto & l : w) {
            
            ofxPrecisionTextChar & ch = chars[iChars];
            if (hasIndex(structure.h1, iChars)||hasIndex(structure.h2, iChars)||hasIndex(structure.h3, iChars)) {
                int ii = (iChars > 0) ? iChars - 1 : 0;
                ofxPrecisionTextChar prev = chars[ii];
                hershey.setScale( (1.0 / 31.0) * prev.fontSize);
                linePix = (int)(prev.fontSize * (s.lineHeight) );
            }
            ch.bounds = getBounds(ch.letter, ch.fontSize, iX, iY);
            ch.bounds.height = ch.fontSize * s.lineHeight;
            if (ch.letter == "\n") {
                iY += linePix;
                iX = 0;
            } else {
                iX += ch.bounds.width + s.letterSpacing;
                ww += ch.bounds.width + s.letterSpacing;
            }
            if (iX > maxWidth) maxWidth = iX;
            iChars += 1;
            i += 1;
        }
        
        //        ofLog() << wc;
        //        if (w == "\nCurabitur") ofLog() << w << (int)boundingBox.width << (int)iX << (int)ww;
        
        if (iX + 1 > boundingBox.width && ww < boundingBox.width) {
            
            /*-- Hack to go back and offset last chars --*/
            
            for (int i = iChars - w.size(); i < iChars; i++) {
                ofxPrecisionTextChar & ch = chars[i];
                linePix = (int)(ch.fontSize * s.lineHeight);
                ch.bounds.x -= xx + getBounds("_", ch.fontSize, 0, 0).width + s.letterSpacing;
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
    
    if (!dontDraw) {
        
        for (auto & ch : chars) {
            
            if ( int(ch.letter[0]) != 10 && int(ch.letter[0]) != 0 ) {
                
                ofFill();
                ofSetColor( (ch.isLink) ? s.linkColor : s.strokeColor);
                hershey.setScale( (1.0 / 31.0) * ch.fontSize);
                hershey.setColor( (ch.isLink) ? s.linkColor : s.strokeColor );
                hershey.setStroke( (ch.isBold) ? ofClamp(s.strokeWidth + (0.75 * dpi), 0, 2 * dpi) : s.strokeWidth );
                hershey.setItalic( ch.isItalic , 4 );
                
                drawString(ch.letter, ch.fontSize, ch.bounds.x, ch.bounds.y);
                ofNoFill();
                ofSetLineWidth(1);
                
                //                if (ch.isLineEnd) {
                //                    ofSetLineWidth(5);
                //                    ofSetColor(255,0,0);
                //                    ofDrawRectangle(ch.bounds);
                //                }
                //                if (ch.isBold) ofSetColor(255,0,0, 255);
                //                if (!ch.isBold) ofSetColor(255,255,0, 100);
            }
        }
    }
    
    structure.bounds.width = maxWidth;
    float descender = s.fontSize * 0.25;
    structure.bounds.height = iY + linePix;
    
    
    structure.chars = chars;
    
    return structure;
}

bool ofxPrecisionText::hasIndex(vector<int> indexes, int i) {
    
    if ( std::find(indexes.begin(), indexes.end(), i) != indexes.end()) return true;
    return false;
}

void ofxPrecisionText::flagRedraw() {
    
    /*-- Force redraw on FBO cache --*/
    
    shouldRedraw = true;
}

void ofxPrecisionText::setup(string fontLocation) {
    
    dpi = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
    ofxPrecisionTextSettings settings;
    s = settings;
    
    /*-- Setup + optionally load a folder of TTFs --*/
    
    shouldRedraw = false;
    
#ifdef TARGET_OPENGLES
    fboType = GL_RGBA;
#else
    fboType = GL_RGBA32F_ARB;
#endif
    
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


void ofxPrecisionText::clearFboCache() {
    fboCache.clear();
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, glm::vec3 originPoint, ofxPrecisionTextSettings settings) {
    ofPoint p(originPoint.x, originPoint.y);
    return ofxPrecisionText::draw(text, p, settings);
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofPoint originPoint, ofxPrecisionTextSettings settings) {
    
    ofRectangle r(originPoint.x, originPoint.y, 444, 444);
    return ofxPrecisionText::draw(text, r, settings, true);
    
}
ofxPrecisionTextStructure ofxPrecisionText::draw(string text, int x, int y, int width, int height, ofxPrecisionTextSettings settings) {
    
    ofRectangle r(x,y,width,height);
    return ofxPrecisionText::draw(text, r, settings);
    
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofRectangle boundingBox, ofxPrecisionTextSettings settings, bool isPoint) {
    
    s = settings;
    
    string key = getFboKey(text);
    
    if (allocateFbo( key, text, boundingBox, isPoint ) || shouldRedraw) {
        
        fboCache[key]->begin();
        ofClear(255,0);
        ofSetColor(255);
        ofPushMatrix();
        string textCopy = text;
        structCache[key] = drawFbo(textCopy, boundingBox, false, isPoint);
        ofPopMatrix();
        fboCache[key]->end();
        shouldRedraw = false;
    }
    
    ofxPrecisionTextStructure & structure = structCache[key];
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
    fboCache[key]->draw(structCache[key].bounds);
    return structCache[key];
    
}

