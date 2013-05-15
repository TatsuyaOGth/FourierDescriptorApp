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
    
    Figure(const vector<ofPoint> pts, const int ID);
    ~Figure();
    
    void update();
    void draw();
    void debugDraw();
    void sendPlay();
    
    int getID(){return mID;};
    bool getAlive();
    
private:
    
    int mID;
    ofxVectorGraphics mVecOut;
    vector<ofPoint> mPts;
        
};

#endif /* defined(__DrawWave__Figure__) */
