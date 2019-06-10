#pragma once
#include "ofxPrecisionDocument.h"

class ofxPrecisionInput : public ofxPrecisionDocument {
private:
    
    string placeholder = "Default";
    string internalText = "Hello world hello world hello world";
public:
    
    ofxPrecisionStructure structure;
    void pasteFromClipboard() {
        insert(ofGetWindowPtr()->getClipboardString());
    }
    
    void insert(string s) {
        
    }
    
    void draw(ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint = false) {
        
        structure = ofxPrecisionDocument::draw(internalText, boundingBox, settings, false);
        
        int a = (pos[0] < pos[1]) ? pos[0] : pos[1];
        int b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
        
        int xx = structure.bounds.x;
        int yy = structure.bounds.y;
        
        ofPushMatrix();
        ofTranslate(xx,yy);
        
        ofSetColor(255);
        ofSetLineWidth(2);
        ofNoFill();
        bool l = (a==structure.chars.size());
        ofxPrecisionTextChar ch = structure.chars[(l) ? a - 1 : a];
        ofVec3f tl = (a < structure.chars.size()) ? ch.bounds.getTopLeft() : ch.bounds.getTopRight();
        ofVec3f br = (a < structure.chars.size()) ? ch.bounds.getBottomLeft() : ch.bounds.getBottomRight();
        ofDrawLine(tl, br);
        
        ofPopMatrix();
        
    }
    
    void keyPressed(int ch) {
        
        int & a = (pos[0] < pos[1]) ? pos[0] : pos[1];
        int & b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
        
        if(ch == OF_KEY_BACKSPACE || ch == OF_KEY_DEL){
            int size = 1;
            if (a == b) {
                if (a == 0) {
                    b = a;
                } else {
                    a -= 1;
                    b = a;
                }
            } else {
                size = b - a;
                b = a;
            }
            internalText = internalText.erase(a, size);
        } else if (ch == OF_KEY_LEFT){
            if (a == b) {
                a -= 1;
                b = a;
            } else {
                b = a;
            }
            
        } else if (ch == OF_KEY_RIGHT){
            if (a == b) {
                a += 1;
                b = a;
            } else {
                a = b;
            }
            
        } else if (ch == OF_KEY_RETURN){
            insertChar('\n');
        } else {
            if(ch == ' ' ) insertChar(' ');
            if(ch >= '!' && ch <= '~') insertChar(ch);
            if(ch >= '0' && ch <= '9') insertChar(ch);
            if (ch == '.') insertChar('.');
            if (ch == '-') insertChar('-');
        }
    }
    void insertChar(int c) {
        
        int & a = (pos[0] < pos[1]) ? pos[0] : pos[1];
        int & b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
        
        internalText.insert(a, ofToString((char)c));
        a += 1;
        b = a;
    }
    
    void setup() {
        
        ofxPrecisionDocument::setup();
    }
};
