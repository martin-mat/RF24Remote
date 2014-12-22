#ifndef __RF24UsbDevice__
#define __RF24UsbDevice__
#include "RF24Usb.h"

class RF24UsbDevice: public RF24Usb
{
private:
public:
    RF24UsbDevice(void);
    void begin(void);
    void update(void);
    int executeCommand(void);
};

extern RF24UsbDevice rf24usb;

#endif //__RF24UsbDevice__

