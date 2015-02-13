#define _RF24_FW
#include "Arduino.h"
#include "RF24RemoteBackend.h"
#include "RF24Serial.h"

uint8_t buffer[256];
uint8_t buff_pos;
uint8_t buff_remaining;
uint8_t buff_size;
uint8_t command_to_execute;

RF24Serial rf24serial;

RF24Serial::RF24Serial(void)
{
    serial = NULL;
};

void RF24Serial::setup(HardwareSerial* _serial, int speed)
{
    buff_pos = 0;
    buff_remaining = 0;
    buff_size = 0;
    command_to_execute = 0;

    serial = _serial;
    serial->begin(115200);    
}

void RF24Serial::update(void)
{
    if (serial == NULL)
        return;


    if (serial->available())
    {
        buffer[0] = serial->read();
        serial->readBytes(buffer+1, buffer[0]-1);
        rf24serial.parse(IPAR, buffer+2);
        rf24serial.executeCommand();
        rf24serial.store(OPAR, buffer+2, &buff_size);
        buffer[0] = buff_size+2;
        serial->write(buffer, buffer[0]);
        serial->flush();
    }
}


