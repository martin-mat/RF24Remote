#############################################################################
#
# Makefile for RF24Frontend examples
#
# License: GPL (General Public License)
# Author:  mz-fuzzy <mzfuzzy800@gmail.com>
#
# Description:
# ------------
# use make to build the examples
#
prefix := /usr/local

# The recommended compiler flags for the Raspberry Pi
CCFLAGS=
#CCFLAGS=

# define all programs
#PROGRAMS = scanner pingtest gettingstarted
PROGRAMS = pingpair_dyn transfer
SOURCES = ${PROGRAMS:=.cpp}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -I. -I../ -I../../RF24Remote -I../../../RF24 -I../../RF24Serial -I../../RF24VUsb -D_RF24_FRONTEND $@.cpp ../RF24Frontend.cpp ../../RF24Remote/RF24Remote.cpp ../RF24ComVUsb.cpp ../RF24ComSerial.cpp ../opendevice.c -lusb -o $@

clean:
	rm -rf $(PROGRAMS)


