#FourierDescriptorApp [取扱説明書:README]
###Pure Data Japan 1st Session - 2013.05.29
©小楠竜也 2013 [@TatsyaOGs](https://twitter.com/TatsuyaOGs)

FourierDescriptorAppは、openFrameworks（以下oF）とPureData（以下Pd）とをOSCで連携して使用するドローイング&サウンドパフォーマンスツールです。ここに用意したのは、その基本的な機能をもったプロトタイプです。また、開発中は[GitHub](https://github.com/TatsuyaOGth/FourierDescriptorApp)に随時プッシュするので、よかったらそちらもチェックしてみてください。
  

##はじめに
* 前提条件として、Xcode（バージョン４以降）とoF（バージョンv0073,v0074で動作確認済）と、Pd（0.43以降）が必要です。無い場合はoFは[ここ](http://www.openframeworks.cc/download/)、Pdは[ここ](http://puredata.info/downloads/pd-extended)からダウンロードしてください。
* あと、oFはMac版（Xcode）のプロジェクトファイルしかありません。Windowsの方すみません…

##使い方
* ZIPファイルを解凍するとFourierDescriptorAppというフォルダがあります。これを、**oFのappsフォルダ直下に置いてください。**こうしないとoFが使えないので。（注意：myAppsフォルダではなく、appsフォルダです）
* フォルダの中にoF_DrawWaveとPd_WaveTablesの２つのフォルダがあります。

###oF_DrawWaveの起動
* oF_DrawWaveの中のXCodeプロジェクトファイル（DrawWaves.xcodeproj）をXcodeで開いてください。
* とりあえずRunボタンを押して起動してみてください。アプリケーションの説明は下記参照。
* もしエラーが出た場合はプロジェクトの設定でBaseSDKを変更してみてください。デフォルトでは10.8になっているので、OSによっては10.7にする必要があります。

###Pd_WaveTablesの起動
* Pd_WaveTablesのフォルダにあるlive.pdをPdで開いてください。たぶん正常に動きます。
* もし開けない場合はmain.pdを開いてみてください。live.pdは実際はmain.pdを内部で開いているだけなので、main.pdさえ開ければ問題ありません。
* 音を出すためにPdウィンドウのDSPボタンをONにしてください。これで準備OKです。

（以下書きかけ 2013.05.21）