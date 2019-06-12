#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    float dpi = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
    s.dpi = dpi;
    ofSetWindowShape(ofGetWidth() * dpi, ofGetHeight() * dpi);
    
    
    t.setup();
    ofLog::setAutoSpace(true);
    ofSetFrameRate(120.0f);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(10);
    
    int w = ofGetWidth()/ s.dpi;
    int h = ofGetHeight()/ s.dpi;
    int ww = w/2 - 250;
    int hh = h - 300;
    
    
    ofRectangle A(w - ww - 100 - ww, 200, ww, hh);
    ofRectangle B(w - ww - 50, 200 + hh/2 + 25, ww, (hh/2 ) - 25);
    ofRectangle C(w - ww - 50, 200, ww, (hh/2) -25 );
    ofRectangle D(50, 200, w - ww - ww - 200, hh/2);
    ofRectangle E(50, 200 + hh/2, w - ww - ww - 200, hh/2);
    
    A.scale( ofPoint(s.dpi, s.dpi) );
    B.scale( ofPoint(s.dpi, s.dpi) );
    C.scale( ofPoint(s.dpi, s.dpi) );
    D.scale( ofPoint(s.dpi, s.dpi) );
    E.scale( ofPoint(s.dpi, s.dpi) );
    A.setPosition( A.getPosition() * s.dpi );
    B.setPosition( B.getPosition() * s.dpi );
    C.setPosition( C.getPosition() * s.dpi );
    D.setPosition( D.getPosition() * s.dpi );
    E.setPosition( E.getPosition() * s.dpi );
    
    ofxPrecisionSettings s;
    
    
    vector<ofxPrecisionStructure> structures;
    
    ofPoint hA(B.x, 100 * s.dpi);
    ofPoint hB(A.x, 100 * s.dpi);
    ofPoint hC(D.x, 100 * s.dpi);
    
    structures.push_back( t.draw("ofPoint", hA, s) );
    structures.push_back( t.draw("ofRectangle", hB, s) );
    structures.push_back( t.draw("ofxPrecisionText", hC, s) );
    
    
    structures.push_back( t.draw("Hello World", B.getCenter(), s) );
    structures.push_back( t.draw("Hello World", C, s) );
    structures.push_back( t.draw("The quick brown fox jumps over the lazy dog.", A, s) );
    
    
    ofxPrecisionRow row(0,0,ofGetWidth(), ofGetHeight());
    row.addCol();
    row.addCol();
    for (auto & rr : row.addCol()) {
        ofSetColor(255,255,0);
        ofNoFill();
        ofDrawRectangle(rr);
    }
    
    
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
