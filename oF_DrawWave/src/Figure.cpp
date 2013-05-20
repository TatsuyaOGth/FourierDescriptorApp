//
//  Figure.cpp
//  DrawWave
//
//  Created by Tatsuya Ogusu on 2013/05/06.
//
//

#include "Figure.h"

ofPoint cacCentroid(const vector<ofPoint> pts);

Figure::Figure()
{
    mID = -1;
    mMode = -1;
    mAlph = 255;
}

/**
 更新
 */
void Figure::update()
{
    if (mAlph > 127) {
        mAlph--;
    }
}

/**
 描画
 */
void Figure::draw()
{
    ofPushStyle();
    ofEnableAlphaBlending();
    
    if (mMode == 1) {
        ofSetColor(255, 255, 0, mAlph);
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
    } else if (mMode == 2) {
        ofSetColor(0, 255, 255, mAlph);
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

    }
    ofDisableAlphaBlending();
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
    //重心を描画
    ofSetColor(255, 255, 0);
    ofCircle(mPos, 2);
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
 @param mode モード
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
 同時に重心を計算
 */
void Figure::setEdgePts(const vector<ofPoint> edgePts)
{
    mEdgePts = edgePts;
    mPos = cacCentroid(edgePts);
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

/**
 重心を計算
 */
ofPoint cacCentroid(const vector<ofPoint> pts)
{
    if (pts.size()) {
        float sumX = 0;
        float sumY = 0;
        for (int i=0; i < pts.size(); i++) {
            sumX += pts[i].x;
            sumY += pts[i].y;
        }
        return ofPoint(sumX/pts.size(), sumY/pts.size());
    } else {
        cout << "[ERROR] faild get centroid " << endl;
        return ofPoint(-1,-1);
    }
}



