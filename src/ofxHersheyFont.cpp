/*
*  ofxHersheyFont.cpp
*
*  Branched by Gilbert Sinnott, www.autr.tv, June 2019
*
*  + parser for .dhf files (see dhf-parser)
*  + simplex Hershey font sets (inc. cyrillic, greek, japanese)
*  + integration w. ofxPrecisionText
*
*  ofxHersheyFont originally created by Tobias Zimmer, August 2016.
*  www.tobiaszimmer.net
*
*  Fonts originally developed by Dr. Allen V. Hershey in 1967.
*  Font vectors made available by Paul Bourke.
*  paulbourke.net/dataformats/hershey
*
*/

#include "ofxHersheyFont.h"
#include "simplexCharacterSet.h"
#include "hersheyCharacterSets.h"

void ofxHersheyFont::setFontList(vector<string> list) {
    names = list;
}
vector<string> ofxHersheyFont::getFontList() {
    return names;
}

//--------------------------------------------------------------
ofxHersheyFont::ofxHersheyFont(){
	color = ofColor(0);
    scale = 1;
    stroke = 1;
    centered = false;
    angle = 0;
    fontIndex = 0;
    
    names = {"default", "gothicita", "symbolic", "scripts", "timesrb", "cyrillic", "markers", "meteorology", "gothitt", "gothiceng", "mathlow", "scriptc", "gothgbt", "gothicger", "mathupp", "timesg", "astrology", "japanese", "timesr", "music", "cyrilc_1", "greek", "rowmans", "rowmand", "timesi", "cursive", "futuram", "gothgrt", "futural", "rowmant", "timesib", "greekc", "greeks"};
    
    fonts["default"] = basic;
    fonts["gothicita"] = gothicita;
    fonts["symbolic"] = symbolic;
    fonts["scripts"] = scripts;
    fonts["timesrb"] = timesrb;
    fonts["cyrillic"] = cyrillic;
    fonts["markers"] = markers;
    fonts["meteorology"] = meteorology;
    fonts["gothitt"] = gothitt;
    fonts["gothiceng"] = gothiceng;
    fonts["mathlow"] = mathlow;
    fonts["scriptc"] = scriptc;
    fonts["gothgbt"] = gothgbt;
    fonts["gothicger"] = gothicger;
    fonts["mathupp"] = mathupp;
    fonts["timesg"] = timesg;
    fonts["astrology"] = astrology;
    fonts["japanese"] = japanese;
    fonts["timesr"] = timesr;
    fonts["music"] = music;
    fonts["cyrilc_1"] = cyrilc_1;
    fonts["greek"] = greek;
    fonts["rowmans"] = rowmans;
    fonts["rowmand"] = rowmand;
    fonts["timesi"] = timesi;
    fonts["cursive"] = cursive;
    fonts["futuram"] = futuram;
    fonts["gothgrt"] = gothgrt;
    fonts["futural"] = futural;
    fonts["rowmant"] = rowmant;
    fonts["timesib"] = timesib;
    fonts["greekc"] = greekc;
    fonts["greeks"] = greeks;
    
    simplex = fonts[names[fontIndex]];
}


void ofxHersheyFont::setFont(int i) {
    if (i >= names.size()) i = names.size() - 1;
    if (i < 0) i = 0;
    fontIndex = i;
    simplex = fonts[names[fontIndex]];
    
    auto it = charCache.find(names[fontIndex]);
    if (it == charCache.end()) {
        
        ofLogNotice("[ofxPrecisionText]") << "Loading Hershey" << names[fontIndex];
        map<int, ofPath> charSet;
        map<int, int> widths;
        int height = 0;
        for (int i = 0; i < simplex.size(); i++) {
           
            int asciiValue = i + 32;
            
            ofPath chPath;
            chPath.moveTo(getSimplex(asciiValue - 32, 2, asciiValue - 32, 3), getSimplex(asciiValue - 32, 3));
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
            
            chPath.setFilled(false);
            charSet[i] = chPath;
            widths[i] = simplex[asciiValue - 32][1];
            
        }
        
        widthCache[names[fontIndex]] = widths;
        heightCache[names[fontIndex]] = height;
        charCache[names[fontIndex]] = charSet;
    }
}

int ofxHersheyFont::getNumFonts() {
    return names.size();
}
//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos) {
    draw(stringValue, xPos, yPos, stroke, color);
}
//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float str) {
    stroke = str;
    draw(stringValue, xPos, yPos, stroke, color);
}

//--------------------------------------------------------------
void ofxHersheyFont::draw(string stringValue, float xPos, float yPos, float str, ofColor col) {
	
    
    if (stringValue.size() <= 0) return;
    
    stroke = str;
    color = col;
//    centered = cent;
//    angle = ang;
    
	float characterXPos = 0;
	float center = 0;
	if (centered) center = getWidth(stringValue) / 2;

	ofPushMatrix();
	ofTranslate(xPos, yPos);
//    ofRotate(angle);
	ofTranslate(-center, 0);
	
		//iterate through each character of the input string
		for (int i = 0; i < stringValue.size(); i++)
		{
			ofPushMatrix();
			ofTranslate(characterXPos, 0);
			ofScale(scale, -scale);

				//get ascii value of specific character from the input string
				int asciiValue = stringValue.at(i);

				//if character is not available, use questionmark
				if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
            
				//draw the character
				drawChar(asciiValue, stroke);

				//update xPos / starting position for the next character
				float charWidth = simplex[asciiValue - 32][1] * scale;
				characterXPos += charWidth;

			ofPopMatrix();
		}

	ofPopMatrix();
}



