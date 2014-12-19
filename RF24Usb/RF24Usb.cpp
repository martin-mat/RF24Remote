#include <stdio.h>
#ifdef _USB_FRONTEND
    #include <usb.h>
    #include "usbconfig.h"
    #include "opendevice.h"
#endif
#include "RF24Usb.h"
#define USB_TIMEOUT 500

PROGMEM ERF24ParamType RF24Commands[][2][MAX_PARAMS]=
{
    /*begin*/ {{}, {}},
    /*startListening*/ {{}, {}},
    /*stopListening*/ {{}, {}},
    /*available*/ {{}, {RF24_bool}},
    /*availablePipe*/ {{}, {RF24_bool, RF24_uint8}},
    /*read*/ {{RF24_uint8}, {RF24_buff}},
    /*write*/ {{RF24_buff, RF24_uint8}, {RF24_bool}},
    /*writeMulticast*/ {{RF24_buff, RF24_uint8, RF24_bool}, {RF24_bool}},
    /*openWritingPipe*/ {{RF24_buff}, {}},
    /*openWritingPipe40*/ {{RF24_uint64}, {}},
    /*openReadingPipe*/ {{RF24_uint8, RF24_buff}, {}},
    /*openReadingPipe40*/ {{RF24_uint8, RF24_uint64}, {}},
    /*printDetails*/ {{}, {RF24_buff}},
    /*rxFifoFull*/ {{}, {RF24_bool}},
    /*powerDown*/ {{}, {}},
    /*powerUp*/ {{}, {}},
    /*writeFast*/ {{RF24_buff, RF24_uint8}, {RF24_bool}},
    /*writeFastMulticast*/ {{RF24_buff, RF24_uint8, RF24_bool}, {RF24_bool}},
    /*writeBlocking*/ {{RF24_buff, RF24_uint8, RF24_uint32}, {RF24_bool}},
    /*txStandBy*/ {{}, {RF24_bool}},
    /*txStandByTimeout*/ {{RF24_uint32}, {RF24_bool}},
    /*writeAckPayload*/ {{RF24_uint8, RF24_buff, RF24_uint8}, {}},
    /*enableDynamicAck*/ {{}, {}},
    /*isAckPayloadAvailable*/ {{}, {RF24_bool}},
    /*whatHappened*/ {{}, {RF24_bool, RF24_bool, RF24_bool}},
    /*startFastWrite*/ {{RF24_buff, RF24_uint8, RF24_bool}, {}},
    /*startWrite*/ {{RF24_buff, RF24_uint8, RF24_bool}, {}},
    /*reUseTX*/ {{}, {}},
    /*flush_tx*/ {{}, {RF24_uint8}},
    /*testCarrier*/ {{}, {RF24_bool}},
    /*testRPD*/ {{}, {RF24_bool}},
    /*isValid*/ {{}, {RF24_bool}},
    /*maskIRQ*/ {{RF24_bool, RF24_bool, RF24_bool}, {}},
    /*setAddressWidth*/ {{RF24_uint8}, {}},
    /*closeReadingPipe*/ {{RF24_uint8}, {}},
    /*setRetries*/ {{RF24_uint8, RF24_uint8}, {}},
    /*setChannel*/ {{RF24_uint8}, {}},
    /*setPayloadSize*/ {{RF24_uint8}, {}},
    /*getPayloadSize*/ {{}, {RF24_uint8}},
    /*getDynamicPayloadSize*/ {{}, {RF24_uint8}},
    /*enableAckPayload*/ {{}, {}},
    /*enableDynamicPayloads*/ {{}, {}},
    /*isPVariant*/ {{}, {RF24_bool}},
    /*setAutoAck*/ {{RF24_bool}, {}},
    /*setAutoAckPipe*/ {{RF24_uint8, RF24_bool}, {}},
    /*setPALevel*/ {{RF24_uint8}, {}},
    /*getPALevel*/ {{}, {RF24_uint8}},
    /*setDataRate*/ {{RF24_uint8}, {RF24_bool}},
    /*getDataRate*/  {{}, {RF24_uint8}},
    /*setCRCLength*/ {{RF24_uint8}, {}},
    /*getCRCLength*/ {{}, {RF24_uint8}},
    /*disableCRC*/ {{}, {}},
    /*getFailureDetected*/ {{}, {RF24_bool}},
    /*setFailureDetected*/ {{RF24_bool}, {}},
};

#ifndef _USB_FRONTEND

