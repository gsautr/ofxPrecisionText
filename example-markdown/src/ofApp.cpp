#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    
    doc.setup();
    input.setup();
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofBackground(0);
    string defaultText = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque vel scelerisque sapien. Quisque sit amet semper ante, sed venenatis arcu. Ut commodo lectus non condimentum blandit. Vestibulum accumsan, purus at mollis tincidunt, quam nisi tincidunt dolor, vel tristique nisi libero eget tellus. Integer pulvinar ex cursus purus porttitor convallis. Ut eget est efficitur, placerat felis vitae, tincidunt nunc. Pellentesque commodo sem nec nibh eleifend fermentum. Nam feugiat est lorem, sed molestie ante convallis at. Aliquam suscipit pharetra velit vitae convallis. Aenean suscipit a ante sed faucibus. Donec justo arcu, pulvinar quis ligula vitae, lacinia sollicitudin sem.";
    
    settings.markdown = false;
    settings.cache = false;
    settings.fontSize = 16;
//    settings.strokeColor = ofColor(0);
    
    ofRectangle bounds( 100, 100, ofGetWidth()/2 - 200, ofGetHeight() - 200 );
    
    input.setText(defaultText);
    string inputText = input.draw(bounds, settings);
    
    settings.markdown = true;
    
    bounds.x += ofGetWidth()/2;
    ofxPrecisionStructure s = doc.draw(inputText, bounds, settings);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & e){
    
    if (e.key == 'c') doc.copyToClipboard();
//    input.keyPressed(e.key);
    
    
    if (e.key == 'a') settings.horizontalAlign = -1;
    if (e.key == 's') settings.horizontalAlign = 0;
    if (e.key == 'd') settings.horizontalAlign = 1;
    if (e.key == 'q') settings.verticalAlign = -1;
    if (e.key == 'w') settings.verticalAlign = 0;
    if (e.key == 'e') settings.verticalAlign = 1;
    
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
