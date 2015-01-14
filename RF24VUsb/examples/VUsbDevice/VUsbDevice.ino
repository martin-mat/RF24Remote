#include <SPI.h>
#include <RF24VUsb.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24vusb.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24vusb.update();
}
