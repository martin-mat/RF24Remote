#include <stdio.h>
#include <usb.h>
#include "usbconfig.h"
#include "opendevice.h"
//#define DEBUG printf
#define DEBUG(args ...)
#include "RF24UsbFrontend.h"
#define USB_TIMEOUT 100


RF24::RF24(uint8_t _cepin, uint8_t _cspin)
{
}

/*********************************************************************
**            RF24 methods override for calls from USB frontend     **
**********************************************************************/ 

void RF24UsbFrontend::callUsb(ERF24Command cmd)
{
    uint8_t ln;
    char buffer[128];
    char *buf_to_send;
    int ret;

    uint16_t lValue;
    uint16_t lIndex;

    DEBUG("\ncallUsb:%d\n", cmd);

    command = cmd;
    store(IPAR, buffer, &ln);
    DEBUG("callUsb:%d, len=%d\n", cmd, ln);

    lValue = buffer[1] + (buffer[2]<<8);
    lIndex = buffer[3] + (buffer[4]<<8);
    if (ln<=6)   /* short input data, handle everything in one shot */
    {
        ln = 0;
        DEBUG("callUsb short\n");
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, command, lValue, lIndex, buffer, 256, USB_TIMEOUT);
        DEBUG("usb_control_device ret:%d\n", ret);
        if (ret<0)
            fprintf(stderr, "usb_control short sending usb data failed\n");
   } else {
        buf_to_send = buffer + 5;
        ln -= 5;
        DEBUG("callUsb long1\n");
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, command, lValue, lIndex, buf_to_send, ln, USB_TIMEOUT);
        DEBUG("usb_control_device ret:%d\n", ret);
        if (ret<0)
            fprintf(stderr, "usb_control long 1 sending usb data failed\n");
        DEBUG("callUsb long2\n");
        ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, 250, lValue, lIndex, buffer, 256, USB_TIMEOUT);
        DEBUG("usb_control_device ret:%d\n", ret);
        if (ret<0)
            fprintf(stderr, "usb_control long 2 sending usb data failed\n");
    }


    DEBUG("callUsb parsing results\n");
    parse(OPAR, buffer);
}

void RF24UsbFrontend::begin(void)
{
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    int vid, pid;

    usb_init();

    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];

    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        return;
    }


    callUsb(RF24_begin);
}

void RF24UsbFrontend::startListening(void)
{
    callUsb(RF24_startListening);
}

void RF24UsbFrontend::stopListening(void)
{
    callUsb(RF24_stopListening);
}

