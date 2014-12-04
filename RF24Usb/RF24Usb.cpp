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
    SerializedCall(RF24_stopListening, {}, {}),
    SerializedCall(RF24_available, {}, {RF24_bool}),
    SerializedCall(RF24_available_pipe, {}, {RF24_bool, RF24_uint8}),
    SerializedCall(RF24_read, {RF24_uint8}, {RF24_buff}),
    SerializedCall(RF24_write, {RF24_buff, RF24_uint8}, {RF24_bool}),
    SerializedCall(RF24_writeMulticast, {RF24_buff, RF24_uint8, RF24_bool}, {RF24_bool}),
    SerializedCall(RF24_openWritingPipe, {RF24_buff}, {}),
    SerializedCall(RF24_openWritingPipe40, {RF24_uint64}, {}),
    SerializedCall(RF24_openReadingPipe, {RF24_uint8, RF24_buff}, {}),
    SerializedCall(RF24_openReadingPipe40, {RF24_uint8, RF24_uint64}, {}),
    SerializedCall(RF24_printDetails, {}, {RF24_buff}),
    SerializedCall(RF24_rxFifoFull, {}, {RF24_bool}),
    SerializedCall(RF24_powerDown, {}, {}),
    SerializedCall(RF24_powerUp, {}, {}),
    SerializedCall(RF24_writeFast, {RF24_buff, RF24_uint8}, {RF24_bool}),
    SerializedCall(RF24_writeFastMulticast, {RF24_buff, RF24_uint8, RF24_bool}, {RF24_bool}),
    SerializedCall(RF24_writeBlocking, {RF24_buff, RF24_uint8, RF24_uint32}, {RF24_bool}),
    SerializedCall(RF24_txStandBy, {}, {RF24_bool}),
    SerializedCall(RF24_txStandByTimeout, {RF24_uint32}, {RF24_bool}),
    SerializedCall(RF24_writeAckPayload, {RF24_uint8, RF24_buff, RF24_uint8}, {}),
    SerializedCall(RF24_enableDynamicAck, {}, {}),
    SerializedCall(RF24_isAckPayloadAvailable, {}, {RF24_bool}),
    SerializedCall(RF24_whatHappened, {}, {RF24_bool, RF24_bool, RF24_bool}),
    SerializedCall(RF24_startFastWrite, {RF24_buff, RF24_uint8, RF24_bool}, {}),
    SerializedCall(RF24_startWrite, {RF24_buff, RF24_uint8, RF24_bool}, {}),
    SerializedCall(RF24_reUseTX, {}, {}),
    SerializedCall(RF24_flush_tx, {}, {RF24_uint8}),
    SerializedCall(RF24_testCarrier, {}, {RF24_bool}),
    SerializedCall(RF24_testRPD, {}, {RF24_bool}),
    SerializedCall(RF24_isValid, {}, {RF24_bool}),
    SerializedCall(RF24_maskIRQ, {RF24_bool, RF24_bool, RF24_bool}, {}),
    SerializedCall(RF24_setAddressWidth, {RF24_uint8}, {}),
    SerializedCall(RF24_closeReadingPipe, {RF24_uint8}, {}),
    SerializedCall(RF24_setRetries, {RF24_uint8, RF24_uint8}, {}),
    SerializedCall(RF24_setChannel, {RF24_uint8}, {}),
    SerializedCall(RF24_setPayloadSize, {RF24_uint8}, {}),
    SerializedCall(RF24_getPayloadSize, {}, {RF24_uint8}),
    SerializedCall(RF24_getDynamicPayloadSize, {}, {RF24_uint8}),
    SerializedCall(RF24_enableAckPayload, {}, {}),
    SerializedCall(RF24_enableDynamicPayloads, {}, {}),
    SerializedCall(RF24_isPVariant, {}, {RF24_bool}),
    SerializedCall(RF24_setAutoAck, {RF24_bool}, {}),
    SerializedCall(RF24_setAutoAckPipe, {RF24_uint8, RF24_bool}, {}),
    SerializedCall(RF24_setPALevel, {RF24_uint8}, {}),
    SerializedCall(RF24_getPALevel, {}, {RF24_uint8}),
    SerializedCall(RF24_setDataRate, {RF24_uint8}, {RF24_bool}),
    SerializedCall(RF24_getDataRate,  {}, {RF24_uint8}),
    SerializedCall(RF24_setCRCLength, {RF24_uint8}, {}),
    SerializedCall(RF24_getCRCLength, {}, {RF24_uint8}),
    SerializedCall(RF24_disableCRC, {}, {}),
    SerializedCall(RF24_getFailureDetected, {}, {RF24_bool}),
    SerializedCall(RF24_setFailureDetected, {RF24_bool}, {}),


