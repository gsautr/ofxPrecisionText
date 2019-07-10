#pragma once
#include "ofxPrecisionText.h"

#define FIND_NEXT 1
#define FIND_PREV 2

#define SELECT_LEFT 1
#define SELECT_RIGHT 2

class ofxPrecisionDocument : public ofxPrecisionText {
private:
    
    string clipboard;
    int selectDirection;
public:
    ofxPrecisionText text;
    ofxPrecisionStructure structure;
    float pressTimestamp;
    int pressCounter;
    float pressTimeLimit;
    vector<int> pos;
    ofPoint lastPress;
    
    int sanitise( int i ) {
    
        if (i >= structure.chars.size()) i = structure.chars.size() - 1;
        if (i < 0) i = 0;
        return i;
    }
    
    void setA( int i ) {
        pos[0] = sanitise( i );
    }
    void setB( int i ) {
        pos[1] = sanitise( i );
    }
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
        int i =  getNearestCharacter(x, y);
        for(auto & ch : structure.chars) {
            if (ch.index == i) ofLog() << i << ch.index << ch.letter << ch.bounds.x << ch.bounds.y << ch.bounds.width << ch.bounds.height;
        }
        setA( i );
        
    }
    
    int findSibling( int i, vector<string> matches, int dir = FIND_NEXT) {
        
        vector<ofxPrecisionTextChar> c = structure.chars;
        if (dir == FIND_PREV) {
            for ( ; i >= 0; i--) {
                if (i == 0) return i;
                for (auto & m : matches) if (c[i-1].letter == m) return i;
            }
        }
        if (dir == FIND_NEXT) {
            for ( ; i < c.size(); i++) {
                if (i == c.size()) return i;
                for (auto & m : matches) if (c[i].letter == m) return i;
            }
        }
        return i;
    }
    
    void keyPressed(int k) {
        
        
        int & a = (pos[0] < pos[1]) ? pos[0] : pos[1];
        int & b = (pos[0] >= pos[1]) ? pos[0] : pos[1];
        
        if (k == OF_KEY_LEFT){
            if (a == b) selectDirection = SELECT_LEFT;
            if (selectDirection == SELECT_LEFT) {
                a -= 1;
                if (ofGetKeyPressed(OF_KEY_ALT)) a = findSibling( a, {" ", "\n"}, FIND_PREV);
                    if (!ofGetKeyPressed(OF_KEY_SHIFT)) b = a;
            }
            if (selectDirection == SELECT_RIGHT) {
                b -= 1;
                if (ofGetKeyPressed(OF_KEY_ALT)) b = findSibling( b, {" ", "\n"}, FIND_PREV);
                    if (!ofGetKeyPressed(OF_KEY_SHIFT)) a = b;
            }
        } else if (k == OF_KEY_RIGHT){
            if (a == b) selectDirection = SELECT_RIGHT;
            if (selectDirection == SELECT_RIGHT) {
                b += 1;
                if (ofGetKeyPressed(OF_KEY_ALT)) b = findSibling( b, {" ", "\n"}, FIND_NEXT);
                    if (!ofGetKeyPressed(OF_KEY_SHIFT)) a = b;
            }
            if (selectDirection == SELECT_LEFT) {
                a += 1;
                if (ofGetKeyPressed(OF_KEY_ALT)) a = findSibling( a, {" ", "\n"}, FIND_NEXT);
                    if (!ofGetKeyPressed(OF_KEY_SHIFT)) b = a;
            }
        } else if (k == OF_KEY_UP ) {
            int cursor = (selectDirection == SELECT_RIGHT) ? b : a;
            int i = structure.chars[cursor].line - 1;
            if (i < 0) {
                a = 0;
            } else {
                int chosen = NULL;
                float dist = 9999;
                for (auto & ch : structure.lines[i]) {
                    float d = abs(ch.bounds.x - structure.chars[cursor].bounds.x);
                    if ( d < dist ) {
                        chosen = ch.index;
                        dist = d;
                    }
                }
                a = chosen;
            }
            if (!ofGetKeyPressed(OF_KEY_SHIFT)) b = a;
            
        } else if (k == OF_KEY_DOWN ) {
            int cursor = (selectDirection == SELECT_RIGHT) ? b : a;
            int i = structure.chars[cursor].line + 1;
            if (i >= structure.lines.size()) {
                a = structure.chars.size();
            } else {
                int chosen = NULL;
                float dist = 9999;
                for (auto & ch : structure.lines[i]) {
                    float d = abs(ch.bounds.x - structure.chars[cursor].bounds.x);
                    if ( d < dist ) {
                        chosen = ch.index;
                        dist = d;
                    }
                }
                a = chosen;
            }
            if (!ofGetKeyPressed(OF_KEY_SHIFT)) b = a;
            
        }
        
        int max = structure.chars.size();
        if (a > max) a = max;
        if (b > max) b = max;
        if (a < 0) a = 0;
        if (b < 0) b = 0;
    }
    
    void released( int x, int y) {
        
    }
    
    void pressed(int x, int y){
        
        vector<ofxPrecisionTextChar> c = structure.chars;
        ofRectangle outerBox = structure.outerBox;
        ofRectangle innerBox = structure.innerBox;
        
        bool isInside = outerBox.inside(x, y);
        if (!isInside) {
            isActive = false;
            ofLog() << "NOT INSIDE";
            return;
        } else {
            bool lastActive = isActive;
            isActive = true;
            if (!lastActive) {
                ofLog() << "WAS NOT ACTIVE";
            }
        }
        int bottom = c.back().bounds.y + c.back().bounds.height + innerBox.y;
        int top = innerBox.y + c[0].bounds.y;
        bool isBelow = (y > bottom);
        bool isAbove = (y < top);
        int aa, bb;
        if (isBelow || isAbove) {
            
            if (isAbove) {
                ofLog() << "ABOVE";
                aa = 0;
                bb = 0;
            }
            if (isBelow) {
                ofLog() << "BELOW";
                aa = c.size();
                bb = c.size();
            }
        } else {
            
            aa = getNearestCharacter(x, y);
            bb = aa;
        }
        
        float time = ofGetElapsedTimef();
        
        if (time - pressTimestamp < pressTimeLimit) {
            
            
            
            pressCounter += 1;
            
            ofPoint p = lastPress;
            if (x < p.x -2 || x > p.x + 2) pressCounter = 0;
            if (y < p.y -2 || y > p.y + 2) pressCounter = 0;
            
            if (pressCounter == 1) {
                aa = findSibling( aa, {" ", "\n"}, FIND_PREV);
                bb = findSibling( aa, {" ", "\n"}, FIND_NEXT);
            }
            if (pressCounter == 2) {
                aa = findSibling( aa, {"\n"}, FIND_PREV);
                bb = findSibling( aa, {"\n"}, FIND_NEXT);
            }
            
        } else {
            pressCounter = 0;
        }
        
        if (pressCounter == 0) pressTimestamp = time;
        
        pos[0] = aa;
        pos[1] = bb;
//        setA( aa );
//        setB( bb );
        
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
