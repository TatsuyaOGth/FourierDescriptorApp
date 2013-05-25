//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29
//  http://ogsn.org @TatsuyaOGs
//  license http://creativecommons.org/licenses/by/3.0/
//

#ifndef __DrawWave__Figure__
#define __DrawWave__Figure__

#include "ofMain.h"
#include "ofxVectorGraphics.h"

enum FigureMode{
    STATIC_MODE=0,
    AROUND_MODE
};

/**
 形態クラス
 @class Figure
 */
class Figure {
public:
    
    Figure();

    void update();
    void draw();
    void debugDraw();
    void sendPlay();
    
    void setID(const int ID);
    void setPts(const vector<ofPoint> pts);
    void setEdgePts(const vector<ofPoint> edgePts);
    void setMode(const FigureMode mode);
    int getID(){return mID;};
    bool getAlive();
    
private:
    
    int mID;
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts;
    vector<ofPoint> mEdgePts;
    bool mAlive;
    int mAlp;
    
    //update
    float mPosX, mPosY, mPosZ;
    float mSpX, mSpY, mSpZ;
    
    FigureMode mMode;
    
    ofPoint mCentPos;
    int mCurrentAroundNum;
    
        
};

#endif /* defined(__DrawWave__Figure__) */
