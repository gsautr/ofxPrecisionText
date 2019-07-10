#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    
    float dpi = ((ofAppGLFWWindow*)ofGetWindowPtr())->getPixelScreenCoordScale();
    s.dpi = dpi;
    ofSetWindowShape(ofGetWidth() * dpi, ofGetHeight() * dpi);
    
    
    t.setup();
    ofLog::setAutoSpace(true);
    ofSetFrameRate(120.0f);
    
    charBegin = t.charBegin.newListener([this](ofxPrecisionTextChar & ch){
        
        ofSetColor(255,255,0);
        ofSetLineWidth(1);
        ofNoFill();
//        ofDrawRectangle(0,0,ch.bounds.width, ch.bounds.height);
    });
    
    
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
    
    vector<ofRectangle> rects = {A, B, C, D, E };
    
    vector<ofxPrecisionStructure> structures;
    
    ofPoint hA(B.x, 100 * s.dpi);
    ofPoint hB(A.x, 100 * s.dpi);
    ofPoint hC(D.x, 100 * s.dpi);
    
    
    
    structures.push_back( t.draw("Hello World", B.getCenter(), s) );
    structures.push_back( t.draw("Hello World", C, s) );
    structures.push_back( t.draw("The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. ", A, s) );
    
    /*-- draw headings --*/
    
    ofxPrecisionSettings headerSettings;
    headerSettings.horizontalAlign = 1;
    headerSettings.fontSize = 32;
    headerSettings.strokeWidth = 2;
    
    structures.push_back( t.draw("ofPoint", hA, headerSettings) );
    structures.push_back( t.draw("ofRectangle", hB, headerSettings) );
    structures.push_back( t.draw("ofxPrecisionText", hC, headerSettings) );
    
    for (auto & r : rects) {
        ofSetColor(0,255,255);
        ofNoFill();
        ofSetLineWidth(1 * s.dpi);
        ofDrawRectangle(r);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'q') s.fontSize -= 0.25;
    if (key == 'w') s.fontSize += 0.25;
    
    if (key == 'e') s.strokeWidth -= 0.2;
    if (key == 'r') s.strokeWidth += 0.2;
    
    if (key == 'a') s.lineHeight -= 0.1;
    if (key == 's') s.lineHeight += 0.1;
    
    if (key == 'd') s.letterSpacing -= 0.5;
    if (key == 'f') s.letterSpacing += 0.5;
    
    if (key == 'z') s.fontIndex -= 1;
    if (key == 'x') s.fontIndex += 1;
    if (s.fontIndex < 0) s.fontIndex = t.fontList.size() - 1;
    if (s.fontIndex >= t.fontList.size()) s.fontIndex = 0;
    
    if (key == OF_KEY_LEFT) s.horizontalAlign -= 1;
    if (key == OF_KEY_RIGHT) s.horizontalAlign += 1;
    if (s.horizontalAlign > 1) s.horizontalAlign = -1;
    if (s.horizontalAlign < -1) s.horizontalAlign = 1;
    
    if (key == OF_KEY_DOWN) s.verticalAlign -= 1;
    if (key == OF_KEY_UP) s.verticalAlign += 1;
    if (s.verticalAlign > 1) s.verticalAlign = -1;
    if (s.verticalAlign < -1) s.verticalAlign = 1;
    
    if (key == ' ') {
        s.debug = !s.debug;
    }
    
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
