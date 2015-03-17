#include <cstdlib>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <RF24/RF24.h>
#include <mysql/mysql.h>
#include "dht11.h"
#include "ReadSensors.h"

const char* MYSQLHOST = "localhost";
const char* MYSQLUSER = "root";
const char* MYSQLPASSWD = "password";
const char* MYSQLDB = "Monitoring";

extern float lastTemp[MEAN_WINDOW];
extern float lastRH[MEAN_WINDOW];

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char** argv)
{
    if ( wiringPiSetup() == -1 )
        exit( 1 );

//    piHiPri(55);

    dht_clear_last(lastTemp);
    dht_clear_last(lastRH);

    char SQLstring[64]; // string to send to SQL engine

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
        finish_with_error(con);

    if (mysql_real_connect(con, MYSQLHOST, MYSQLUSER, MYSQLPASSWD, MYSQLDB, 0, NULL, 0) == NULL)
        finish_with_error(con);

    // declare DHTobj
    DHTobj DHTdata;
    DHTobj DHTlocal;

    radio.begin(); // Setup and configure rf radio

    // Set the PA level
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_250KBPS);
//  radio.setAutoAck(1);                     // Ensure autoACK is enabled
    radio.setRetries(2,15);                  // Optionally, increase the delay between retries & # of retries
    radio.setCRCLength(RF24_CRC_8);

    // Open a writing and reading pipe on each radio, with opposite addresses
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
    radio.openReadingPipe(2, addresses[2]);

    // print rf24 info
    radio.printDetails();

    // Start the radio listening for data
    radio.startListening();

    // forever loop
    while (1)
    {
        if( radio.available())
        {
	    // get payload
            radio.read( &DHTdata, sizeof(DHTdata) );

            // get current time
            time_t rawtime;
            struct tm * timeinfo;
            time (&rawtime);
            timeinfo = localtime(&rawtime);

            // print values for logfile
            printf("Time: %s\n", asctime(timeinfo));
            printf("Sensor %d:\n", DHTdata.sensor);
            printf("Temperature: %3.1f\n", DHTdata.temperature);
            printf("Relative Humidity: %3.1f\n\n", DHTdata.humidity);

            // insert values into database
            sprintf(SQLstring,"INSERT INTO TempHumid VALUES(unix_timestamp(now()), %5.1f, %5.1f, %d)", DHTdata.temperature, DHTdata.humidity, DHTdata.sensor);

            if (mysql_query(con, SQLstring))
                finish_with_error(con);

            while(!dht11_read_obj(&DHTlocal))
                usleep(50000);

            if(DHTdata.sensor == 2)
            {
            // print values for logfile
            printf("Time: %s\n", asctime(timeinfo));
            printf("Local Sensor:\n");
            printf("Temperature: %3.1f\n", DHTlocal.temperature);
            printf("Relative Humidity: %3.1f\n\n", DHTlocal.humidity);

            // insert values into database
            sprintf(SQLstring,"INSERT INTO TempHumid VALUES(unix_timestamp(now()), %5.1f, %5.1f, 6)", DHTlocal.temperature, DHTlocal.humidity);

            if (mysql_query(con, SQLstring))
                finish_with_error(con);
            }
            radio.stopListening();                          // First, stop listening so we can talk
            DHTdata.ack = 1;                                // send ack
            radio.write( &DHTdata, sizeof(DHTdata) );       // Send the final one back.
            radio.startListening();                         // Now, resume listening so we catch the next packets.
            //printf("Sent response: ACK\n");
            fflush(stdout);
        } // if
        usleep(50000);
    } // while
} // main