int RF24Usb::executeCommand(void)
{
    switch (command)
    {
        case RF24_begin: RF24::begin(); break;
        case RF24_startListening: RF24::startListening(); break;
        case RF24_stopListening: RF24::stopListening(); break;
        case RF24_available: p_bool[OPAR][0] = RF24::available(); break;
        case RF24_availablePipe: p_bool[OPAR][0] = RF24::available(&p_uint8[OPAR][0]); break;
        case RF24_read: RF24::read(p_buf[OPAR], p_uint8[IPAR][0]); p_buf_ln[OPAR]=p_uint8[IPAR][0]>RF24::getPayloadSize()?RF24::getPayloadSize():p_uint8[IPAR][0]; break;
        case RF24_write: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0]); break;
        case RF24_writeMulticast: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_openWritingPipe: RF24::openWritingPipe((uint8_t *)p_buf[IPAR]); break;
        case RF24_openWritingPipe40: RF24::openWritingPipe(p_uint64[IPAR][0]); break;
        case RF24_openReadingPipe: RF24::openReadingPipe(p_uint8[IPAR][0], (uint8_t *)p_buf[IPAR]); break;
        case RF24_openReadingPipe40: RF24::openReadingPipe(p_uint8[IPAR][0], p_uint64[IPAR][0]); break;
//        case RF24_printDetails: RF24::printDetails(out_buf); break;
        case RF24_rxFifoFull: p_bool[OPAR][0] = RF24::rxFifoFull(); break;
        case RF24_powerDown: RF24::powerDown(); break;
        case RF24_powerUp: RF24::powerUp(); break;
        case RF24_writeFast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0]); break;
        case RF24_writeFastMulticast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_writeBlocking: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_uint32[IPAR][0]); break;
        case RF24_txStandBy: p_bool[OPAR][0] = RF24::txStandBy(); break;
        case RF24_txStandByTimeout: p_bool[OPAR][0] = RF24::txStandBy(p_uint32[IPAR][0]); break;
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
    }
}

#endif

int RF24Usb::parse(int paramtype, const char *p)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    ERF24ParamType param;

    command = (ERF24Command)*p++;
    printf("Command:%d\n",command);
    while ((param = (ERF24ParamType)*p++) != RF24_none)
    {
        printf("Param:%d\n",param);
        switch (param)
        {
            case RF24_bool: p_bool[paramtype][param_cnt_bool++] = (bool)*p++; break;
            case RF24_uint8: p_uint8[paramtype][param_cnt_uint8++] = (uint8_t)*p++; break;
            case RF24_uint16: p_uint16[paramtype][param_cnt_uint16++] = (uint16_t)*p; p+=2; break;
            case RF24_uint32: p_uint32[paramtype][param_cnt_uint32++] = (uint32_t)*p; p+=4; break;
            case RF24_uint64: p_uint64[paramtype][param_cnt_uint64++] = (uint32_t)*p; p+=8; break;
            case RF24_buff:
                p_buf_ln[paramtype] = (uint8_t) *p++;
                p_buf_ln[paramtype] = p_buf_ln[paramtype]>MAX_BUFF?MAX_BUFF:p_buf_ln[paramtype];
                memcpy(p_buf[paramtype], p, p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                printf("buf len:%d\n", p_buf_ln[paramtype]);
                break;
        } 
    }
}

int RF24Usb::store(int paramtype, char *p, uint8_t &ln)
{
    uint8_t param_cnt_bool = 0;
    uint8_t param_cnt_uint8 = 0;
    uint8_t param_cnt_uint16 = 0;
    uint8_t param_cnt_uint32 = 0;
    uint8_t param_cnt_uint64 = 0;
    uint8_t cnt=0;
    char *start = p;

    printf("Command:%d\n", command);
    
    *(ERF24Command *)p = command; p++;
    while ((*p = RF24Commands[command][paramtype][cnt]) != RF24_none)
    {
        cnt++;
        switch (*p++)
        {
            case RF24_bool: *((bool *)p) = p_bool[paramtype][param_cnt_bool++]; p++; break;
            case RF24_uint8: *((uint8_t *)p) = p_uint8[paramtype][param_cnt_uint8++]; p++; break;
            case RF24_uint16: *((uint16_t *)p) = p_uint16[paramtype][param_cnt_uint16++]; p+=2; break;
            case RF24_uint32: *((uint32_t *)p) = p_uint32[paramtype][param_cnt_uint32++]; p+=4; break;
            case RF24_uint64: *((uint32_t *)p) = p_uint64[paramtype][param_cnt_uint64++]; p+=8; break;
            case RF24_buff:
                *((uint8_t *)p) = p_buf_ln[paramtype]; p++;
                memcpy(p, p_buf[paramtype], p_buf_ln[paramtype]); p+=p_buf_ln[paramtype];
                break;
        }
    }
    ln = p - start;
}

/*********************************************************************
**            RF24 methods override for calls from USB frontend     **
**********************************************************************/ 
#ifdef _USB_FRONTEND

