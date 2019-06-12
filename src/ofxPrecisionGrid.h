
#pragma once
#include "ofMain.h"


class ofxPrecisionRow {
public:
    ofRectangle bounds;
    
    vector<ofxPrecisionRow *> chil;
    ofxPrecisionRow() {};
    ofxPrecisionRow(float x, float y, float w, float h) {
        ofRectangle r(x, y, w, h);
        set(r);
    }
    void set(float x, float y, float w, float h) {
        ofRectangle r(x, y, w, h);
        set(r);
    }
    ofxPrecisionRow(ofRectangle r) {
        set(r);
    }
    void setPosition(float x, float y) {
        set(x, y, bounds.width, bounds.height);
    }
    void set(ofRectangle r) {
        bounds = r;
    }
    
    void draw() {
        int i = 0;
        for (auto & ch : chil) {
            
            ofColor c;
            c.setHsb( ofMap(i, 0, chil.size(), 0, 255), 255, 255);
            ofSetColor(c);
            ofNoFill();
            ofSetLineWidth(10);
            ofDrawRectangle(ch->bounds);
            ofLog() << ch->chil.size();
            ch->draw();
            i += 1;
        }
    }
    
    void refresh() {
        
    }
    
    ofxPrecisionRow & add(int idx = -1) {
        
        
        /*-- Create rect --*/
        
        float w = bounds.width / ( chil.size() + 1 );
        
        ofxPrecisionRow * ch = new ofxPrecisionRow;
        ch->set(bounds.x, bounds.y, w, bounds.height);
        
        /*-- Insert new rect --*/
        
        if (idx > chil.size() || idx < 0) idx = chil.size();
        chil.insert(chil.begin() + idx, ch);
        
        /*-- Iterate widths --*/
        
        int iter = 0;
        for (int i = 0; i < chil.size(); i ++) {
            int x;
            int w = chil[i]->bounds.width;
            x = bounds.x + iter;
            if (i != idx) {
                w -= ( ch->bounds.width / (chil.size() - 1) );
            }
            
            /*-- Trigger inner children --*/
            
            chil[i]->set(x, chil[i]->bounds.y, w, chil[i]->bounds.height);
            
            iter += chil[i]->bounds.width;
        }
        
        /*-- Return vector --*/
        
        return * ch;
    }
    
//    vector<ofxPrecisionRow> setWidth(int idx, float w) {
//
//        //        if (idx == cols.size() - 1) return {};
//        //
//        //        /*-- Set rect width --*/
//        //
//        //        w *= bounds.width; // new width
//        //        float rm = w - chil[idx].width;
//        //        cols[idx].width = w;
//        //
//        //        /*-- Iterate widths --*/
//        //
//        //        for (int i = idx + 1; i < cols.size(); i ++) {
//        //            cols[i].width -= rm/ (cols.size() - 1);
//        //        }
//        //        /*-- Iterate X positions --*/
//        //
//        //        width = 0;
//        //        for (auto & c : cols) {
//        //            c.x = bounds.x + width;
//        //            width += c.width;
//        //        }
//        return chil;
//    }
};
