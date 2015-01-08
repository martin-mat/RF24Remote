#ifndef _RF24COMSERIAL_I
#define _RF24COMSERIAL_I

#include "RF24Com.h"

class RF24ComSerial: public RF24Com
{
    private:
        int fd;
        char sp[64];
    public:
        RF24ComSerial(const char *ser_port);
        virtual void initialize(void);
        virtual void sendRequest(uint8_t *buffer);
        virtual void getResponse(uint8_t *buffer);
        virtual void close(void) {};
};


#endif
