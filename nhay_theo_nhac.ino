#include <ESP8266WiFi.h>               // thu vien mang cho esp8266 
#include <WiFiClient.h>                  // 
#include <ESP8266WebServer.h>            // webserver 
#include <ESP8266mDNS.h>               // thu vien dinh nghia ten mien
#include <ESP8266HTTPUpdateServer.h>  // thu vien update
#include <DNSServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define NUMPIXELS 18
#define PIN        D1 // On Trinket or Gemma, suggest changing this to 1
#define tdelay 30
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const char* ssid = "Dịu";
const char* password = "23091998";

const char* host = "boot";
const char* updatePath = "/update";
const char* updateUsername = "";     //password
const char* updatePassword = "";      // password
IPAddress local_IP(192, 168, 0, 201);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
WiFiServer wifiServer(88);
int ff = 0;
int nguong  = 10;
int mod = 0;
int addr = 0;
int stat = 1;
unsigned char R[3] = {200, 31, 86};
unsigned char G[3] = {0, 255, 47};
unsigned char B[3] = {150, 165, 209};
int curl = 8;
int curr = 9;
void intro() {

  pixels.clear(); // Set all pixel colors to 'off'
  int j = NUMPIXELS ;
  while (j > 0) {
    for (int i = 0; i < j; i++) {
      if (i - 1 >= 0) {
        pixels.setPixelColor(i - 1, pixels.Color(0, 0, 0));
      }
      pixels.setPixelColor(i, pixels.Color(R[mod], G[mod], B[mod]));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(tdelay);
    }
    j--;
  }
  Serial.println("run back");
  delay(100);
  while (j <= NUMPIXELS) {
    for (int i = j - 1; i >= 0 ; i--) {
      if (i - 1 >= 0) {
        pixels.setPixelColor(i - 1, pixels.Color(R[mod], G[mod], B[mod]));
      }
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));

      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(tdelay);
    }
    j++;
  }
}

void wifiSuccess() {
  int i = 9;
  int j = 8;
  for (int t = 0 ; t <= 9; t++) {
    if (i + t < NUMPIXELS && j - t >= 0) {
      pixels.setPixelColor(i + t,  pixels.Color(R[mod], G[mod], B[mod]));
      pixels.setPixelColor(j - t, pixels.Color(R[mod], G[mod], B[mod]));
    }

    pixels.setPixelColor(i + t - 1, pixels.Color(0, 0, 0));
    pixels.setPixelColor(j - t + 1, pixels.Color(0, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(100);
  }
}

void effect(int dinh) {
  if (9 + dinh > curr) {
    for(int i = curr+1; i<=9+dinh;i++){
      
      }
  } else {

  }
  curr = 9+dinh; 
  if (8 - dinh > curl) {
    
  } else {
    
  }
  curl = 8-dinh; 
}

void setup() {
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  intro();
  ff = 0;
  EEPROM.begin(512);  //Initialize EEPROM
  char m = EEPROM.read(addr);
  if (m == 'n') {// cogfig wifi
    for (int j = 0 ; j < 5 ; j++) {
      for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...

        // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
        // Here we're using a moderately bright green color:
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));

        pixels.show();   // Send the updated pixel colors to the hardware.
      }
      delay(1000);
      pixels.clear(); // Set all pixel colors to 'off'
      delay(1000);
    }
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.autoConnect("Nguyen Tiem Speaker config");
    EEPROM.write(addr, 'y');
    EEPROM.commit();
  }
  else {// connect to wifi
    int  dem = 0;
    WiFi.mode(WIFI_STA);
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED && dem < 5) {
      delay(1000);
      dem++;
    }
    if (dem >= 5) {// can't connect
      EEPROM.write(addr, 'n');
      EEPROM.commit();
      Serial.println(WiFi.localIP());
    } else {
      WiFi.config(local_IP, gateway, subnet);
      wifiServer.begin();
      ff = 1;
      wifiSuccess();
    }
  }
  // neu khong conect dduoc van thuc hien xuong duoi


}

void loop() {
  if (ff == 0) {
    WiFiClient client = wifiServer.available();
    if (client) {
      Serial.println("new client");
      char intruc ;
      while (client.connected()) {
        while (client.available() > 0) {
          intruc = client.read();
          Serial.write(intruc);
        }

      }
      unsigned long myTime = millis();
      //plus  minus next  pre play
      if (intruc == '0') {
        mod = 0 ;
      }
      if (intruc == '1') {
        mod = 1;
      }
      if (intruc == '2') {
        mod = 3;
      }
      if (intruc == '3') {
        nguong += 10;
      }
      if (intruc == '4') {
        nguong -= 10;
      }

      client.stop();
    }
  }

  if (stat == 1) {
    int do = analogRead(A0) - 9;

  }
}
