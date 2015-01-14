# RF24Serial: library/firmware for an arduino-based device with nRF24l01 for connection with RF24Frontend using Serial connection

* ATmega controller/Arduino with this firmware can be connected to a box with RF24Frontend application via Serial port.
* firmware written as an Arduino library
* called from a simple sketch:
```
#include <SPI.h>
#include <RF24Serial.h>
#include <RF24Remote.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24serial.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24serial.update();
}
```
* it's possible to extend the sketch with additional functionality so it can do something more than proxying RF24 calls (be aware that usage of blocking calls will impact RF24Frontend behaviour, better not to use such calls - delays etc)

##Preconditions
* Serial connection to an ATmega/Arduino using HardwareSerial
* nRF24l01 module connected with ATmega/Arduino as usual for RF24

##Installation
* download complete RF24 repository
* move/link RF24 directory to Arduino library directory (for now it's a must to use FR24 from this repository, not any different)
* move/link RF24Serial and RF24Remote directories to Arduino library directory so they're on the same level as RF24 library
* enter RF24Serial directory
* edit Makefile.config and insert right values for CE and CS pins used for nRF24l01
* make
* in Arduino environment, load RF24Serial/examples/SerialDevice/SerialDevice.ino sketch, compile, upload to the controller
* Follow instructions for RF24Frontend installation

##Usage
* once correctly set up, RF24Frontend/examples can be run
* you can re-use your RF24 sketches to run with RF24Frontend
