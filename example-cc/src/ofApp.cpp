#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    t.setup();
    s.cache = false;
    s.markdown = false;
    s.horizontalAlign = 0;
    s.verticalAlign = 0;
    s.fontSize = 48;
    
    ofSetFrameRate(120);
    
    ofBackground(0);
    ofSetBackgroundAuto(false);
    
    text = "Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World Hello World ";

    for (int i = 0; i < 100; i ++) r.push_back(ofRandom(0, 1));
    
    charBegin = t.charBegin.newListener([this](ofxPrecisionTextChar & ch){
        
        float max = 2;
        float min = 0.5;
        float a = sin( (ofGetElapsedTimef() * ofMap(r[0], 0, 1, min, max) ) + ch.index );
        float b = sin( (ofGetElapsedTimef() * ofMap(r[1], 0, 1, min, max)  ) + ch.index );
        float c = sin( (ofGetElapsedTimef() * ofMap(r[2], 0, 1, min, max) ) + ch.index );
        float d = sin( (ofGetElapsedTimef() * ofMap(r[3], 0, 1, min, max) ) + ch.index );
        float e = sin( (ofGetElapsedTimef() * ofMap(r[4], 0, 1, min, max) ) + ch.index );
        
        
        ofTranslate(0, ofMap(a, -1, 1, -10 * r[10], 10 * r[11] ));
        ofRotateX(b * 40 * r[21]);
        ofRotateY(b * 40 * r[22]);
        ofRotateZ(b * 40 * r[23]);
        ofScale( e * 6 * r[3]);
        ofColor co;
        co.setBrightness( ofMap(d, -1, 1, ofMap( r[4], 0, 1, 100, 255) , ofMap( r[5], 0, 1, 200, 300) ) );
        co.setSaturation( ofMap(e, -1, 1, ofMap( r[6], 0, 1, 0, 255) , ofMap( r[7], 0, 1, 0, 255) ) );
        co.setHue( ofMap(c, -1, 1, 255  * r[8], 255 * r[9]) );
        co.a = 30;
        ch.color = co;//ofColor(255,0,0);
        ch.strokeWidth = ofMap(d, -1, 1, 0.5, 4);
    });
    
//    st = generateStructure(text, ofGetWidth(), ofGetHeifght());
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableAlphaBlending();
    ofFill();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(0,0,0,4.0);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    t.draw(text, ofPoint(ofGetWidth()/2, ofGetHeight()/2), s);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        s.fontIndex = (int)ofRandom(0, t.hershey.getNumFonts());
        for (auto & f : r) f = ofRandom(0, 1);
    }
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
