#ifdef _USB_FRONTEND
    #include <stdio.h>
    //#define DEBUG(args ...)
    #define DEBUG printf
#else
    #define DEBUG(args ...)
#endif
#include "RF24Usb.h"
#define USB_TIMEOUT 500

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
    /*openWritingPipe40*/ {{RF24_uint64, RF24_none}, {RF24_uint64, RF24_none}},
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
};


int RF24Usb::parse(int paramtype, const uint8_t *p)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    ERF24ParamType param;

    command = (ERF24Command)*p++;
    DEBUG("Parse Command:%d\n",command);
    while ((param = (ERF24ParamType)*p++) != RF24_none)
    {
        DEBUG("Parse Param:%d, ",param);
        switch (param)
        {
            case RF24_bool: DEBUG("%d", *(uint8_t *)p); p_bool[paramtype][param_cnt_bool++] = *(bool *)p++; break;
            case RF24_uint8: DEBUG("%d", *(uint8_t *)p); p_uint8[paramtype][param_cnt_uint8++] = *(uint8_t *)p++; break;
            case RF24_uint16: DEBUG("%d", *(uint16_t *)p); p_uint16[paramtype][param_cnt_uint16++] = *(uint16_t *)p; p+=2; break;
            case RF24_uint32: DEBUG("%d", *(uint32_t *)p); p_uint32[paramtype][param_cnt_uint32++] = *(uint32_t *)p; p+=4; break;
            case RF24_uint64: DEBUG("%llx", *(uint64_t *)p);p_uint64[paramtype][param_cnt_uint64++] = *(uint64_t *)p; p+=8; break;
            case RF24_buff:
                p_buf_ln[paramtype] = (uint8_t) *p++;
                p_buf_ln[paramtype] = p_buf_ln[paramtype]>MAX_BUFF?MAX_BUFF:p_buf_ln[paramtype];
                memcpy(p_buf[paramtype], p, p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                DEBUG("%d", p_buf_ln[paramtype]);
                break;
            case RF24_none: break;
        } 
    DEBUG("\n");
    }
    return 0;
}

int RF24Usb::store(int paramtype, uint8_t *p, uint8_t *ln)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    uint8_t cnt=0;
    uint8_t *start = p;

    DEBUG("Store Command:%d\n", command);
    
    *(ERF24Command *)p = command; p++;
    while ((*p = RF24Commands[command][paramtype][cnt]) != RF24_none)
    {
        cnt++;
        DEBUG("Store Param:%d, ",*p);
        switch (*p++)
        {
            case RF24_bool: *((bool *)p) = p_bool[paramtype][param_cnt_bool++]; DEBUG("%d", *(uint8_t *)p); p++; break;
            case RF24_uint8: *((uint8_t *)p) = p_uint8[paramtype][param_cnt_uint8++]; DEBUG("%d", *(uint8_t *)p); p++; break;
            case RF24_uint16: *((uint16_t *)p) = p_uint16[paramtype][param_cnt_uint16++]; DEBUG("%d", *(uint16_t *)p); p+=2; break;
            case RF24_uint32: *((uint32_t *)p) = p_uint32[paramtype][param_cnt_uint32++]; DEBUG("%d", *(uint32_t *)p); p+=4; break;
            case RF24_uint64: *((uint64_t *)p) = p_uint64[paramtype][param_cnt_uint64++]; DEBUG("%llx", *(uint64_t *)p); p+=8; break;
            case RF24_buff:
                DEBUG("%d", p_buf_ln[paramtype]);
                *((uint8_t *)p) = p_buf_ln[paramtype]; p++;
                memcpy(p, p_buf[paramtype], p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                break;
        }
        DEBUG("\n");
    }
    p++;
    *ln = p - start;
    DEBUG("Store ln:%d '" , *ln);
    for (cnt=0; cnt<*ln; cnt++)
        DEBUG("%02X ", start[cnt]);
    DEBUG("'\n");
    return 0;
}

