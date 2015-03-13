#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "dht11.h"

int combine(int a, int b)
{
    int times = 1;

    while (times <= b)
        times *= 10;

    return a * times + b;
}

unsigned int dht11_read_obj(DHTobj* DHTdata)
{
    int dht11_val[5] = {0,0,0,0,0};

    if(!dht11_read(dht11_val))
        return 0;

    DHTdata->temperature = combine(dht11_val[2], dht11_val[3]);
    DHTdata->humidity = combine(dht11_val[0], dht11_val[1]);
    DHTdata->sensor = 6;
    return 1;
}

unsigned int dht11_read(int* dht11_val)
{
    uint8_t lstState = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    // clear values
    for(i=0; i<5; ++i)
        dht11_val[i] = 0;

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

    // verify cheksum and print the verified data
    if((j >= 40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)))
        return 1;
    else
        return 0;
}
