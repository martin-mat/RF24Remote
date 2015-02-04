
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.

 Added Arduino Due support from https://github.com/mcrosson/
 */
 
 /* spaniakos <spaniakos@gmail.com>
  Added __ARDUINO_X86__ support
*/

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

  /*** USER DEFINES:  ***/  
  //#define FAILURE_HANDLING
  //#define SERIAL_DEBUG
  //#define MINIMAL
  //#define SPI_UART  // Requires library from https://github.com/TMRh20/Sketches/tree/master/SPI_UART
  //#define SOFTSPI   // Requires library from https://github.com/greiman/DigitalIO
  /**********************/
  #define rf24_max(a,b) (a>b?a:b)
  #define rf24_min(a,b) (a<b?a:b)
  #define VIRTUAL virtual
  #define byte uint8_t
  
  #include <stdint.h>
  #include <stdio.h>
  #include <time.h>
  #include <string.h>
  #include <sys/time.h>
  #include <stddef.h>

  // GCC a Arduino Missing
  #define _BV(x) (1<<(x))
  #define pgm_read_word(p) (*(p))
  #define pgm_read_byte(p) (*(p))
  
  //typedef uint16_t prog_uint16_t;
  #define PSTR(x) (x)
  #define printf_P printf
  #define strlen_P strlen
  #define PROGMEM
  #define PRIPSTR "%s"

  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
#define pinMode(pin, mode)
#define digitalWrite(pin, value)
#define delayMicroseconds(us)
#define delay(ms)
#define millis() 0

class SPIClass {
public:
  static byte transfer(byte _data) { return 0; };

  // SPI Configuration methods

  inline static void attachInterrupt() {};
  inline static void detachInterrupt() {}; // Default

  static void begin() {}; // Default
  static void end() {};

  static void setBitOrder(uint8_t) {};
  static void setDataMode(uint8_t) {};
  static void setClockDivider(uint8_t) {};
};
extern SPIClass SPI;
#define _SPI SPI

#endif // __RF24_CONFIG_H__

