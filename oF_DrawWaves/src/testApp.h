//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29
//  http://ogsn.org @TatsuyaOGs
//  license http://creativecommons.org/licenses/by/3.0/
//

#pragma once

#include "ofMain.h"
#include "ofxVectorGraphics.h"
#include "ofxOsc.h"
#include "Figure.h"

/** 波形パラメータ構造体 */
typedef struct _wave{
    double dist;
    double bit;
    ofPoint contPts; //!<計算した輪郭座標点配列
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
        
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts; //!< 描画した線の点配列
    ofImage mGrabImage;
    ofImage mTmpWindow;
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
    
    
    //Audio in
    vector <float> mLefts;
    vector <float> mRights;
    vector <float> mVolHistory;
    int 	mBufferCounter;
    int 	mDrawCounter;
    float mSmoothedVol;
    float mScaledVol;
    ofSoundStream mSoundStream;
    
    ofCamera mCamera;
    
};
