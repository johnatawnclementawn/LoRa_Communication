/*
  Control an LED Remotely via 2 Heltec LoRa boards: Reciever
  Author Johnathan Clementi
  Date: 2022/03/30

  This project builds on the example showing the Heltec.LoRa sended data in OLED from Aaron Lee
  at Heltec. This project uses 2 Heltec WiFi LoRa 32 V2 boards, one as a transmitter connected
  to two push buttons, and one as a receiver connected to an LED. When buttons A/B are pressed,
  the transmitter LoRa board will send a LoRa packet containing a string based on the button 
  that has been pressed. When this packet is recieved by the receiver, it the packet will be 
  parsed, compared to a predefined string, and the signal sent to an LED will be determined by
  this string comparison.

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

// Output LED pin
// help from https://docs.arduino.cc/tutorials/mkr-wan-1310/lora-button-press
String buttonAPress = "button A pressed"; // When Button A is pressed on the transmitter circuit
                                          // This text will be included in the packet. 
String buttonBPress = "button B pressed"; // When Button B is pressed on the transmitter circuit
                                          // This text will be included in the packet. 

int ledPin = 17; // Pin 17 controls the signal sent to the LED

#define BAND 915E6  //Band 915E6 is for use in N. America, 868E6 is for Europe
String rssi = "RSSI --"; // RSSI stands for Recieved Signal Strength Indication
                         // and indicates the strength of the signal between the LoRa boards
String packSize = "--";
String packet ;

void logo(){// This routine will display the Heltec logo on the OLED display
            // The hexidecimal values are held in the images.h file
            // These values could be edited to display other images
  Heltec.display->clear(); // Clear the display
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits); // interpret the hex values
  Heltec.display->display(); // show the image on the OLED display
}

void LoRaData(){
  Heltec.display->clear(); // Clear the display
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT); // Left justify text
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 15 , "Received "+ packSize + " bytes"); // Indicate how big the packet recieved is
  Heltec.display->drawStringMaxWidth(0 , 26 , 128, packet); // parse the packet size 
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) { // This routine will parse the packet and respond based on its contents
  
  packet ="";  // Start with a blank string for the packet
  packSize = String(packetSize,DEC);  // read packet
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }   // parse packet
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ; // RSSI = Recieved Signal Strength Indication
  LoRaData(); // display the packet information on the OLED
    
  if(packet.equals(buttonAPress)){ // check if the packet contains Button A text
    digitalWrite(ledPin, HIGH); // if button A was pressed, turn the LED on
  }
  if(packet.equals(buttonBPress)){ // check if the packet contains Button A text
    digitalWrite(ledPin, LOW); // if button A was pressed, turn the LED on
  }

}


void setup() {
   pinMode(ledPin, OUTPUT); // initialize pin 17 as the pin output
   digitalWrite(ledPin, HIGH); // initialize LED in On state
   
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init(); // initialze the display
  Heltec.display->flipScreenVertically();  // ensure the correct orientation of the OLED display
  Heltec.display->setFont(ArialMT_Plain_10);
  logo(); // Show the logo for 1.5 seconds
  delay(1500);
  Heltec.display->clear(); // remove the logo from the screen
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!"); // initialize LoRa
  Heltec.display->drawString(0, 10, "Wait for incoming data..."); // Indicate waiting for data
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() { // This routine will wait until a packet is recieved
              // once a packet is recieved, low-level parse then interpret it using cbk function created above
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10); // wait 10 milliseconds after last packet is recieved to start listening again
}
