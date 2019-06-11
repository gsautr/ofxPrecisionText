#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    
    doc.setup();
    input.setup();
    
    
    //    string txt;
    //    for (int i = 0; i < 256; i++) {
    //        txt += (char)i;
    //        txt += " ";
    //    }
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofBackground(0);
    string txt = "Hello [link](google.com) [another](fsdfds) ";
    
    settings.markdown = true;
    settings.cache = false;
    settings.horizontalAlign = 0;
    settings.verticalAlign = 0;
    settings.fontSize = 32;
    
    ofRectangle bounds( 100, 100, ofGetWidth() - 200, ofGetHeight()/2 - 200 );
    
    ofxPrecisionStructure s = doc.draw(txt, bounds, settings);
    
    settings.markdown = false;
    
    bounds.y += ofGetHeight()/2;
    //    input.draw(bounds, settings);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & e){
    
    if (e.key == 'c') doc.copyToClipboard();
    input.keyPressed(e.key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    doc.dragged(x,y);
    input.dragged(x,y);
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    doc.pressed(x,y);
    input.pressed(x,y);
    
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
