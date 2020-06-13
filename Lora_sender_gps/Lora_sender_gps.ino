#include "heltec.h"
#include "images.h"
#include <TinyGPS++.h>
#include "Polygon.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;
double lat;// "-26.87900";
double lng; // "-48.64850";

const int S1RX = 22;
const int S1TX = 23;

TinyGPSPlus gps;
Poly poly;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup(){
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();

  Serial2.begin(9600,SERIAL_8N1,S1RX,S1TX);
  delay(1500);  
}

void loop(){
  while(Serial2.available())
    gps.encode(Serial2.read());
   
    lat = gps.location.lat();
    lng = gps.location.lng();


  if (poly.IsPointInPolygon(lat, lng)) {
    onOffLed();
    printDisplay("AREA PROIBIDA");
  } else {
    printDisplay("");
  }
      
  sendMessage();  
}

String getMessage(){
  String result = "m3,"+ String(lat)+ "/" + String(lng);

  return result;
}

void printDisplay(String message) {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  Heltec.display->drawString(0, 0, "Eviando pacote");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->drawString(0, 10, "ID: m3");
  Heltec.display->drawString(0,20, "Lat");
  Heltec.display->drawString(30,20, String(lat));
  Heltec.display->drawString(0,30, "Long");
  Heltec.display->drawString(30,30, String(lng));
  Heltec.display->drawString(40,40, message);
    
  Heltec.display->display();
}

void sendMessage(){
  LoRa.beginPacket();  
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(getMessage());
  LoRa.endPacket();

  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
}


void onOffLed(){
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(LED, LOW); // turn the LED off by making the voltage LOW
  delay(100);
}
