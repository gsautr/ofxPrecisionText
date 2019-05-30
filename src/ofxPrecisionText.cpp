
#pragma once
#include "ofxPrecisionText.h"
#include <regex>
    
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

ofRectangle ofxPrecisionText::getBounds(string text, float x, float y) {

    /*-- Get bounds for single text string --*/

    if (text == " ") text = "_";

    ofRectangle b;
    
    if (fontIndex == 0) {
        b = hershey.getBounds(text, x, y + (fontSize * 0.32));
    } else {
        string fontKey = defineFont();
        b = fontCache[fontKey].getStringBoundingBox(text, x, y);
        b.y = y + (fontSize * 1);
    }
    
    return b;
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


void ofxPrecisionText::drawString(string fontKey, string text, float xx, float yy) {
    
    if (pixelAligned) {
        xx = (int)xx;
        yy = (int)yy;
    }
    
    float spc = ((1.0 - lineHeight) / 2) * fontSize;
    
    if (fontIndex == 0) {
        hershey.draw(text, xx, yy + (fontSize * 0.68) - spc);
    } else {
        fontCache[fontKey].drawString(text, xx, yy - spc);
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
ofxPrecisionTextStructure ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox, bool dontDraw, bool useHorizontal, bool isPoint) {

    
    /*-- Draw text string with line breaks and formatting --*/
    
    string fontKey = defineFont();
    
    ofxPrecisionTextStructure structure;
    
    /*-- Parse hyperlinks --*/
    
    int iPos = 0;
    for (auto & reg : getMatchedStrings(text, "\\[\\w+?\\](\\([-:/.[:alnum:]]+?\\))")) {
        
        int mid = reg.match.find("](");
        string url = reg.match.substr(mid + 2, reg.match.size() - mid);
        string title = reg.match.substr(1, mid - 1);
        
        text.replace(reg.start - iPos, reg.match.size(),  title);
        
        ofxPrecisionTextHyperlink link;
        link.start = reg.start - iPos;
        link.end = reg.start + title.size() - iPos;
        link.url = url;
        structure.hyperLinks.push_back(link);
        
        iPos += reg.match.size() - title.size();
    }
    
    /*-- Parse REGEX formatting --*/
    
    structure.boldIndexes = regexReplace(text, "[**][\w\W]*[**]");
    structure.italicIndexes = regexReplace(text, "\_(.*?)\_");
    
//    for (auto & i : regexReplace(text, "\# (.*)\n")) {
//        ofLog() << "REG" << i;
//    }
//    for (auto & i : structure.italicIndexes) {
//        ofLog() << "Italic" << i << text[i];
//    }
    
    
    vector<ofxPrecisionTextChar> chars;
    string outputString = "";
    
    /* -- Generate chars -- */
    
    bool isBold = false;
    bool isItalic = false;
    
    for (int i = 0; i < text.size(); i++) {
        
        if (hasIndex(structure.boldIndexes, i)) {
            isBold = !isBold;
            i +=2;
        }
        if (hasIndex(structure.italicIndexes, i)) {
            isItalic = !isItalic;
            i +=1;
        }
        
        ofxPrecisionTextChar ch;
        ch.fontSize = fontSize;
        ch.isBold = false;
        ch.isItalic =  false;
        ch.isLink = false;
        ch.isNewline = false;
        ch.isBold = isBold;
        ch.isItalic = isItalic;
        
        ofxPrecisionTextHyperlink link;
        if (hasLink(structure.hyperLinks, i, link)) {
            ch.isLink = true;
        }
        
        
        hershey.setScale( (1.0 / 31.0) * ch.fontSize );
        string l = ofToString(text[i]);
        ch.letter = l;
        ch.bounds = getBounds(l, 0,0);
        chars.push_back(ch);
        outputString += l;
        
    }
    
    int linePix = (int)(fontSize * lineHeight);
    float iX = 0;
    float iY = 0;
    float maxWidth = 0;
    int iChars = 0;
    bool nl = false;
    
    structure.newLines.push_back(0);
    
    vector<int> spaces = regexReplace(outputString, "[\n# $&:\n\t]");
    vector<string> words = splitString(0, outputString, spaces);
    
    for (auto & w : words) {
        
        int ww = 0;
        int xx = iX;
        int i = 0;
        for (auto & l : w) {
            
            ofxPrecisionTextChar & ch = chars[iChars];
            ch.bounds = getBounds(ch.letter, iX, iY);
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
        
        if (iX > boundingBox.width) {
            
            /*-- Hack to go back and offset last chars --*/
            
            for (int i = iChars - w.size(); i < iChars; i++) {
                ofxPrecisionTextChar & ch = chars[i];
                ch.bounds.x -= xx + getBounds("_", 0, 0).width + letterSpacing;
                ch.bounds.y += linePix;
            }
            
            /*-- Iterate height and width --*/
            
            iY += linePix;
            iX = ww;
            maxWidth = boundingBox.width;
            
            
        }
        
    }
    
    
    
    /*--- Hacky horizontal  --*/
    
    if (horizontalAlign != -1 && useHorizontal) {
    
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
                hershey.setColor( (ch.isLink) ? linkColor : strokeColor );
                hershey.setStroke( (ch.isBold) ? ofClamp(hersheyStroke + 1, 0, 2) : hersheyStroke );
                hershey.setItalic( ch.isItalic , 4 );
            
                drawString(fontKey, ch.letter, ch.bounds.x, ch.bounds.y);
                ofNoFill();
//                ofDrawRectangle(ch.bounds);
            }
        }
    }
    
    /*-- Update bounding box structure for FBO --*/
    
    if (isPoint) {
        ofLog() << "POINT:" << text;
        ofLog() << maxWidth << iY;
        maxWidth = 400;
    }
    
    structure.bounds.width = maxWidth;
    float descender = fontSize * 0.25;
    structure.bounds.height = iY + linePix + descender;
    structure.bounds.x = boundingBox.x;
    structure.bounds.y = boundingBox.y;
    
    if (horizontalAlign == 0) structure.bounds.x += (boundingBox.width - maxWidth)/2;
    if (horizontalAlign == 1) structure.bounds.x += (boundingBox.width - maxWidth);
    if (verticalAlign == 0) structure.bounds.y += (boundingBox.height - structure.bounds.height) / 2;
    if (verticalAlign == -1) structure.bounds.y += (boundingBox.height - structure.bounds.height);
    
    /*--- Offset all for external use --*/
    
    for (auto & ch : chars) {
        ch.bounds.x += structure.bounds.x;
        ch.bounds.y += structure.bounds.y;
    }
    
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
    fontIndex = 0;
    letterSpacing = 0;
    hersheyStroke = 1.5;
    lineHeight = 1.4;
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
ofxPrecisionTextStructure ofxPrecisionText::draw(string text, glm::vec3 originPoint, float fSize, int horzAlign, int vertAlign) {
    ofPoint p(originPoint.x, originPoint.y);
    return ofxPrecisionText::draw(text, p, fSize, horzAlign, vertAlign);
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofPoint originPoint, float fSize, int horzAlign, int vertAlign) {
    
    ofRectangle r(originPoint.x, originPoint.y, 444, 444);
    return ofxPrecisionText::draw(text, r, fSize, horzAlign, vertAlign, true);
    
}

ofxPrecisionTextStructure ofxPrecisionText::draw(string text, ofRectangle boundingBox, float fSize, int horzAlign, int vertAlign, bool isPoint) {
    
    fontSize = fSize;
    horizontalAlign = horzAlign;
    verticalAlign = vertAlign;
    
    string key = getFboKey(text);
    if (allocateFbo( key, text, boundingBox ) || shouldRedraw) {
    
        fboCache[key]->begin();
        ofClear(255,0);
        ofSetColor(255);
        ofPushMatrix();
        string textCopy = text;
        structCache[key] = drawFbo(textCopy, boundingBox, false, true, isPoint);
        ofPopMatrix();
        fboCache[key]->end();
        shouldRedraw = false;
    }
    
    ofSetColor(255);
    fboCache[key]->draw(structCache[key].bounds);
    return structCache[key];
    
}
    
