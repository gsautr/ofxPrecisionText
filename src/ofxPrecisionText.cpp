
#pragma once
#include "ofxPrecisionText.h"
#include <regex>


/**
 * Bool
 */
bool need_paragraph = true;
bool current_paragraph = false;
bool current_list = false;


bool ofxPrecisionText::allocateFbo(string fboKey, string text, ofRectangle boundingBox, bool isPoint) {
    
    auto it = fboCache.find(fboKey);
    if (it == fboCache.end()) {
        ofFbo * fbo = new ofFbo();
        structCache[fboKey] = drawFbo(text, boundingBox, true, isPoint);;
        fbo->allocate(structCache[fboKey].bounds.width, structCache[fboKey].bounds.height, fboType, numSamples);
        samplesChanged = false;
        ofLogNotice("[ofxPrecisionText]") << "Adding FBO with " << numSamples << " samples, " << structCache[fboKey].bounds.width << " x " << structCache[fboKey].bounds.height;
        fboCache[fboKey] = fbo;
        return true;
    } else {
        return false;
    }
}
    
string ofxPrecisionText::getFboKey(string text) {

    /*-- Get unique hash for FBO cache --*/

    string key = fontList[fontIndex] + ofToString(fontSize) + ofToString(strokeColor.r) + ofToString(strokeColor.g) + ofToString(strokeColor.b) + ofToString(strokeColor.a) + ofToString(numSamples) + text + ofToString(horizontalAlign) + ofToString(lineHeight) + ofToString(pixelAligned) + ofToString(letterSpacing);
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
            results.push_back(m);
            next++;
        }
    } catch (std::regex_error& e) {
        // Syntax error in the regular expression
        ofLogError("Error in regular expression");
    }
    return results;
}

