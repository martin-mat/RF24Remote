#ifndef __RF24Dongle_H
#define __RF24Dongle_H

#include "RF24.h"

#ifndef ARDUINO
#define PROGMEM
#endif

#define MAX_PARAMS_TYPE 3
#define MAX_BUFF 32
#define MAX_PARAMS 5

#define IPAR 0
#define OPAR 1

typedef enum
{
    RF24_none=0,
    RF24_bool,
    RF24_uint8,
    RF24_uint16,
    RF24_uint32,
    RF24_uint64,
    RF24_buff,
} ERF24ParamType;

typedef enum
{
    RF24_begin = 0,
    RF24_startListening,
    RF24_stopListening,
    RF24_available,
    RF24_availablePipe,
    RF24_read,
    RF24_write, /* blocking  */
    RF24_writeMulticast,
    RF24_openWritingPipe,
    RF24_openWritingPipe40,
    RF24_openReadingPipe,
    RF24_openReadingPipe40,
    RF24_printDetails,
    RF24_rxFifoFull,
    RF24_powerDown,
    RF24_powerUp,
    RF24_writeFast, /* blocking */
    RF24_writeFastMulticast, /* blocking */
    RF24_writeBlocking, /* blocking */
    RF24_txStandBy, /* blocking */
    RF24_txStandByTimeout, /* blocking */
    RF24_writeAckPayload,
    RF24_enableDynamicAck,
    RF24_isAckPayloadAvailable,
    RF24_whatHappened,
    RF24_startFastWrite,
    RF24_startWrite,
    RF24_reUseTX,
    RF24_flush_tx,
    RF24_testCarrier,
    RF24_testRPD,
    RF24_isValid,
    RF24_maskIRQ,
    RF24_setAddressWidth,
    RF24_closeReadingPipe,
    RF24_setRetries,
    RF24_setChannel,
    RF24_setPayloadSize,
    RF24_getPayloadSize,
    RF24_getDynamicPayloadSize,
    RF24_enableAckPayload,
    RF24_enableDynamicPayloads,
    RF24_isPVariant,
    RF24_setAutoAck,
    RF24_setAutoAckPipe,
    RF24_setPALevel,
    RF24_getPALevel,
    RF24_setDataRate,
    RF24_getDataRate,
    RF24_setCRCLength,
    RF24_getCRCLength,
    RF24_disableCRC,
    RF24_getFailureDetected,
    RF24_setFailureDetected,
} ERF24Command;


//typedef struct
//{
//    ERF24ParamType in_params[MAX_PARAMS];
//    ERF24ParamType out_params[MAX_PARAMS];
//} RF24CommandParams_t;


extern PROGMEM ERF24ParamType RF24Commands[][2][MAX_PARAMS];

class RF24Usb: public RF24
{
public:
  RF24Usb(uint8_t _cepin, uint8_t _cspin): RF24(_cepin, _cspin) {};
  int parse(int paramtype, const char *p);
  int store(int paramtype, char *p);
  int executeCommand(void);
  void callUsb(ERF24Command cmd);


  void begin(void);
  void startListening(void);
  void stopListening(void);
  bool available(void);
  void read( void* buf, uint8_t len );
  bool write( const void* buf, uint8_t len );
  void openWritingPipe(const uint8_t *address);
  void openReadingPipe(uint8_t number, const uint8_t *address);
  void printDetails(void);
  bool available(uint8_t* pipe_num);
  bool rxFifoFull();
  void powerDown(void);
  void powerUp(void) ;
  bool write( const void* buf, uint8_t len, const bool multicast );
  bool writeFast( const void* buf, uint8_t len );
  bool writeFast( const void* buf, uint8_t len, const bool multicast );
  bool writeBlocking( const void* buf, uint8_t len, uint32_t timeout );
  bool txStandBy();
  bool txStandBy(uint32_t timeout);
  void writeAckPayload(uint8_t pipe, const void* buf, uint8_t len);
  void enableDynamicAck();
  bool isAckPayloadAvailable(void);
  void whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready);
  void startFastWrite( const void* buf, uint8_t len, const bool multicast );
  void startWrite( const void* buf, uint8_t len, const bool multicast );
  void reUseTX();
  uint8_t flush_tx(void);
  bool testCarrier(void);
  bool testRPD(void) ;
  bool isValid();
  void maskIRQ(bool tx_ok,bool tx_fail,bool rx_ready);
  void setAddressWidth(uint8_t a_width);
  void closeReadingPipe( uint8_t pipe ) ;
  void setRetries(uint8_t delay, uint8_t count);
  void setChannel(uint8_t channel);
  void setPayloadSize(uint8_t size);
  uint8_t getPayloadSize(void);
  uint8_t getDynamicPayloadSize(void);
  void enableAckPayload(void);
  void enableDynamicPayloads(void);
  bool isPVariant(void) ;
  void setAutoAck(bool enable);
  void setAutoAck( uint8_t pipe, bool enable ) ;
  void setPALevel ( uint8_t level );
  uint8_t getPALevel( void );
  bool setDataRate(rf24_datarate_e speed);
  rf24_datarate_e getDataRate( void ) ;
  void setCRCLength(rf24_crclength_e length);
  rf24_crclength_e getCRCLength(void);
  void disableCRC( void ) ;
  void openReadingPipe(uint8_t number, uint64_t address);
  void openWritingPipe(uint64_t address);
    char to_send[256];
private:
    ERF24Command command;
    bool p_bool[2][MAX_PARAMS_TYPE];
    uint8_t p_uint8[2][MAX_PARAMS_TYPE];
    uint16_t p_uint16[2][MAX_PARAMS_TYPE];
    uint32_t p_uint32[2][MAX_PARAMS_TYPE];
    uint64_t p_uint64[2][MAX_PARAMS_TYPE];
    char p_buf[2][MAX_BUFF];
    uint8_t p_buf_ln[2];
};

#endif


