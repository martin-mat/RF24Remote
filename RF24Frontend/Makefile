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
CCFLAGS=
#CCFLAGS=

# define all programs
#PROGRAMS = scanner pingtest gettingstarted
PROGRAMS = pingpair_dyn transfer
SOURCES = ${PROGRAMS:=.cpp}

all: ${PROGRAMS}

${PROGRAMS}: ${SOURCES}
	g++ ${CCFLAGS} -Wall -I../ -I../RF24Usb/ -I. -D_USB_FRONTEND $@.cpp RF24Frontend.cpp ../RF24Usb/RF24Usb.cpp RF24ComUsb.cpp opendevice.c -lusb -o $@

clean:
	rm -rf $(PROGRAMS)

install: all
	test -d $(prefix) || mkdir $(prefix)
	test -d $(prefix)/bin || mkdir $(prefix)/bin
	for prog in $(PROGRAMS); do \
	  install -m 0755 $$prog $(prefix)/bin; \
	done

.PHONY: install
