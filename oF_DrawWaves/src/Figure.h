//
//  Figure.h
//  DrawWave
//
//  Created by Tatsuya Ogusu on 2013/05/06.
//
//

#ifndef __DrawWave__Figure__
#define __DrawWave__Figure__

#include "ofMain.h"
#include "ofxVectorGraphics.h"


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
    void setMode(const int mode);
    void setPts(const vector<ofPoint> pts);
    void setEdgePts(const vector<ofPoint> edgePts);
    int getID(){return mID;};
    int getMode(){return mMode;};
    ofPoint getPos(){return mPos;};
    
    bool getAlive();
    
private:
    
    int mID;
    int mMode;
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts;
    vector<ofPoint> mEdgePts;
    
    int mAlph;
    ofPoint mPos;
        
};

#endif /* defined(__DrawWave__Figure__) */
