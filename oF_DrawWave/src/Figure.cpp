//
//  Figure.cpp
//  DrawWave
//
//  Created by Tatsuya Ogusu on 2013/05/06.
//
//

#include "Figure.h"

/**
 コンストラクタ
 @param pts 輪郭点の配列
 */
Figure::Figure(const vector<ofPoint> pts, const int ID)
{
    if (pts.size()) {
        mPts = pts;
    } else {
        cout << "[ERROR] points null" << endl;
    }
    
    mID = ID;
}

/**
 デストラクタ
 */
Figure::~Figure()
{
}

/**
 更新
 */
void Figure::update()
{
    //test
    for (int i=0; i < mPts.size(); i++) {
        mPts[i].x -= 1;
    }
}

/**
 描画
 */
void Figure::draw()
{
    ofPushStyle();

    if( mPts.size() > 0 ) {
        int numPts = mPts.size();
        int rescaleRes = 1;
        mVecOut.fill();
        mVecOut.beginShape();
        for(int i = 0; i < numPts; i++){
            if(i == 0 || i == numPts -1){
                mVecOut.curveVertex(mPts[i].x, mPts[i].y);
            }
            if(i % rescaleRes == 0) mVecOut.curveVertex(mPts[i].x, mPts[i].y);
        }
        mVecOut.endShape();
    }
    
    ofPopStyle();
}

/**
 デバッグ表示
 */
void Figure::debugDraw()
{
    ofPushStyle();
    for (int i=0; i < mPts.size(); i++) {
        ofFill();
        ofSetColor(255, 0, 0);
        ofCircle(mPts[i], 2);
    }
    ofPopStyle();
}


/**
 輪郭点配列が０以上あるか
 */
bool Figure::getAlive()
{
    if (mPts.size() > 0) {
        return true;
    } else {
        return false;
    }
}



