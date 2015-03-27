#############################################################################
#
# Makefile for RF24_remote_dht
#
# License: GPL (General Public License)
# Author:  MeisterEder1
# Date:    2015/05/13
#
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
#CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s

# The recommended compiler flags for the Raspberry Pi 2
CCFLAGS=-Ofast -march=armv7-a -mfpu=neon -mfloat-abi=hard
#-mtune=cortex-a7 -mfpu=neon -mfloat-abi=hard

# define all programs
CC=g++
LDLIBS  = -lrf24-bcm -lmysqlclient -lwiringPi
DEPS = dht11.h
OBJ = ReadSensors.o dht11.o
PROGRAM = ReadSensors

%.o: %.cpp ${DEPS}
	${CC} -c -o $@ $< ${CCFLAGS}

${PROGRAM}: ${OBJ}
	${CC} -o $@ $^ ${CCFLAGS} ${LDLIBS}

clean:
	rm -rf ${PROGRAM}
	rm -f *.o
install:
	/etc/init.d/ReadSensors stop
	cp ${PROGRAM} /usr/bin
	/etc/init.d/ReadSensors start
