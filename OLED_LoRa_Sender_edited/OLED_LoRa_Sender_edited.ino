/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  https://heltec.org
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"
#include "images.h"

#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

// LED handling //
//int ledPin = 23;
int buttonApin = 2;
int buttonBpin = 17;

byte leds = 0;
// // // // // // //

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup()
{

//  pinMode(ledPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);  
  pinMode(buttonBpin, INPUT_PULLUP);
   
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();


  if (digitalRead(buttonApin) == LOW)
    {
      LoRa.beginPacket();
      LoRa.setTxPower(5,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print("button A pressed");
      LoRa.endPacket();
      counter++;
      Serial.print("Sending packet: ");
      Serial.println(counter);
      Serial.println("Turn LED on");
      delay(500);
    }
    
  if (digitalRead(buttonBpin) == LOW)
    {
      LoRa.beginPacket();
      LoRa.setTxPower(5,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print("button B pressed");
      LoRa.endPacket();
      counter++;
      Serial.println("Sending packet: ");
      Serial.println(counter);
      Serial.print("Turn LED off");
      delay(500);
    }
}
