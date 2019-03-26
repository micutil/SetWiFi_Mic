/*
 * Set WiFi with terminal software...
 * by micono
 * 
 * ver 1.0.0 19/3/27
 * 
 * License
 * CC BY 4.0 Micono (http://github.com/micutil/SetWiFi_Mic). 
 * https://creativecommons.org/licenses/by/4.0/
 * 
 */

#include <M5Stack.h>
#include "M5StackUpdater.h"
#include <WiFi.h>
#include <Preferences.h>

int setstep=0;
String apdata[]={ "", "" };

int selmode=0;

void getAp(bool s, bool p) {
  Preferences preferences;
  preferences.begin("wifi-config");
  if(s) apdata[0] = preferences.getString("WIFI_SSID");
  if(p) apdata[1] = preferences.getString("WIFI_PASSWD");
  preferences.end();
}

void putAp(bool s, bool p) {
  Preferences preferences;
  preferences.begin("wifi-config");
  if(s) preferences.putString("WIFI_SSID", apdata[0]);
  if(p) preferences.putString("WIFI_PASSWD", apdata[1]);
  preferences.end();
}

void DrawSetting() {
  setstep=0;
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println("WiFi Current Setting: "); Serial.println("WiFi Current Setting: ");
  getAp(true,true);
  M5.Lcd.print("   SSID: "); M5.Lcd.println(apdata[0]);
  M5.Lcd.print(" PASSWD: "); M5.Lcd.println(apdata[1]);
  Serial.print("   SSID: "); Serial.println(apdata[0]);
  Serial.print(" PASSWD: "); Serial.println(apdata[1]);
  if(apdata[0].length()>0 && apdata[1].length()>0) {
    WiFi.disconnect();
    WiFi.begin(apdata[0].c_str(),apdata[1].c_str());
    int64_t timeout = millis() + 5000;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("."); M5.Lcd.print(".");
      if (timeout - millis() < 0) {
        break;
      }
    }
  }
  M5.Lcd.setCursor(0,16*3);
  M5.Lcd.print("LocalIP: "); M5.Lcd.println(WiFi.localIP());
  
  apdata[0]="";
  apdata[1]="";
  
  M5.Lcd.println("");Serial.println("");
  M5.Lcd.println("Change: skip -> Btn C");Serial.println("Change: skip -> Btn C");
  M5.Lcd.println("input with terminal.");Serial.println("input with terminal.");

  DrawMessage();
}

void DrawMessage() {
  M5.Lcd.setTextSize(2);
  switch(setstep) {
    case 0:
      M5.Lcd.print("   SSID: ");Serial.print("   SSID: ");
      break;
    case 1:
      M5.Lcd.print(" PASSWD: ");Serial.print(" PASSWD: ");
      break;
  }
  M5.Lcd.setTextSize(1);
  M5.Lcd.drawCentreString("Cancel", 70, 220, 2);
  M5.Lcd.drawCentreString("SmartConf", 160, 220, 2);
  M5.Lcd.drawCentreString("Skip", 250, 220, 2);
}

void saveWiFiSetting() {
  //if(apdata[0].length()>0 && apdata[1].length()>0) {
    putAp(true,true);
  //}
}

void wifiScan() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
}

void drawEsptouchURL(int n) {
  bool a=true,b=true;
  int p=0,q=180;
  switch(n) {
    case 0:
      p=90;
      b=false;
      break;
    case 1:
      q=90;
      a=false;
      break;
  }
  M5.Lcd.fillRect(0, 1, 320, 150, 0);
  if(a) M5.Lcd.qrcode("https://itunes.apple.com/jp/app/id1071176700", p, 10, 140, 3);
  if(b) M5.Lcd.qrcode("https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk", q, 10, 140, 5);
  ////M5.Lcd.qrcode("http://www.micutil.com/EspressifApp/EsptouchForAndroid/esptouch.apk", 180, 10, 140, 4);
}

void smartConfigStart() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();
  M5.Lcd.setCursor(0,160);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("Start Esptouch."); 
  //M5.Lcd.setTextSize(1);
  M5.Lcd.println("Input ssid password.");
  M5.Lcd.println("Then, push comfirm."); 
}

void smartConfigClose() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.stopSmartConfig();
  selmode=0;
  DrawSetting();
}

void setup() {
  Serial.begin(115200);
  Serial.print("M5Stack initializing...");
  M5.begin();
  Wire.begin();
  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  M5.Lcd.setTextSize(2);
  
  wifiScan();
  
  DrawSetting();

  
}

void loop() {
  M5.update();

  switch(selmode) {
    case 0:
      if(Serial.available()) {
        M5.Lcd.setTextSize(2);
        char v=(char)Serial.read();
        if(v==0xA || v==0xD) {
          M5.Lcd.println("");//Serial.println("");
          switch(setstep) {
            case 0:
              setstep=1;
              DrawMessage();
              break;
            case 1:
              saveWiFiSetting();
              DrawSetting();
              break;
          }
        } else if(v>=0x20&&v<=0x7F) {
          M5.Lcd.print(v);//Serial.print(v);
          apdata[setstep].concat(v);
        }
      }

      if(M5.BtnA.wasPressed()) {
        setstep=0;
        DrawSetting();
      }
      if(M5.BtnB.wasPressed()) {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setTextSize(1);
        M5.Lcd.drawCentreString("iOS(A)", 70, 220, 2);
        M5.Lcd.drawCentreString("Back(B)", 160, 220, 2);
        M5.Lcd.drawCentreString("Android(C)", 250, 220, 2);
        drawEsptouchURL(2);
        selmode=1;
        smartConfigStart();
      }
      
      if(M5.BtnC.wasPressed()) {
        M5.Lcd.setTextSize(2);
        switch(setstep) {
          case 0:
            getAp(true,false);
            M5.Lcd.setCursor(0,16*7);
            M5.Lcd.print("   SSID: "); M5.Lcd.println(apdata[0]);
            //Serial.println(""); Serial.print("   SSID: "); Serial.println(apdata[0]);
            setstep=1;
            DrawMessage();
            break;
          case 1:
            getAp(false,true);
            M5.Lcd.setCursor(0,16*8);
            M5.Lcd.print(" PASSWD: "); M5.Lcd.println(apdata[1]);
            //Serial.println(""); Serial.print(" PASSWD: "); Serial.println(apdata[1]);
            setstep=0;
            saveWiFiSetting();
            DrawSetting();
            break;
        }
      }
      break;

    case 1://SmartConfig
      if(M5.BtnA.wasPressed()) {
        drawEsptouchURL(0);
      }
      if(M5.BtnB.wasPressed()) {
        smartConfigClose();
      }
      if(M5.BtnC.wasPressed()) {
        drawEsptouchURL(1);
      }

      if(WiFi.smartConfigDone() && WiFi.status() == WL_CONNECTED) {
        //M5.Lcd.setTextSize(1);
        //M5.Lcd.println("Done !");
        //M5.Lcd.println("SSID : " + WiFi.SSID());
        Preferences preferences;
        preferences.begin("wifi-config");
        preferences.putString("WIFI_SSID", WiFi.SSID());
        preferences.putString("WIFI_PASSWD", WiFi.psk());
        preferences.end();
        delay(1000);
        smartConfigClose();
      } else {
        delay(10);
      }      
  }
}
