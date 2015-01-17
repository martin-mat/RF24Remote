#ifdef _RF24_FRONTEND
    #include <stdio.h>
    #include <inttypes.h>
#endif
#include "RF24Remote.h"

const ERF24ParamType RF24Commands[][2][MAX_PARAMS]  =
{
    /*begin*/ {{RF24_none}, {RF24_none}},
    /*startListening*/ {{RF24_none}, {RF24_none}},
    /*stopListening*/ {{RF24_none}, {RF24_none}},
    /*available*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*availablePipe*/ {{RF24_none}, {RF24_bool, RF24_uint8, RF24_none}},
    /*read*/ {{RF24_uint8, RF24_none}, {RF24_buff, RF24_none}},
    /*write*/ {{RF24_buff, RF24_uint8, RF24_none}, {RF24_bool, RF24_none}},
    /*writeMulticast*/ {{RF24_buff, RF24_uint8, RF24_bool, RF24_none}, {RF24_bool, RF24_none}},
    /*openWritingPipe*/ {{RF24_buff, RF24_none}, {RF24_none}},
    /*openWritingPipe40*/ {{RF24_uint64, RF24_none}, {RF24_none}},
    /*openReadingPipe*/ {{RF24_uint8, RF24_buff, RF24_none}, {RF24_none}},
    /*openReadingPipe40*/ {{RF24_uint8, RF24_uint64, RF24_none}, {RF24_none}},
    /*printDetails*/ {{RF24_none}, {RF24_buff, RF24_none}},
    /*rxFifoFull*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*powerDown*/ {{RF24_none}, {RF24_none}},
    /*powerUp*/ {{RF24_none}, {RF24_none}},
    /*writeFast*/ {{RF24_buff, RF24_uint8, RF24_none}, {RF24_bool, RF24_none}},
    /*writeFastMulticast*/ {{RF24_buff, RF24_uint8, RF24_bool, RF24_none}, {RF24_bool, RF24_none}},
    /*writeBlocking*/ {{RF24_buff, RF24_uint8, RF24_uint32, RF24_none}, {RF24_bool, RF24_none}},
    /*txStandBy*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*txStandByTimeout*/ {{RF24_uint32, RF24_none}, {RF24_bool, RF24_none}},
    /*writeAckPayload*/ {{RF24_uint8, RF24_buff, RF24_uint8, RF24_none}, {RF24_none}},
    /*enableDynamicAck*/ {{RF24_none}, {RF24_none}},
    /*isAckPayloadAvailable*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*whatHappened*/ {{RF24_none}, {RF24_bool, RF24_bool, RF24_bool, RF24_none}},
    /*startFastWrite*/ {{RF24_buff, RF24_uint8, RF24_bool, RF24_none}, {RF24_none}},
    /*startWrite*/ {{RF24_buff, RF24_uint8, RF24_bool, RF24_none}, {RF24_none}},
    /*reUseTX*/ {{RF24_none}, {RF24_none}},
    /*flush_tx*/ {{RF24_none}, {RF24_uint8, RF24_none}},
    /*testCarrier*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*testRPD*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*isValid*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*maskIRQ*/ {{RF24_bool, RF24_bool, RF24_bool, RF24_none}, {RF24_none}},
    /*setAddressWidth*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*closeReadingPipe*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*setRetries*/ {{RF24_uint8, RF24_uint8, RF24_none}, {RF24_none}},
    /*setChannel*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*setPayloadSize*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*getPayloadSize*/ {{RF24_none}, {RF24_uint8, RF24_none}},
    /*getDynamicPayloadSize*/ {{RF24_none}, {RF24_uint8, RF24_none}},
    /*enableAckPayload*/ {{RF24_none}, {RF24_none}},
    /*enableDynamicPayloads*/ {{RF24_none}, {RF24_none}},
    /*isPVariant*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*setAutoAck*/ {{RF24_bool, RF24_none}, {RF24_none}},
    /*setAutoAckPipe*/ {{RF24_uint8, RF24_bool, RF24_none}, {RF24_none}},
    /*setPALevel*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*getPALevel*/ {{RF24_none}, {RF24_uint8, RF24_none}},
    /*setDataRate*/ {{RF24_uint8, RF24_none}, {RF24_bool, RF24_none}},
    /*getDataRate*/  {{RF24_none}, {RF24_uint8, RF24_none}},
    /*setCRCLength*/ {{RF24_uint8, RF24_none}, {RF24_none}},
    /*getCRCLength*/ {{RF24_none}, {RF24_uint8, RF24_none}},
    /*disableCRC*/ {{RF24_none}, {RF24_none}},
    /*getFailureDetected*/ {{RF24_none}, {RF24_bool, RF24_none}},
    /*setFailureDetected*/ {{RF24_bool, RF24_none}, {RF24_none}},
    /*getProtocolVersion*/ {{RF24_none}, {RF24_uint8, RF24_none}},
};


