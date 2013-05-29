//
//  FourierDescriptorApp | Pure Data Japan 1st Session @ Shibuya 2.5D
//  Created by Tatsuya Ogusu 2013/05/29~
//  http://ogsn.org @TatsuyaOGs
//  license http://creativecommons.org/licenses/by/3.0/
//

#include "testApp.h"

//Local Meothod
ofPoint convertIndex2Points(int i, int w);
bool isSide(int i, int x, int h);
vector<ofPoint> getContourPoints(const ofPixels src);

static const string         HOST = "localhost";
static const unsigned int   SEND_PORT = 50001;
static const unsigned int   RECEIVE_PORT = 50000;
static const int            MAX_FIG_ID = 30;

//--------------------------------------------------------------
void testApp::setup()
{
    
    ofBackground(0);
    ofSetFrameRate(60);
    ofSetCircleResolution(80);
    ofSetVerticalSync(true);
    
    //----------
    // 初期化
    //----------
    mInterval = 30;
    bDebugMode = true;
    bDrawing = false;
    bProcessGetDraw = false;
    bCircleMode = false;
    mPtsRect.set(-1, -1, -1, -1);
    
    //osc
    sender.setup(HOST, SEND_PORT);
    receiver.setup(RECEIVE_PORT);
    mSeqCurrentNum = 0;
    
    mFMode = STATIC;
    
    //予めモード数の分要素数を確保
    mFigures.resize(3);
    
    //----------
    //Sound stream
    //----------
    mSoundStream.listDevices();
    int mBufferSize = 256;
	
	mLefts.assign(mBufferSize, 0.0);
	mRights.assign(mBufferSize, 0.0);
	mVolHistory.assign(400, 0.0);
	mBufferCounter	= 0;
	mDrawCounter		= 0;
	mSmoothedVol     = 0.0;
	mScaledVol		= 0.0;
	mSoundStream.setup(this, 0, 2, 44100, mBufferSize, 4);

}

void testApp::update()
{
    
    //----------
    // 形態を更新
    //----------
    //死んでた場合は配列から削除
    for (int i=0; i < mFigures.size(); i++) {
        for (int j=0; j < mFigures[i].size(); j++) {
            if (!mFigures[i][j].getAlive()) {
                mFigures[i].erase(mFigures[i].begin()+j);
            }
        }
    }
    
    //OSC受信による図形の更新
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        for (int i=0; i < mFigures.size(); i++) {
            for (int j=0; j < mFigures[i].size(); j++) {
                if (mFigures[i][j].getModeID() == AROUND && m.getAddress() == "/around_num") {
                    if (m.getArgAsInt32(0) == j) {
                        int tmp = ofMap(m.getArgAsInt32(1), 0, 255, 0, mFigures[i][j].getEdgePts().size());
                        mFigures[i][j].setCurrentAroundNum(tmp);
                    }
                }
            }
        }
        
        //OSCで送られてくるシーケンサーの現在の数値を取得
        if (m.getAddress() == "/seq") {
            mSeqCurrentNum = m.getArgAsInt32(0);
        }
    }
    
    //STATICモードのシーケンサーを更新
    for (int i=0; i < mFigures[0].size(); i++) {
        mFigures[0][i].setSeqNum(mSeqCurrentNum);
    }
    
    
    //----------
    // Sound
    //----------
	mScaledVol = ofMap(mSmoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    mVolHistory.push_back( mScaledVol );
    if (mVolHistory.size() >= 400 ){
		mVolHistory.erase(mVolHistory.begin(), mVolHistory.begin()+1);
	}    
    //図形に音量値を渡す
    for (int i = 0; i < mFigures.size(); i++) {
        for (int j = 0; j < mFigures[i].size(); j++) {
            mFigures[i][j].mVol = mScaledVol;
        }
    }

}

