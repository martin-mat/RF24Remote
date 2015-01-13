#include <SPI.h>
#include <RF24SerialDevice.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24serial.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24serial.update();
}
