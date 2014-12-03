#ifndef __RF24Dongle_H
#define __RF24Dongle_H

typeded enum
{
    RF24_bool,
    RF24_uint8,
    RF24_uint16,
    RF24_uint32,
    RF24_uint64,
    RF24_buff,
} ERF24ParamType;

typedef enum
{
    RF24_RF24,
    RF24_RF24SPI,
    RF24_begin,
    RF24_startListening,
    RF24_stopListening,
    RF24_available,
    RF24_available_pipe,
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
    RF24_failureDetected,
} ERF24Command;


class RF24Usb
{
public:
    RF24Dongle(void);
private:
    RF24 *radio;
}

#endif
