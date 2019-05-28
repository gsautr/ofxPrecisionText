#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    text.setup();
    horizontalAlign = 0;
    verticalAlign = 0;
    fontSize = 14;
    fontStroke = 1.4;
    fontIndex = 0;
    lineHeight = 1;
    
    ofSetFrameRate(60.0f);
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(10);
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    int ww = w/2 - 250;
    int hh = h - 300;
    
    ofRectangle A(w - ww - 100 - ww, 200, ww, hh);
    ofRectangle B(w - ww - 50, 200, ww, hh/2);
    ofRectangle C(w - ww - 50, 200 + hh/2, ww, hh/2);
    ofRectangle D(50, 200, w - ww - ww - 200, hh/2);
    ofRectangle E(50, 200 + hh/2, w - ww - ww - 200, hh/2);
    
    
    ofSetColor(255);
    text.setColor(ofColor(255));
    text.setFont(fontIndex);
    text.setLineHeight(lineHeight);
    
    ofSetLineWidth(fontStroke);
    text.setStroke(fontStroke);
    
    ofPoint point = B.getCenter();
    
    vector<ofRectangle> fboRects;
    
//    fboRects.push_back( text.draw("ofPoint", ofPoint(B.x, 100), 26, -1, 0) );
//
//    fboRects.push_back( text.draw("Hello Worldqgp", point, fontSize, horizontalAlign , verticalAlign) );
//    fboRects.push_back( text.draw("Hello World", C, fontSize, horizontalAlign , verticalAlign) );
//
//    fboRects.push_back( text.draw("ofRectangle", ofPoint(A.x, 100), 26, -1, 0) );
//
    fboRects.push_back( text.draw("Hello [Hrlink](www.google.com) Wofsdfsdf \n fdsf \nrld fds dfds fdsf ds [Link2](www.wiki.com) ", A, fontSize, horizontalAlign , verticalAlign) );
    
    
    text.setStroke(1.2);
    text.setFont(0);
    
    fboRects.push_back( text.draw("ofxPrecision**Bold**And**Italic**", ofPoint(D.x, 100), 26, -1, 0) );
    
    text.setLineHeight(2);
    
    string outputA = "";
    
    outputA += "Font: " + ofToString(text.fontList[fontIndex]);
    outputA += "\nFont Size: " + ofToString(fontSize);
    outputA += "\nFont Stroke: " + ofToString(fontStroke);
    outputA += "\nLine Height: " + ofToString(lineHeight);
    outputA += "\nFBO Samples: " + ofToString(text.numSamples);
    outputA += "\nVert Align: " + ofToString(verticalAlign);
    outputA += "\nHorz Align: " + ofToString(horizontalAlign);
    
    string outputB = "";
    ;
    outputB += "\n\nz/X: -/+ Font";
    outputB += "\nW/E: -/+ Font Size";
    outputB += "\nA/S: -/+ Font Stroke";
    outputB += "\nD/F: -/+ Line Height";
    outputB += "\nE/R: -/+ FBO Samples";
    outputB += "\nArrow Keys: Vert/Horz Align";
    
//    text.draw(outputA, D, 12, -1, 1);
//    text.setColor(ofColor(0,255,255));
//    text.draw(outputB, E, 12, -1, 1);
    
    ofNoFill();
    ofSetColor(0,255,255, 50);
    for (auto & r : fboRects) ofDrawRectangle(r);
    ofSetColor(0,255,255);
    ofDrawRectangle(A);
    ofDrawRectangle(C);
    ofFill();
    ofDrawCircle(B.getCenter(), 4);
    ofNoFill();
    
    
    ofSetColor(255);
//    ofDrawLine(fboRects[5].getBottomLeft(), fboRects[5].getBottomRight());
//    ofDrawLine(fboRects[5].getBottomLeft(), fboRects[5].getBottomRight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'c') text.clearFboCache();

    if (key == 'a') fontStroke -= 0.2;
    if (key == 's') fontStroke += 0.2;
    
    if (key == 'z') fontIndex -= 1;
    if (key == 'x') fontIndex += 1;
    
    int fontMax = text.fontList.size() - 1;
    if (fontIndex < 0) fontIndex = fontMax;
    if (fontIndex > fontMax) fontIndex = 0;
    
    if (key == 'd') lineHeight -= 0.05;
    if (key == 'f') lineHeight += 0.05;
    
    if (key == 'e') { text.setFboSamples(text.numSamples - 1); }
    if (key == 'r') { text.setFboSamples(text.numSamples + 1); }
    
    
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
