#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>
//#define DEBUG printf
#define DEBUG(args ...)
#include "RF24Frontend.h"
#include "nRF24L01.h"
#define USB_TIMEOUT 100


static const char * const rf24_datarate_str[] = {
    "1MBPS",
    "2MBPS",
    "250KBPS",
};

static const char * const rf24_model_str[] = {
    "nRF24L01",
    "nRF24L01+",
};

static const char * const rf24_crclength_str[] = {
    "Disabled",
    "8 bits",
    "16 bits",
};
static const char * const rf24_pa_dbm_str[] = {
    "PA_MIN",
    "PA_LOW",
    "PA_HIGH",
    "PA_MAX",
};

void fatal(int retval, const char* format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    exit(retval);
}


RF24::RF24(uint8_t _cepin, uint8_t _cspin)
{
}

/*********************************************************************
**            RF24 methods override for calls from USB frontend     **
**********************************************************************/ 
unsigned long millisd(void)
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return (tm.tv_sec * 1000) + (tm.tv_usec / 1000);
}

int RF24Frontend::callRemote(ERF24Command cmd)
{
    uint8_t ln;
    uint8_t buffer[128];
    unsigned long ms;
    uint8_t i;

    ms = millisd();
    DEBUG("\ncallRemote:%d\n", cmd);

    command = cmd;
    store(IPAR, buffer+2, &ln);
    DEBUG("callRemote:%d, len=%d, ", cmd, ln);
    for (i=0; i<ln; i++)
        DEBUG("%02x ", buffer[i+2]);
    DEBUG("\n");

    buffer[0] = ln+2;
    buffer[1] = request_nr;
    com_device.sendRequest(buffer);

    DEBUG("callUsb expecting data from driver, command %d\n", command);
    com_device.getResponse(buffer);

    if (buffer[1] != request_nr)
        fatal(-1, "getResponse returned wrong command index. Expected:%d, obtained %d\n", request_nr, buffer[1]);

    if (buffer[2] != command)
        fatal(-1, "getResponse returned wrong command. Expected:%d, obtained %d\n", command, buffer[2]);

    DEBUG("received reply to command:%d\n", buffer[1]);

    DEBUG("callRemote parsing results\n");
    parse(OPAR, buffer+2);
    request_nr++;
    DEBUG("callRemote took %lu ms\n", millisd() - ms);
    return 0;
}

void RF24Frontend::begin(void)
{
    request_nr = 0;
    com_device.initialize();
    callRemote(RF24_begin);
}

void RF24Frontend::startListening(void)
{
    callRemote(RF24_startListening);
}

void RF24Frontend::stopListening(void)
{
    callRemote(RF24_stopListening);
}

bool RF24Frontend::available(void)
{
    callRemote(RF24_available);
    return p_bool[OPAR][0];
}

void RF24Frontend::read( void* buf, uint8_t len )
{
    p_uint8[IPAR][0] = len;
    callRemote(RF24_read);
    memcpy(buf, p_buf[OPAR], p_buf_ln[OPAR]);
}

bool RF24Frontend::write( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callRemote(RF24_write);
    return p_bool[OPAR][0];
}

void RF24Frontend::openWritingPipe(const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    callRemote(RF24_openWritingPipe);
}

