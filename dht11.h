#ifndef _DHT11_H_
#define _DHT11_H_

#define MAX_TIME 85
#define DHT11PIN 8
#define MEAN_WINDOW 8

struct DHTobj
{
    float temperature;
    float humidity;
    uint8_t sensor;
    bool ack;
};

void dht_clear_last(float* last);
bool read_dht11(uint8_t* dht11_dat);
bool dht11_read(uint8_t* dht11_val);
bool dht11_read_obj(DHTobj* DHTdata);

#endif
