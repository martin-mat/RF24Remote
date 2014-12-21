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

RF24Usb rf24usb(4,5);

RF24UsbDevice::RF24UsbDevice(void)
{
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
        rf24usb.store(OPAR, buffer, buff_size);
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


