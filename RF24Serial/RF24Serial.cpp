#include "Arduino.h"
#include "RF24Remote.h"
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

void RF24Serial::begin(HardwareSerial* _serial, int speed)
{
    // disable timer 0 overflow interrupt (used for millis)
    buff_pos = 0;
    buff_remaining = 0;
    buff_size = 0;
    command_to_execute = 0;

    serial = _serial;
    //serial->begin(115200);    
    Serial1.begin(115200);
}

void RF24Serial::update(void)
{
    uint8_t seq;
    if (serial == NULL)
        return;


    if (serial->available())
    {
        buffer[0] = serial->read();
        Serial1.println();
        Serial1.println();
        Serial1.print("length:");
        Serial1.println(buffer[0]);
        if (buffer[0] < 4)
            return;
        buff_pos = 1;
        while (buff_pos < buffer[0])
        {
            buffer[buff_pos++] = serial->read();
        }
   
        while(serial->available())
            serial->read();
        Serial1.print("cmd index:");
        Serial1.println(buffer[1]);

        Serial1.print("command:");
        Serial1.println(buffer[2]);
        Serial1.flush();

        seq = buffer[1];
        rf24serial.parse(IPAR, buffer+2);
        rf24serial.executeCommand();
        rf24serial.store(OPAR, buffer+2, &buff_size);
        //buff_size = 4;
        buffer[0] = buff_size+2; // size
        buffer[1] = seq;
        Serial1.print("Sending buffer:");
        for (int i=0; i<buffer[0]; i++)
        {
            Serial1.print(buffer[i], DEC);
            Serial1.print(" ");
        }
        Serial1.println();
        Serial1.println();
        Serial1.flush();

        serial->write(buffer, buffer[0]);
        serial->flush();
    }
}


