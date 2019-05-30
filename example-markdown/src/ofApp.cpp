#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    text.setup();
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    
    int a = 0;
    int b = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    text.setColor(ofColor(255));
    text.letterSpacing = 0.4;
    string txt = "An h1 header\n============\n\nParagraphs are separated by a blank line.\n\n2nd paragraph. *Italic*, **bold**, and `monospace`. Itemized lists\nlook like:\n\n  * this one\n  * that one\n  ";
    ofRectangle r(100,100,ofGetWidth() - 200, ofGetHeight() - 200);
    structure = text.draw(txt, r, 24, -1, 0);
    
    for (auto & ch : structure.chars) {
        
        ofSetColor(0,255,255,150);
        ofFill();
        ofDrawRectangle(ch.bounds.x, ch.bounds.y, ch.bounds.width, ch.bounds.height);
    }
//    if (a == b) {
//        ofSetColor(255,0,0);
//        ofSetLineWidth(4);
//        ofNoFill();
//        ofDrawLine(structure.chars[a].bounds.getTopLeft(), structure.chars[a].bounds.getBottomLeft());
//    } else {
    
//        int a = (pressed < dragged) ? pressed : dragged;
//        int b = (pressed <= dragged) ? dragged : pressed;
//        for (int i = a; i < b; i++) {
//            ofSetColor(255,255,255,150);
//            ofFill();
//            ofRectangle r = structure.chars[i].bounds;
//            ofDrawRectangle(r);
//        }
//    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

int ofApp::getNearestCharacter(int x, int y) {
    
    for (int i = 0; i < structure.chars.size(); i++) {
        if (structure.chars[i].bounds.inside(x, y)) {
            return i;
        }
    }
    int chosen = 0;
    float d = 99999;
    for (int i = 0; i < structure.chars.size(); i++) {
        if (y > structure.chars[i].bounds.getTop() && y <= structure.chars[i].bounds.getBottom()) {
            ofPoint p = structure.chars[i].bounds.getCenter();
            float dd = p.distance( ofVec3f(x, y) );
            if (dd < d) {
                d = dd;
                chosen = i;
            }
        }
    }
    return chosen;
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    int dragged = getNearestCharacter(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    pressed = getNearestCharacter(x, y);
//    b = getNearestCharacter(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

//    b = getNearestCharacter(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
