#pragma once
#include "ofxPrecisionDocument.h"
class ofxPrecisionInput : public ofxPrecisionDocument {
private:
    
    string placeholder = "Placeholder Text";
    string internalText = "";
    float timestamp;
    bool isAwaitingUpdate = false;
public:
    
    ofEvent<ofxPrecisionStructure &> updated;
    ofEvent<ofxPrecisionStructure &> entered;
    ofxPrecisionStructure structure;
    void pasteFromClipboard() {
        insert(ofGetWindowPtr()->getClipboardString());
    }
    
    void insert(string s) {
        
    }
    
    void setText(string s) {
        internalText = s;
    }
    void setPlaceholderText(string s) {
        placeholder = s;
    }
    
    string draw(ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint = false) {
        
        
        structure = ofxPrecisionDocument::draw(internalText, boundingBox, settings, false);
        if (structure.text.size() == 0) return "";
        
        if (ofGetElapsedTimef() - timestamp > 1 && isAwaitingUpdate) {
            ofNotifyEvent(updated, structure);
            isAwaitingUpdate = false;
        }
        
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
        ofxPrecisionTextChar ch = structure.chars[(l) ? sanitise( a - 1 ) : sanitise( a ) ];
        ofVec3f tl = (a < structure.chars.size()) ? ch.bounds.getTopLeft() : ch.bounds.getTopRight();
        ofVec3f br = (a < structure.chars.size()) ? ch.bounds.getBottomLeft() : ch.bounds.getBottomRight();
        ofDrawLine(tl, br);
        
        ofPopMatrix();
        
        if (s.debug) {
            vector<ofxPrecisionTextChar> c = structure.chars;
            ofSetColor(255);
            ofPushMatrix();
            ofTranslate( structure.bounds.x, structure.bounds.y );
            ofDrawBitmapStringHighlight( ofToString( a ), c[ sanitise(a) ].bounds.getTopLeft());
            ofDrawBitmapStringHighlight( ofToString( b ), c[ sanitise(b) ].bounds.getTopLeft());
            ofPopMatrix();
        }
        return internalText;
        
    }
    
    
    void keyPressed(int k) {
        
        timestamp = ofGetElapsedTimef();
        isAwaitingUpdate = true;
        
        ofxPrecisionDocument::keyPressed( k );
        
        int & a = (pos[0] < pos[1]) ? pos[0] : pos[1];
        int & b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
        
        
        if(k == OF_KEY_BACKSPACE || k == OF_KEY_DEL){
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
        } else if (k == OF_KEY_RETURN){
            insertChar('\n');
        } else {
            if(k == ' ' ) insertChar(' ');
            if(k >= '!' && k <= '~') insertChar(k);
            if(k >= '0' && k <= '9') insertChar(k);
            if (k == '.') insertChar('.');
            if (k == '-') insertChar('-');
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
