//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29~
//  http://ogsn.org @TatsuyaOGs
//  license http://creativecommons.org/licenses/by/3.0/
//

#include "Figure.h"

Figure::Figure()
{
    mPosX = 0;
    mPosY = 0;
    mPosZ = 0;
    mSpX = ofRandom(-1,1);
    mSpY = ofRandom(-1,1);
    mSpZ = ofRandom(-2,0);
    mAlive = true;
    mAlp = 255;
    
    mMode = STATIC;
    mCurrentAroundNum = 0;
    mCentPos.set(0, 0);
}

Figure::~Figure()
{
    
}

/**
 更新
 */
void Figure::update()
{
}

/**
 描画
 */
void Figure::draw()
{
    ofEnableAlphaBlending();

    //==========
    // Static Mode
    //==========
    if (mMode == STATIC) {
        ofPushStyle();
        ofSetColor(255, 255, 255);
        if(mPts.size() > 0) {
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
    
    //==========
    // Floating Mode
    //==========
    else if (mMode == FLORTING) {
        mPosX += mSpX;
        mPosY += mSpY;
        mPosZ += mSpZ;
        if (mAlp > 0) mAlp--;
        else mAlive = false;
        
        ofPushMatrix();
        ofPushStyle();
        
        ofTranslate(mPosX, mPosY, mPosZ);
        ofSetColor(255, 255, 255, mAlp);
        
        if(mPts.size() > 0) {
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
        ofPopMatrix();
        ofPopStyle();
    }
        
    //==========
    // Around Mode
    //==========
    else if (mMode == AROUND) {
//        mCurrentAroundNum = (mCurrentAroundNum + 1) % mEdgePts.size();
        
        ofPushStyle();
        ofSetColor(255, 255, 255);

        if(mPts.size() > 0) {
            int numPts = mPts.size();
            int rescaleRes = 1;
            mVecOut.noFill();
            mVecOut.beginShape();
            for(int i = 0; i < numPts; i++){
                if(i == 0 || i == numPts -1){
                    mVecOut.curveVertex(mPts[i].x, mPts[i].y);
                }
                if(i % rescaleRes == 0) mVecOut.curveVertex(mPts[i].x, mPts[i].y);
            }
            mVecOut.endShape();
        }
        // edge points
        ofFill();
        for (int i=0; i < mEdgePts.size(); i++) {
            ofFill();
            ofSetColor(255,0,0);
            ofCircle(mEdgePts[i], 2);
        }
        // centroid point
        ofSetColor(255, 255, 0);
        ofCircle(mCentPos, 5);
        // around points
        if (mEdgePts.size()) {
            ofLine(mCentPos, mEdgePts[mCurrentAroundNum]);
            ofSetColor(255, 0, 0);
            ofCircle(mEdgePts[mCurrentAroundNum], 10);
        }
        ofPopStyle();
    }

    ofDisableAlphaBlending();
}

/**
 デバッグ表示
 */
void Figure::debugDraw()
{
    ofPushStyle();
    //必要なら何か追加
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
    
    //重心を計算
    float sumX = 0;
    float sumY = 0;
    for (int i = 0; i < mEdgePts.size(); i++) {
        sumX += mEdgePts[i].x;
        sumY += mEdgePts[i].y;
    }
    mCentPos.set(sumX/mEdgePts.size(), sumY/mEdgePts.size());
}

/**
 描画時のサイズからインターバルをとった輪郭点座標配列をセット
 */
void Figure::setResizedPts(const vector<ofPoint> resizedPts)
{
    mResizedPts = resizedPts;
}

/**
 生きているかどうか
 */
bool Figure::getAlive()
{
    return mAlive;
}

/**
 モードをセット
 */
void Figure::setMode(const FigureMode mode)
{
    mMode = mode;
}

/**
 現在の周回点をセット
 */
void Figure::setCurrentAroundNum(const int num)
{
    mCurrentAroundNum = num % mEdgePts.size();
}

/**
 現在の周回点をインクリメント
 */
void Figure::plusCurrentAroundNum()
{
    mCurrentAroundNum = (mCurrentAroundNum + 1) % mEdgePts.size();
}
