#pragma once
#include "ofxPrecisionText.h"

class ofxPrecisionDocument : public ofxPrecisionText {
private:
    
    string clipboard;
public:
    bool isActive;
    ofxPrecisionText text;
    ofxPrecisionStructure structure;
    float pressTimestamp;
    int pressCounter;
    float pressTimeLimit;
    vector<int> pos;
    ofPoint lastPress;
    
    void copyToClipboard() {
        if (pos[0] != pos[1]) ofGetWindowPtr()->setClipboardString(clipboard);
    }
    ofRectangle getPreciseRect(int i) {
        
        ofRectangle r = structure.chars[i].bounds;
        
        r.x = (int)r.x;
        r.y = (int)r.y - 1;
        r.width = (int)r.width;
        r.height = (int)r.height;
        if (i < structure.chars.size() - 1 && !structure.chars[i].isLineEnd) {
            int rx = r.x + r.width;
            int rrx = (int)structure.chars[i+1].bounds.x;
            if (rx < rrx) r.width += rrx - rx;
        }
        return r;
    }
    
    void dragged(int x, int y){
        
        if (!isActive) return;
        
        pos[0] = getNearestCharacter(x, y);
        
    }
    
    void pressed(int x, int y){
        
        
        isActive = structure.bounds.inside(x, y);
        
        if (!isActive) {
            pos[0] = pos[1];
            return;
        }
        
        int aa = getNearestCharacter(x, y);
        int bb = aa;
        float time = ofGetElapsedTimef();
        
        vector<ofxPrecisionTextChar> c = structure.chars;
        
        if (time - pressTimestamp < pressTimeLimit) {
            
            
            
            pressCounter += 1;
            
            ofPoint p = lastPress;
            if (x < p.x -2 || x > p.x + 2) pressCounter = 0;
            if (y < p.y -2 || y > p.y + 2) pressCounter = 0;
            
            if (pressCounter == 1) {
                for ( ; aa >= 0; aa--) if (c[aa - 1].letter == " " || aa == 0) break;
                for ( ; bb < c.size(); bb++) if (c[bb].letter == " " || bb == c.size()) break;
            }
            if (pressCounter == 2) {
                for ( ; aa >= 0; aa--) if (c[aa - 1].letter == "\n" || aa == 0) break;
                for ( ; bb < c.size(); bb++) if (c[bb].letter == "\n" || bb == c.size()) break;
            }
            
        } else {
            pressCounter = 0;
        }
        
        if (pressCounter == 0) pressTimestamp = time;
        
        pos[0] = aa;
        pos[1] = bb;
        
        lastPress.x = x;
        lastPress.y = y;
        
        
    }
    
    int getNearestCharacter(int x, int y) {
        
        x -= structure.bounds.x;
        y -= structure.bounds.y;
        
        for (int i = 0; i < structure.chars.size(); i++) {
            ofxPrecisionTextChar ch;
            if (ch.bounds.inside(x, y)) {
                int ii = (x > ch.bounds.x + (ch.bounds.width/2)) ? i + 1 : i;
                return ii;
            }
        }
        
        if (y < 0) return 0;
        if (y > structure.bounds.height) return structure.chars.size();
        
        int chosen = 0;
        float d = 99999;
        for (int i = 0; i < structure.chars.size(); i++) {
            ofxPrecisionTextChar ch = structure.chars[i];
            if (y > ch.bounds.getTop() && y <= ch.bounds.getBottom()) {
                ofPoint p = ch.bounds.getCenter();
                p.x = ch.bounds.getLeft();
                float dd = p.distance( ofVec3f(x, y) );
                if (dd < d) {
                    d = dd;
                    chosen = i;
                    if ((i == structure.chars.size() - 1)&&(x > ch.bounds.x + (ch.bounds.width/2))) chosen += 1;
                }
            }
        }
        return chosen;
    }
    
    
    ofxPrecisionStructure draw(string text, ofRectangle boundingBox, ofxPrecisionSettings settings, bool isPoint = false) {
        
        structure = ofxPrecisionText::draw(text, boundingBox, settings, false);
        
        if (pos[0] != pos[1]) {
            
            int a = (pos[0] < pos[1]) ? pos[0] : pos[1];
            int b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
            
            int xx = structure.bounds.x;
            int yy = structure.bounds.y;
            
            ofPushMatrix();
            ofTranslate(xx,yy);
            
            
            clipboard = "";
            for (int i = a; i < b; i++) {
                ofSetColor(ofMap(i, a, b, 255, 0),255,ofMap(i, a, b, 0, 255),120);
                ofFill();
                clipboard += structure.chars[i].letter;
                ofDrawRectangle(getPreciseRect(i));
            }
            ofPopMatrix();
        }
        
        return structure;
        
    }
    
    void setup() {
        
        ofxPrecisionText::setup();
        pressTimestamp = ofGetElapsedTimef();
        pos.push_back(0);
        pos.push_back(0);
        pressCounter = 0;
        pressTimeLimit = 1;
    }
};
