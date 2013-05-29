//
// you must add ofxBox2d
// https://github.com/vanderlin/ofxBox2d
//
// reference:
// http://yoppa.org/geidai_graphb11/3408.html
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofBackground(255);
    ofSetFrameRate(16);
    
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createBounds();
    mBox2d.setFPS(20.0);
    mBox2d.registerGrabbing();
        
    mSender.setup("localhost", 50002);
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    mBox2d.update();
    
//    mRects[0].setPosition(mouseX, mouseY);
    
    mAngles.clear();
    float sumX = 1;
    float sumY = 1;
    for (int i = 0; i < mRects.size(); i++) {
        double rnd = atan2((double)(mRects[0].getPosition().y - mRects[i].getPosition().y),
                           (double)(mRects[0].getPosition().x - mRects[i].getPosition().x));
        mAngles.push_back(sin(rnd));
        
        sumX += mRects[i].getPosition().x;
        sumY += mRects[i].getPosition().y;
    }
    mCentroid.set(sumX/mRects.size(), sumY/mRects.size());
    

    for (int i = 0; i < mAngles.size(); i++) {
        ofxOscMessage m;
        m.setAddress("/ang");
        m.addFloatArg((float)mAngles[i]);
        mSender.sendMessage(m);
    }
    
    ofxOscMessage m;
    m.setAddress("/cent");
    m.addFloatArg(mCentroid.x/ofGetWidth());
    m.addFloatArg(mCentroid.y/ofGetHeight());
    mSender.sendMessage(m);
    
}

//--------------------------------------------------------------
void testApp::draw(){

    for (int i = 0; i < mJoints.size(); i++) {
        ofNoFill();
        ofSetColor(127);
        mJoints[i].draw();
    }
    
    for (int i = 0; i < mRects.size(); i++) {
        ofFill();
        ofSetColor(0);
        mRects[i].draw();
    }
    
    ofNoFill();
    ofCircle(mCentroid, 10);
}

void testApp::exit(){
    ofxOscMessage m;
    m.setAddress("/set");
    m.addStringArg("bang");
    mSender.sendMessage(m);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == OF_KEY_BACKSPACE || key == OF_KEY_DEL) {
        mRects.pop_back();
        mJoints.pop_back();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    for (int i = 0; i < mRects.size(); i++) {
        mRects[i].addRepulsionForce(x, y, -0.1);
    }
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    ofxBox2dRect rect;
    rect.setPhysics(10.0, 0.99, 0.0);
    
    rect.setup(mBox2d.getWorld(), x, y, 2, 2);
    mRects.push_back(rect);
    
    if (mRects.size() > 1) {
        mJoints.clear();
        for (int i = 1; i < mRects.size(); i++) {
            ofxBox2dJoint joint;
            joint.setup(mBox2d.getWorld(), mRects[i-1].body, mRects[i].body);
            joint.setFrequency(0.2);
            joint.setDamping(0.1);
            joint.setLength(200);
            mJoints.push_back(joint);
        }
        ofxOscMessage m;
        m.setAddress("/size");
        m.addIntArg(mRects.size());
        mSender.sendMessage(m);
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createBounds();
    mBox2d.setFPS(20.0);
    mBox2d.registerGrabbing();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}