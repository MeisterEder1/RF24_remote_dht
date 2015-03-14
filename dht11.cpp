#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "dht11.h"

float lastTemp = 0.0;
float lastRH = 0.0;

// combine two binary values to one uint8_t
int combine(int a, int b)
{
    int times = 1;

    while (times <= b)
        times *= 10;

    return a * times + b;
}

// read dht11 object
bool dht11_read_obj(DHTobj* DHTdata)
{
    int dht11_val[5] = {0,0,0,0,0};

    if(!dht11_read(dht11_val))
        return false;

    float temp = combine(dht11_val[2], dht11_val[3]);
    float rh = combine(dht11_val[0], dht11_val[1]);

    if(!lastTemp)
        lastTemp = temp;

    if(!lastRH)
        lastRH = rh;

    temp = (temp + lastTemp) / 2;
    rh = (rh + lastRH) / 2;

    DHTdata->temperature = temp;
    DHTdata->humidity = rh;
    DHTdata->sensor = 6;
    return true;
}

// dht11 read function from http://www.rpiblog.com/2012/11/interfacing-temperature-and-humidity.html
bool dht11_read(int* dht11_val)
{
    uint8_t lstState = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    // clear values
    dht11_val[0] = dht11_val[1] = dht11_val[2] = dht11_val[3] = dht11_val[4] = 0;

    pinMode(DHT11PIN, OUTPUT);
    digitalWrite(DHT11PIN, LOW);
    delay(18);
    digitalWrite(DHT11PIN, HIGH);
    delayMicroseconds(40);
    pinMode(DHT11PIN, INPUT);

    for(i=0; i<MAX_TIME; ++i)
    {
        counter = 0;
        while(digitalRead(DHT11PIN) == lstState)
        {
            ++counter;
            delayMicroseconds(1);

            if(counter == 255)
                break;
        }

        lstState = digitalRead(DHT11PIN);

        if(counter == 255)
            break;

        // top 3 transistions are ignored
        if((i >= 4) && (i % 2 == 0))
        {
            dht11_val[j/8] <<= 1;

            if(counter > 16)
                dht11_val[j/8] |= 1;
            ++j;
        }
    }

    // verify checksum and print verified data
    if((j >= 40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)))
        return true;
    else
        return false;
}
