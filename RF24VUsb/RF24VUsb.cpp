#include "RF24Remote.h"

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

void RF24VUsb::begin(void)
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
        rf24vusb.executeCommand(usbPoll);
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
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"{
#endif
/* ------------------------------------------------------------------------- */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (usbRequest_t *)data;

    if (rq->bRequest == RF24REMOTE_CMD_VERSIONCHECK) // version check
    {
        buffer[0] = RF24REMOTE_PROTOCOL_VERSION;
        usbMsgPtr = buffer;
        return 1;
    } else 
    {
        buffer[0] = rq->wIndex.bytes[0];
        buffer[1] = rq->bRequest;
        buff_pos = 2;
        buff_remaining = rq->wLength.word;
   
        if (buff_remaining > sizeof(buffer)) // limit to buffer size
            buff_remaining = sizeof(buffer);
        return USB_NO_MSG; 
    }
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


