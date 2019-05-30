#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    text.setup();
    ofLog::setAutoSpace(true);
    ofSetFrameRate(60.0f);
    
    int a = 0;
    int b = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    text.setColor(ofColor(255));
    text.letterSpacing = 0.4;
//    string txt = "\n# Heading A\n## Heading B\n### Heading C\n#### Heading D\n##### Heading E\n###### Heading F\n\n\n## Horizontal Rules\n\n___\n\n---\n\n***\n\n\n## Typographic replacements\n\nEnable typographer option to see result.\n\n(c) (C) (r) (R) (tm) (TM) (p) (P) +-\n\ntest.. test... test..... test?..... test!....\n\n!!!!!! ???? ,,  -- ---\n\n\"Smartypants, double quotes\" and 'single quotes'\n\n\n## Emphasis\n\n**This is bold text**\n\n__This is bold text__\n\n*This is italic text*\n\n_This is italic text_\n\n~~Strikethrough~~\n\n\n## Blockquotes\n\n\n> Blockquotes can also be nested...\n>> ...by using additional greater-than signs right next to each other...\n> > > ...or with spaces between arrows.\n\n\n## Lists\n\nUnordered\n\n+ Create a list by starting a line with `+`, `-`, or `*`\n+ Sub-lists are made by indenting 2 spaces:\n  - Marker character change forces new list start:\n    * Ac tristique libero volutpat at\n    + Facilisis in pretium nisl aliquet\n    - Nulla volutpat aliquam velit\n+ Very easy!\n\nOrdered\n\n1. Lorem ipsum dolor sit amet\n2. Consectetur adipiscing elit\n3. Integer molestie lorem at massa\n\n\n1. You can use sequential numbers...\n1. ...or keep all the numbers as `1.`\n\nStart numbering with offset:\n\n57. foo\n1. bar\n\n\n## Code\n\nInline `code`\n\nIndented code\n\n    // Some comments\n    line 1 of code\n    line 2 of code\n    line 3 of code\n\n\nBlock code \"fences\"\n\n```\nSample text here...\n```\n\nSyntax highlighting\n\n``` js\nvar foo = function (bar) {\n  return bar++;\n};\n\nconsole.log(foo(5));\n```\n\n## Tables\n\n| Option | Description |\n| ------ | ----------- |\n| data   | path to data files to supply the data that will be passed into templates. |\n| engine | engine to be used for processing templates. Handlebars is the default. |\n| ext    | extension to be used for dest files. |\n\nRight aligned columns\n\n| Option | Description |\n| ------:| -----------:|\n| data   | path to data files to supply the data that will be passed into templates. |\n| engine | engine to be used for processing templates. Handlebars is the default. |\n| ext    | extension to be used for dest files. |\n\n\n## Links\n\n[link text](http://dev.nodeca.com)\n\n[link with title](http://nodeca.github.io/pica/demo/ \"title text!\")\n\nAutoconverted link https://github.com/nodeca/pica (enable linkify to see)\n\n\n## Images\n\n![Minion](https://octodex.github.com/images/minion.png)\n![Stormtroopocat](https://octodex.github.com/images/stormtroopocat.jpg \"The Stormtroopocat\")\n\nLike links, Images also have a footnote style syntax\n\n![Alt text][id]\n\nWith a reference later in the document defining the URL location:\n\n[id]: https://octodex.github.com/images/dojocat.jpg  \"The Dojocat\"\n\n\n## Plugins\n\nThe killer feature of `markdown-it` is very effective support of\n[syntax plugins](https://www.npmjs.org/browse/keyword/markdown-it-plugin).\n\n\n### [Emojies](https://github.com/markdown-it/markdown-it-emoji)\n\n> Classic markup: :wink: :crush: :cry: :tear: :laughing: :yum:\n>\n> Shortcuts (emoticons): :-) :-( 8-) ;)\n\nsee [how to change output](https://github.com/markdown-it/markdown-it-emoji#change-output) with twemoji.\n\n\n### [Subscript](https://github.com/markdown-it/markdown-it-sub) / [Superscript](https://github.com/markdown-it/markdown-it-sup)\n\n- 19^th^\n- H~2~O\n\n\n### [\<ins>](https://github.com/markdown-it/markdown-it-ins)\n\n++Inserted text++\n\n\n### [\<mark>](https://github.com/markdown-it/markdown-it-mark)\n\n==Marked text==\n\n\n### [Footnotes](https://github.com/markdown-it/markdown-it-footnote)\n\nFootnote 1 link[^first].\n\nFootnote 2 link[^second].\n\nInline footnote^[Text of inline footnote] definition.\n\nDuplicated footnote reference[^second].\n\n[^first]: Footnote **can have markup**\n\n    and multiple paragraphs.\n\n[^second]: Footnote text.\n\n\n### [Definition lists](https://github.com/markdown-it/markdown-it-deflist)\n\nTerm 1\n\n:   Definition 1\nwith lazy continuation.\n\nTerm 2 with *inline markup*\n\n:   Definition 2\n\n        { some code, part of Definition 2 }\n\n    Third paragraph of definition 2.\n\n_Compact style:_\n\nTerm 1\n  ~ Definition 1\n\nTerm 2\n  ~ Definition 2a\n  ~ Definition 2b\n\n\n### [Abbreviations](https://github.com/markdown-it/markdown-it-abbr)\n\nThis is HTML abbreviation example.\n\nIt converts \"HTML\", but keep intact partial entries like \"xxxHTMLyyy\" and so on.\n\n*[HTML]: Hyper Text Markup Language\n\n### [Custom containers](https://github.com/markdown-it/markdown-it-container)\n\n::: warning\n*here be dragons*\n:::  ";
    string txt = "Hello **world** \n# Heading A\nHello world Hello world Hello world Hello world Hello world Hello world \n## Heading B\n### Heading C\nHello world Hello world Hello world Hello world Hello world Hello world \n## [Lorem Ipsum](google.com) \nDonec aliquam lectus vel facilisis iaculis. Integer ut tortor ac tortor accumsan semper id eu turpis. In consectetur et purus sit amet blandit. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Mauris vitae ante eget dolor consequat feugiat. Suspendisse eu lacus tellus. Ut rhoncus id eros eget tristique. \n# Header\nPraesent in elit nec urna auctor posuere non in magna. \n## Header\nMorbi et egestas ex. Curabitur sodales, quam quis \n### Header\nconsectetur facilisis, nunc tortor vulputate risus, sit amet eleifend lacus magna id mi. Sed at ex in massa ornare cursus eget sit amet massa. Etiam mollis mollis consequat. \nPraesent ac velit # ullamcorper\n, gravida odio sit amet, pellentesque ipsum. Nunc ullamcorper dui dui, vel gravida sapien finibus in. Ut feugiat enim tellus, quis faucibus eros pretium id. Quisque a ex sed arcu sagittis viverra.";
    
    
    int excess = (ofGetHeight() - structure.bounds.height);
    int y = ofMap( ofGetMouseY(), 150, ofGetHeight() - 150, 0, excess);
    
    ofRectangle r(100,y,ofGetWidth() - 200, ofGetHeight() - 200);
    structure = text.draw(txt, r, -1, 0);
    
    ofSetColor(255,0,0);
    ofNoFill();
    ofDrawRectangle(r);
