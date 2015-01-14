#include "Arduino.h"
#include "RF24Usb.h"
#include "RF24SerialDevice.h"

uint8_t buffer[256];
uint8_t buff_pos;
uint8_t buff_remaining;
uint8_t buff_size;
uint8_t command_to_execute;

RF24SerialDevice rf24serial;

RF24SerialDevice::RF24SerialDevice(void)
{
    serial = NULL;
};

int RF24SerialDevice::executeCommand(void)
{
    switch (command)
    {
        case RF24_begin: RF24::begin(); break;
        case RF24_startListening: RF24::startListening(); break;
        case RF24_stopListening: RF24::stopListening(); break;
        case RF24_available: p_bool[OPAR][0] = RF24::available(); break;
        case RF24_availablePipe: p_bool[OPAR][0] = RF24::available(&p_uint8[OPAR][0]); break;
        case RF24_read: RF24::read(p_buf[OPAR], p_uint8[IPAR][0]); p_buf_ln[OPAR]=p_uint8[IPAR][0]>RF24::getPayloadSize()?RF24::getPayloadSize():p_uint8[IPAR][0]; break;
        case RF24_write: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0], false); break;
        case RF24_writeMulticast: p_bool[OPAR][0] = RF24::write(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_openWritingPipe: RF24::openWritingPipe((uint8_t *)p_buf[IPAR]); break;
        case RF24_openWritingPipe40: RF24::openWritingPipe(p_uint64[IPAR][0]); p_uint64[OPAR][0] = p_uint64[IPAR][0]; break;
        case RF24_openReadingPipe: RF24::openReadingPipe(p_uint8[IPAR][0], (uint8_t *)p_buf[IPAR]); break;
        case RF24_openReadingPipe40: RF24::openReadingPipe(p_uint8[IPAR][0], p_uint64[IPAR][0]); break;
        case RF24_printDetails: p_buf_ln[OPAR]=RF24::dumpRegisters((uint8_t *)p_buf[OPAR]); break;
        case RF24_rxFifoFull: p_bool[OPAR][0] = RF24::rxFifoFull(); break;
        case RF24_powerDown: RF24::powerDown(); break;
        case RF24_powerUp: RF24::powerUp(); break;
        case RF24_writeFast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], false); break;
        case RF24_writeFastMulticast: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_writeBlocking: p_bool[OPAR][0] = RF24::writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_uint32[IPAR][0]); break;
        case RF24_txStandBy: p_bool[OPAR][0] = RF24::txStandBy(0); break;
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


void RF24SerialDevice::begin(HardwareSerial* _serial, int speed)
{
    // disable timer 0 overflow interrupt (used for millis)
    buff_pos = 0;
    buff_remaining = 0;
    buff_size = 0;
    command_to_execute = 0;

    serial = _serial;
    //serial->begin(115200);    
    Serial1.begin(115200);
}

void RF24SerialDevice::update(void)
{
    uint8_t seq;
    if (serial == NULL)
        return;


    if (serial->available())
    {
        buffer[0] = serial->read();
        Serial1.println();
        Serial1.println();
        Serial1.print("length:");
        Serial1.println(buffer[0]);
        if (buffer[0] < 4)
            return;
        buff_pos = 1;
        while (buff_pos < buffer[0])
        {
            buffer[buff_pos++] = serial->read();
        }
   
        while(serial->available())
            serial->read();
        Serial1.print("cmd index:");
        Serial1.println(buffer[1]);

        Serial1.print("command:");
        Serial1.println(buffer[2]);
        Serial1.flush();

        seq = buffer[1];
        rf24serial.parse(IPAR, buffer+2);
        rf24serial.executeCommand();
        rf24serial.store(OPAR, buffer+2, &buff_size);
        //buff_size = 4;
        buffer[0] = buff_size+2; // size
        buffer[1] = seq;
        Serial1.print("Sending buffer:");
        for (int i=0; i<buffer[0]; i++)
        {
            Serial1.print(buffer[i], DEC);
            Serial1.print(" ");
        }
        Serial1.println();
        Serial1.println();
        Serial1.flush();

        serial->write(buffer, buffer[0]);
        serial->flush();
    }
}


