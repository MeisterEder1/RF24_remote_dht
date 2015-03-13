#############################################################################
#
# Makefile for librf24 examples on Raspberry Pi
#
# License: GPL (General Public License)
# Author:  gnulnulf <arco@appeltaart.mine.nu>
# Date:    2013/02/07 (version 1.0)
#
# Description:
# ------------
# use make all and make install to install the examples
# You can change the install directory by editing the prefix line
#
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
#CCFLAGS=-Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
# The recommended compiler flags for the Raspberry Pi 2
CCFLAGS=-Ofast -march=armv7-a -mtune=cortex-a7 -mfpu=neon -mfloat-abi=hard

# define all programs
CC=g++
#LDLIBS  = -lrf24-bcm -lwiringPi -lpthread -lm -lmysqlclient -lz -lrt -ldl
LDLIBS  = -lrf24-bcm -lwiringPi -lmysqlclient
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