int RF24Remote::parse(int paramtype, const uint8_t *p)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    ERF24ParamType param;

    command = (ERF24Command)*p++;
    DPRINT("Parse Command:%d\n",command);
    while ((param = (ERF24ParamType)*p++) != RF24_none)
    {
        DPRINT("Parse Param:%d, ",param);
        switch (param)
        {
            case RF24_bool: DPRINT("%d", *(uint8_t *)p); p_bool[paramtype][param_cnt_bool++] = *(bool *)p++; break;
            case RF24_uint8: DPRINT("%d", *(uint8_t *)p); p_uint8[paramtype][param_cnt_uint8++] = *(uint8_t *)p++; break;
            case RF24_uint16: DPRINT("%d", *(uint16_t *)p); p_uint16[paramtype][param_cnt_uint16++] = *(uint16_t *)p; p+=2; break;
            case RF24_uint32: DPRINT("%d", *(uint32_t *)p); p_uint32[paramtype][param_cnt_uint32++] = *(uint32_t *)p; p+=4; break;
            case RF24_uint64: DPRINT("%" PRIx64 "", *(uint64_t *)p);p_uint64[paramtype][param_cnt_uint64++] = *(uint64_t *)p; p+=8; break;
            case RF24_buff:
                p_buf_ln[paramtype] = (uint8_t) *p++;
                p_buf_ln[paramtype] = p_buf_ln[paramtype]>MAX_BUFF?MAX_BUFF:p_buf_ln[paramtype];
                memcpy(p_buf[paramtype], p, p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                DPRINT("%d", p_buf_ln[paramtype]);
                break;
            case RF24_none: break;
        } 
    DPRINT("\n");
    }
    return 0;
}

int RF24Remote::store(int paramtype, uint8_t *p, uint8_t *ln)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    uint8_t cnt=0;
    uint8_t *start = p;

    DPRINT("Store Command:%d\n", command);
    
    *(ERF24Command *)p = command; p++;
    while ((*p = RF24Commands[command][paramtype][cnt]) != RF24_none)
    {
        cnt++;
        DPRINT("Store Param:%d, ",*p);
        switch (*p++)
        {
            case RF24_bool: *((bool *)p) = p_bool[paramtype][param_cnt_bool++]; DPRINT("%d", *(uint8_t *)p); p++; break;
            case RF24_uint8: *((uint8_t *)p) = p_uint8[paramtype][param_cnt_uint8++]; DPRINT("%d", *(uint8_t *)p); p++; break;
            case RF24_uint16: *((uint16_t *)p) = p_uint16[paramtype][param_cnt_uint16++]; DPRINT("%d", *(uint16_t *)p); p+=2; break;
            case RF24_uint32: *((uint32_t *)p) = p_uint32[paramtype][param_cnt_uint32++]; DPRINT("%d", *(uint32_t *)p); p+=4; break;
            case RF24_uint64: *((uint64_t *)p) = p_uint64[paramtype][param_cnt_uint64++]; DPRINT("%" PRIx64 "", *(uint64_t *)p); p+=8; break;
            case RF24_buff:
                DPRINT("%d", p_buf_ln[paramtype]);
                *((uint8_t *)p) = p_buf_ln[paramtype]; p++;
                memcpy(p, p_buf[paramtype], p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                break;
        }
        DPRINT("\n");
    }
    p++;
    *ln = p - start;
    DPRINT("Store ln:%d '" , *ln);
    for (cnt=0; cnt<*ln; cnt++)
        DPRINT("%02X ", start[cnt]);
    DPRINT("'\n");
    return 0;
}

