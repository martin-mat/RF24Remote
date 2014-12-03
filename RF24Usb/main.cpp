#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h> /* for sei() */
#include <util/delay.h> /* for _delay_ms() */
#include <avr/pgmspace.h> /* required by usbdrv.h */
#include "usbdrv.h"
#include "oddebug.h" /* This is also an example for using debug macros */
#include "requests.h" /* The custom request numbers we use */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t *rq = (void *)data;
    static uchar dataBuffer[4]; /* buffer must stay valid when usbFunctionSetup returns */
    if (rq->bRequest == CUSTOM_RQ_ECHO)
    { /* echo -- used for reliability tests */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer; /* tell the driver which data to return */
        return 4;
    } else if (rq->bRequest == CUSTOM_RQ_SET_STATUS)
    {
        if(rq->wValue.bytes[0] & 1){ /* set LED */
            LED_PORT_OUTPUT |= _BV(LED_BIT);
        } else { /* clear LED */
            LED_PORT_OUTPUT &= ~_BV(LED_BIT);
        }
    } else if (rq->bRequest == CUSTOM_RQ_GET_STATUS)
    {
        dataBuffer[0] = ((LED_PORT_OUTPUT & _BV(LED_BIT)) != 0);
        usbMsgPtr = dataBuffer; /* tell the driver which data to return */
        return 1; /* tell the driver to send 1 byte */
    }
    return 0; /* default for not implemented requests: return no data back to host */
}



int __attribute__((noreturn)) main(void)
{
    uchar i;
    wdt_enable(WDTO_1S);
    /* If you don't use the watchdog, replace the call above with a wdt_disable().
    * On newer devices, the status of the watchdog (on/off, period) is PRESERVED
    * OVER RESET!
    */
    /* RESET status: all port bits are inputs without pull-up.
    * That's the way we need D+ and D-. Therefore we don't need any
    * additional hardware initialization.
    */
    odDebugInit();
    DBG1(0x00, 0, 0); /* debug output: main starts */
    usbInit();
    usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i)
    { /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();
    LED_PORT_DDR |= _BV(LED_BIT); /* make the LED bit an output */
    sei();
    DBG1(0x01, 0, 0); /* debug output: main loop starts */
    for(;;)
    { /* main event loop */
        DBG1(0x02, 0, 0); /* debug output: main loop iterates */
        wdt_reset();
        usbPoll();
    }   
}
