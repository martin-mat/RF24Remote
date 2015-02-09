#ifndef __RF24Serial__
#define __RF24Serial__
#define _RF24_FW
#include "RF24Remote.h"

class RF24Serial: public RF24Remote
{
private:
    HardwareSerial *serial;
public:
    RF24Serial(void);
    void setup(HardwareSerial *_serial=&Serial, int speed=115200);
    void update(void);
};

extern RF24Serial rf24serial;

#endif //__RF24Serial__

