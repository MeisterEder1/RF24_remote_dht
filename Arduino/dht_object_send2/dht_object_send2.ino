
#include <dht.h>
#include <SPI.h>
#include "RF24.h"
//#define _USE_SERIAL_
#define DHTTYPE 22
#define DELAYTIME 15000

byte addresses[][6] = {"1Node","2Node","3Node"};

/****************** User Config ***************************/
/***      Set this radio as radio number 0, 1, 2       ***/
byte radioNumber = 1;

// dht pin config
dht DHT;
#define DHT_PIN 5

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

void setup() 
{
#ifdef _USE_SERIAL_
  Serial.begin(115200);
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
  radio.openWritingPipe(addresses[radioNumber]);
  radio.openReadingPipe(1, addresses[0]);
  
  // Start the radio listening for data
  radio.startListening();
}

// Create a data structure for transmitting and receiving data
struct dataStruct
{
  float temperature = 0.0;
  float humidity = 0.0;
  byte sensor = radioNumber;
  bool ack = 0;
}DHTdata;

void loop() 
{  
  radio.stopListening();  // First, stop listening so we can talk.
    
  if(DHTTYPE == 22)
    DHT.read22(DHT_PIN);
  else
    DHT.read11(DHT_PIN);    

#ifdef _USE_SERIAL_
    Serial.println(F("Now sending"));
    Serial.println(DHT.temperature);
    Serial.println(DHT.humidity);
#endif
    DHTdata.temperature = (float)DHT.temperature;
    DHTdata.humidity = (float)DHT.humidity;
    DHTdata.sensor = (byte)radioNumber;
    
    if (!radio.write( &DHTdata, sizeof(DHTdata) ))
    {
#ifdef _USE_SERIAL_
      Serial.println(F("failed"));
#endif
    }
        
    radio.startListening(); // Now, continue listening
    
    unsigned long started_waiting_at = micros(); // Set up a timeout period, get the current microseconds
    boolean timeout = false; // Set up a variable to indicate if a response was received or not
    
    while ( !radio.available() )
    { // While nothing is received
      if ((micros() - started_waiting_at) > 200000 )
      {            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout )
    { // Describe the results
#ifdef _USE_SERIAL_
        Serial.println(F("Failed, response timed out."));
#endif
    }
    else
    { // Grab the response, compare, and send to debugging spew
        radio.read( &DHTdata, sizeof(DHTdata) );
#ifdef _USE_SERIAL_
        unsigned long time = micros();
#endif        
        // Spew it
#ifdef _USE_SERIAL_
        Serial.print(F("Sent "));
        Serial.print(time);
        Serial.print(F(", Got response "));
        Serial.println(DHTdata.ack);
#endif
    }

    // Try again 1s later
    delay(DELAYTIME);
} // Loop

