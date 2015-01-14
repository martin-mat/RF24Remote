#ifndef __RF24Frontend__
#define __RF24Frontend__

#include "RF24Remote.h"
#include "RF24Com.h"

class RF24Frontend: public RF24Remote
{
private:
    RF24Com &com_device;
    uint8_t request_nr;
public:
    RF24Frontend(RF24Com &_dev):com_device(_dev) {};
    int callRemote(ERF24Command cmd);

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

};


#endif //__RF24Frontend__

