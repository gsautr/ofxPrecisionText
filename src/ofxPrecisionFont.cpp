/*
 *  ofxPrecisionFont
 *  Gilbert Sinnott
 *  www.autr.tv, June 2019
 *
 *  ofxPrecisionFont originally created by Tobias Zimmer, August 2016.
 *  www.tobiaszimmer.net
 *
 *  Fonts originally developed by Dr. Allen V. Hershey in 1967.
 *  Font vectors made available by Paul Bourke.
 *  paulbourke.net/dataformats/hershe
 *
 */

#include "ofxPrecisionFont.h"
#include "simplexCharacterSet.h"
#include "hersheyCharacterSets.h"

void ofxPrecisionFont::setFontList(vector<string> list) {
    names = list;
}
vector<string> ofxPrecisionFont::getFontList() {
    return names;
}

//--------------------------------------------------------------
ofxPrecisionFont::ofxPrecisionFont(){
    color = ofColor(0);
    scale = 1;
    stroke = 1;
    centered = false;
    angle = 0;
    fontIndex = 0;
    
    names = {"default", "gothicita", "symbolic", "scripts", "timesrb", "cyrillic", "markers", "meteorology", "gothitt", "gothiceng", "mathlow", "scriptc", "gothgbt", "gothicger", "mathupp", "timesg", "astrology", "japanese", "timesr", "music", "cyrilc_1", "greek", "rowmans", "rowmand", "timesi", "cursive", "futuram", "gothgrt", "futural", "rowmant", "timesib", "greekc", "greeks"};
    
    hersheyFonts["default"] = basic;
    hersheyFonts["gothicita"] = gothicita;
    hersheyFonts["symbolic"] = symbolic;
    hersheyFonts["scripts"] = scripts;
    hersheyFonts["timesrb"] = timesrb;
    hersheyFonts["cyrillic"] = cyrillic;
    hersheyFonts["markers"] = markers;
    hersheyFonts["meteorology"] = meteorology;
    hersheyFonts["gothitt"] = gothitt;
    hersheyFonts["gothiceng"] = gothiceng;
    hersheyFonts["mathlow"] = mathlow;
    hersheyFonts["scriptc"] = scriptc;
    hersheyFonts["gothgbt"] = gothgbt;
    hersheyFonts["gothicger"] = gothicger;
    hersheyFonts["mathupp"] = mathupp;
    hersheyFonts["timesg"] = timesg;
    hersheyFonts["astrology"] = astrology;
    hersheyFonts["japanese"] = japanese;
    hersheyFonts["timesr"] = timesr;
    hersheyFonts["music"] = music;
    hersheyFonts["cyrilc_1"] = cyrilc_1;
    hersheyFonts["greek"] = greek;
    hersheyFonts["rowmans"] = rowmans;
    hersheyFonts["rowmand"] = rowmand;
    hersheyFonts["timesi"] = timesi;
    hersheyFonts["cursive"] = cursive;
    hersheyFonts["futuram"] = futuram;
    hersheyFonts["gothgrt"] = gothgrt;
    hersheyFonts["futural"] = futural;
    hersheyFonts["rowmant"] = rowmant;
    hersheyFonts["timesib"] = timesib;
    hersheyFonts["greekc"] = greekc;
    hersheyFonts["greeks"] = greeks;
    
    simplex = hersheyFonts[names[fontIndex]];
}


void ofxPrecisionFont::setFont(int i) {
    
    if (i >= names.size()) i = names.size() - 1;
    if (i < 0) i = 0;
    fontIndex = i;
    
    
    auto it = fontCache.find(names[fontIndex]);
    if (it == fontCache.end()) {
        
        ofxPrecisionFontStructure font;
        simplex = hersheyFonts[names[fontIndex]];
        
        ofLogNotice("[ofxPrecisionText]") << "Loading Hershey" << names[fontIndex];
        
        int maxHeight = 0;
        int maxWidth = 0;
        for (int i = 0; i < simplex.size(); i++) {
            
            int asciiValue = i + 32;
            
            ofPath chPath;
            chPath.moveTo(getSimplex(asciiValue - 32, 2, asciiValue - 32, 3), getSimplex(asciiValue - 32, 3));
            
            int height = 0;
            int width = simplex[asciiValue - 32][1];
            
            for (int i = 4; i <= simplex[asciiValue - 32][0] * 2; i += 2)
            {
                int x = getSimplex(asciiValue - 32, i, asciiValue - 32,i + 1);
                int y = getSimplex(asciiValue - 32,i + 1);
                if (y > height) height = y;
                
                if (x != -1) chPath.lineTo(x, y);
                if (x == -1) {
                    int xx = getSimplex(asciiValue - 32, i + 2, asciiValue - 32, i + 3);
                    int yy = getSimplex(asciiValue - 32, i + 3);
                    
                    chPath.moveTo(xx, yy);
                    i += 2;
                }
            }
            
            if (height > maxHeight) maxHeight = height;
            if (width > maxWidth) maxWidth = width;
            
            chPath.setFilled(false);
            
            /*-- Set font char --*/
            
            ofxPrecisionFontChar ch;
            ch.width = width;
            ch.height = height;
            ch.path = chPath;
            font.chars[i] = ch;
            
        }
        
        /*--- Set font char heights --*/
        
        font.maxCharWidth = maxWidth;
        font.maxCharHeight = maxHeight;
        
        fontCache[names[fontIndex]] = font;
        
        
    }
}

int ofxPrecisionFont::getNumFonts() {
    return names.size();
}


void ofxPrecisionFont::setItalic(bool b, float slant) {
    italic = (b) ? slant : 0.0;
}


float ofxPrecisionFont::getSimplex(int a, int b, int aa, int bb) {
    float ita = 0;
    if (aa != -1 && bb != -1) {
        float h = simplex[aa][bb];
        ita = (h / 21.0) * italic;
    }
    return simplex[a][b] + ita;
}

ofPath ofxPrecisionFont::getPath(int i) {
    return fontCache[names[fontIndex]].chars[i - 32].path;
}


ofRectangle ofxPrecisionFont::getBounds(string str, float fSize, int x, int y) {
    float s = (1.0 / 31.0) * fSize;
    return ofRectangle(x, y, getWidth(str) * s, getHeight() * s);
}

//--------------------------------------------------------------
float ofxPrecisionFont::getWidth(string str){
    return fontCache[names[fontIndex]].chars[str[0] - 32].width;
}

//--------------------------------------------------------------
float ofxPrecisionFont::getHeight() {
    return fontCache[names[fontIndex]].maxCharHeight;
}