int SerializedCall::executeCommand(void)
{
    switch (command)
    {
        case RF24_RF24: radio = new RF24(in_uint8[0], in_uint8[1]); break;
        case RF24_RF24SPI: radio = new RF24(in_uint8[0], in_uint8[1], in_uint32[0]); break;
        case RF24_begin: radio->begin(); break;
        case RF24_startListening: radio->startListening(); break;
        case RF24_stopListening: radio->stopListening(); break;
        case RF24_available: out_bool[0] = radio->available(); break;
        case RF24_available_pipe: out_bool[0] = radio->available(&out_int8[0]); break;
        case RF24_read: radio->read(out_buf[0], in_int8[0]); break;
        case RF24_write: out_bool[0] = radio->write(in_buff, in_int8[0]); break;
        case RF24_writeMulticast: out_bool[0] = radio->write(in_buff, in_int8[0], in_bool[0]); break;
        case RF24_openWritingPipe: radio->openWritingPipe((uint8_t *)in_buff); break;
        case RF24_openWritingPipe40: radio->openWritingPipe(in_int64[0]); break;
        case RF24_openReadingPipe: radio->openWritingPipe(in_int8[0], (uint8_t *)in_buff); break;
        case RF24_openReadingPipe40: radio->openWritingPipe(in_int8[0], in_int64[0]); break;
        case RF24_printDetails: radio->printDetails(out_buff); break;
        case RF24_rxFifoFull: out_bool[0] = radio->rxFifoFull(); break;
        case RF24_powerDown: radio->powerDown(); break;
        case RF24_powerUp: radio->powerUp(); break;
        case RF24_writeFast: out_bool[0] = radio->writeFast(in_buff, in_uint8[0]); break;
        case RF24_writeFastMulticast: out_bool[0] = radio->writeFast(in_buff, in_uint8[0], in_bool[0]); break;
        case RF24_writeBlocking: out_bool[0] = radio->writeFast(in_buff, in_uint8[0], in_uint32[0]); break;
        case RF24_txStandBy: out_bool[0] = radio->txStandBy(); break;
        case RF24_txStandByTimeout: out_bool[0] = radio->txStandBy(); break;
        case RF24_writeAckPayload: radio->writeAckPayload(in_uint8[0], in_buff, in_uint8[1]); break;
        case RF24_enableDynamicAck: radio->enableDynamicAck(); break;
        case RF24_isAckPayloadAvailable: out_bool[0] = radio->isAckPayloadAvailable(); break;
        case RF24_whatHappened: radio->whatHappened(out_bool[0], out_bool[1], out_bool[2]); break;
        case RF24_startFastWrite: radio->startFastWrite(in_buff, in_uint8[0], in_bool[0]); break;
        case RF24_startWrite: radio->startWrite(in_buff, in_uint8[0], in_bool[0]); break;
        case RF24_reUseTX: radio->reUseTX(); break;
        case RF24_flush_tx: out_uint8[0] = radio->flush_tx(); break;
        case RF24_testCarrier: out_bool[0] = radio->testCarrier(); break;
        case RF24_testRPD: out_bool[0] = radio->testRPD(); break;
        case RF24_isValid: out_bool[0] = radio->isValid(); break;
        case RF24_maskIRQ: radio->maskIRQ(in_bool[0], in_bool[1], in_bool[2]); break;
        case RF24_setAddressWidth: radio->setAddressWidth(in_uint8[0]); break;
        case RF24_closeReadingPipe: radio->closeReadingPipe(in_uint8[0]); break;
        case RF24_setRetries: radio->setRetries(in_uint8[0], in_uint8[1]); break;
        case RF24_setChannel: radio->setChannel(in_uint8[0]); break;
        case RF24_setPayloadSize: radio->setPayloadSize(in_uint8[0]); break;
        case RF24_getPayloadSize: out_uint8[0] = radio->getPayloadSize(); break;
        case RF24_getDynamicPayloadSize: out_uint8[0] = radio->getDynamicPayloadSize();  break;
        case RF24_enableAckPayload: radio->enableAckPayload(); break;
        case RF24_enableDynamicPayloads: radio->enableDynamicPayloads(); break;
        case RF24_isPVariant: out_bool[0] = radio->isPVariant(); break;
        case RF24_setAutoAck: radio->setAutoAck(in_bool[0]); break;
        case RF24_setAutoAckPipe: radio->setAutoAck(in_int8[0], in_bool[0]); break;
        case RF24_setPALevel: radio->setPALevel(in_int8[0]); break;
        case RF24_getPALevel: out_int8[0] = radio->getPALevel(); break;
        case RF24_setDataRate: out_bool[0] = radio->setDataRate(in_uint8[0]); break;
        case RF24_getDataRate: out_uint8[0] = radio->getDataRate(); break;
        case RF24_setCRCLength: radio->setCRCLength(in_int8[0]); break;
        case RF24_getCRCLength: out_int8[0] = radio->getCRCLength(); break;
        case RF24_disableCRC: radio->disableCRC(); break;
        case RF24_getFailureDetected: out_bool[0] = radio->failureDetected; break;
        case RF24_setFailureDetected: radio->failureDetected = in_bool[0]; break;
    }
}


