#ifndef _READSENSORS2_H_
#define _READSENSORS2_H_
#define SENSOR_PIN  8
#define CYCLETIME  60
#define RETRIES    3
#define RETRYTIME  3
#define REVISION "3.0 18.01.2015"

// Hardware configuration
// (CE Pin, CSN Pin, SPI Speed)

// Setup for GPIO 22 CE and CE1 CSN with SPI Speed @ 1Mhz
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_1MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 4Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);
//RF24 radio(RPI_V2_GPIO_P1_22, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

// NEW: Setup for RPi B+
RF24 radio(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_24, BCM2835_SPI_SPEED_8MHZ);

// Setup for GPIO 22 CE and CE0 CSN with SPI Speed @ 8Mhz
//RF24 radio(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ);

int main(int argc, char** argv);

uint8_t addresses[][6] = {"1Node","2Node","3Node"};

void exit(int i);

void finish_with_error(MYSQL *con);

#endif
