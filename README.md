# FourierDescriptorApp [取扱説明書:README]
### Pure Data Japan 1st Session - 2013.05.29 -

FourierDescriptorAppは、openFrameworks（以下oF）とPureData（以下Pd）とをOSCで連携して使用するドローイング&サウンドパフォーマンスツールです。ここに用意したのは、その基本的な機能をもったプロトタイプです。また、開発中は[GitHub](https://github.com/TatsuyaOGth/FourierDescriptorApp)に随時プッシュするので、よかったらそちらもチェックしてみてください。

ちなみに、プログラムにおいて計算が間違ってる可能性は否めませんが、そこはご了承ください^^; また、ソースコードはなるべく丁寧に書いてますが、わりとこういうのは「動いちゃえば良い主義」なので、冗長だったり、goto文とか普通に書いてたりします。
  

## はじめに
* 前提条件として、Xcode（バージョン４以降）とoF（バージョンv0073,v0074で動作確認済）と、Pd（0.43以降）が必要です。無い場合はoFは[ここ](http://www.openframeworks.cc/download/)、Pdは[ここ](http://puredata.info/downloads/pd-extended)からダウンロードしてください。
* あと、oFはMac版（Xcode）のプロジェクトファイルしかありません。Windowsの方すみません…

## 使い方
### 準備
* ZIPファイルを解凍するとFourierDescriptorAppというフォルダがあります。これを、**oFのappsフォルダ直下に置いてください。**こうしないとoFが使えないので。（注意：myAppsフォルダではなく、appsフォルダです）
* フォルダの中にoF_DrawWavesとPd_WaveTablesの２つのフォルダがあります。

#### oF_DrawWavesの起動
* oF_DrawWavesの中のXCodeプロジェクトファイル（DrawWaves.xcodeproj）をXcodeで開いてください。
* とりあえずRunボタンを押して起動してみてください。
* もしエラーが出た場合はプロジェクトの設定でBaseSDKを変更してみてください。デフォルトでは10.8になっているので、OSによっては10.7にする必要があります。
* また、コメントが文字化けする場合は文字コードを変更してください。

#### Pd_WaveTablesの起動
* Pd_WaveTablesのフォルダにあるlive.pdをPdで開いてください。たぶん正常に動きます。　
* もし開けない場合はmain.pdを開いてみてください。live.pdは実際はmain.pdを内部で開いているだけなので、main.pdさえ開ければ問題ありません。
* 音を出すためにPdウィンドウのDSPボタンをONにしてください。これで準備OKです。

## とりあえず使ってみる
かんたんな使い方の動画を用意しました。[こちら](https://sites.google.com/site/tatsuyaogusu/fourierdescriptorapp)を御覧ください。

## 操作方法

#### oF_DrawWavesの操作
* fキー：フルスクリーン表示切り替え
* スペースキー：デバッグ表示モード切り替え
* cキー：サークルモード切り替え。真円を描画したいときはサークルモードにします。
* [←][→]キー：フーリエ記述子の計算に使用する図形の輪郭点の間隔（距離）の設定。デフォルトは30（ユークリッド距離で30ピクセルの間隔になってるはず…）


## その他
* デフォルトのOSC設定は送信側（oF）ホスト=localhost, ポート=50001、受信側（Pd）も50001です。ないとは思いますが、他のPdパッチなどのOSC設定とポートが被らないようにしてください。
* いちおうこのプログラムはCCライセンス（下記）の条件に従う限り自由に使用していただいて構いません。
* ではでは。

<a rel="license" href="http://creativecommons.org/licenses/by/3.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by/3.0/88x31.png" />  
</a>This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/3.0/">Creative Commons Attribution 3.0 Unported License</a>  
Tatsuya Ogusu 2013 | [おぐすん.org](http://ogsn.org) | [@TatsyaOGs](https://twitter.com/TatsuyaOGs)