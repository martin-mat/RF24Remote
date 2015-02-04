#include <SPI.h>
#include <RF24Serial.h>
#include <RF24Remote.h>
#include <RF24.h>

void setup() {
  // put your setup code here, to run once:
  rf24serial.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rf24serial.update();
}
