#include <SPI.h>
#include <RF24.h>
#include <RF24Usb.h>
#include <RF24UsbDevice.h>

void setup() {
  // put your setup code here, to run once:
  rf24usb.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24usb.update();
}
