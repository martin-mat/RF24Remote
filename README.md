#RF24Remote: remote driver for a nRF24l01 device using RF24 library interface

Goal:
* provide interface for communication via nRF24L01 modules from devices that don't have SPI/GPIO - typically a PC, linux boxes/development boards
* keep (almost) the same interface as RF24 library so applications can be reused
* libraries utilizing RF24 (RF24Network, RF24Mesh) can use this library as well
* primary developed and tested on Linux, but should be possible to adapt for other OSes

Supported communication interfaces:
* [VUsb](http://www.obdev.at/products/vusb/index.html)
* [Serial](http://en.wikipedia.org/wiki/Serial_port)

Components:
* RF24Remote - common library
* RF24VUsb - library + examples for an arduino device connected via VUsb
* RF24Serial - library + examples for an arduino device connected via serial port
* RF24Frontend - library + examples for driving nRF24l01 device remotely via specified interface

Installation:
1. Prepare a device which will drive nRF24l01 module, either via VUsb or Serial.
  * for Serial:
    * an Arduino board with nRF24l01 module attached, and connected via serial port to a frontend machine
  * for VUsb:
    * an Arduino board with nRF24l01 module attached, and with USB connected as [described in V-Usb project](http://vusb.wikidot.com/hardware)
    * one of [VUsb prototyping boards](http://vusb.wikidot.com/hardware) with nRF24l01 attached
    * nRF24L01 USB stick is in preparation as a part of this project

2. Download and prepare Arduino libraries for backend
  * clone this repository or download as a package
  * in RF24VUsb,  edit Makefile.config and fill configuration parameters accordingly to your VUsb and nRF24l01 setup; execute ```make``` afterwards
  * in RF24Serial, exit Makefile.config and fill configuration parameters accordingly to your nRF24l01 connection; execute ```make``` afterwards
  * link or copy RF24Remote, RF24VUsb and RF24Serial into Arduino's library directory
  * in Arduino GUI, load example for RF24Serial (SerialDevice) or RF24VUsb (VUsbDevice)
  * compile, upload

3. Compile frontend libraries
  * enter RF24Frontend directory
  * ```make```
  * ```sudo make install```

4. compile & run examples
  * enter RF24Frontend/examples directory
  * ```make```
  * run compiled examples

Usage of RF24Frontend libraries:
* see example files:
* instantiate one of RF24Frontend's communication drivers: RF24ComVUsb or RF24ComSerial
```
RF24ComVUsb rf24_vusbdev;
```
* instantiate RF24Frontend, give the communication driver as constructor's param
```
RF24Frontend radio(rf24_vusbdev);
```
* use the RF24Frontend instance in any code where RF24 library is expected (RF24Frontend is inherited from RF24)
