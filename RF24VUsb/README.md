# RF24VUsb: library/firmware for an arduino-based device with nRF24l01 for connection with RF24Frontend using [V-USB](http://www.obdev.at/products/vusb/index.html)

* ATmega controller/Arduino with this firmware can be connected to a box with RF24Frontend application via USB port.
* firmware written as an Arduino library
* called from a simple sketch:
```
#include <SPI.h>
#include <RF24VUsb.h>
#include <RF24Remote.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24vusb.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24vusb.update();
}
```
* it's possible to extend the sketch with additional functionality so it can do something more than proxying RF24 calls (but be aware - no blocking calls, no dealys, and also no millis - timer0 interrupt is disabled)

##Preconditions
* USB port connected to an ATmega/Arduino as [described in V-Usb project](http://vusb.wikidot.com/hardware) (you can also use one of V-Usb prototyping boards available)
* nRF24l01 module connected with ATmega/Arduino as usual for RF24

##Installation
* download complete RF24 repository
* move/link RF24 directory to Arduino library directory (for now it's a must to use FR24 from this repository, not any different)
* move/link RF24VUsb and RF24Remote directories to Arduino library directory so they're on the same level as RF24 library
* enter RF24VUsb directory
* edit Makefile.config and insert right values for MCU, V-Usb connection, CE and CS pins used for nRF24l01
* make
* in Arduino environment, load RF24VUsb/examples/VUsbDevice/VUsbDevice.ino sketch, compile, upload to the controller
* Follow instructions for RF24Frontend installation

##Usage
* once correctly set up, RF24Frontend/examples can be run
* you can re-use your RF24 sketches to run with RF24Frontend