int RF24Remote::executeCommand(void (*poll)(void))
{
#ifndef _RF24_FRONTEND
    switch (command)
    {
        case RF24_begin: RF24::begin(); break;
        case RF24_startListening: RF24::startListening(); break;
        case RF24_stopListening: RF24::stopListening(); break;
        case RF24_available: p_bool[OPAR][0] = RF24::available(); break;
        case RF24_availablePipe: p_bool[OPAR][0] = RF24::available(&p_uint8[OPAR][0]); break;
        case RF24_read: RF24::read(p_buf[OPAR], p_uint8[IPAR][0]); p_buf_ln[OPAR]=p_uint8[IPAR][0]>RF24::getPayloadSize()?RF24::getPayloadSize():p_uint8[IPAR][0]; break;
        case RF24_write: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0], false, poll); break;
        case RF24_writeMulticast: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0], poll); break;
        case RF24_openWritingPipe: RF24::openWritingPipe((uint8_t *)p_buf[IPAR]); break;
        case RF24_openWritingPipe40: RF24::openWritingPipe(p_uint64[IPAR][0]); break;
        case RF24_openReadingPipe: RF24::openReadingPipe(p_uint8[IPAR][0], (uint8_t *)p_buf[IPAR]); break;
        case RF24_openReadingPipe40: RF24::openReadingPipe(p_uint8[IPAR][0], p_uint64[IPAR][0]); break;
        case RF24_printDetails: p_buf_ln[OPAR]=RF24::dumpRegisters((uint8_t *)p_buf[OPAR]); break;
        case RF24_rxFifoFull: p_bool[OPAR][0] = RF24::rxFifoFull(); break;
        case RF24_powerDown: RF24::powerDown(); break;
        case RF24_powerUp: RF24::powerUp(); break;
        case RF24_writeFast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], false, poll); break;
        case RF24_writeFastMulticast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0], poll); break;
        case RF24_writeBlocking: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_uint32[IPAR][0], poll); break;
        case RF24_txStandBy: p_bool[OPAR][0] = RF24::txStandBy(0, poll); break;
        case RF24_txStandByTimeout: p_bool[OPAR][0] = RF24::txStandBy(p_uint32[IPAR][0], poll); break;
        case RF24_writeAckPayload: RF24::writeAckPayload(p_uint8[IPAR][0], p_buf[IPAR], p_uint8[IPAR][1]); break;
        case RF24_enableDynamicAck: RF24::enableDynamicAck(); break;
        case RF24_isAckPayloadAvailable: p_bool[OPAR][0] = RF24::isAckPayloadAvailable(); break;
        case RF24_whatHappened: RF24::whatHappened(p_bool[OPAR][0], p_bool[OPAR][1], p_bool[OPAR][2]); break;
        case RF24_startFastWrite: RF24::startFastWrite(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_startWrite: RF24::startWrite(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_reUseTX: RF24::reUseTX(); break;
        case RF24_flush_tx: p_uint8[OPAR][0] = RF24::flush_tx(); break;
        case RF24_testCarrier: p_bool[OPAR][0] = RF24::testCarrier(); break;
        case RF24_testRPD: p_bool[OPAR][0] = RF24::testRPD(); break;
        case RF24_isValid: p_bool[OPAR][0] = RF24::isValid(); break;
        case RF24_maskIRQ: RF24::maskIRQ(p_bool[IPAR][0], p_bool[IPAR][1], p_bool[IPAR][2]); break;
        case RF24_setAddressWidth: RF24::setAddressWidth(p_uint8[IPAR][0]); break;
        case RF24_closeReadingPipe: RF24::closeReadingPipe(p_uint8[IPAR][0]); break;
        case RF24_setRetries: RF24::setRetries(p_uint8[IPAR][0], p_uint8[IPAR][1]); break;
        case RF24_setChannel: RF24::setChannel(p_uint8[IPAR][0]); break;
        case RF24_setPayloadSize: RF24::setPayloadSize(p_uint8[IPAR][0]); break;
        case RF24_getPayloadSize: p_uint8[OPAR][0] = RF24::getPayloadSize(); break;
        case RF24_getDynamicPayloadSize: p_uint8[OPAR][0] = RF24::getDynamicPayloadSize();  break;
        case RF24_enableAckPayload: RF24::enableAckPayload(); break;
        case RF24_enableDynamicPayloads: RF24::enableDynamicPayloads(); break;
        case RF24_isPVariant: p_bool[OPAR][0] = RF24::isPVariant(); break;
        case RF24_setAutoAck: RF24::setAutoAck(p_bool[IPAR][0]); break;
        case RF24_setAutoAckPipe: RF24::setAutoAck(p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_setPALevel: RF24::setPALevel(p_uint8[IPAR][0]); break;
        case RF24_getPALevel: p_uint8[OPAR][0] = RF24::getPALevel(); break;
        case RF24_setDataRate: p_bool[OPAR][0] = RF24::setDataRate((rf24_datarate_e)p_uint8[IPAR][0]); break;
        case RF24_getDataRate: p_uint8[OPAR][0] = (uint8_t)RF24::getDataRate(); break;
        case RF24_setCRCLength: RF24::setCRCLength((rf24_crclength_e)p_uint8[IPAR][0]); break;
        case RF24_getCRCLength: p_uint8[OPAR][0] = (uint8_t)RF24::getCRCLength(); break;
        case RF24_disableCRC: RF24::disableCRC(); break;
        case RF24_getFailureDetected: p_bool[OPAR][0] = RF24::failureDetected; break;
        case RF24_setFailureDetected: RF24::failureDetected = p_bool[IPAR][0]; break;
        case RF24_getProtocolVersion: p_uint8[OPAR][0]=RF24REMOTE_PROTOCOL_VERSION; break;
    }
#endif
    return 0;
}



