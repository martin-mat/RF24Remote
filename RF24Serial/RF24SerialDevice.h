#ifndef __RF24SerialDevice__
#define __RF24SerialDevice__
#include "RF24Usb.h"

class RF24SerialDevice: public RF24Usb
{
private:
    HardwareSerial *serial;
public:
    RF24SerialDevice(void);
    void begin(HardwareSerial *_serial=&Serial, int speed=115200);
    void update(void);
    int executeCommand(void);
};

extern RF24SerialDevice rf24serial;

#endif //__RF24UsbDevice__

