#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    text.setup();
    horizontalAlign = 0;
    verticalAlign = 0;
    fontSize = 14;
    fontStroke = 2;
    fontIndex = 0;
    lineHeight = 1.2;
    
    ofSetFrameRate(60.0f);
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(10);
    ofRectangle A(100, 100, ofGetWidth()/2 - 150, ofGetHeight() - 400);
    ofRectangle B(ofGetWidth()/2 + 50, 100, ofGetWidth()/2 - 150, ofGetHeight() - 400);
    ofRectangle C(100, ofGetHeight() - 200, ofGetWidth() - 200, 100);
    
    
    ofSetColor(255);
    text.setColor(ofColor(255));
    text.setStroke(fontStroke);
    text.setFont(fontIndex);
    text.setLineHeight(lineHeight);
    ofSetLineWidth(fontStroke);
    ofPoint point = B.getCenter();
    text.draw("Hello World", point, fontSize, horizontalAlign , verticalAlign);
    text.draw("In pretium leo ut erat auctor auctor. Donec in est imperdiet, faucibus tellus at, ullamcorper mauris. Phasellus at erat tincidunt, tempus arcu et, aliquam est. \n\n\nPhasellus at erat tincidunt, tempus arcu et, aliquam est. Phasellus at erat tincidunt, tempus arcu et, aliquam est.", A, fontSize, horizontalAlign , verticalAlign);
    
    
//    text.draw("Arrow Keys = Alignment, Q/W = FontSize, E/R = FBO Samples, Spacebar = Enable FBO, B = Enable FBO Background", C, fontSize, horizontalAlign , verticalAlign);
    
    
    ofNoFill();
    ofSetColor(0,255,255);
    ofDrawRectangle(A);
    ofDrawCircle(B.getCenter(), 4);
    ofDrawRectangle(C);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 'a') fontStroke -= 0.2;
    if (key == 's') fontStroke += 0.2;
    
    if (key == 'd') lineHeight -= 0.05;
    if (key == 'f') lineHeight += 0.05;
    
    if (key == 'e') { text.enableCache(text.numSamples - 1); }
    if (key == 'r') { text.enableCache(text.numSamples + 1); }
    
    if (key == ' ') {
        if (text.isCachingFbo) {
            text.disableCache();
        } else {
            text.enableCache(text.numSamples);
        }
        ofLog() << "FBO Caching: " << ((text.isCachingFbo) ? "Enabled" : "Disabled");
    }
    if (key == 'b') {
        if (text.isBackgroundEnabled) {
            text.disableBackground();
        } else {
            text.enableBackground(ofColor(10));
        }
        ofLog() << "Background: " << ((text.isBackgroundEnabled) ? "Enabled" : "Disabled");
    }
    
    if (key == OF_KEY_LEFT && horizontalAlign > -1) horizontalAlign -= 1;
    if (key == OF_KEY_RIGHT && horizontalAlign < 1) horizontalAlign += 1;
    if (key == OF_KEY_DOWN && verticalAlign > -1) verticalAlign -= 1;
    if (key == OF_KEY_UP && verticalAlign < 1) verticalAlign += 1;
    
    if (key == 'q') fontSize -= 0.5;
    if (key == 'w') fontSize += 0.5;
    key -= 49;
    if (key >= 0 && key <= 9) fontIndex = key;
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
