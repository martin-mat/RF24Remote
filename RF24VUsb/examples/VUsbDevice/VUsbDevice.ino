#include <SPI.h>
#include <RF24VUsb.h>
#include <RF24Remote.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24vusb.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24vusb.update();
}