//    for (auto & ch : structure.chars) {
//
//        ofSetColor(0,255,255,150);
//        ofFill();
//        ofDrawRectangle(ch.bounds.x, ch.bounds.y, ch.bounds.width, ch.bounds.height);
//    }
//    if (a == b) {
//        ofSetColor(255,0,0);
//        ofSetLineWidth(4);
//        ofNoFill();
//        ofDrawLine(structure.chars[a].bounds.getTopLeft(), structure.chars[a].bounds.getBottomLeft());
//    } else {
    
//        int a = (pressed < dragged) ? pressed : dragged;
//        int b = (pressed <= dragged) ? dragged : pressed;
//        for (int i = a; i < b; i++) {
//            ofSetColor(255,255,255,150);
//            ofFill();
//            ofRectangle r = structure.chars[i].bounds;
//            ofDrawRectangle(r);
//        }
//    }
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

int ofApp::getNearestCharacter(int x, int y) {
    
    for (int i = 0; i < structure.chars.size(); i++) {
        if (structure.chars[i].bounds.inside(x, y)) {
            return i;
        }
    }
    int chosen = 0;
    float d = 99999;
    for (int i = 0; i < structure.chars.size(); i++) {
        if (y > structure.chars[i].bounds.getTop() && y <= structure.chars[i].bounds.getBottom()) {
            ofPoint p = structure.chars[i].bounds.getCenter();
            float dd = p.distance( ofVec3f(x, y) );
            if (dd < d) {
                d = dd;
                chosen = i;
            }
        }
    }
    return chosen;
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

    int dragged = getNearestCharacter(x, y);
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

    pressed = getNearestCharacter(x, y);
//    b = getNearestCharacter(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

//    b = getNearestCharacter(x, y);
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
