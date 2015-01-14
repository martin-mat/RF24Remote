#include <stdio.h>
#include <usb.h>
#include <time.h>
#include <stdarg.h>
#include "usbconfig.h"
#include "opendevice.h"
#include "RF24Remote.h"
#define DEBUG printf
//#define DEBUG(args ...)
#include "RF24ComVUsb.h"

#define USB_TIMEOUT 1000

void RF24ComVUsb::initialize(void)
{
    const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
    char vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
    int vid, pid;
    int ret;
    uint8_t buffer[128];

    usb_init();

    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];

    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
        fatal(-1, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);

    // exmptiy thei interrupt queue
    DEBUG("Emptying usb interrupt queue...");
    ret = usb_interrupt_read(handle, USB_ENDPOINT_IN | 1, (char *)buffer, sizeof(buffer), 100);
    DEBUG("done, returned %d.\n", ret);

    DEBUG("Verifying protocol version\n");
    ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, RF24REMOTE_CMD_VERSIONCHECK, 0, 0, (char *)buffer, sizeof(buffer), USB_TIMEOUT);
    DEBUG("usb_control_msg ret:%d\n", ret);
    if (ret != 1)
        fatal(-1, "usb client returned unexpected reply on version check, ret=%d\n", ret);

    if (buffer[0] != RF24REMOTE_PROTOCOL_VERSION)
        fatal(-1, "usb client returned wrong protocol version. Expected:%d, received:%d\n", RF24REMOTE_PROTOCOL_VERSION, buffer[0]);
}


void RF24ComVUsb::sendRequest(uint8_t *buffer)
{
    int ret;
    uint16_t lValue;
    uint16_t lIndex;

    lIndex = buffer[1];
    lValue = 0;
    DEBUG("USB send request usb_control_msg call\n");
    ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, buffer[2], lValue, lIndex, (char *)buffer + 3,  buffer[0] - 3, USB_TIMEOUT);
    DEBUG("usb_control_msg ret:%d\n", ret);
    if (ret<0)
        fatal(-1, "usb_control_msg sending usb data failed:%d\n", ret);
}


void RF24ComVUsb::getResponse(uint8_t *buffer)
{
    int ret;
    uint8_t buff_pos;

    buff_pos = 0;
    do
    {
        ret = usb_interrupt_read(handle, USB_ENDPOINT_IN | 1, (char *)buffer + buff_pos, 256, USB_TIMEOUT);
        DEBUG("callUsb usb_interrupt_read:%d, expected:%d\n", ret, buffer[0]);
        if (ret>0)
        {
            buff_pos += ret;
        }
    } while ((buff_pos < buffer[0]) && (ret > 0));

    if (ret<0)
        fatal(-1, "usb_interrupt_read failed:%d '%s'\n", ret, usb_strerror());

}




