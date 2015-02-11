#include "RF24RemoteBackend.h"
#include "nRF24L01.h"

int RF24RemoteBackend::executeCommand(void)
{
    switch (command)
    {
        case RF24_begin: begin(); break;
        case RF24_startListening: startListening(); break;
        case RF24_stopListening: stopListening(); break;
        case RF24_available: p_bool[OPAR][0] = available(); break;
        case RF24_availablePipe: p_bool[OPAR][0] = available(&p_uint8[OPAR][0]); break;
        case RF24_read: read(p_buf[OPAR], p_uint8[IPAR][0]); p_buf_ln[OPAR]=p_uint8[IPAR][0]>getPayloadSize()?getPayloadSize():p_uint8[IPAR][0]; break;
        case RF24_write: p_bool[OPAR][0] = write(p_buf[IPAR], p_uint8[IPAR][0], false); break;
        case RF24_writeMulticast: p_bool[OPAR][0] = write(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_openWritingPipe: openWritingPipe((uint8_t *)p_buf[IPAR]); break;
        case RF24_openWritingPipe40: openWritingPipe(p_uint64[IPAR][0]); break;
        case RF24_openReadingPipe: openReadingPipe(p_uint8[IPAR][0], (uint8_t *)p_buf[IPAR]); break;
        case RF24_openReadingPipe40: openReadingPipe(p_uint8[IPAR][0], p_uint64[IPAR][0]); break;
        case RF24_printDetails: p_buf_ln[OPAR]=dumpRegisters((uint8_t *)p_buf[OPAR]); break;
        case RF24_rxFifoFull: p_bool[OPAR][0] = rxFifoFull(); break;
        case RF24_powerDown: powerDown(); break;
        case RF24_powerUp: powerUp(); break;
        case RF24_writeFast: p_bool[OPAR][0] = writeFast(p_buf[IPAR], p_uint8[IPAR][0], false); break;
        case RF24_writeFastMulticast: p_bool[OPAR][0] = writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_writeBlocking: p_bool[OPAR][0] = writeFast(p_buf[IPAR], p_uint8[IPAR][0], p_uint32[IPAR][0]); break;
        case RF24_txStandBy: p_bool[OPAR][0] = txStandBy(0); break;
        case RF24_txStandByTimeout: p_bool[OPAR][0] = txStandBy(p_uint32[IPAR][0]); break;
        case RF24_writeAckPayload: writeAckPayload(p_uint8[IPAR][0], p_buf[IPAR], p_uint8[IPAR][1]); break;
        case RF24_enableDynamicAck: enableDynamicAck(); break;
        case RF24_isAckPayloadAvailable: p_bool[OPAR][0] = isAckPayloadAvailable(); break;
        case RF24_whatHappened: whatHappened(p_bool[OPAR][0], p_bool[OPAR][1], p_bool[OPAR][2]); break;
        case RF24_startFastWrite: startFastWrite(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_startWrite: startWrite(p_buf[IPAR], p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_reUseTX: reUseTX(); break;
        case RF24_flush_tx: p_uint8[OPAR][0] = flush_tx(); break;
        case RF24_testCarrier: p_bool[OPAR][0] = testCarrier(); break;
        case RF24_testRPD: p_bool[OPAR][0] = testRPD(); break;
        case RF24_isValid: p_bool[OPAR][0] = isValid(); break;
        case RF24_maskIRQ: maskIRQ(p_bool[IPAR][0], p_bool[IPAR][1], p_bool[IPAR][2]); break;
        case RF24_setAddressWidth: setAddressWidth(p_uint8[IPAR][0]); break;
        case RF24_closeReadingPipe: closeReadingPipe(p_uint8[IPAR][0]); break;
        case RF24_setRetries: setRetries(p_uint8[IPAR][0], p_uint8[IPAR][1]); break;
        case RF24_setChannel: setChannel(p_uint8[IPAR][0]); break;
        case RF24_setPayloadSize: setPayloadSize(p_uint8[IPAR][0]); break;
        case RF24_getPayloadSize: p_uint8[OPAR][0] = getPayloadSize(); break;
        case RF24_getDynamicPayloadSize: p_uint8[OPAR][0] = getDynamicPayloadSize();  break;
        case RF24_enableAckPayload: enableAckPayload(); break;
        case RF24_enableDynamicPayloads: enableDynamicPayloads(); break;
        case RF24_isPVariant: p_bool[OPAR][0] = isPVariant(); break;
        case RF24_setAutoAck: setAutoAck(p_bool[IPAR][0]); break;
        case RF24_setAutoAckPipe: setAutoAck(p_uint8[IPAR][0], p_bool[IPAR][0]); break;
        case RF24_setPALevel: setPALevel(p_uint8[IPAR][0]); break;
        case RF24_getPALevel: p_uint8[OPAR][0] = getPALevel(); break;
        case RF24_setDataRate: p_bool[OPAR][0] = setDataRate((rf24_datarate_e)p_uint8[IPAR][0]); break;
        case RF24_getDataRate: p_uint8[OPAR][0] = (uint8_t)getDataRate(); break;
        case RF24_setCRCLength: setCRCLength((rf24_crclength_e)p_uint8[IPAR][0]); break;
        case RF24_getCRCLength: p_uint8[OPAR][0] = (uint8_t)getCRCLength(); break;
        case RF24_disableCRC: disableCRC(); break;
        case RF24_getFailureDetected: p_bool[OPAR][0] = failureDetected; break;
        case RF24_setFailureDetected: failureDetected = p_bool[IPAR][0]; break;
        case RF24_getProtocolVersion: p_uint8[OPAR][0]=RF24REMOTE_PROTOCOL_VERSION; break;
    }
    return 0;
}

uint8_t RF24RemoteBackend::dumpRegisters(uint8_t *str)
{
    uint8_t reg=0;
    uint8_t aw;
    uint8_t *begin=str;
    *str = get_status(); str++;
    *str = (uint8_t)isPVariant(); str++;
    while (reg<=FEATURE) // FEATURE is the last register
    {
        // address registers read multiple bytes
        if ((reg == RX_ADDR_P0) || (reg == RX_ADDR_P1) || (reg == TX_ADDR))
        {
            read_register(reg, str, aw); str+=aw; //aw was already read
            reg++;
        } else
        {
            *str = read_register(reg);
            if (reg == SETUP_AW)
                aw = *str+2;
            if (reg == FIFO_STATUS) // gap from FIFO_STATUS to FYNPD
                reg = DYNPD;
            else
                reg++;
            str++;
        }
    }
    return str-begin; // return length of dump
} 


