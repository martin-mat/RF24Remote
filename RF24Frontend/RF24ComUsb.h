#ifndef _RF24COMUSB_I
#define _RF24COMUSB_I

#include <usb.h>
#include "RF24Com.h"

class RF24ComUsb: public RF24Com
{
    private:
        usb_dev_handle *handle;
    public:
        RF24ComUsb() {};
        virtual void initialize(void);
        virtual void sendRequest(uint8_t *buffer);
        virtual void getResponse(uint8_t *buffer);
        virtual void close(void) {};
};

#endif