bool RF24UsbFrontend::available(void)
{
    callUsb(RF24_available);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::read( void* buf, uint8_t len )
{
    p_uint8[IPAR][0] = len;
    callUsb(RF24_read);
    memcpy(buf, p_buf[OPAR], p_buf_ln[OPAR]);
}

bool RF24UsbFrontend::write( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callUsb(RF24_write);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::openWritingPipe(const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    callUsb(RF24_openWritingPipe);
}

void RF24UsbFrontend::openReadingPipe(uint8_t number, const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = number;
    callUsb(RF24_openWritingPipe);
}

void RF24UsbFrontend::printDetails(void)
{
    char *str;

    callUsb(RF24_printDetails);
    str = p_buf[OPAR];

    print_status(str[0]);
    printf("Addr length\t = %d\n", str[SETUP_AW+1]+3);
    printf("RX_ADDR_P0-1\t %
}

bool RF24UsbFrontend::available(uint8_t* pipe_num)
{
    callUsb(RF24_availablePipe);
    *pipe_num = p_uint8[OPAR][0];
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::rxFifoFull()
{
    callUsb(RF24_rxFifoFull);
    return p_bool[OPAR][0]; 
}

void RF24UsbFrontend::powerDown(void)
{
    callUsb(RF24_powerDown);
}

void RF24UsbFrontend::powerUp(void)
{
    callUsb(RF24_powerUp);
}

bool RF24UsbFrontend::write(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_write);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::writeFast( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callUsb(RF24_writeFast);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::writeFast( const void* buf, uint8_t len, const bool multicast )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_writeFastMulticast);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::writeBlocking( const void* buf, uint8_t len, uint32_t timeout )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_uint32[IPAR][0] = timeout;
    callUsb(RF24_writeBlocking);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::txStandBy()
{
    callUsb(RF24_txStandBy);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::txStandBy(uint32_t timeout)
{
    p_uint32[IPAR][0] = timeout;
    callUsb(RF24_txStandByTimeout);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
    p_uint8[IPAR][0] = pipe;
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][1] = len;
    callUsb(RF24_writeAckPayload);
}

void RF24UsbFrontend::enableDynamicAck()
{
    callUsb(RF24_enableDynamicAck);
}

bool RF24UsbFrontend::isAckPayloadAvailable(void)
{
    callUsb(RF24_isAckPayloadAvailable);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
    callUsb(RF24_whatHappened);
    tx_ok = p_bool[OPAR][0];
    tx_fail = p_bool[OPAR][1];
    rx_ready = p_bool[OPAR][2];
}

void RF24UsbFrontend::startFastWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_startFastWrite);
}

void RF24UsbFrontend::startWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callUsb(RF24_startWrite);
}

void RF24UsbFrontend::reUseTX(void)
{
    callUsb(RF24_reUseTX);
}

uint8_t RF24UsbFrontend::flush_tx(void)
{
    callUsb(RF24_flush_tx);
    return p_uint8[OPAR][0];
}

bool RF24UsbFrontend::testCarrier(void)
{
    callUsb(RF24_testCarrier);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::testRPD(void)
{
    callUsb(RF24_testRPD);
    return p_bool[OPAR][0];
}

bool RF24UsbFrontend::isValid()
{
    callUsb(RF24_isValid);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready)
{
    p_bool[IPAR][0] = tx_ok;
    p_bool[IPAR][1] = tx_fail;
    p_bool[IPAR][2] = rx_ready;
    callUsb(RF24_maskIRQ);
}

void RF24UsbFrontend::setAddressWidth(uint8_t a_width)
{
    p_uint8[IPAR][0] = a_width;
    callUsb(RF24_setAddressWidth);
}

void RF24UsbFrontend::closeReadingPipe(uint8_t pipe)
{
    p_uint8[IPAR][0] = pipe;
    callUsb(RF24_closeReadingPipe);
}

void RF24UsbFrontend::setRetries(uint8_t delay, uint8_t count)
{
    p_uint8[IPAR][0] = delay;
    p_uint8[IPAR][1] = count;
    callUsb(RF24_setRetries);
}

void RF24UsbFrontend::setChannel(uint8_t channel)
{
    p_uint8[IPAR][0] = channel;
    callUsb(RF24_setChannel);
}

void RF24UsbFrontend::setPayloadSize(uint8_t size)
{
    p_uint8[IPAR][0] = size;
    callUsb(RF24_setPayloadSize);
}

uint8_t RF24UsbFrontend::getPayloadSize(void)
{
    callUsb(RF24_getPayloadSize);
    return p_uint8[OPAR][0];
}

uint8_t RF24UsbFrontend::getDynamicPayloadSize(void)
{
    callUsb(RF24_getDynamicPayloadSize);
    return p_uint8[OPAR][0];

}

void RF24UsbFrontend::enableAckPayload(void)
{
    callUsb(RF24_enableAckPayload);
}

void RF24UsbFrontend::enableDynamicPayloads(void)
{
    callUsb(RF24_enableDynamicPayloads);
}

bool RF24UsbFrontend::isPVariant(void) 
{
    callUsb(RF24_isPVariant);
    return p_bool[OPAR][0];
}

void RF24UsbFrontend::setAutoAck(bool enable)
{
    p_bool[IPAR][0] = enable;
    callUsb(RF24_setAutoAck);
}

void RF24UsbFrontend::setAutoAck(uint8_t pipe, bool enable)
{
    p_uint8[IPAR][0] = pipe;
    p_bool[IPAR][0] = enable;
    callUsb(RF24_setAutoAckPipe);
}

void RF24UsbFrontend::setPALevel (uint8_t level)
{
    p_uint8[IPAR][0] = level;
    callUsb(RF24_setPALevel);
}

uint8_t RF24UsbFrontend::getPALevel(void)
{
    callUsb(RF24_getPALevel);
    return p_uint8[OPAR][0];
}

bool RF24UsbFrontend::setDataRate(rf24_datarate_e speed)
{
    p_uint8[IPAR][0] = (uint8_t)speed;
    callUsb(RF24_setDataRate);
    return p_bool[OPAR][0];
}

rf24_datarate_e RF24UsbFrontend::getDataRate(void)
{
    callUsb(RF24_getDataRate);
    return (rf24_datarate_e)p_uint8[OPAR][0];
}

void RF24UsbFrontend::setCRCLength(rf24_crclength_e length)
{
    p_uint8[IPAR][0] = (uint8_t)length;
    callUsb(RF24_setDataRate);
}

rf24_crclength_e RF24UsbFrontend::getCRCLength(void)
{
    callUsb(RF24_getCRCLength);
    return (rf24_crclength_e)p_uint8[OPAR][0];
}

void RF24UsbFrontend::disableCRC(void)
{
    callUsb(RF24_disableCRC);
}

/* bool RF24UsbFrontend::failureDetected */

void RF24UsbFrontend::openReadingPipe(uint8_t number, uint64_t address)
{
    p_uint8[IPAR][0] = number;
    p_uint64[IPAR][0] = address;
    callUsb(RF24_openReadingPipe40);
}

void RF24UsbFrontend::openWritingPipe(uint64_t address)
{
    p_uint64[IPAR][0] = address;
    callUsb(RF24_openWritingPipe40);
}

/*

bool RF24UsbFrontend::waitForFIFO(bool reuse, int8_t timeout, void (*poll)(void))
{
}

bool RF24UsbFrontend::waitForTransfer(void (*poll)(void))
{
}

bool RF24UsbFrontend::txStandByPoll(uint32_t timeout, void (*poll)(void))
{
}
*/

 
