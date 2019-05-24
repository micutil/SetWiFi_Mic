/*
 * Set WiFi with terminal software...
 * by micono
 * 
 * ver 1.0.1 19/5/24 Odroid-GO compatible
 * ver 1.0.0 19/3/27
 * 
 * License
 * CC BY 4.0 Micono (http://github.com/micutil/SetWiFi_Mic). 
 * https://creativecommons.org/licenses/by/4.0/
 * 
 */

#include <M5Stack.h>
#include "M5StackUpdater.h"

#ifdef ARDUINO_ODROID_ESP32
  String DevName="ODROID-GO";
  bool abcSel=true;
#else
  String DevName="M5Stack";
  bool abcSel=false;
#endif

#include <WiFi.h>
#include <Preferences.h>

int setstep=0;
String apdata[]={ "", "" };

int selmode=0;
int abcNum=0;
String btnName[2][3]={{" Cancel "," SmartConf "," Skip "},{" iOS "," Back "," Android "}};

void setForeBackColor(int ps) {
  uint16_t fc=TFT_WHITE;
  uint16_t bc=TFT_BLACK;
  if(abcSel) {
    if(ps==abcNum) {
      fc=TFT_BLACK;
      bc=TFT_WHITE;
    }
  }
  M5.Lcd.setTextColor(fc, bc);
}

void drawButtonMessage() {
  M5.Lcd.setTextSize(1);
  setForeBackColor(0);
  M5.Lcd.drawCentreString(btnName[selmode][0], 70, 220, 2);
  setForeBackColor(1);
  M5.Lcd.drawCentreString(btnName[selmode][1], 160, 220, 2);
  setForeBackColor(2);
  M5.Lcd.drawCentreString(btnName[selmode][2], 250, 220, 2);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}


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
  
  drawButtonMessage();
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
  if(b) M5.Lcd.qrcode("https://git.io/fjUSl", q, 10, 140, 3);
  ////M5.Lcd.qrcode("https://github.com/micutil/EsptouchForAndroid/raw/master/releases/apk/esptouch.apk", 180, 10, 140, 4);
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
  M5.begin();
  Serial.print(DevName+" initializing...");
  //M5.Speaker.setVolume(0);

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

  #ifdef ARDUINO_ODROID_ESP32
  int joyXwasPressed=M5.JOY_X.wasAxisPressed();
  if(joyXwasPressed==2) {
    abcNum-=1;if(abcNum<0) abcNum=2;
    drawButtonMessage();
    return;
  } else if(joyXwasPressed==1) {
    abcNum+=1;if(abcNum>2) abcNum=0;
    drawButtonMessage();
    return;
  }
  
  bool btnAwasPressed=M5.BtnA.wasPressed();
  #endif

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

      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==0&&btnAwasPressed) {
      #else
      if(M5.BtnA.wasPressed()) {
      #endif      
        setstep=0;
        DrawSetting();
      }
      
      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==1&&btnAwasPressed) {
      #else
      if(M5.BtnB.wasPressed()) {
      #endif      
        M5.Lcd.fillScreen(TFT_BLACK);
        selmode=1;
        drawButtonMessage();
        drawEsptouchURL(2);
        smartConfigStart();
      }
      
      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==2&&btnAwasPressed) {
      #else
      if(M5.BtnC.wasPressed()) {
      #endif
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
      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==0&&btnAwasPressed) {
      #else
      if(M5.BtnA.wasPressed()) {
      #endif      
        drawEsptouchURL(0);
      }

      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==1&&btnAwasPressed) {
      #else
      if(M5.BtnB.wasPressed()) {
      #endif
        smartConfigClose();
      }

      #ifdef ARDUINO_ODROID_ESP32
      if(abcNum==2&&btnAwasPressed) {
      #else
      if(M5.BtnC.wasPressed()) {
      #endif
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