void RF24Frontend::openReadingPipe(uint8_t number, const uint8_t *address)
{
    p_buf_ln[IPAR] = 5;
    memcpy(p_buf[IPAR], address, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = number;
    callRemote(RF24_openWritingPipe);
}

void RF24Frontend::printDetails(void)
{
    uint8_t *str;
    uint8_t status;
    uint8_t aw;
    uint8_t i;
    uint8_t dr;
    rf24_datarate_e rate;

    callRemote(RF24_printDetails);
    str = (uint8_t *)p_buf[OPAR];

    status = str[0];
    printf("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n",
           status,
           (status & _BV(RX_DR))?1:0,
           (status & _BV(TX_DS))?1:0,
           (status & _BV(MAX_RT))?1:0,
           ((status >> RX_P_NO) & 0b111),
           (status & _BV(TX_FULL))?1:0
          );
    aw = str[SETUP_AW+1]+2;
    printf("Addr width\t = %d\n", aw);
    printf("RX_ADDR_P0-1\t = 0x");
    for (i=aw; i > 0; i--)
        printf("%02x", str[RX_ADDR_P0+i]);
    printf(" 0x");
    for (i=aw; i >0; i--)
        printf("%02x", str[RX_ADDR_P1+i+aw-1]);
    printf("\n");
    printf("RX_ADDR_P2-5\t = ");
    for (i=0; i<4; i++)
        printf("0x%02x ", str[RX_ADDR_P2 + 2*(aw-1) + 1 + i]);
    printf("\n");
    printf("TX_ADDR\t\t = 0x");
    for (i=aw; i > 0 ; i--)
        printf("%02x", str[TX_ADDR + 2*(aw-1) + i]);
    printf("\n");
    printf("RX_PW_P0-6\t = ");
    for (i=0; i<6; i++)
        printf("0x%02x ", str[RX_PW_P0 + 3*(aw-1) + 1 + i]);
    printf("\n");
    printf("EN_AA\t\t = 0x%02x\n", str[EN_AA + 1]);
    printf("EN_RXADDR\t = 0x%02x\n", str[EN_RXADDR + 1]);
    printf("RF_CH\t\t = 0x%02x\n", str[RF_CH + 1]);
    printf("RF_SETUP\t = 0x%02x\n", str[RF_SETUP + 1]);
    printf("CONFIG\t = 0x%02x\n", str[CONFIG + 1]);
    printf("DYNPD/FEATURE\t = 0x%02x 0x%02x\n", str[DYNPD + 1 + 3*(aw-1) + 4], str[FEATURE + 1 + 3*(aw-1) + 4]);
    dr = str[RF_SETUP + 1]  & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
    if ( dr == _BV(RF_DR_LOW) )
    {
        // '10' = 250KBPS
        rate = RF24_250KBPS ;
    }
    else if ( dr == _BV(RF_DR_HIGH) )
    {
        // '01' = 2MBPS
        rate = RF24_2MBPS ;
    }
    else
    {
        // '00' = 1MBPS
        rate = RF24_1MBPS ;
    }

    printf("Data rate\t = %s\n", rf24_datarate_str[rate]);
    printf("Model\t = ");
    printf("CRC Length\t = ");
    printf("Model\t = ");
    printf("PA Power\t = ");
    
}

bool RF24Frontend::available(uint8_t* pipe_num)
{
    callRemote(RF24_availablePipe);
    *pipe_num = p_uint8[OPAR][0];
    return p_bool[OPAR][0];
}

bool RF24Frontend::rxFifoFull()
{
    callRemote(RF24_rxFifoFull);
    return p_bool[OPAR][0]; 
}

void RF24Frontend::powerDown(void)
{
    callRemote(RF24_powerDown);
}

void RF24Frontend::powerUp(void)
{
    callRemote(RF24_powerUp);
}

bool RF24Frontend::write(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callRemote(RF24_write);
    return p_bool[OPAR][0];
}

bool RF24Frontend::writeFast( const void* buf, uint8_t len )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    callRemote(RF24_writeFast);
    return p_bool[OPAR][0];
}

bool RF24Frontend::writeFast( const void* buf, uint8_t len, const bool multicast )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callRemote(RF24_writeFastMulticast);
    return p_bool[OPAR][0];
}

bool RF24Frontend::writeBlocking( const void* buf, uint8_t len, uint32_t timeout )
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_uint32[IPAR][0] = timeout;
    callRemote(RF24_writeBlocking);
    return p_bool[OPAR][0];
}

bool RF24Frontend::txStandBy()
{
    callRemote(RF24_txStandBy);
    return p_bool[OPAR][0];
}

bool RF24Frontend::txStandBy(uint32_t timeout)
{
    p_uint32[IPAR][0] = timeout;
    callRemote(RF24_txStandByTimeout);
    return p_bool[OPAR][0];
}

void RF24Frontend::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
    p_uint8[IPAR][0] = pipe;
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][1] = len;
    callRemote(RF24_writeAckPayload);
}

void RF24Frontend::enableDynamicAck()
{
    callRemote(RF24_enableDynamicAck);
}

bool RF24Frontend::isAckPayloadAvailable(void)
{
    callRemote(RF24_isAckPayloadAvailable);
    return p_bool[OPAR][0];
}

void RF24Frontend::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
    callRemote(RF24_whatHappened);
    tx_ok = p_bool[OPAR][0];
    tx_fail = p_bool[OPAR][1];
    rx_ready = p_bool[OPAR][2];
}

void RF24Frontend::startFastWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callRemote(RF24_startFastWrite);
}

void RF24Frontend::startWrite(const void* buf, uint8_t len, const bool multicast)
{
    p_buf_ln[IPAR] = len>MAX_BUFF?MAX_BUFF:len;
    memcpy(p_buf[IPAR], buf, p_buf_ln[IPAR]);
    p_uint8[IPAR][0] = len;
    p_bool[IPAR][0] = multicast;
    callRemote(RF24_startWrite);
}