void testApp::draw()
{
    
    //=====================
    // 入力画像抽出処理開始
    //=====================
    if (bProcessGetDraw) {
        bool bError = false;
        mTmpWindow.grabScreen(0, 0, ofGetWidth(), ofGetHeight()); //表示中の画面を取得
        
        //----------
        // 描画がある場合は波形変換を開始
        //----------
        if (mPts.size()) {
            
            //----------
            // 図形の格子範囲を取得
            //----------
            ofRectangle scaleRect;
            float minX = ofGetWidth();
            float minY = ofGetHeight();
            float maxX = 0;
            float maxY = 0;
            for (int i=0; i < mPts.size(); i++) {
                if (mPts[i].x < minX) minX = mPts[i].x;
                if (mPts[i].y < minY) minY = mPts[i].y;
                if (mPts[i].x > maxX) maxX = mPts[i].x;
                if (mPts[i].y > maxY) maxY = mPts[i].y;
            }
            //正確に取得できたかチェック
            if (minX > maxX || minY > maxY) {
                cout << "[ERROR] 描画領域の取得に失敗しました" << endl;
                bError = true;
                goto _FAILD_GET_DRAWING;
            }
            scaleRect.set(minX, minY, maxX-minX, maxY-minY);
            mPtsRect = scaleRect;
            
            //----------
            // 格子範囲を正規化
            //----------
            scaleRect.setPosition((ofGetWidth()/2)-(scaleRect.width/2), (ofGetHeight()/2)-(scaleRect.height/2));
            float tScale = 1.0f;
            if (scaleRect.width < scaleRect.height) {
                tScale = (float)ofGetHeight() / scaleRect.height;
            } else {
                tScale = (float)ofGetHeight() / scaleRect.width;
            }
            scaleRect.scaleFromCenter(tScale);
            scaleRect.setPosition(0, 0);

            //----------
            // 正規化した格子範囲に合わせて図形の大きさを変更した座標配列を取得
            //----------
            vector<ofPoint> tPts;
            for (int i=0; i < mPts.size(); i++) {
                float tX = ofMap(mPts[i].x, minX, maxX, scaleRect.x, scaleRect.width);
                float tY = ofMap(mPts[i].y, minY, maxY, scaleRect.y, scaleRect.height);
                ofPoint tPoint = ofPoint(tX, tY);
                tPts.push_back(tPoint);
            }
            
            if (bCircleMode) {
                //----------
                // 円モードの場合は円を描画
                //----------
                if( mPts.size() > 0 ) {
                    float size = ofDist(mPts[0].x, mPts[0].y, mPts[mPts.size()-1].x, mPts[mPts.size()-1].y);
                    mVecOut.circle(mPts[0].x, mPts[0].y, size);
                }
            } else {
                //----------
                // 描いた図形をfillして描画
                //----------
                int numPts = tPts.size();
                int rescaleRes = 2;
                mVecOut.fill();
                mVecOut.beginShape();
                for(int i = 0; i < numPts; i++){
                    if(i == 0 || i == numPts -1){
                        mVecOut.curveVertex(tPts[i].x, tPts[i].y);
                    }
                    if(i % rescaleRes == 0) mVecOut.curveVertex(tPts[i].x, tPts[i].y);
                }
                mVecOut.endShape();
            }
            
            //スクリーンショット
            mGrabImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            mTmpWindow.draw(0, 0); //元の画面を描画
            
            bProcessGetDraw = false;
            
            
            //----------
            // フーリエ記述子の計算 - 波形変換 - OSC送信
            //----------
            calcFourier();
            
            //----------
            // Figureクラスのインスタンスを生成して輪郭座標点を渡す
            //----------
            Figure tmpF;
            
            //図形IDを設定
            if (mFMode==STATIC) {
                //STATICモードの場合はOSCで送られてくるシーケンサーの現在の数値をIDとする
                tmpF.setID(mSeqCurrentNum);
            } else if (mFMode==FLORTING) {
                tmpF.setID(0);
            } else if (mFMode==AROUND) {
                tmpF.setID(mFigures[2].size());
            }
            
            //円モードの場合は計算に使用した輪郭点を描画点として置き換える
            if (bCircleMode) {
            
                vector<ofPoint> edgePts;
                for (int i=0; i < mResizedPts.size(); i++) {
                    edgePts.push_back(mResizedPts[i]);
                }
                tmpF.setPts(edgePts);
                tmpF.setEdgePts(edgePts);
            
            } else {
            
                //計算に使用した輪郭点座標を元の大きさにスケーリングして取得
                vector<ofPoint> edgePts;
                for (int i=0; i < mEdgeBits.size(); i++) {
                    ofPoint tmpPoint;
                    tmpPoint.set(ofMap(mResizedPts[i].x, scaleRect.x, scaleRect.width, minX, maxX),
                                 ofMap(mResizedPts[i].y, scaleRect.y, scaleRect.height, minY, maxY));
                    edgePts.push_back(tmpPoint);
                }
                tmpF.setEdgePts(edgePts);
                tmpF.setPts(mPts);

            }
            //モードをセットしてモードごとの配列に格納
            //MEMO:現状、図形モードは配列の要素数とインスタンス毎のモード設定の２つで管理しているので、どっちかにしたい
            tmpF.setMode(mFMode);
            switch (mFMode) {
                case STATIC: mFigures[0].push_back(tmpF); break;
                case FLORTING: mFigures[1].push_back(tmpF); break;
                case AROUND: mFigures[2].push_back(tmpF); break;
            }
            
            //----------
            // OSCでもろもろ送信
            // 1.モードID 2.図形ID 3.全ビット 4.重心座標
            //----------
            sendModeId((int)mFMode);
            switch (mFMode) {
                case STATIC: sendBits(0, tmpF.getID()); sendPos(0, tmpF.getID(), tmpF.getCentPos()); break;
                case FLORTING: sendBits(1, 0); sendPos(1, 0, tmpF.getCentPos()); break;
                case AROUND: sendBits(2, mFigures[2].size()-1); break;
            }
            
            mPts.clear();
        }
    
    _FAILD_GET_DRAWING:
        if (bError) {
            cout << "[ERROR] 画像の取得に失敗しました" << endl;
        }
        
    //====================
    // 入力画像抽出処理終了
    //====================
        
    } else {
        
        //----------
        // ルーラー
        //----------
        ofPushStyle();
        ofSetColor(40);
        for (int i=0; i < ofGetWidth(); i += 10){
            if (i%100 == 0) {
                ofSetLineWidth(2);
                ofLine(i, 0, i, ofGetHeight());
            } else {
                ofSetLineWidth(1);
                ofLine(i, 0, i, ofGetHeight());
            }
        }
        for (int i=0; i < ofGetHeight(); i += 10){
            if (i%100 == 0) {
                ofSetLineWidth(2);
                ofLine(0, i, ofGetWidth(), i);
            } else {
                ofSetLineWidth(1);
                ofLine(0, i, ofGetWidth(), i);
            }
        }
        
        //描画中
        if (bDrawing) {
            ofSetLineWidth(1);
            ofSetColor(255);
            for (int i=0; i < ofGetWidth(); i += 10){
                if (i%100 == 0)
                    ofLine(i, 0, i, 15);
                else
                    ofLine(i, 0, i, 10);
            }
            for (int i=0; i < ofGetHeight(); i += 10){
                if (i%100 == 0)
                    ofLine(0, i, 15, i);
                else
                    ofLine(0, i, 10, i);
            }
            ofSetColor(120, 150, 120);
            ofLine(mouseX, 0, mouseX, ofGetHeight());
            ofLine(0, mouseY, ofGetWidth(), mouseY);
            ofSetColor(220);
            ofDrawBitmapString(ofToString(mouseX, 2), mouseX+10, 25);
            ofDrawBitmapString(ofToString(mouseY, 2), 15, mouseY+10);
        }
        ofPopStyle();
        
        //----------
        // 形態を描画
        //----------
        for (int i=0; i < mFigures.size(); i++) {
            for (int j=0; j < mFigures[i].size(); j++) {
                mFigures[i][j].draw();
                if (bDebugMode) {
                    mFigures[i][j].debugDraw();
                }
            }
        }
        
        if (bCircleMode) {
            //----------
            // 円モードの場合は円を描画
            //----------
            if( mPts.size() > 0 ) {
                float size = ofDist(mPts[0].x, mPts[0].y, mPts[mPts.size()-1].x, mPts[mPts.size()-1].y);
                mVecOut.circle(mPts[0].x, mPts[0].y, size);
            }
        } else {            
            //----------
            // 自由線画像を描画
            //----------
            if( mPts.size() > 0 ) {
                int numPts = mPts.size();
                int rescaleRes = 2;
                mVecOut.noFill();
                mVecOut.beginShape();
                for(int i = 0; i < numPts; i++){
                    if(i == 0 || i == numPts -1){
                        mVecOut.curveVertex(mPts[i].x, mPts[i].y);
                    }
                    if(i % rescaleRes == 0) mVecOut.curveVertex(mPts[i].x, mPts[i].y);
                }
                mVecOut.endShape();
                
                //もし開始点に近い場合はマークを表示
                if (ofDist(mPts[0].x, mPts[0].y, mouseX, mouseY) < 20 && mPts.size() > 10) {
                    ofPushStyle();
                    ofSetColor(255, 0, 0);
                    ofNoFill();
                    ofSetLineWidth(2);
                    ofCircle(mPts[0], 20);
                    ofPopStyle();
                }
            }
        }
    }
    
    if (bDebugMode) {
        debugDraw();
    }
}

