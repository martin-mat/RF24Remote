#ifndef __RF24VUsb__
#define __RF24VUsb__

#define _RF24_FW
#include "RF24RemoteBackend.h"

class RF24VUsb: public RF24RemoteBackend
{
private:
public:
    RF24VUsb(void);
    void setup(void);
    void update(void);

    bool write( const void* buf, uint8_t len, const bool multicast );
    bool writeFast( const void* buf, uint8_t len, const bool multicast );
    bool writeBlocking( const void* buf, uint8_t len, uint32_t timeout );
    bool txStandBy();
    bool txStandBy(uint32_t timeout, bool startTx = 0);

};

extern RF24VUsb rf24vusb;

#endif //__RF24VUsb__

