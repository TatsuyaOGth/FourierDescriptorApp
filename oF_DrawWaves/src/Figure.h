//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29~
//  http://ogsn.org @TatsuyaOGs
//  license http://creativecommons.org/licenses/by/3.0/
//

#ifndef __DrawWave__Figure__
#define __DrawWave__Figure__

#include "ofMain.h"
#include "ofxVectorGraphics.h"

enum FigureMode{
    STATIC=0,
    FLORTING,
    AROUND
};

/**
 形態クラス
 @class Figure
 */
class Figure {
public:
    
    Figure();
    ~Figure();

    void update();
    void draw();
    void debugDraw();
    void sendPlay();
    
    void setID(const int ID);
    void setPts(const vector<ofPoint> pts);
    void setEdgePts(const vector<ofPoint> edgePts);
    void setResizedPts(const vector<ofPoint> resizedPts);
    void setMode(const FigureMode mode);
    void setCurrentAroundNum(const int num);
    void plusCurrentAroundNum();
    void setSeqNum(const int num);
    int getID() const {return mID;};
    int getModeID() const {return (int)mMode;};
    vector<ofPoint> getEdgePts() const {return mEdgePts;};
    vector<double> getBits() const {return mBits;};
    ofPoint getCentPos() const {return mCentPos;};
    bool getAlive();
    
    float mVol;
    
private:
    
    int mID;
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts;
    vector<ofPoint> mEdgePts;
    vector<ofPoint> mResizedPts;
    vector<double> mBits;
    bool mAlive;
    int mAlp;
    int mPtsMinus;
    
    //update
    float mPosX, mPosY, mPosZ;
    float mSpX, mSpY, mSpZ;
    
    FigureMode mMode;

    ofPoint mCentPos;
    int mCurrentAroundNum;
    
    int mSeqNum;
    
};

#endif /* defined(__DrawWave__Figure__) */
