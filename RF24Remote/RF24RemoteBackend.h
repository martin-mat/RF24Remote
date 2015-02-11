#ifndef __RF24RemoteBackend_H
#define __RF24RemoteBackend_H

#include "RF24Remote.h"
#include "RF24Remote_config.h"

class RF24RemoteBackend: public RF24Remote
{
public:
    RF24RemoteBackend(void): RF24Remote() {};
    int executeCommand(void);
    uint8_t dumpRegisters(uint8_t *str);

};

#endif


