#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    t.setup();
    s.cache = false;
    s.markdown = false;
    s.horizontalAlign = 0;
    s.verticalAlign = 0;
    s.fontSize = 48;
    
    charBegin = t.charBegin.newListener([this](ofxPrecisionTextChar & ch){
        
        float sine = sin( (ofGetElapsedTimef() * 10 ) + ch.index );
        float y = ofMap(sine, -1, 1, -10, 10 );
        ofTranslate(0, y);
        ofRotateX(y * 2);
        ch.color = ofColor(255,0,0);
//        ofLog() << "B" << ch.color;
        //        ofLog() << "YO!" << ch.bounds.width;
    });
//    charDrawn = t.charBegin.newListener([this](ofxPrecisionTextChar & ch){
//        ofTranslate(ofRandom( -4, 4), ofRandom(-10, 10));
//        //        ofLog() << "YO!" << ch.bounds.width;
//    });
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    t.draw("Hello World", ofPoint(ofGetWidth()/2, ofGetHeight()/2), s);
    t.clearCache();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
