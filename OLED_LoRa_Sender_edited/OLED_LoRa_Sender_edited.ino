/*
  Control an LED Remotely via 2 Heltec LoRa boards 
  Author Johnathan Clementi
  Date: 2022/03/30

  This project builds on the example showing the Heltec.LoRa sended data in OLED from Aaron Lee
  at Heltec. This project uses 2 Heltec WiFi LoRa 32 V2 boards, one as a transmitter connected
  to two push buttons, and one as a receiver connected to an LED. When buttons A/B are pressed,
  the transmitter LoRa board will send a LoRa packet containing a string based on the button 
  that has been pressed. When this packet is recieved by the receiver, it the packet will be 
  parsed, compared to 

  A note from Aaron Lee about the functionality of the OLED display:
      The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
      OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
      ware to OLED's reset pin, the low-level signal at least 5ms.
    
      OLED pins to ESP32 GPIOs via this connecthin:
      OLED_SDA -- GPIO4
      OLED_SCL -- GPIO15
      OLED_RST -- GPIO16
      
      by Aaron.Lee from HelTec AutoMation, ChengDu, China
      https://heltec.org
      this project also realess in GitHub:
      https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h" 
#include "images.h"

#define BAND    915E6  //Band 915E6 is for use in N. America, 868E6 is for Europe

// LED handling //
int buttonApin = 2; // We will read the input of Button A at pin 2
int buttonBpin = 17; // We will read the input of Button B at pin 17

unsigned int counter = 0; // The counter counts how many packets have been sent
String rssi = "RSSI --"; // RSSI stands for Recieved Signal Strength Indication
                         // and indicates the strength of the signal between the LoRa boards
String packSize = "--";  
String packet ;


void logo() // This routine will display the Heltec logo on the OLED display
            // The hexidecimal values are held in the images.h file
            // These values could be edited to display other images
{
  Heltec.display->clear(); // Clear the display
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits); // interpret the hex values
  Heltec.display->display(); // show the image on the OLED display
}



void setup()
{

  pinMode(buttonApin, INPUT_PULLUP); // Define ButtonA pin (2) as an Input Pullup for the button 
  pinMode(buttonBpin, INPUT_PULLUP); // Define ButtonB pin (17) as an Input Pullup for the button
   
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init(); // initialize the OLED display
  Heltec.display->flipScreenVertically();  // ensure the correct orientation of the OLED display
  Heltec.display->setFont(ArialMT_Plain_10); 
  logo(); // Show the logo for 1.5 seconds
  delay(1500);
  Heltec.display->clear(); // remove the logo from the screen
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");  // initialize LoRa
  Heltec.display->display();
  delay(1000);
}

void loop()
{
  Heltec.display->clear(); // clear setup text
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); // Left justify text
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: "); // This indicates how many packets have been sent
                                                        // aka the number of times a button has been pressed
  Heltec.display->drawString(90, 0, String(counter));   // show the text for packet incrementing the packet counter
  Heltec.display->display();                            // display that text


  if (digitalRead(buttonApin) == LOW) // check if Button A has been pressed, do nothing if not (HIGH)
    {
      LoRa.beginPacket(); // initialize a packet
      LoRa.setTxPower(5,RF_PACONFIG_PASELECT_PABOOST); // set the signal strength to 5 decibles - useful for debugging
                                                       // lower signal strength = shorter travel of signal
      LoRa.print("button A pressed"); // this is the text that will be included in the packet
      LoRa.endPacket(); // close the packet and send it out
      counter++; // increment the packet counter
      delay(500); // delay half a second before a button and packet can be sent so there is enough time to
                  // finish sending a packet
    }
    
  if (digitalRead(buttonBpin) == LOW)
    {
      LoRa.beginPacket(); // initialize a packet
      LoRa.setTxPower(5,RF_PACONFIG_PASELECT_PABOOST); // set the signal strength to 5 decibles - useful for debugging
                                                       // lower signal strength = shorter travel of signal
      LoRa.print("button B pressed"); // this is the text that will be included in the packet
      LoRa.endPacket();
      counter++;
      delay(500); // delay half a second before a button and packet can be sent so there is enough time to
                  // finish sending a packet
    }
}
