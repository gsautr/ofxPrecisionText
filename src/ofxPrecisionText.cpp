
#pragma once
#include "ofxPrecisionText.h"
#include <regex>
    
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


vector<int> replaceAndGenerateIndexes(string & text, string find) {
    vector<int> indexes;
    
    int i = 0;
    for (auto & split : ofSplitString(text, find)) {
        i += split.size();
        indexes.push_back(i);
        ofLog() << "SPLIT: " << split;
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
        ofLog() << t << " " << pos;
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
        ofLog() << "Regex inner: " << reg.match << " " << reg.start;
        text.replace(reg.start - iter, reg.match.size(), "");
        indexes.push_back(iter);
        iter += reg.match.size();
    }
    
    return indexes;
}

void ofxPrecisionText::formatParagraphs(string & text, ofRectangle & boundingBox) {
    
    /*-- Format a text string into paragraphs and line breaks, and update bounding box to reflect this --*/

    
    ofxPrecisionTextStructure structure;
    
    int iter = 0;
    for (auto & reg : getMatchedStrings(text, "\\[\\w+?\\](\\([-:/.[:alnum:]]+?\\))")) {
        ofLog() << "Links " << reg.match << " " << reg.start;
        
        int mid = reg.match.find("](");
        string url = reg.match.substr(mid + 2, reg.match.size() - mid);
        string title = reg.match.substr(1, mid - 1);

        text.replace(reg.start - iter, reg.match.size(),  title);
        
        ofxPrecisionTextHyperlink link;
        link.start = reg.start;
        link.end = reg.start + reg.match.size();
        link.url = url;
        structure.hyperLinks.push_back(link);
        
        iter += reg.match.size() - title.size();
    }
//    structure.boldIndexes = regexReplace(text, "[**][\w\W]*[**]");
//    structure.italicIndexes = regexReplace(text, "[*][\w\W]*[*]");
    structure.newLines = regexReplace(text, "\n");
    
    
    
    int totalHeight = 0;
    int totalWidth = 0;
    
    int iChars = 0;
    int iWidth = 0;
    int iX = 0;
    int iY = 0;
    
    vector<ofRectangle> charRects;
    
    string fontKey = defineFont();
    
    int iterateChars = 0;
    
    
    for (auto & paragraph : ofSplitString(text, "\n")) {
        
        /*-- Produces total width + height, and line breaks for a paragraph string --*/
        
        int iterateWidth = 0;
        
        totalHeight += (int)(fontSize * lineHeight);
        
        for (auto & word : ofSplitString(paragraph, " ")) {
            
            ofRectangle b = getBounds(word + "_", 0, 0);
            
            iterateWidth += b.width;
            if (iterateWidth > totalWidth) {
                totalWidth = iterateWidth;
            }
            if (iterateWidth > boundingBox.width) {
                iterateWidth = b.width;
                totalHeight += (int)(fontSize * lineHeight);
                text.insert(iterateChars, "\n");
            }
            iterateChars += word.size() + 1;
        }
    }
    
    
    
    
    
    boundingBox.width = totalWidth;
    boundingBox.height = totalHeight;
    boundingBox.height += (fontIndex != 0) ? (fontSize * 0.25) + 1 : fontSize * 0.5;
    
    structure.boundingBox = boundingBox;
    
}

void ofxPrecisionText::drawString(string fontKey, string text, int xx, int yy) {
    
    ofSetColor(strokeColor);
    hershey.setStroke( (isBold) ? hersheyStroke + 1 : hersheyStroke );
    
    if (fontIndex == 0) {
        hershey.draw(text, xx, yy + (fontSize * 0.25));
    } else {
        fontCache[fontKey].drawString(text, xx, yy);
    }
}

ofRectangle ofxPrecisionText::drawFbo(string text, ofRectangle boundingBox) {

    isBold = false;
    isItalic = false;
    isHyperlink = false;
    
    /*-- Draw text string with line breaks and formatting --*/
    
    ofLogNotice("[ofxPrecisionText]") << "Drawing fbo, " << boundingBox.width << "x" << boundingBox.height;
    
    int initialWidth = getBounds(text, 0, 0).width;
    int initialHeight = getBounds("ITPBgyq", 0, 0).height;
    int descenderHeight = (fontIndex != 0) ? initialHeight * 1.75 : initialHeight + 1;
    
    ofRectangle b = boundingBox;
    ofRectangle fboBox = boundingBox;
    
    
    ofFill();
    ofSetColor(strokeColor);
    string fontKey = defineFont();
    
    formatParagraphs(text, fboBox);
    
    int linePix = (int)(fontSize * lineHeight);
    
    int iLine = 0;
    int iterChars = 0;
    
    vector<ofRectangle> charRects;
    
    
    
    
    for (auto & line : ofSplitString(text, "\n")) {
        
        /*-- Define horizontal & vertical positioning --*/
        
        int xx = 0;
        int yy = (linePix * iLine) + descenderHeight;
        ofRectangle rect = getBounds(line, 0, yy);
        if (horizontalAlign == 0) xx = (fboBox.width - rect.width) /2;
        if (horizontalAlign == 1) xx = (fboBox.width - rect.width) ;
        
        
        int iterX = xx;
        
        for ( int i = 0; i < line.size() ; i++) {
            string letter = line.substr(i, 1);
            charRects.push_back( getBounds( letter, iterX, yy) );
            drawString(fontKey, letter, charRects.back().x, charRects.back().y);
            iterX += charRects.back().width;
        }
        
//        for (auto & piece : ofSplitString(line, "**")) {
        
//            for (auto & pieceItal : ofSplitString(boldPiece, "))
//            drawString(fontKey, piece, iterBoldWidth, yy );
//            iterBoldWidth += getBounds(piece, 0, 0).width;
//            if (piece != ofSplitString(line, "**").back() ) isBold = !isBold;
//        }
        
        
        
        iLine  += 1;
        
        
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
        b = drawFbo(textCopy, b);
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
    
