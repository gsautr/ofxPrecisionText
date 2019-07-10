#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    TIME_SAMPLE_SET_FRAMERATE(60.0f); //specify a target framerate

    doc.setup();
    input.setup();
    
    string defaultText = "# Using Markdown\n\nCurrently supports stuff.";
    markdownText = defaultText;
    input.setText(defaultText);
    
    
    inputUpdated = input.updated.newListener([this](ofxPrecisionStructure & s){
        markdownText = s.text;
    });
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    int margin = 40;
    ofBackground(20);
    ofEnableSmoothing();
    settings.markdown = false;
    settings.fontSize = 18;
    settings.margin = margin;
    settings.debug = true;
//    settings.cache = false;
    ofRectangle bounds( 0, 0, ofGetWidth()/2, ofGetHeight());
    
    string inputText = input.draw(bounds, settings);
    
    ofxPrecisionSettings settingsRender;
    settingsRender.fontSize = 18;
    settingsRender.strokeColor = ofColor(20);
    settingsRender.debug = true;
    settingsRender.margin = margin;
    bounds.x += ofGetWidth()/2;
    ofFill();
    ofSetColor( 255, 250, 240 );
    ofDrawRectangle(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    ofxPrecisionStructure s = doc.draw(markdownText, bounds, settingsRender);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & e){
    
    if (e.key == 'c' && ofGetKeyPressed(OF_KEY_COMMAND)) doc.copyToClipboard();
    input.keyPressed(e.key);
    
//    if (e.key == ' ') settings.cache = !settings.cache;
//    if (e.key == 'a') ofEnableSmoothing();
//    if (e.key == 's') ofDisableSmoothing();
    
//    if (e.key == 'a') settings.horizontalAlign = -1;
//    if (e.key == 's') settings.horizontalAlign = 0;
//    if (e.key == 'd') settings.horizontalAlign = 1;
//    if (e.key == 'q') settings.verticalAlign = -1;
//    if (e.key == 'w') settings.verticalAlign = 0;
//    if (e.key == 'e') settings.verticalAlign = 1;
    
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