void ofxHersheyFont::draw(string stringValue, ofRectangle rectangle, int horizontalAlign, int verticalAlign, float str, ofColor col) {
    
    if (stringValue.size() <= 0) return;
    
    stroke = str;
    color = col;
    
    int xPos = rectangle.getX();
    int yPos = rectangle.getY();
    int width = rectangle.getWidth();
    int height = rectangle.getHeight();
    
    float characterXPos = 0;
    float center = 0;
    
    /* Horizontal - center */
    
    if (horizontalAlign == 0) {
        center = getWidth(stringValue) / 2;
        xPos += width/2;
    }
    
    /* Horizontal - right */
    
    if (horizontalAlign == 1) {
        center = getWidth(stringValue);
        xPos += width;
    }
    
    /* Vertical - top */
    
    if (verticalAlign == -1) {
        yPos += getHeight();
    }
    
    /* Vertical - middle */
    
    if (verticalAlign == 0) {
        yPos += ( height / 2 ) + ( getHeight() / 2 );
    }
    
    /* Vertical - bottom */
    
    if (verticalAlign == 1) {
        yPos += height;
    }
    
    ofPushMatrix();
    ofTranslate(xPos, yPos);
    ofTranslate(-center, 0);
    
    for (int i = 0; i < stringValue.size(); i++) {
        ofPushMatrix();
        ofTranslate(characterXPos, 0);
        ofScale(scale, -scale);
        
        //get ascii value of specific character from the input string
        int asciiValue = stringValue.at(i);
        
        //if character is not available, use questionmark
        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
        
        //draw the character
        drawChar(asciiValue, stroke);
        
        //update xPos / starting position for the next character
        float charWidth = simplex[asciiValue - 32][1] * scale;
        characterXPos += charWidth;
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}


void ofxHersheyFont::setItalic(bool b, float slant) {
    italic = (b) ? slant : 0.0;
}


float ofxHersheyFont::getSimplex(int a, int b, int aa, int bb) {
    float ita = 0;
    if (aa != -1 && bb != -1) {
        float h = simplex[aa][bb];
        ita = (h / 21.0) * italic;
    }
    return simplex[a][b] + ita;
}

ofPath ofxHersheyFont::getPath(int i) {
    return charCache[names[fontIndex]][i - 32];
}

//--------------------------------------------------------------
void ofxHersheyFont::drawChar(int asciiValue, float stroke) {
	
	ofPath chPath = charCache[names[fontIndex]][asciiValue - 32];
	chPath.setStrokeColor(color);
	chPath.setStrokeWidth(stroke);
    
    chPath.draw();
}


ofRectangle ofxHersheyFont::getBounds(string stringValue, float fSize, int x, int y) {
    float s = (1.0 / 31.0) * fSize;

    return ofRectangle(x, y, widthCache[names[fontIndex]][stringValue[0] - 32] * s, heightCache[names[fontIndex]] * s);
}

//--------------------------------------------------------------
float ofxHersheyFont::getWidth(string stringValue){
	float stringWidth = 0;

	for (int i = 0; i < stringValue.size(); i++)
	{
		int asciiValue = stringValue.at(i);

		//questionmark if character not available
		if (asciiValue < 32 || asciiValue > 126) asciiValue = 63; 
		
		stringWidth += (float)simplex[asciiValue - 32][1] * scale;
	}

	return stringWidth;
}

//--------------------------------------------------------------
float ofxHersheyFont::getHeight() {
	//the height of a capital letter is 21px (scale 1)
	float stringHeight = (float) heightCache[names[fontIndex]] * scale;

	return stringHeight;
}


//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float scale) {
	return getPath(stringValue, 0, 0, scale);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float xPos, float yPos, float scale) {
    
    ofPath path;
    
    //iterate through each character of the input string
    for (int i = 0; i < stringValue.size(); i++)
    {
        //get ascii value of specific character from the inout string
        int asciiValue = stringValue.at(i);
        
        //only draw character if vectors are available, otherwise draw questionmark
        if (asciiValue < 32 || asciiValue > 126) asciiValue = 63;
        
        //moveto first coordinate of the character
        
        int xx = xPos + getSimplex(asciiValue - 32, 2, asciiValue - 32, 3) * scale;
        int yy = yPos + (-1) * getSimplex(asciiValue - 32, 3) * scale;
        
        
        
        path.moveTo(xx, yy);
        
        //iterate through points of each character
        for (int j = 4; j <= getSimplex(asciiValue - 32, 0) * 2; j += 2)
        {
            int x = getSimplex(asciiValue - 32, j, asciiValue - 32, j + 1);
            int y = (-1) * getSimplex(asciiValue - 32, j + 1);
            
            if (x != -1) {
                int xa = xPos + x * scale;
                int ya = yPos + y * scale;
                path.lineTo(xa, ya);
            }
            
            if (x == -1) {
                int xb = xPos + getSimplex(asciiValue - 32, j + 2, asciiValue - 32, j + 3) * scale;
                int yb = yPos + (-1) * getSimplex(asciiValue - 32, j + 3) * scale;
                path.moveTo(xb, yb);
                j += 2;
            }
        }
        
        //at the end of each character, set xPos to starting coordinate of next character
        xPos += (float)simplex[asciiValue - 32][1] * scale;
    }
    
    path.setStrokeColor(color);
    path.setStrokeWidth(1);
    path.setFilled(false);
    
    return path;
}