void RF24Frontend::reUseTX(void)
{
    callRemote(RF24_reUseTX);
}

uint8_t RF24Frontend::flush_tx(void)
{
    callRemote(RF24_flush_tx);
    return p_uint8[OPAR][0];
}

bool RF24Frontend::testCarrier(void)
{
    callRemote(RF24_testCarrier);
    return p_bool[OPAR][0];
}

bool RF24Frontend::testRPD(void)
{
    callRemote(RF24_testRPD);
    return p_bool[OPAR][0];
}

bool RF24Frontend::isValid()
{
    callRemote(RF24_isValid);
    return p_bool[OPAR][0];
}

void RF24Frontend::maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready)
{
    p_bool[IPAR][0] = tx_ok;
    p_bool[IPAR][1] = tx_fail;
    p_bool[IPAR][2] = rx_ready;
    callRemote(RF24_maskIRQ);
}

void RF24Frontend::setAddressWidth(uint8_t a_width)
{
    p_uint8[IPAR][0] = a_width;
    callRemote(RF24_setAddressWidth);
}

void RF24Frontend::closeReadingPipe(uint8_t pipe)
{
    p_uint8[IPAR][0] = pipe;
    callRemote(RF24_closeReadingPipe);
}

void RF24Frontend::setRetries(uint8_t delay, uint8_t count)
{
    p_uint8[IPAR][0] = delay;
    p_uint8[IPAR][1] = count;
    callRemote(RF24_setRetries);
}

void RF24Frontend::setChannel(uint8_t channel)
{
    p_uint8[IPAR][0] = channel;
    callRemote(RF24_setChannel);
}

void RF24Frontend::setPayloadSize(uint8_t size)
{
    p_uint8[IPAR][0] = size;
    callRemote(RF24_setPayloadSize);
}

uint8_t RF24Frontend::getPayloadSize(void)
{
    callRemote(RF24_getPayloadSize);
    return p_uint8[OPAR][0];
}

uint8_t RF24Frontend::getDynamicPayloadSize(void)
{
    callRemote(RF24_getDynamicPayloadSize);
    return p_uint8[OPAR][0];
}

void RF24Frontend::enableAckPayload(void)
{
    callRemote(RF24_enableAckPayload);
}

void RF24Frontend::enableDynamicPayloads(void)
{
    callRemote(RF24_enableDynamicPayloads);
}

bool RF24Frontend::isPVariant(void) 
{
    callRemote(RF24_isPVariant);
    return p_bool[OPAR][0];
}

void RF24Frontend::setAutoAck(bool enable)
{
    p_bool[IPAR][0] = enable;
    callRemote(RF24_setAutoAck);
}

void RF24Frontend::setAutoAck(uint8_t pipe, bool enable)
{
    p_uint8[IPAR][0] = pipe;
    p_bool[IPAR][0] = enable;
    callRemote(RF24_setAutoAckPipe);
}

void RF24Frontend::setPALevel (uint8_t level)
{
    p_uint8[IPAR][0] = level;
    callRemote(RF24_setPALevel);
}

uint8_t RF24Frontend::getPALevel(void)
{
    callRemote(RF24_getPALevel);
    return p_uint8[OPAR][0];
}

bool RF24Frontend::setDataRate(rf24_datarate_e speed)
{
    p_uint8[IPAR][0] = (uint8_t)speed;
    callRemote(RF24_setDataRate);
    return p_bool[OPAR][0];
}

rf24_datarate_e RF24Frontend::getDataRate(void)
{
    callRemote(RF24_getDataRate);
    return (rf24_datarate_e)p_uint8[OPAR][0];
}

void RF24Frontend::setCRCLength(rf24_crclength_e length)
{
    p_uint8[IPAR][0] = (uint8_t)length;
    callRemote(RF24_setCRCLength);
}

rf24_crclength_e RF24Frontend::getCRCLength(void)
{
    callRemote(RF24_getCRCLength);
    return (rf24_crclength_e)p_uint8[OPAR][0];
}

void RF24Frontend::disableCRC(void)
{
    callRemote(RF24_disableCRC);
}

void RF24Frontend::openReadingPipe(uint8_t number, uint64_t address)
{
    p_uint8[IPAR][0] = number;
    p_uint64[IPAR][0] = address;
    callRemote(RF24_openReadingPipe40);
}

void RF24Frontend::openWritingPipe(uint64_t address)
{
    p_uint64[IPAR][0] = address;
    callRemote(RF24_openWritingPipe40);
}

