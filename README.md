# SetWiFi_Mic

SetWiFi_Mic is an M5Stack program for setting up WiFi. Record the SSID and password on M5Stack's NVS (non-volatile storage device). SetWiFi_Mic supports calling of [SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater), so please put SetWiFi_Mic.bin file in microSD and load it using SD-Uploader. You can easily install [SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater) by using [M5Burner_Mic](https://github.com/micutil/M5Burner_Mic).

SetWiFi_MicはWiFi設定するためのM5Stackのプログラム。SSIDとパスワードをM5StackのNVS（不揮発記憶装置）に記録します。[SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater)の呼び出しに対応しています。SetWiFi_MicをmicroSDに入れて、SD-Uploaderでロードしてお使いください。[M5Burner_Mic](https://github.com/micutil/M5Burner_Mic)を使うと[SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater)を簡単にインストールでます。

## How to enter SSID and Password
### 1, Input with serial monitor or terminal software
SetWiFi_Micを起動すると以下の画像の様な画面になります。Arduinoのシリアルモニターや、一般的なターミナルソフトで、115200bpsで接続して、SSIDとパスワードを入力できます。
なお、入力間違いした場合など、パックスペースなど効きかないので、Aボタンを押して最初から入力し直してください。

![MacDown logo](images/IMG_9566_2.png)

### 2, Use the WiFi SmartConfig App
M5Stackは、[SmartConfig](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/network/esp_smartconfig.html)によるWiFi設定ができます。この機能はESP8266やESP32が対応している設定方法で、スマホのアプリを使ってスマホの接続しているWiFi環境と同じ設定を行えるものです。

1. Bボタンを押すSmartConfigのモードになります。
2. 画面はiOSアプリ用とAndroid用のアプリのダウンロード先のQRコードが表示されます。またAボタン、Cボタンを押すと、それぞれ一方のQRコードしか表示されませんので、読み取りにくい場合は片方だけ表示させてスマホで読み取ってください。

![MacDown logo](images/IMG_9565_2.png)

インストールしていただくスマホのアプリはM5StackのマイコンであるESP32の会社のEspressifのEsptouchとうアプリです。

iOS用 Esptouch
[https://itunes.apple.com/jp/app/id1071176700](https://itunes.apple.com/jp/app/id1071176700)

Android用 Esptouch
[https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk](https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk)

![MacDown logo](images/IMG_9563_2.png)   ![MacDown logo](images/IMG_9562_2.png)

3. EsptouchのPasswordの欄にSSIDのパスワードを入力します。
4. Confirmボタンを押してしばらく待つと設定が完了します。


## To call the set SSID and Password
設定したSSIDとパスワードは、以下のコードで呼び出すことができます（[LovyanLauncher](https://github.com/lovyan03/M5Stack_LovyanLauncher)が設定するものと同じものにしてあります）。

Preferences preferences;<br>
preferences.begin("wifi-config");<br>
mySSID = preferences.getString("WIFI_SSID");<br>
myPSWD = preferences.getString("WIFI_PASSWD");<br>
preferences.end();<br>

## License

[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/) Micono (http://github.com/micutil/SetWiFi_Mic).