RF24::RF24(uint8_t _cepin, uint8_t _cspin)
{
};

void RF24Usb::callUsb(ERF24Command cmd)
{
    uint8_t ln;
    usb_dev_handle *handle = NULL;
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    char buffer[256];
    char *buf_to_send;
    int cnt, vid, pid, isOn;
    int ret;

    uint16_t lValue;
    uint16_t lIndex;

    usb_init();

    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];

    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        return;
    }

    command = cmd;
    store(IPAR, buffer, ln);

    lValue = to_send[1] + to_send[2]>>8;
    lIndex = to_send[3] + to_send[4]>>8;
    if (ln>5)   /* short input data, handle everything in one shot */
    {
        ln = 0;
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, command, lValue, lIndex, buffer, ln, USB_TIMEOUT);
    } else {
        buf_to_send = buffer + 5;
        ln -= 5;
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, command, lValue, lIndex, buf_to_send, ln, USB_TIMEOUT);
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 250, lValue, lIndex, buffer, 1, USB_TIMEOUT);
    }

    if (ret<0)
        fprintf(stderr, "usb_controll sending usb data failed");

    parse(OPAR, buffer);
}

/*
RF24Usb::RF24Usb(uint8_t _cepin, uint8_t _cspin)
{
};
*/

void RF24Usb::begin(void)
{
    callUsb(RF24_begin);
}

void RF24Usb::startListening(void)
{
    callUsb(RF24_startListening);
}

void RF24Usb::stopListening(void)
{
    callUsb(RF24_stopListening);
}

bool RF24Usb::available(void)
{
    callUsb(RF24_available);
    return p_bool[OPAR][0];
}

void RF24Usb::read( void* buf, uint8_t len )
{
    p_uint8[IPAR][0] = len;
    callUsb(RF24_read);
    memcpy(buf, p_buf[OPAR], p_buf_ln[OPAR]);
}

bool RF24Usb::write( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callUsb(RF24_write);
    return p_bool[OPAR][0];
}

void RF24Usb::openWritingPipe(const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    callUsb(RF24_openWritingPipe);
}

void RF24Usb::openReadingPipe(uint8_t number, const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = number;
    callUsb(RF24_openWritingPipe);
}

void RF24Usb::printDetails(void)
{
    callUsb(RF24_printDetails);
}

bool RF24Usb::available(uint8_t* pipe_num)
{
    callUsb(RF24_availablePipe);
    *pipe_num = p_uint8[OPAR][0];
    return p_bool[OPAR][0];
}

bool RF24Usb::rxFifoFull()
{
    callUsb(RF24_rxFifoFull);
    return p_bool[OPAR][0]; 
}

void RF24Usb::powerDown(void)
{
    callUsb(RF24_powerDown);
}

void RF24Usb::powerUp(void)
{
    callUsb(RF24_powerUp);
}

bool RF24Usb::write(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_write);
    return p_bool[OPAR][0];
}

bool RF24Usb::writeFast( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callUsb(RF24_writeFast);
    return p_bool[OPAR][0];
}

bool RF24Usb::writeFast( const void* buf, uint8_t len, const bool multicast )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_writeFastMulticast);
    return p_bool[OPAR][0];
}

bool RF24Usb::writeBlocking( const void* buf, uint8_t len, uint32_t timeout )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_uint32[IPAR][0] = timeout;
    callUsb(RF24_writeBlocking);
    return p_bool[OPAR][0];
}

bool RF24Usb::txStandBy()
{
    callUsb(RF24_txStandBy);
    return p_bool[OPAR][0];
}

bool RF24Usb::txStandBy(uint32_t timeout)
{
    p_uint32[IPAR][0] = timeout;
    callUsb(RF24_txStandByTimeout);
    return p_bool[OPAR][0];
}

void RF24Usb::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
    p_uint8[IPAR][0] = pipe;
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][1] = len;
    callUsb(RF24_writeAckPayload);
}

void RF24Usb::enableDynamicAck()
{
    callUsb(RF24_enableDynamicAck);
}

bool RF24Usb::isAckPayloadAvailable(void)
{
    callUsb(RF24_isAckPayloadAvailable);
    return p_bool[OPAR][0];
}

void RF24Usb::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
    callUsb(RF24_whatHappened);
    tx_ok = p_bool[OPAR][0];
    tx_fail = p_bool[OPAR][1];
    rx_ready = p_bool[OPAR][2];
}

void RF24Usb::startFastWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_startFastWrite);
}

void RF24Usb::startWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_startWrite);
}

void RF24Usb::reUseTX(void)
{
    callUsb(RF24_reUseTX);
}

