/*
*  ofxHersheyFont.cpp
*
*  Created by Tobias Zimmer, August 2016.
*  www.tobiaszimmer.net
*
*  Font originally developed by Dr. Allen V. Hershey in 1967.
*  Font vectors made available by Paul Bourke.
*  paulbourke.net/dataformats/hershey/
*  
*  A simple single line font for machines like CNC, Lasercutter, ...
*  Available characters: ASCII codes 32 to 126.
*
*/

#include "ofxHersheyFont.h"
#include "simplexCharacterSet.h"


//--------------------------------------------------------------
ofxHersheyFont::ofxHersheyFont(){
	color = ofColor(0);
    scale = 1;
    stroke = 1;
    centered = false;
    angle = 0;
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

//--------------------------------------------------------------
void ofxHersheyFont::drawChar(int asciiValue, float stroke) {
	
	ofPath chPath;

	//move to first coordinate
	chPath.moveTo(simplex[asciiValue - 32][2], simplex[asciiValue - 32][3]);

	//iterate through points of the character
	for (int i = 4; i <= simplex[asciiValue - 32][0] * 2; i += 2)
	{
		int x = simplex[asciiValue - 32][i];
		int y = simplex[asciiValue - 32][i + 1];

		if (x != -1) chPath.lineTo(x, y);

		//skip -1,-1 value -> equals pen up operation / end of a line
		//and move to next point
		if (x == -1) {
			chPath.moveTo(simplex[asciiValue - 32][i + 2], simplex[asciiValue - 32][i + 3]);
			i += 2;
		}
	}
	
	chPath.setStrokeColor(color);
	chPath.setStrokeWidth(stroke);
	chPath.setFilled(false);
    
//    ofEnableAntiAliasing();
	chPath.draw();
    
//    ofDisableAntiAliasing();
}


//--------------------------------------------------------------
void ofxHersheyFont::setColor(ofColor c) {
    color = c;
}

//--------------------------------------------------------------
void ofxHersheyFont::setScale(float s) {
    scale = s;
}

//--------------------------------------------------------------
void ofxHersheyFont::setStroke(float s) {
    stroke = s;
}

ofRectangle ofxHersheyFont::getBounds(string stringValue, int x, int y) {
    return ofRectangle(x, y, getWidth(stringValue), getHeight());
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
    
	float stringHeight = (float) 21 * scale;

	return stringHeight;
}


//--------------------------------------------------------------
ofPath ofxHersheyFont::getPath(string stringValue, float scale) {
	return getPath(stringValue, 0, 0, scale);
}

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
		path.moveTo(xPos + simplex[asciiValue - 32][2] * scale, yPos + (-1) * simplex[asciiValue - 32][3] * scale);

		//iterate through points of each character
		for (int j = 4; j <= simplex[asciiValue - 32][0] * 2; j += 2)
		{
			int x = simplex[asciiValue - 32][j];
			int y = (-1) * simplex[asciiValue - 32][j + 1];

			if (x != -1) path.lineTo(xPos + x * scale, yPos + y * scale);

			if (x == -1) {
				path.moveTo(xPos + simplex[asciiValue - 32][j + 2] * scale, yPos + (-1) * simplex[asciiValue - 32][j + 3] * scale);
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