/**
 デバッグ表示
 */
void testApp::debugDraw()
{
    if (!bDrawing) {
        ofPushStyle();
        ofEnableAlphaBlending();
        stringstream str;
        
        ofSetLineWidth(1);
        ofSetColor(0, 255, 0, 255);
        ofNoFill();
        
        ofSetColor(255, 255, 0, 255);
        ofNoFill();
        
        //波形を描画
        ofSetColor(127, 127, 255);
        ofSetLineWidth(1);
        ofPushMatrix();
        ofTranslate(ofGetWidth()-266-110-210, 10);
        ofPoint tPos = ofPoint(0, 100/2);
        for (int i=0; i < mEdgeBits.size(); i++){
            float j = ofMap(i, 0, mEdgeBits.size()-1, 0, 200);
            ofPoint pos = ofPoint(j, mEdgeBits[i] * 50 + (100/2));
            ofLine(tPos, pos);
            tPos.set(pos);
            if (i == mEdgeBits.size()-1) {
                ofLine(tPos.x, tPos.y, 200, 100/2);
            }
        }
        ofSetColor(255);
        ofRect(0, 0, 200, 100);
        ofPopMatrix();
        
        str << "display width   : " << ofGetWidth() << endl
        << "display height  : " << ofGetHeight() << endl
        << "Points Interval : " << mInterval << endl
        << "Seq num : " << mSeqCurrentNum << endl
        ;
        if (bCircleMode) str << "Circle Mode ON" << endl;
        ofSetColor(255);
        ofDrawBitmapString(str.str(), 10, 15);
        
        ofDisableAlphaBlending();
        ofPopStyle();
        
        //---------
        // 入力音の波形を描画
        //----------
        //draw wave
        ofPushStyle();
        ofNoFill();
        ofPushMatrix();
        ofTranslate(ofGetWidth()-266, 10, 0);
        
        ofSetColor(225);
        ofDrawBitmapString("Audio Input", 4, 18);
        
        ofSetLineWidth(1);
        ofRect(0, 0, 256, 100);
        
        ofSetColor(180);
        ofSetLineWidth(2);
        
        ofBeginShape();
        for (int i = 0; i < mLefts.size(); i++){
            ofVertex(i, 50 -mLefts[i]*180.0f);
        }
        ofEndShape(false);
        
        ofPopMatrix();
        ofPopStyle();
        ofNoFill();
        
        // draw the average volume:
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(ofGetWidth()-266-110, 10, 0);
        
        ofSetColor(225);
        ofDrawBitmapString("Peak: " + ofToString(mScaledVol * 100.0, 0), 4, 18);
        ofRect(0, 0, 100, 100);
        
        ofSetColor(180);
        ofFill();
        ofCircle(50, 50, mScaledVol * 100.0f);
        
        ofPopMatrix();
        ofPopStyle();
        
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_LEFT:
            mInterval--;
            mInterval = MAX(mInterval, 1);
            break;
            
        case OF_KEY_RIGHT:
            mInterval++;
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case ' ':
            bDebugMode = !bDebugMode;
            break;
            
        case 'c':
            bCircleMode = !bCircleMode;
            break;
            
        case OF_KEY_BACKSPACE:
        case OF_KEY_DEL:
            switch (mFMode) {
                case STATIC:
                    if (mFigures[0].size()) {
                        sendSet(mFigures[0].back().getModeID(), mFigures[0].back().getID());
                        mFigures[0].pop_back();
                    }
                    break;
                case FLORTING:
                    if (mFigures[1].size()) {
                        sendSet(mFigures[1].back().getModeID(), mFigures[1].back().getID());
                        mFigures[1].pop_back();
                    }
                    break;
                case AROUND:
                    if (mFigures[2].size()) {
                        sendSet(mFigures[2].back().getModeID(), mFigures[2].back().getID());
                        mFigures[2].pop_back();
                    }
                    break;
            }
            break;
        
        // set mode
        case '1':mFMode = STATIC;break;
        case '2':mFMode = FLORTING;break;
        case '3':mFMode = AROUND;break;


    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    //----------
    // 自由線を描画
    //----------
    mPts.push_back(ofPoint());
	int last = mPts.size()-1;
	mPts[last].x = x;
	mPts[last].y = y;
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //----------
    // 自由線を描画
    //----------
    mPts.clear();
    mPts.push_back(ofPoint());
	mPts[0].x = x;
	mPts[0].y = y;
    bDrawing = true;
    
    //既に計算した輪郭座標配列は削除
    if (mEdgeBits.size()) mEdgeBits.clear();

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bDrawing = false;
    
    //もし開始点と遠ければ図形を描かない
    if (ofDist(mPts[0].x, mPts[0].y, mouseX, mouseY) < 20 || bCircleMode) {
        bProcessGetDraw = true;
    } else {
        mPts.clear();
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::audioIn(float * input, int bufferSize, int nChannels){
	
	float curVol = 0.0;
    int numCounted = 0;
    
    for (int i = 0; i < bufferSize; i++){
		mLefts[i]  = input[i*2]*0.5;
		mRights[i] = input[i*2+1]*0.5;
        
		curVol += mLefts[i] * mLefts[i];
		curVol += mRights[i] * mRights[i];
		numCounted+=2;
	}
	
	curVol /= (float)numCounted;
    curVol = sqrt( curVol );
	
	mSmoothedVol *= 0.93;
	mSmoothedVol += 0.07 * curVol;
	
	mBufferCounter++;
}

//--------------------------------------------------------------

/**
 フーリエ記述子などの計算
 */
void testApp::calcFourier()
{
    //既に計算した輪郭座標配列は削除
    mEdgeBits.clear();
    mResizedPts.clear();
    
    //周波数変換
    if (mGrabImage.isAllocated()) {
        ofPixels px;
        ofPoint tPos;
        
        px.allocate(mGrabImage.width, mGrabImage.height, 1);
        
        //----------
        // 入力画像から２値画像を生成
        //----------
        mGrabImage.setImageType(OF_IMAGE_GRAYSCALE);
        px.set(0);
        for (int i=0; i < (mGrabImage.width * mGrabImage.height); i++) {
            if (mGrabImage.getPixels()[i] > 0) {
                px.getPixels()[i] = 255;
            } else {
                px.getPixels()[i] = 0;
            }
        }
        
        //----------
        // ２値画像を輪郭追跡メソッドに引き渡して描く輪郭点を取得
        //----------
        vector<ofPoint> pts = getContourPoints(px);
        if (pts.empty()) return;
        
        //----------
        // 指定した間隔毎のポイントから開始点との角度を取得
        //----------
        //開始点の0を追加
        mEdgeBits.push_back(0.0f);
        tPos.set(pts[0]);
        
        //一周分指定した間隔で前角度を計算
        for (int i=0; i < pts.size(); i++) {
            double dstWave;
            //開始点との（輪郭線上の）距離を計算
            //もし間隔が一定以上出ない場合はその点を除外
            double tDist = ofDist(tPos.x, tPos.y, pts[i].x, pts[i].y);
            if (tDist < mInterval) continue;
            //角度を計算
            double rnd = atan2((double)(pts[i].y - tPos.y),(double)(pts[i].x - tPos.x));
            dstWave = sin(rnd);
            //波形ビット配列に代入
            mEdgeBits.push_back(dstWave);
            //値を保持
            tPos = pts[i];
            
            //描画線座標配列からインターバルをとった座標配列にセット
            mResizedPts.push_back(pts[i]);
        }
        
        //debug print
//        if (mEdgeBits.size()) {
//            cout << "size" << mEdgeBits.size() << endl;
//            for (int i=0; i < mEdgeBits.size(); i++) cout << mEdgeBits[i].dist << ", " << mEdgeBits[i].bit << endl;
//            cout << "\n\n";
//        }
    }
}

/**
 ２値画像から輪郭追跡を実行し、各輪郭点を順番に格納した配列を返します.
 @param src 入力画像ピクセル配列
 */
vector<ofPoint> getContourPoints(const ofPixels src)
{
    
    //参考: http://homepage2.nifty.com/tsugu/sotuken/binedge/#detailed
    //格子の開始点(vec)
    //  _____
    // |0 7 6|
    // |1 p 5|
    // |2 3 4|
    //  -----
    
    vector<ofPoint> dstPts;
    int w = src.getWidth();
    int h = src.getHeight();
    
    if (src.getNumChannels() != 1) return dstPts;
    
    //画像内を捜査し有効画素を探す
    for(int i=0; i < w * h; i++)
    {
        
        //捜査画素が端なら何もしない
        if (isSide(i, w, h)) continue;
        
        //有効画素があった場合は追跡処理の開始
        if( src.getPixels()[i] != 0 ) {
            int p = i;      //注目画素index
            int tp = 0;     //追跡画素index
            int vec = 2;    //最初の調査点を左下にセットする
            bool isFirst = true;
            
            dstPts.push_back(convertIndex2Points(p, w));
            
            //追跡開始点と追跡点が同じ座標なるまで輪郭追跡処理
            while( p != i || isFirst ) {
                switch(vec) {
                    case 0:    //左上を調査
                        tp = p - w - 1;
                        //追跡画素が端っこでなければ調査開始、端っこなら次の画素を追跡
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                vec = 6;
                                break;
                            }
                        }
                    case 1:    //左を調査
                        tp = p - 1;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                vec = 0;
                                break;
                            }
                        }
                    case 2:    //左下を調査
                        tp = p + w - 1;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                isFirst = false;
                                vec = 0;
                                break;
                            }
                        }
                    case 3:    //下を調査
                        tp = p + w;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                isFirst = false;
                                vec = 2;
                                break;
                            }
                        }
                    case 4:    //右下を調査
                        tp = p + w + 1;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                isFirst = false;
                                vec = 2;
                                break;
                            }
                        }
                    case 5:    //右を調査
                        tp = p + 1;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                isFirst = false;
                                vec = 4;
                                break;
                            }
                        }
                        else {
                            //孤立点であった場合
                            if( isFirst ) {
                                isFirst = false;
                                break;
                            }
                        }
                    case 6:    //右上を調査
                        tp = p - w + 1;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                vec = 4;
                                break;
                            }
                        }
                    case 7:    //上を調査
                        tp = p - w;
                        if (!isSide(tp, w, h)) {
                            if( src.getPixels()[tp] != 0 ) {
                                p = tp;
                                dstPts.push_back(convertIndex2Points(p, w));
                                vec = 6;
                                break;
                            }
                        }
                        vec = 0;
                }
            }
            return dstPts; //追跡終了
        }
    }
    cout << "[ERROR] 有効画素が見つかりません" << endl;
    return dstPts;
}

