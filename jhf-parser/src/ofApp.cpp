
#include "ofApp.h"
extern "C" {
    #include "hersheyfont.h"
}

//--------------------------------------------------------------
void ofApp::setup(){

    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    
    doc.setup();
    fontIndex = 0;
    
    ofDirectory dir("jhf");
    
    dir.listDir();
    
    string fin = "";
    
    string intro = "/* Simplex sets: \n*";
    
    for (auto & f : dir.getFiles()) {
        ofLog() << f.getBaseName();
        const char * fontname = f.getBaseName().c_str();
    
        struct hershey_font *hf = hershey_font_load(fontname);
        
        intro += "\n* "+(string)fontname;
        
        string c = "";
        vector<vector<int>> v = {};
        
        for (int i = 0; i < 256; i++) {
            
            struct hershey_glyph *hg = hershey_font_glyph(hf, i);
            if ( hg->width > 0 )  {
                
                v.push_back({});
                v.back().push_back(0);
                int place = v.back().size() - 1; // Store path counter index
                v.back().push_back(hg->width);
                int count = 0;
                
                struct hershey_path *hp;
                for ( hp=hg->paths; hp; hp=hp->next ) {
                    
                    int i;
                    for ( i=0; i<hp->nverts; i++ ) {
                        short x = hp->verts[i].x;
                        short y = hp->verts[i].y;
                        count += 1;
                        
                        v.back().push_back(x);
                        v.back().push_back(y);
                    }
                    v.back().push_back(-1);
                    v.back().push_back(-1);
                    count += 1;
                    
                }
                count -= 1;
                
                
                v.back()[place] = count; // Set path counter variable
            } else {
                v.push_back({});
            }
        }
        
        int start = 32;
        string out = "\n/* " + (string)fontname + " */\n\nvector<vector<int>> ";
        out += fontname;
        out += " = {\n";
        int count = 0;
        
        int ch = start;
        for (int c = start; c < v.size(); c++) {
            vector<int> & ve = v[c];
            out += "\n{\n    ";
            for(int i = 0; i < 112; i++) {
                
                if (i == 2) out += " /* Char " + ofToString((char)ch) + " ("+ ofToString(ch) +  ") */ \n    ";
                
                if (i < ve.size()) {
                    out += ofToString(ve[i]);
                } else {
                    out += " -1";
                }
                count += 1;
                if (i != 111) out += ",";
            }
            out += "\n}";
            if (ch != v.size() - 1) out += ",";
            ch += 1;
        }
        
        out += "\n};\n";
        
        fin += out;
        
    }
    
    intro += "\n*\n*/";
    string fina = intro + fin;
    
    ofBuffer b;
    b.set(fina);
    string fi = "hersheyCharacterSets.h";
    
    ofBufferToFile(fi, b); // save file to local data folder
    ofBufferToFile("../../../src/" + fi, b); // save file to addon src

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);
    ofxPrecisionSettings s;
    s.fontSize = 36;
    ofRectangle r( 100, 100, ofGetWidth() - 200, ofGetHeight() - 200 );
    doc.draw("Hello world, this is a sentence", r, s);
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
