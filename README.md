# 空気のよどみセンサー

![空気のよどみセンサー](https://www.code4susono.org/wp-content/uploads/2022/01/co2_LED01.jpg)

ここでは、[空気のよどみセンサーをつくろう](https://www.code4susono.org/co2sensor)で作成したソースを公開しています。

# 使用しているライブラリ

* [FastLED](https://github.com/FastLED/FastLED)
* [Adafruit CCS811](https://github.com/adafruit/Adafruit_CCS811)

# ビルド手順

## 環境要件

* Windows / Mac に Arduino IDE がインストールされていること
* PCとArduino Nano（互換機）をUSBケーブルで接続し、Arudino IDEで正しく認識できていること

尚、実際に動作を確認をした環境は下記になります。
* Windows 10 pro. : Arduino 1.8.15

## 事前準備

1. Arduino IDEを起動し、[使用しているライブラリ](#使用しているライブラリ)に記載した2つのライブラリをArduino IDEに読み込む
1. 本プロジェクト用のスケッチを保存するフォルダを作成する。
1. 本リポジトリをクローンする  
    ```
    > git clone https://github.com/CodeforSusono/CO2_monitor_arduino.git
    ```
1. CO2_monitor_arduinoフォルダからco2_monitor_arduino.inoファイルをコピーし、本プロジェクト用に作成したフォルダに保存する。

## 本手順

1. Arduino IDEで上記で保存したco2_monitor_arduino.inoを開く。
1. スケッチ>検証・コンパイル（もしくはCTRL+R）を実行する。
1. エラーが発生していないことを確認し、スケッチ>マイコンボードに書き込む（もしくはCTRL+U）を実行する。

上記で正常に書き込めれば、空気よどみセンサーは動作しているはずです。

# 空気よどみセンサーを使用するには

USBケーブルを電源を供給すれば、空気よどみセンサーは使用することができます。

異常が発生した場合（LEDが消灯する、LEDが点滅する、煙がでる…等）は、USBケーブルを電源から外し、使用を中止して下さい。

# LICENSEについて

本ソースはMITライセンスで公開しています。MITライセンスの詳細は[LICENSE](https://github.com/CodeforSusono/CO2_monitor_arduino/blob/a1e77dfeec3c32b7da979c12bdd98637ee2fffd2/LICENSE)をご確認下さい。

このソフトウェアには保証はついていません。このソフトウェアを利用したことで問題が起きた際に、ソフトウェアの製作者は一切の責任を負いません。





