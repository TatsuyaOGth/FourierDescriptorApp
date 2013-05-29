#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
private:
    
    ofxBox2d mBox2d;
    vector<ofxBox2dRect> mRects;
    vector<ofxBox2dJoint> mJoints;
    
    ofxOscSender mSender;
    
    vector<double> mAngles;
    ofPoint mCentroid;
    
};
