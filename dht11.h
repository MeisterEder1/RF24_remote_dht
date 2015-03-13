#ifndef _DHT11_H_
#define _DHT11_H_

#define MAX_TIME 85
#define DHT11PIN 8

struct DHTobj
{
    float temperature;
    float humidity;
    uint8_t sensor;
    bool ack;
};

unsigned int dht11_read(int* dht11_val);
unsigned int dht11_read_obj(DHTobj* DHTdata);

#endif
