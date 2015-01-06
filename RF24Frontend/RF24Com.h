#ifndef _RF24COM_I
#define _RF24COM_I

#include <stdint.h>

class RF24Com
{
    public:
        RF24Com() {};
        virtual void initialize(void)=0;
        virtual void sendRequest(uint8_t *buffer)=0;
        virtual void getResponse(uint8_t *buffer)=0;
        virtual void close(void)=0;
};

void fatal(int retval, const char* format, ...);

#endif
