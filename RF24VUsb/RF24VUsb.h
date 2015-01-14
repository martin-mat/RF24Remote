#ifndef __RF24VUsb__
#define __RF24VUsb__
#include "RF24Remote.h"

class RF24VUsb: public RF24Remote
{
private:
public:
    RF24VUsb(void);
    void begin(void);
    void update(void);
};

extern RF24VUsb rf24vusb;

#endif //__RF24VUsb__

