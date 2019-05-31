#pragma once
#include "ofxPrecisionDocument.h"

class ofxPrecisionInput : public ofxPrecisionDocument {
private:
    
    string placeholder = "Default";
    string internalText = "HEljhsdflkfjsdf ";
public:
    
    ofxPrecisionStructure structure;
    void pasteFromClipboard() {
        insert(ofGetWindowPtr()->getClipboardString());
    }
    
    void insert(string s) {
        
    }
    
    void draw(ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint = false) {
        
        structure = ofxPrecisionDocument::draw(internalText, boundingBox, settings, false);
        
        int a = indexA;
        int b = indexB;
        
        int xx = structure.bounds.x;
        int yy = structure.bounds.y;
        
        ofPushMatrix();
        ofTranslate(xx,yy);
        
        ofSetColor(255,0,0);
        ofSetLineWidth(4);
        ofNoFill();
        bool l = (a==structure.chars.size());
        ofxPrecisionTextChar ch = structure.chars[(l) ? a - 1 : a];
        ofVec3f tl = (a < structure.chars.size()) ? ch.bounds.getTopLeft() : ch.bounds.getTopRight();
        ofVec3f br = (a < structure.chars.size()) ? ch.bounds.getBottomLeft() : ch.bounds.getBottomRight();
        ofDrawLine(tl, br);
        
        ofPopMatrix();
        
    }
    
    void keyPressed(int ch) {
        
        if (ch == '\n') {
            internalText.insert(indexA, "\n");
        } else {
            internalText.insert(indexA, ofToString((char)ch));
        }
        indexA += 1;
        indexB = indexA;
    }
    
    void setup() {
        
        ofxPrecisionDocument::setup();
    }
};
