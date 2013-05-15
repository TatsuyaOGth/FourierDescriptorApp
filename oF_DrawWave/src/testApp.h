#pragma once

#include "ofMain.h"
#include "ofxVectorGraphics.h"
#include "ofxOpenCv.h"
#include "ofxControlPanel.h"
#include "ofxOsc.h"
#include "Figure.h"

/** 波形パラメータ構造体 */
typedef struct _wave{
    float dist;
    float bit;
} wave;

/**
 メインクラス
 @class testApp
 */
class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void debugDraw();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float * input, int bufferSize, int nChannels);

    
private:
    
    void calcFourier();
    vector<wave> getShapeFrequency(const ofImage src, const unsigned interval);
    void sendSet();
    void sendFigId(const int figId);
    void sendBits();
    
    ofxControlPanel gui;
    
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts; //!< 描画した線の点配列
    ofImage mGrabImage;
    vector<wave> mEdgeBits; //!< mPtsを波形変換したもの
    unsigned int mInterval;
    
    ofRectangle mPtsRect;
    ofRectangle mScaleRect;
    bool bDebugMode;
    bool bDrawing;
    bool bProcessGetDraw;
    bool bCircleMode;
    
    vector<Figure> mFigures;
    
    //osc
    ofxOscSender sender;
    
    //temp
    float mMouseX, mMouseY;
    ofImage tmpBackImg;
    
    //Audio in
    vector <float> mLefts;
    vector <float> mRights;
    vector <float> mVolHistory;
    int 	mBufferCounter;
    int 	mDrawCounter;
    float mSmoothedVol;
    float mScaledVol;
    ofSoundStream mSoundStream;
    
};
