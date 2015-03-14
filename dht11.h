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
bool read_dht11(int* dht11_dat);
bool dht11_read(int* dht11_val);
bool dht11_read_obj(DHTobj* DHTdata);

#endif
