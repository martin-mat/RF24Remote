#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <termios.h> // POSIX terminal control definitions 
#include "RF24Usb.h"
#define DEBUG printf
//#define DEBUG(args ...)
#include "RF24ComSerial.h"

void RF24ComSerial::initialize(void)
{
    struct termios toptions;
    
    fd = open(serialport, O_RDWR | O_NONBLOCK );
    if (fd == -1) 
        fatal(-1, "serialport_init: Unable to open port ");

    if (tcgetattr(fd, &toptions) < 0)
        fatal(-1, "serialport_init: Couldn't get term attributes");

    speed_t brate = B115200;

    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;
    //toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid reset
    toptions.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN] = 0;
    toptions.c_cc[VTIME] = 0;
    //toptions.c_cc[VTIME] = 20;
    tcsetattr(fd, TCSANOW, &toptions);
    if( tcsetattr(fd, TCSAFLUSH, &toptions) < 0) 
        fatal("init_serialport: Couldn't set term attributes");


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
    ret = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN, USB_CMD_VERSIONCHECK, 0, 0, (char *)buffer, sizeof(buffer), USB_TIMEOUT);
    DEBUG("usb_control_msg ret:%d\n", ret);
    if (ret != 1)
        fatal(-1, "usb client returned unexpected reply on version check, ret=%d\n", ret);

    if (buffer[0] != USB_PROTOCOL_VERSION)
        fatal(-1, "usb client returned wrong protocol version. Expected:%d, received:%d\n", USB_PROTOCOL_VERSION, buffer[0]);
}


void RF24ComSerial::sendRequest(uint8_t *buffer)
{

    uint8_t len = str[0];
    int n = write(fd, str, len);
    if( n!=len )
        fatal(-1, "serialport_write: couldn't write whole string\n");
    return 0;
}

void RF24ComSerial::getResponse(uint8_t *buffer)
{
    char b[1]; // read expects an array, so we give it a 1-byte array
    int i=0;
    int tries = 2000;
    int n;
    do
    {
        n = read(fd, b, 1); // read a char at a time
        if( n==-1)
            fatal(-1, "serial read failed - 1st byte\n");
        if( n==0 ) {
            usleep(1000); // wait 1 msec try again
            tries--;
        };
    } while ((n==0) && (tries>0));
    if (n==0)
        fatal(-1, "serial read did not get reply\n");

    DEBUG("serial read length: %d\n",b[0]);

    buffer[0] = b[0];
    read(fd, buffer+1, b[0]-1);

    return 0;
}
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




