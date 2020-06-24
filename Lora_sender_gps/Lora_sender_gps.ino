#include "heltec.h"
#include "images.h"
#include "SoftwareSerial.h"
#include "TinyGPS.h"
#include "Polygon.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6
#define INTERVAL 500
long lastSendTime = 0;

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

float lat = -26.87900;
float lng = -48.64850;

const int RX_PIN = 22;
const int TX_PIN = 23;

TinyGPS gps;
//SoftwareSerial ss(S1RX, S1TX);
Poly poly;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup(){
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, false /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();

  delay(1500);
  //ss.begin(9600);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop(){
  //double rotaLat[15] = {-26.8804774, -26.8802264,-26.8800404,-26.8795604,-26.8794234,-26.8788334,-26.8788334,-26.8787324,-26.8785328,-26.8784566,-26.878529,-26.8786696,-26.8787806,-26.8789306,-26.8791296};
  //double rotaLng[15] = {-48.6501205,-48.6503495,-48.6504615,-48.6505095,-48.6506125,-48.6507885,-48.6505155,-48.6503335,-48.6499883,-48.6496129,-48.648788,-48.6492979,-48.6491969,-48.6491089,-48.6490519};

  //if (counter == 14) {
  //  counter = 0;
  //}
  unsigned long age;
  bool newData = false;
  Serial.println(counter);
  
  if (millis() - lastSendTime > INTERVAL){
    lastSendTime = millis();
    while (Serial2.available()){
      char c = Serial2.read();
      if (gps.encode(c))
        newData = true;
      Serial.println(c);
    }
  }

  if (newData)
    gps.f_get_position(&lat, &lng, &age);
  
  
  if (poly.IsPointInPolygon(lat, lng)) {
    onOffLed();
    printDisplay("AREA PROIBIDA");
  } else {
    printDisplay("");
  }
      
  sendMessage();  
}

String valueOf(float values){
  char s[9];

  dtostrf(values, 8, 4, s);
  return String(s);
}

String getMessage(){
  String result = "m3,"+ valueOf(lat) + "/" + valueOf(lng);

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
  Heltec.display->drawString(30,20, valueOf(lat));
  Heltec.display->drawString(0,30, "Long");
  Heltec.display->drawString(30,30, valueOf(lng));
  Heltec.display->drawString(40,40, message);
    
  Heltec.display->display();
}

void sendMessage(){
  LoRa.beginPacket();  
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(getMessage());
  LoRa.endPacket();

  counter++;
}

void onOffLed(){
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(LED, LOW); // turn the LED off by making the voltage LOW
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(LED, LOW); // turn the LED off by making the voltage LOW
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);
  digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
  
}
