# RF24Frontend: remote driver for a nRF24l01 device using RF24 library interface

Goal:
* provide interface for communication via nRF24L01 modules from devices that don't have SPI/GPIO - typically a PC, linux boxes/development boards
* keep (almost) the same interface as RF24 library so applications can be reused
* libraries utilizing RF24 (RF24Network, RF24Mesh) can also use RF24Frontend
* primary developed and tested on Linux, but should be possible to adapt for other OSes

Supported communication interfaces:
* [VUsb](http://www.obdev.at/products/vusb/index.html)
* [Serial](http://en.wikipedia.org/wiki/Serial_port)

##Installation
for now no shared libraries, no installation.
* switch do RF24Frontend/examples
* make
* precondition for running examples: prepared VUsb or serial RF24 device, proper firmware uploaded, connected in appropriate way


##Usage
Full RF24 library interface is available via RF24Frontend class.
See RF24Frontend/examples/*.cpp 

* instantiate an interface driver:

```
    RF24ComSerial rf24_serialdev("/dev/ttyACM0");
    or
    RF24ComVUsb rf24_vusbdev;
```

* instantiate RF24Frontend, give the driver as a param:

```
    RF24Frontend radio(rf24_serialdev);
    or
    RF24Frontend radio(rf24_vusbdev);
```

* use the RF24Frontend instance the same way as RF24:

```
    radio.begin();
    radio.setChannel(40);
    ...
    ...
```
