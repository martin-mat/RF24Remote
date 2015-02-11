#define _RF24_FW
#include "RF24RemoteBackend.h"
#include "nRF24L01.h"

#ifdef __cplusplus
extern "C"{
#endif
#include "usbdrv.h"
#ifdef __cplusplus
} // extern "C"
#endif

#include "RF24VUsb.h"

uint8_t buffer[256];
uint8_t buff_pos;
uint8_t buff_remaining;
uint8_t buff_size;
uint8_t command_to_execute;

RF24VUsb rf24vusb;

RF24VUsb::RF24VUsb(void)
{
};

void RF24VUsb::setup(void)
{
    // disable timer 0 overflow interrupt (used for millis)
    TIMSK0&=!(1<<TOIE0);

    buff_pos = 0;
    buff_remaining = 0;
    buff_size = 0;
    command_to_execute = 0;

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

void RF24VUsb::update(void)
{
    uint8_t bytes_to_write;
    usbPoll();
    if (command_to_execute)
    {
        command_to_execute = 0;
        rf24vusb.parse(IPAR, buffer+1);
        rf24vusb.executeCommand();
        rf24vusb.store(OPAR, buffer+2, &buff_size);
        buffer[1] = buffer[0]; // command index
        buffer[0] = buff_size+2; // size
        buff_size += 2;
        buff_pos = 0;
        bytes_to_write = 8;

        while (bytes_to_write == 8)
        {
            usbPoll();
            if(usbInterruptIsReady()){               // only if previous data was sent
                bytes_to_write = ((buff_size - buff_pos)>8)?8:(buff_size - buff_pos);
                usbSetInterrupt(buffer+buff_pos, bytes_to_write);
                buff_pos += bytes_to_write;
            }
        }
    }
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- RF24Remote overrides ---------------------- */
/* ------------------------------------------------------------------------- */

#define WRITE_LOOPS_PER_MS (10*(F_CPU/1000000))
bool RF24VUsb::write(const void* buf, uint8_t len, const bool multicast)
{
    //Start Writing
    startFastWrite(buf,len,multicast);
    //Wait until complete or failed
    uint32_t timeout_counter = 0;
    while( ! ( get_status() & ( _BV(TX_DS) | _BV(MAX_RT) ))) 
    {
        if(timeout_counter > 85 * WRITE_LOOPS_PER_MS)
        {
            errNotify();
            return 0;
        }
        usbPoll();
    }
    ce(LOW);
    uint8_t status = write_register(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );
    //Max retries exceeded
    if( status & _BV(MAX_RT))
    {
        flush_tx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
        return 0;
    }
    //TX OK 1 or 0
    return 1;
}
#undef WRITE_LOOPS_PER_MS

#define WRITEFAST_LOOPS_PER_MS (10*(F_CPU/1000000))
bool RF24VUsb::writeFast(const void* buf, uint8_t len, const bool multicast)
{
    //Block until the FIFO is NOT full.
    //Keep track of the MAX retries and set auto-retry if seeing failures
    //Return 0 so the user can control the retrys and set a timer or failure counter if required
    //The radio will auto-clear everything in the FIFO as long as CE remains high
    uint32_t timeout_counter = 0;
    while( ( get_status() & ( _BV(TX_FULL) ))) 
    { //Blocking only if FIFO is full. This will loop and block until TX is successful or fail
        timeout_counter++;
        if( get_status() & _BV(MAX_RT))
        {
            //reUseTX(); //Set re-transmit
            write_register(STATUS,_BV(MAX_RT) ); //Clear max retry flag
            return 0; //Return 0. The previous payload has been retransmitted
            //From the user perspective, if you get a 0, just keep trying to send the same payload
        }
        if(timeout_counter > WRITEFAST_LOOPS_PER_MS * 85)
        {
            errNotify();
            return 0;
        }
        usbPoll();
    }

    //Start Writing
    startFastWrite(buf,len,multicast);
    return 1;
}
#undef WRITEFAST_LOOPS_PER_MS

#define WRITEBLOCKING_LOOPS_PER_MS (10*(F_CPU/1000000))
bool RF24VUsb::writeBlocking(const void* buf, uint8_t len, uint32_t timeout)
{
    //Block until the FIFO is NOT full.
    //Keep track of the MAX retries and set auto-retry if seeing failures
    //This way the FIFO will fill up and allow blocking until packets go through
    //The radio will auto-clear everything in the FIFO as long as CE remains high
    uint32_t timeout_counter = 0;
    while ((get_status() & ( _BV(TX_FULL)))) 
    { //Blocking only if FIFO is full. This will loop and block until TX is successful or timeout
        timeout_counter++;
        if (get_status() & _BV(MAX_RT))
        { //If MAX Retries have been reached
            reUseTX(); //Set re-transmit and clear the MAX_RT interrupt flag
            if(timeout_counter > timeout * WRITEBLOCKING_LOOPS_PER_MS)
            {
                return 0; 
            } //If this payload has exceeded the user-defined timeout, exit and return 0
        }
        if (timeout_counter > (timeout+85) * WRITEBLOCKING_LOOPS_PER_MS)
        {
            errNotify();
            return 0;
        }
        usbPoll();
    }
    //Start Writing
    startFastWrite(buf,len,0); //Write the payload if a buffer is clear
    return 1; 
}
#undef WRITEBLOCKING_LOOPS_PER_MS

#define TX_STANDBY_LOOPS_PER_MS (10*(F_CPU/1000000))
bool RF24VUsb::txStandBy()
{
    uint32_t timeout_counter = 0;
    while( ! (read_register(FIFO_STATUS) & _BV(TX_EMPTY)) )
    {
        timeout_counter++;
        if( get_status() & _BV(MAX_RT))
        {
            write_register(STATUS,_BV(MAX_RT) );
            ce(LOW);
            flush_tx(); //Non blocking, flush the data
            return 0;
        }
        if( timeout_counter > TX_STANDBY_LOOPS_PER_MS * 85)
        {
            errNotify();
            return 0;
        }
        usbPoll();
    }
    ce(LOW); //Set STANDBY-I mode
    return 1;
}
#undef TX_STANDBY_LOOPS_PER_MS

#define TX_STANDBYTIMEOUT_LOOPS_PER_MS (10*(F_CPU/1000000))
bool RF24VUsb::txStandBy(uint32_t timeout, bool startTx)
{
    if(startTx)
    {
        stopListening();
        ce(HIGH);
    }

    uint32_t timeout_counter = 0;
    while( ! (read_register(FIFO_STATUS) & _BV(TX_EMPTY)) )
    {
        timeout_counter++;
        if( get_status() & _BV(MAX_RT))
        {
            write_register(STATUS,_BV(MAX_RT) );
            ce(LOW); //Set re-transmit
            ce(HIGH);
            if(timeout_counter >= TX_STANDBYTIMEOUT_LOOPS_PER_MS * timeout)
            {
                ce(LOW); flush_tx(); return 0;
            }
        }
        if( timeout_counter > TX_STANDBYTIMEOUT_LOOPS_PER_MS * (timeout+85))
        {
            errNotify();
            return 0;
        }
        usbPoll();
    }
    ce(LOW); //Set STANDBY-I mode
    return 1;
}
#undef TX_STANDBYTIMEOUT_LOOPS_PER_MS


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

    buffer[0] = rq->wIndex.bytes[0];
    buffer[1] = rq->bRequest;
    buff_pos = 2;
    buff_remaining = rq->wLength.word;
   
    if (buff_remaining > sizeof(buffer)) // limit to buffer size
        buff_remaining = sizeof(buffer);
    return USB_NO_MSG; 
}

uchar usbFunctionWrite(uchar *data, uchar len)
{
    uint8_t i;

    if(len > buff_remaining)                // if this is the last incomplete chunk
        len = buff_remaining;               // limit to the amount we can store
    buff_remaining -= len;

    for (i=0; i<len; i++)
        buffer[buff_pos++] = data[i];

    if (buff_remaining == 0)
        command_to_execute = 1;
    return buff_remaining == 0;
}


#ifdef __cplusplus
} // extern "C"
#endif