/**
 画素のインデックス値をofPointに変換します
 @param i
 @return ofPoint
 */
ofPoint convertIndex2Points(int i, int w)
{
    int x = i % w;
    int y = (int) i / w;
    return ofPoint(x, y);
}

/**
 画素が上下左右の端にあるかどうかチェックします
 @param i
 @param w
 @param h
 @return 端だった場合は真
 */
bool isSide(int i, int w, int h)
{
    if (i % w <= 0 || i % w == w-1) return true;
    if ((int)(i / w) <= 0 || (int)(i / w) == h-1) return true;
    return false;
}

/**
 モードIDを送信
 @param modeId モードID
 */
void testApp::sendModeId(const int modeId)
{
    ofxOscMessage m;
    m.setAddress("/mode");
    m.addIntArg(modeId);
    sender.sendMessage(m);
}

/**
 IDを送信
 @param figID ID
 */
void testApp::sendFigId(const int figID)
{
    ofxOscMessage m;
    m.setAddress("/id");
    m.addIntArg(figID);
    sender.sendMessage(m);
}

/**
 初期化命令を送信
 @param modeId モードID
 @param figId 図形ID
 */
void testApp::sendSet(const int modeId, const int figId)
{
    ofxOscMessage m;
    m.setAddress("/bitset");
    m.addIntArg(modeId);
    m.addIntArg(figId);
    sender.sendMessage(m);
    m.clear();
    m.setAddress("/set");
    m.addStringArg("bang");
    sender.sendMessage(m);
}

/**
 全ビットを送信
 @param modeId モードID
 @param figId 図形ID
 */
void testApp::sendBits(const int modeId, const int figId)
{
    if (mEdgeBits.size()) {
        ofxOscMessage m;
        m.setAddress("/bitset");
        m.addIntArg(modeId);
        m.addIntArg(figId);
        sender.sendMessage(m);
        
        double tBit = 0;
        for (int i=0; i < 255; i++) {
            m.clear();
            m.setAddress("/bits");
            int j = (int)(ofMap(i, 0, 255, 0, mEdgeBits.size()));
            m.addFloatArg((float)mEdgeBits[j]);
            m.addIntArg(i);
            sender.sendMessage(m);
        }
    }
}

void testApp::sendPos(const int modeId, const int figId, const ofPoint pos)
{
    ofxOscMessage m;
    m.setAddress("/bitset");
    m.addIntArg(modeId);
    m.addIntArg(figId);
    sender.sendMessage(m);
    m.clear();
    m.setAddress("/pos");
    m.addFloatArg(pos.x/ofGetWidth());
    m.addFloatArg(pos.y/ofGetHeight());
    sender.sendMessage(m);
}
