//
//  Figure.cpp
//  DrawWave
//
//  Created by Tatsuya Ogusu on 2013/05/06.
//
//

#include "Figure.h"


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
    for (int i=0; i < mEdgePts.size(); i++) {
        ofFill();
        ofSetColor(255, 0, 0);
        ofCircle(mEdgePts[i], 2);
    }
    ofPopStyle();
}

/**
 IDをセット
 */
void Figure::setID(const int ID)
{
    mID = ID;
}

/**
 モードをセット
 */
void Figure::setMode(const int mode)
{
    mMode = mode;
}

/**
 手書き線座標配列をセット
 */
void Figure::setPts(const vector<ofPoint> pts)
{
    mPts = pts;
}

/**
 実際に計算に使用した輪郭点を取得
 */
void Figure::setEdgePts(const vector<ofPoint> edgePts)
{
    mEdgePts = edgePts;
}


/**
 輪郭点配列が０以上あるか
 */
bool Figure::getAlive()
{
    if (mPts.size() > 0) {
        return false;
    } else {
        return true;
    }
}



