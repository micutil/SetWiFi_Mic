# SetWiFi_Mic

SetWiFi_Mic is an M5Stack program for setting up WiFi. Record the SSID and password on M5Stack's NVS (non-volatile storage device). SetWiFi_Mic supports calling of [SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater), so please put SetWiFi_Mic.bin file in microSD and load it using SD-Uploader. You can easily install [SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater) by using [M5Burner_Mic](https://github.com/micutil/M5Burner_Mic).<br>
(JP: SetWiFi_MicはWiFi設定するためのM5Stackのプログラム。SSIDとパスワードをM5StackのNVS（不揮発記憶装置）に記録します。[SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater)の呼び出しに対応しています。SetWiFi_MicをmicroSDに入れて、SD-Uploaderでロードしてお使いください。[M5Burner_Mic](https://github.com/micutil/M5Burner_Mic)を使うと[SD-Updater](https://github.com/tobozo/M5Stack-SD-Updater)を簡単にインストールでます。)

## How to enter SSID and Password
### 1, Input with serial monitor or terminal software
When you start SetWiFi_Mic, it will look like the image below.<br>
(JP: SetWiFi_Micを起動すると以下の画像の様な画面になります。)

1. You connect to M5Stack at 115,200 bps with Arduino's serial monitor or common terminal software.<br>(JP: Arduinoのシリアルモニターや、一般的なターミナルソフトで、115200bpsで接続します。)
2. You can enter the SSID and password from the software.<br>(JP: そのソフトからSSIDとパスワードを入力します。)<br>

If you make a mistake, SetWiFi_Mic has no function such as "Delete" or "Backspace". In such a case, please press the A button and input from the beginning again.<br>(JP: なお、入力間違いした場合など、パックスペースなど効きかないので、Aボタンを押して最初から入力し直してください。)

![MacDown logo](images/IMG_9566_2.png)

### 2, Use the WiFi SmartConfig App
M5Stack can do WiFi setting by [SmartConfig](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/network/esp_smartconfig.html). This function is the setting method supported by ESP8266 and ESP32, and can use the same application as the connected WiFi environment of the smartphone using the smartphone application.<br>
(JP: M5Stackは、[SmartConfig](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/network/esp_smartconfig.html)によるWiFi設定ができます。この機能はESP8266やESP32が対応している設定方法で、スマホのアプリを使ってスマホの接続しているWiFi環境と同じ設定を行えるものです。)

1. Press B to enter Smart Config mode.<br>(JP: Bボタンを押すSmartConfigのモードになります。)
2. The image below shows the QR code of the download destination of the app for iOS application and for Android. Also, if you press the A button or C button, only one QR code will be displayed, so if it is difficult to read, please display only one and read it with your smartphone.<br>(JP: 下の画像はiOSアプリ用とAndroid用のアプリのダウンロード先のQRコードが表示されます。またAボタン、Cボタンを押すと、それぞれ一方のQRコードしか表示されませんので、読み取りにくい場合は片方だけ表示させてスマホで読み取ってください。)

![MacDown logo](images/IMG_9565_2.png)

The smartphone app that you install is an application called Esptouch of the company Espressif of the ESP32 which is a microcomputer of M5Stack. Below is the download URL of iOS and Android of that App. It is easy for you to have the App installed.<br>
(JP: インストールしていただくスマホのアプリはM5StackのマイコンであるESP32の会社のEspressifのEsptouchというアプリです。以下のURLからあらかじめインストールしておくと良いかもしれません。)

Esptouch for iOS<br>
[https://itunes.apple.com/jp/app/id1071176700](https://itunes.apple.com/jp/app/id1071176700)

Esptouch for Android<br>
[https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk](https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk)

![MacDown logo](images/IMG_9563_2.png)   ![MacDown logo](images/IMG_9562_2.png)

3. Enter the password of the SSID (the smartphone is connected) in the "Password" field in the Esptouch app<br>
(JP: EsptouchのPasswordの欄にSSIDのパスワードを入力します。)
4. Press the "Confirm" button and wait for a while to complete the settings.<br>(JP: Confirmボタンを押してしばらく待つと設定が完了します。)


## To call the set SSID and Password
You can call the configured SSID and password with the following code (These are the same as WiFi settings of [LovyanLauncher](https://github.com/lovyan03/M5Stack_LovyanLauncher)).<br>
(JP: 設定したSSIDとパスワードは、以下のコードで呼び出すことができます（[LovyanLauncher](https://github.com/lovyan03/M5Stack_LovyanLauncher)のWiFi設定と同じです）。)

~~~
Preferences preferences;<br>
preferences.begin("wifi-config");<br>
mySSID = preferences.getString("WIFI_SSID");<br>
myPSWD = preferences.getString("WIFI_PASSWD");<br>
preferences.end();<br>
~~~

## License

[CC BY 4.0](https://creativecommons.org/licenses/by/4.0/) Micono (http://github.com/micutil/SetWiFi_Mic).



