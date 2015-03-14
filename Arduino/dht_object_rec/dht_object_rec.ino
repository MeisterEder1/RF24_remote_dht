
/*
* Getting Started example sketch for nRF24L01+ radios
* This is an example of how to send data from one node to another using data structures
* Updated: Dec 2014 by TMRh20
*/
#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define _USE_SERIAL_

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

byte addresses[][6] = {"1Node","2Node","3Node"};

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);

void displaySensor(byte sensor, float humid, float temp)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Sensor "));
  display.println(sensor);
  display.print(F("RH: "));
  display.println(humid);
  display.print(F("Temp: "));
  display.println(temp);
  display.display();
}

void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("scanning...");
  display.display();  

#ifdef _USE_SERIAL_
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
#endif

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
//  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
  radio.setCRCLength(RF24_CRC_8);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);
  radio.openReadingPipe(2, addresses[2]);
  
  // Start the radio listening for data
  radio.startListening();
}

/**
* Create a data structure for transmitting and receiving data
* This allows many variables to be easily sent and received in a single transmission
* See http://www.cplusplus.com/doc/tutorial/structures/
*/
struct dataStruct{
  float temperature = 0.0;
  float humidity = 0.0;
  byte sensor = 0;
  bool ack = 0;
}DHTdata;

void loop() 
{
/****************** Pong Back Role ***************************/
    if( radio.available())
    {
      while (radio.available()) // While there is data ready
      { 
        radio.read( &DHTdata, sizeof(DHTdata) ); // Get the payload
        #ifdef _USE_SERIAL_
        Serial.println(F("Got response:"));
        Serial.print(F("Sensor: "));
        Serial.println(DHTdata.sensor);
        Serial.print(F("Temperature: "));
        Serial.println(DHTdata.temperature);
        Serial.print(F("Humidity:"));
        Serial.println(DHTdata.humidity);
        #endif
        displaySensor(DHTdata.sensor, DHTdata.humidity, DHTdata.temperature);
      }
     
      radio.stopListening();                          // First, stop listening so we can talk  
      DHTdata.ack = 1;                                // send ack
      radio.write( &DHTdata, sizeof(DHTdata) );       // Send the final one back.      
      radio.startListening();                         // Now, resume listening so we catch the next packets.
      #ifdef _USE_SERIAL_
      Serial.print(F("Sent response "));
      Serial.print(DHTdata.ack);
      #endif
      delay(500);
   }
} // Loop

