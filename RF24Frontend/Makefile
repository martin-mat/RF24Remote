#############################################################################
#
# Makefile for librf24frontend
#
# License: GPL (General Public License)
# Author:  mzfuzzy800@gmail.com
#
# Description:
# ------------
# use make all and mak install to install the library 
# You can change the install directory by editing the LIBDIR line
#
PREFIX=/usr/local

# Library parameters
# where to put the lib
LIBDIR=$(PREFIX)/lib
# lib name 
LIB=librf24frontend
# shared library name
LIBNAME=$(LIB).so.1.0

HEADERS=RF24Com.h RF24ComVUsb.h RF24ComSerial.h RF24Frontend.h RF24_config.h ../RF24Remote/RF24Remote.h ../RF24/RF24.h 

# Where to put the header files
HEADER_DIR=${PREFIX}/include/RF24

# The recommended compiler flags for the Raspberry Pi
CCFLAGS=-I. -I../RF24Remote -I../../RF24 -I../RF24VUsb

# make all
# reinstall the library after each recompilation
all: librf24frontend

# Make the library
librf24frontend: RF24.o RF24Frontend.o RF24Remote.o RF24ComVUsb.o RF24ComSerial.o opendevice.o
	g++ -shared -Wl,-soname,$@.so.1 ${CCFLAGS} -o ${LIBNAME} $^
	
# Library parts
RF24.o: ../../RF24/RF24.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

RF24Frontend.o: RF24Frontend.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

RF24Remote.o: ../RF24Remote/RF24Remote.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

RF24ComVUsb.o: RF24ComVUsb.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

RF24ComSerial.o: RF24ComSerial.cpp
	g++ -Wall -fPIC ${CCFLAGS} -c $^

opendevice.o: opendevice.c
	g++ -Wall -fPIC ${CCFLAGS} -c $^

# clear build files
clean:
	rm -rf *.o ${LIB}.*

install: all install-libs install-headers

# Install the library to LIBPATH
install-libs: 
	@echo "[Installing Libs]"
	@if ( test ! -d $(PREFIX)/lib ) ; then mkdir -p $(PREFIX)/lib ; fi
	@install -m 0755 ${LIBNAME} ${LIBDIR}
	@ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so.1
	@ln -sf ${LIBDIR}/${LIBNAME} ${LIBDIR}/${LIB}.so
	@ldconfig

install-headers:
	@echo "[Installing Headers]"
	@if ( test ! -d ${HEADER_DIR} ) ; then mkdir -p ${HEADER_DIR} ; fi
	@install -m 0644 ${HEADERS} ${HEADER_DIR}
