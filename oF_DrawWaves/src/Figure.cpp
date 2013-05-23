//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29
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
}

/**
 更新
 */
void Figure::update()
{
    mPosX += mSpX;
    mPosY += mSpY;
    mPosZ += mSpZ;
    if (mAlp > 0) mAlp--;
    else mAlive = false;
}

/**
 描画
 */
void Figure::draw()
{
    ofPushMatrix();
    ofPushStyle();

    ofTranslate(mPosX, mPosY, mPosZ);
    ofSetColor(255, 255, 255, mAlp);

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
    ofPopMatrix();
}

/**
 デバッグ表示
 */
void Figure::debugDraw()
{
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(mPosX, mPosY, mPosZ);
    for (int i=0; i < mEdgePts.size(); i++) {
        ofFill();
        ofSetColor(255, 0, 0);
        ofCircle(mEdgePts[i], 2);
    }
    ofPopStyle();
    ofPopMatrix();
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
}

/**
 生きているかどうか
 */
bool Figure::getAlive()
{
    return mAlive;
}
