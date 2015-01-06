#ifndef _RF24COMSERIAL_I
#define _RF24COMSERIAL_I

#include "RF24Com"

class RF24ComSerial: public RF24Com
{
    public:
        RF24ComSerial(void) {};
        virtual void initialize(void);
        virtual void sendRequest(uint8_t *buffer);
        virtual void getResponse(uint8_t *buffer);
        virtual void close(void) {};
};


#endif