vector<int> ofxPrecisionText::regexReplace(string & text, string regex) {
    
    vector<int> indexes;
    int iter = 0;
    for (auto & reg : getMatchedStrings(text, regex)) {
//        ofLog() << "Regex inner: " << reg.match << " " << reg.start << reg.size;
        int start = reg.start;
//        text.replace(start, reg.match.size(), "");
        indexes.push_back(start);
//        iter += reg.match.size();
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
        string fontKey = defineFont();
        b = fontCache[fontKey].getStringBoundingBox(text, x, y);
        b.y = y + (fontSize * 1);
    }
    
    return b;
}
void ofxPrecisionText::drawString(string text, float fSize, float xx, float yy) {
    
    if (pixelAligned) {
        xx = (int)xx;
        yy = (int)yy;
    }
    
//    float spc = ((1.0 - lineHeight) / 2) * fontSize;
    
    if (fontIndex == 0) {
        hershey.setScale( ( 1.0 / 31.0 ) * fSize );
        hershey.draw(text, xx, yy + getBounds(text, fSize, 0,0).height);
    } else {
        string fontKey = defineFont();
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

void ofxPrecisionText::setFontSize(float fSize) {
    fontSize = fSize;
}
ofxPrecisionTextStructure ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox, bool dontDraw, bool isPoint) {

    
    /*-- Draw text string with line breaks and formatting --*/
    
    string fontKey = defineFont();
    ofxPrecisionTextStructure structure = parsetext(text, false);
    text = structure.text;
    
//    for (auto & b : structure.bold) {
//
//        ofLog() << "Bold: " << b;
//    }
//    for (auto & b : structure.italic) {
//
//        ofLog() << "Italic: " << b;
//    }
//    for (auto & b : structure.h1) {
//
//        ofLog() << "H1: " << b;
//    }
//
    
    vector<ofxPrecisionTextChar> chars;
    string outputString = "";
    
    /* -- Generate chars -- */
    
    bool isBold = false;
    bool isItalic = false;
    bool isH1 = false;
    bool isH2 = false;
    bool isH3 = false;
    
    for (int i = 0; i < text.size(); i++) {
        
        if (hasIndex(structure.bold, i)) isBold = !isBold;
        if (hasIndex(structure.italic, i)) isItalic = !isItalic;
        if (hasIndex(structure.h1, i)) isH1 = !isH1;
        if (hasIndex(structure.h2, i)) isH2 = !isH2;
        if (hasIndex(structure.h3, i)) isH3 = !isH3;
        
        ofxPrecisionTextChar ch;
        ch.isHeading = 0;
        ch.fontSize = fontSize;
        ch.isBold = false;
        ch.isItalic =  false;
        ch.isLink = false;
        ch.isNewline = false;
        ch.isBold = isBold;
        ch.isItalic = isItalic;
        
        if (isH1) ch.isHeading += 1;
        if (isH2) ch.isHeading += 2;
        if (isH3) ch.isHeading += 3;
        if (ch.isHeading > 0) {
            float extra = (fontSize * 3) - fontSize;
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
    
    int linePix = (int)(fontSize * lineHeight);
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
                linePix = (int)(prev.fontSize * (lineHeight) );
            }
            ch.bounds = getBounds(ch.letter, ch.fontSize, iX, iY);
            ch.bounds.height = ch.fontSize * lineHeight;
            if (ch.letter == "\n") {
                iY += linePix;
                iX = 0;
            } else {
                iX += ch.bounds.width + letterSpacing;
                ww += ch.bounds.width + letterSpacing;
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
                linePix = (int)(ch.fontSize * lineHeight);
                ch.bounds.x -= xx + getBounds("_", ch.fontSize, 0, 0).width + letterSpacing;
                ch.bounds.y += linePix;
            }
            
            /*-- Iterate height and width --*/
            
            iY += linePix;
            iX = ww;
            maxWidth = boundingBox.width;
            
            
        }
        
    }
    
    
    
    /*--- Hacky horizontal  --*/
    
    if (horizontalAlign != -1 && !isPoint) {
        float lastY = chars[0].bounds.y;
        int iLine = 0;
        for (int i = 0; i < chars.size(); i++) {
            ofxPrecisionTextChar & ch = chars[i];
            
                if (ch.bounds.y != lastY || i+1 == chars.size()) {
                    float tt = maxWidth;
                    string ss ="";
                    for (int ii = iLine; ii < i; ii++) {
                        if (chars[ii].letter[0] != '\n') {
                            tt  -= chars[ii].bounds.width + letterSpacing;
                            ss += chars[ii].letter;
                        }
                    }
                    
                    tt -= letterSpacing;
                    
                    ss = "";
                    
                    if (horizontalAlign == 0) tt /= 2;
                    
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
            
                lastY = ch.bounds.y;
            
        }
    }
    
    if (!dontDraw) {
    
        for (auto & ch : chars) {
            
            if ( int(ch.letter[0]) != 10 && int(ch.letter[0]) != 0 ) {
                
                ofFill();
                ofSetColor( (ch.isLink) ? linkColor : strokeColor);
                hershey.setScale( (1.0 / 31.0) * ch.fontSize);
                hershey.setColor( (ch.isLink) ? linkColor : strokeColor );
                hershey.setStroke( (ch.isBold) ? ofClamp(hersheyStroke + 1, 0, 2) : hersheyStroke );
                hershey.setItalic( ch.isItalic , 4 );
            
                drawString(ch.letter, ch.fontSize, ch.bounds.x, ch.bounds.y);
                ofNoFill();
                ofSetLineWidth(1);
                
                ofSetColor(255,255,0, 100);
//                ofDrawRectangle(ch.bounds);
            }
        }
    }
    
    structure.bounds.width = maxWidth;
    float descender = fontSize * 0.25;
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

void ofxPrecisionText::setLineHeight(float lHeight) {

    lineHeight = lHeight;
}


void ofxPrecisionText::setup(string fontLocation) {
    
    
    
    /*-- Setup + optionally load a folder of TTFs --*/

    shouldRedraw = false;
    samplesChanged = false;
    
#ifdef TARGET_OPENGLES
    fboType = GL_RGBA;
#else
    fboType = GL_RGBA32F_ARB;
#endif
    
#ifdef TARGET_RASPBERRY_PI
    numSamples = 0;
#else
    numSamples = 8;
#endif
    pixelAligned = false;
    cacheCharLimit = 200;
    fontSize = 14;
    fontIndex = 0;
    letterSpacing = 0;
    hersheyStroke = 1.2;
    lineHeight = 1;
    linkColor = ofColor(255,0,0);
    
    ofDirectory dir(fontLocation);
    dir.allowExt("ttf");
    hersheyBaseline = 21.0;

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
ofxPrecisionTextStructure ofxPrecisionText::draw(string text, glm::vec3 originPoint, int horzAlign, int vertAlign) {
    ofPoint p(originPoint.x, originPoint.y);
    return ofxPrecisionText::draw(text, p, horzAlign, vertAlign);
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofPoint originPoint, int horzAlign, int vertAlign) {
    
    ofRectangle r(originPoint.x, originPoint.y, 444, 444);
    return ofxPrecisionText::draw(text, r, horzAlign, vertAlign, true);
    
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofRectangle boundingBox, int horzAlign, int vertAlign, bool isPoint) {
    
    horizontalAlign = horzAlign;
    verticalAlign = vertAlign;
    
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
        
        if (horizontalAlign == 0) structCache[key].bounds.x -= (r.width )/2;
        if (horizontalAlign == -1) structCache[key].bounds.x -= (r.width);
        if (verticalAlign == 0) structCache[key].bounds.y -= (r.height) / 2;
        if (verticalAlign == 1) structCache[key].bounds.y -= (r.height);
        
    } else {
        
        if (horizontalAlign == 0) structCache[key].bounds.x += (boundingBox.width - r.width)/2;
        if (horizontalAlign == 1) structCache[key].bounds.x += (boundingBox.width - r.width);
        if (verticalAlign == 0) structCache[key].bounds.y += (boundingBox.height - r.height) / 2;
        if (verticalAlign == -1) structCache[key].bounds.y += (boundingBox.height - r.height);
        
    }
    
    ofSetColor(255);
    fboCache[key]->draw(structCache[key].bounds);
    return structCache[key];
    
}
    