uint8_t RF24Usb::flush_tx(void)
{
    callUsb(RF24_flush_tx);
    return p_uint8[OPAR][0];
}

bool RF24Usb::testCarrier(void)
{
    callUsb(RF24_testCarrier);
    return p_bool[OPAR][0];
}

bool RF24Usb::testRPD(void)
{
    callUsb(RF24_testRPD);
    return p_bool[OPAR][0];
}

bool RF24Usb::isValid()
{
    callUsb(RF24_isValid);
    return p_bool[OPAR][0];
}

void RF24Usb::maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready)
{
    p_bool[IPAR][0] = tx_ok;
    p_bool[IPAR][1] = tx_fail;
    p_bool[IPAR][2] = rx_ready;
    callUsb(RF24_maskIRQ);
}

void RF24Usb::setAddressWidth(uint8_t a_width)
{
    p_uint8[IPAR][0] = a_width;
    callUsb(RF24_setAddressWidth);
}

void RF24Usb::closeReadingPipe(uint8_t pipe)
{
    p_uint8[IPAR][0] = pipe;
    callUsb(RF24_closeReadingPipe);
}

void RF24Usb::setRetries(uint8_t delay, uint8_t count)
{
    p_uint8[IPAR][0] = delay;
    p_uint8[IPAR][1] = count;
    callUsb(RF24_setRetries);
}

void RF24Usb::setChannel(uint8_t channel)
{
    p_uint8[IPAR][0] = channel;
    callUsb(RF24_setChannel);
}

void RF24Usb::setPayloadSize(uint8_t size)
{
    p_uint8[IPAR][0] = size;
    callUsb(RF24_setPayloadSize);
}

uint8_t RF24Usb::getPayloadSize(void)
{
    callUsb(RF24_getPayloadSize);
    return p_uint8[OPAR][0];
}

uint8_t RF24Usb::getDynamicPayloadSize(void)
{
    callUsb(RF24_getDynamicPayloadSize);
    return p_uint8[OPAR][0];

}

void RF24Usb::enableAckPayload(void)
{
    callUsb(RF24_enableAckPayload);
}

void RF24Usb::enableDynamicPayloads(void)
{
    callUsb(RF24_enableDynamicPayloads);
}

bool RF24Usb::isPVariant(void) 
{
    callUsb(RF24_isPVariant);
    return p_bool[OPAR][0];
}

void RF24Usb::setAutoAck(bool enable)
{
    p_bool[IPAR][0] = enable;
    callUsb(RF24_setAutoAck);
}

void RF24Usb::setAutoAck(uint8_t pipe, bool enable)
{
    p_uint8[IPAR][0] = pipe;
    p_bool[IPAR][0] = enable;
    callUsb(RF24_setAutoAckPipe);
}

void RF24Usb::setPALevel (uint8_t level)
{
    p_uint8[IPAR][0] = level;
    callUsb(RF24_setPALevel);
}

uint8_t RF24Usb::getPALevel(void)
{
    callUsb(RF24_getPALevel);
    return p_uint8[OPAR][0];
}

bool RF24Usb::setDataRate(rf24_datarate_e speed)
{
    p_uint8[IPAR][0] = (uint8_t)speed;
    callUsb(RF24_setDataRate);
    return p_bool[OPAR][0];
}

rf24_datarate_e RF24Usb::getDataRate(void)
{
    callUsb(RF24_getDataRate);
    return (rf24_datarate_e)p_uint8[OPAR][0];
}

void RF24Usb::setCRCLength(rf24_crclength_e length)
{
    p_uint8[IPAR][0] = (uint8_t)length;
    callUsb(RF24_setDataRate);
}

rf24_crclength_e RF24Usb::getCRCLength(void)
{
    callUsb(RF24_getCRCLength);
    return (rf24_crclength_e)p_uint8[OPAR][0];
}

void RF24Usb::disableCRC(void)
{
    callUsb(RF24_disableCRC);
}

/* bool RF24Usb::failureDetected */

void RF24Usb::openReadingPipe(uint8_t number, uint64_t address)
{
    p_uint8[IPAR][0] = number;
    p_uint64[IPAR][0] = address;
    callUsb(RF24_openReadingPipe40);
}

void RF24Usb::openWritingPipe(uint64_t address)
{
    p_uint64[IPAR][0] = address;
    callUsb(RF24_openWritingPipe40);
}

/*

bool RF24Usb::waitForFIFO(bool reuse, int8_t timeout, void (*poll)(void))
{
}

bool RF24Usb::waitForTransfer(void (*poll)(void))
{
}

bool RF24Usb::txStandByPoll(uint32_t timeout, void (*poll)(void))
{
}
*/

#endif /* _USB_FRONTEND */
 
