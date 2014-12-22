#include "RF24Usb.h"

#ifdef __cplusplus
extern "C"{
#endif
#include "usbdrv.h"
#ifdef __cplusplus
} // extern "C"
#endif

#include "RF24UsbDevice.h"

char buffer[256];
uchar buff_pos;
uchar buff_remaining;
uchar buff_size;

RF24UsbDevice rf24usb;

RF24UsbDevice::RF24UsbDevice(void)
{
};

int RF24UsbDevice::executeCommand(void)
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
        case RF24_printDetails: /*RF24::printDetails(out_buf);*/ p_buf_ln[OPAR]=24; break;
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


void RF24UsbDevice::begin(void)
{
    // disable timer 0 overflow interrupt (used for millis)
    TIMSK0&=!(1<<TOIE0);

    buff_pos = 0;
    buff_remaining = 0;
    buff_size = 0;

    cli();

    usbInit();

    usbDeviceDisconnect();
    uchar   i;
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();

}

void RF24UsbDevice::update(void)
{
    usbPoll();
}






/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"{
#endif
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (usbRequest_t *)data;
    if (rq->bRequest != 250)
    {
        buffer[0] = rq->bRequest;
        buffer[1] = rq->wValue.bytes[0];
        buffer[2] = rq->wValue.bytes[1];
        buffer[3] = rq->wIndex.bytes[0];
        buffer[4] = rq->wIndex.bytes[1];
        buffer[5] = 0;
        buff_pos = 5;
        buff_remaining = rq->wLength.word;
    };
    
    if (rq->bmRequestType == 192) // USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN
    {
        rf24usb.parse(IPAR, buffer);
        rf24usb.executeCommand();
        rf24usb.store(OPAR, buffer, &buff_size);
        usbMsgPtr = (uchar *)buffer;
        return buff_size;
    } else // long input
    {
        if (buff_remaining > sizeof(buffer)) // limit to buffer size
            buff_remaining = sizeof(buffer);
        return USB_NO_MSG; 
    }
      
    return 0; /* default for not implemented requests: return no data back to host */
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    uchar i;

    if(len > buff_remaining)                // if this is the last incomplete chunk
        len = buff_remaining;               // limit to the amount we can store
    buff_remaining -= len;

    for (i=0; i<len; i++)
        buffer[buff_pos++] = data[i];

    //if (buff_remaining == 0)
    //{
    //    buffer[buff_pos] = 0;
        //rf24usb.parse(IPAR, buffer);
        //rf24usb.executeCommand();
        //rf24usb.store(OPAR, buffer, buff_size);
    //}        
    return buff_remaining == 0;
}


#ifdef __cplusplus
} // extern "C"
#endif


