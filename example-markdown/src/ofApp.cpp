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
    string txt = "Hello [link](google.com) [another](fsdfds) som**e**thing to [tertre](fdsf) and **[ffsdf](fdsf)** and **something** else\nPellentesque ultricies, lectus at dapibus aliquam, arcu leo ornare sem, in porta metus dolor ac leo. Sed rutrum laoreet ultricies. Praesent suscipit aliquam malesuada. Nullam orci augue, condimentum cursus nibh a, commodo fermentum urna. Cras non porta metus. \n\nPhasellus condimentum tincidunt odio id accumsan. Cras eget feugiat felis, ut congue ex. Donec vel consectetur ex. \n\nNam quis libero ornare, eleifend tellus sit amet, tincidunt nibh. Suspendisse vitae sapien consequat, tincidunt justo a, venenatis lorem. [Nullam](link) bibendum interdum dolor, ut imperdiet libero vestibulum vel. Proin in augue magna. Morbi eget elit feugiat, convallis ex a, lacinia ante. Curabitur nec lobortis mauris. Aliquam fringilla risus diam. Maecenas dapibus risus sit amet quam pellentesque ultrices., eget condimentum nibh commodo. Proin a tincidunt magna. Suspendisse et mollis velit.";
    
    ofRectangle bounds( 100, 100, ofGetWidth() - 200, ofGetHeight()/2 - 200 );
    
    doc.draw(txt, bounds, settings);
    
    settings.markdown = false;
    
    bounds.y += ofGetHeight()/2;
    input.draw(bounds, settings);
    
    
    
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
