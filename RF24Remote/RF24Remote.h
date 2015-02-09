#ifndef __RF24Remote_H
#define __RF24Remote_H

#include "RF24.h"

#ifdef _RF24_FW
    #include "RF24Remote_config.h"
#endif

#ifndef RF24_DEVICE_CEPIN
    #define RF24_DEVICE_CEPIN 0
#endif

#ifndef RF24_DEVICE_CSPIN
    #define RF24_DEVICE_CSPIN 0
#endif

#if ((DEBUG!=0) && defined(_RF24_FRONTEND))
    #define DPRINT printf
#else
    #define DPRINT(...)
#endif

// protocol version
// increase every time when the protocol changes
#define RF24REMOTE_PROTOCOL_VERSION 1

#define MAX_PARAMS_TYPE 3
#define MAX_BUFF 64
#define MAX_PARAMS 4

#define IPAR 0
#define OPAR 1

typedef enum
{
    RF24_none=0,
    RF24_bool,
    RF24_uint8,
    RF24_uint16,
    RF24_uint32,
    RF24_uint64,
    RF24_buff,
} ERF24ParamType;

typedef enum
{
    RF24_begin = 0,
    RF24_startListening,
    RF24_stopListening,
    RF24_available,
    RF24_availablePipe,
    RF24_read,
    RF24_write, /* blocking  */
    RF24_writeMulticast,
    RF24_openWritingPipe,
    RF24_openWritingPipe40,
    RF24_openReadingPipe,

    RF24_openReadingPipe40,
    RF24_printDetails,
    RF24_rxFifoFull,
    RF24_powerDown,
    RF24_powerUp,
    RF24_writeFast, /* blocking */
    RF24_writeFastMulticast, /* blocking */
    RF24_writeBlocking, /* blocking */
    RF24_txStandBy, /* blocking */
    RF24_txStandByTimeout, /* blocking */

    RF24_writeAckPayload,
    RF24_enableDynamicAck,
    RF24_isAckPayloadAvailable,
    RF24_whatHappened,
    RF24_startFastWrite,
    RF24_startWrite,
    RF24_reUseTX,
    RF24_flush_tx,
    RF24_testCarrier,
    RF24_testRPD,

    RF24_isValid,
    RF24_maskIRQ,
    RF24_setAddressWidth,
    RF24_closeReadingPipe,
    RF24_setRetries,
    RF24_setChannel,
    RF24_setPayloadSize,
    RF24_getPayloadSize,
    RF24_getDynamicPayloadSize,
    RF24_enableAckPayload,

    RF24_enableDynamicPayloads,
    RF24_isPVariant,
    RF24_setAutoAck,
    RF24_setAutoAckPipe,
    RF24_setPALevel,
    RF24_getPALevel,
    RF24_setDataRate,
    RF24_getDataRate,
    RF24_setCRCLength,
    RF24_getCRCLength,

    RF24_disableCRC,
    RF24_getFailureDetected,
    RF24_setFailureDetected,
    RF24_getProtocolVersion,
} ERF24Command;


extern const ERF24ParamType RF24Commands[][2][MAX_PARAMS];

class RF24Remote: public RF24
{
public:
    RF24Remote(void): RF24(RF24_DEVICE_CEPIN, RF24_DEVICE_CSPIN) {};
    int parse(int paramtype, const uint8_t *p);
    int store(int paramtype, uint8_t *p, uint8_t *ln);
    int executeCommand(void);
    uint8_t dumpRegisters(uint8_t *str);

protected:
    ERF24Command command;
    bool p_bool[2][MAX_PARAMS_TYPE];
    uint8_t p_uint8[2][MAX_PARAMS_TYPE];
    uint16_t p_uint16[2][MAX_PARAMS_TYPE];
    uint32_t p_uint32[2][MAX_PARAMS_TYPE];
    uint64_t p_uint64[2][MAX_PARAMS_TYPE];
    uint8_t p_buf[2][MAX_BUFF];
    uint8_t p_buf_ln[2];
private:
};

#endif


