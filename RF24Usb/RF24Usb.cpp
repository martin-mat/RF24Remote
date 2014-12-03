#include "RF24Usb.h"


class SerializedCall
{
    public:
        SerializedCall(ERF24Command, EFR24ParamType input_params, ERF24ParamType output_params);
        int addOutputParameter(bool val);
        int addOutputParameter(uint8_t val);
        int addOutputParameter(uint16_t val);
        int addOutputParameter(uint32_t val);
        int addOutputParameter(uint64_t val);
        int addOutputParameter(void *buf, int len);

        int getInputParameter(bool &val);
        int getInputParameter(uint8_t &val);
        int getInputParameter(uint16_t &val);
        int getInputParameter(uint32_t &val);
        int getInputParameter(void *buf, int &len);

        int defineInputParameters(ERF24ParamType param_type[]);
        int defineOutputParameters(ERF24ParamType param_type[]);
    
        int executeCommand(void);

private:
    uint8_t arg_uint8_1;
    uint8_t arg_uint8_2;
    uint8_t arg_uint16_1;
    uint8_t arg_uint32_1;
    void *buff;
    RF24 *radio; 
}


SerializedCall rf24_commands[] =
{
    SerializedCall(RF24_RF24, {RF24_uint8, RF24_uint8}, {}),
    SerializedCall(RF24_RF24SPI, {RF24_uint8, RF24_uint8, RF24_uint32}, {}),
    SerializedCall(RF24_begin, {}, {}),
    SerializedCall(RF24_startListening, {}, {}),

int SerializedCall::executeCommand(void)
{
    switch (command)
    {
        case RF24_RF24:
            getInputParameter(arg_uint8_1);
            getInputParameter(arg_uint8_2);
            radio = new RF24(arg_uint8_1, arg_uint8_2);
            break;
        case RF24_RF24SPI:
            getInputParameter(arg_uint8_1);
            getInputParameter(arg_uint8_2);
            getInputParameter(arg_uint32_1);
            radio = new RF24(arg_uint8_1, arg_uint8_2, arg_uint32_1);
            break;
        case RF24_begin:
            radio->begin();
            break;
        case RF24_startListening:
            radio->startListening();
            break;
        case RF24_stopListening:
            radio->stopListening();
            break;
        case RF24_available:
            arg_int1 = radio->available();
            put_arg_int(argsOut, arg_int1);
            break;
        case RF24_available_pipe:
            arg_int1 = radio->available(&arg_int2);
            put_arg_int(argsOut, arg_int1);
            put_arg_int(argsOut, arg_int2);
            break;
        case RF24_read:
            arg_int1 = get_arg_int(argsIn);
            buf = new char[arg_int1+1];
            radio->read(buf, arg_int1);
            put_arg_buff(argsOut, buff, arg_int1);
            break;
        case RF24_write:
            arg_int2 = radio->write(buf, arg_int1);
            put_arg_int(argsOut, arg_int2);
            break;
        case RF24_writeMulticast:
            break;
        case RF24_openWritingPipe:
            break;
        case RF24_openWritingPipe40:
            break;
        case RF24_openReadingPipe:
            break;
        case RF24_openReadingPipe40:
            break;
        case RF24_printDetails:
            break;
        case RF24_rxFifoFull:
            break;
        case RF24_powerDown:
            break;
        case RF24_powerUp:
            break;
        case RF24_writeFast:
            break;
        case RF24_writeFastMulticast:
            break;
        case RF24_writeBlocking:
            break;
        case RF24_txStandBy:
            break;
        case RF24_txStandByTimeout:
            break;
        case RF24_writeAckPayload:
            break;
        case RF24_enableDynamicAck:
            break;
        case RF24_isAckPayloadAvailable:
            break;
        case RF24_whatHappened:
            break;
        case RF24_startFastWrite:
            break;
        case RF24_startWrite:
            break;
        case RF24_reUseTX:
            break;
        case RF24_flush_tx:
            break;
        case RF24_testCarrier:
            break;
        case RF24_testRPD:
            break;
        case RF24_isValid:
            break;
        case RF24_maskIRQ:
            break;
        case RF24_setAddressWidth:
            break;
        case RF24_closeReadingPipe:
            break;
        case RF24_setRetries:
            break;
        case RF24_setChannel:
            break;
        case RF24_setPayloadSize:
            break;
        case RF24_getPayloadSize:
            break;
        case RF24_getDynamicPayloadSize:
            break;
        case RF24_enableAckPayload:
            break;
        case RF24_enableDynamicPayloads:
            break;
        case RF24_isPVariant:
            break;
        case RF24_setAutoAck:
            break;
        case RF24_setAutoAckPipe:
            break;
        case RF24_setPALevel:
            break;
        case RF24_getPALevel:
            break;
        case RF24_setDataRate:
            break;
        case RF24_getDataRate:
            break;
        case RF24_setCRCLength:
            break;
        case RF24_getCRCLength:
            break;
        case RF24_disableCRC:
            break;
        case RF24_failureDetected:
            break;
    }
}


