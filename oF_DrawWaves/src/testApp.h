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
    vector<ofPoint> getAudioInPoints();
    void sendSet();
    void sendFigId(const int figId);
    void sendBits();
    void sendPos(const ofPoint pos);
    void sendFigMode(const int mode);
    void sendDelete(const int mode, const int figID);    
        
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts; //!< 描画した線の点配列
    ofImage mGrabImage;
    ofImage mTmpWindow;
    vector<wave> mEdgeBits; //!< mPtsを波形変換したもの
    unsigned int mInterval;
    
    bool bDebugMode;
    bool bDrawing;
    bool bProcessGetDraw;
    bool bCircleMode;
    bool bSoundDrawMode;
    
    
    vector<Figure> mFigures1; //!< figモード1
    vector<Figure> mFigures2; //!< figモード2
    vector<vector<int> > mFigIDs; //!< (1)パターン (2)ID
    int mFigMode;
    bool bIDMaxed;
    
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
