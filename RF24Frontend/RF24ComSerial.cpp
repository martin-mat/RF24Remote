#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <termios.h> // POSIX terminal control definitions 
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "RF24Remote.h"
#include "RF24ComSerial.h"
#define DEBUG printf
//#define DEBUG(args ...)

RF24ComSerial::RF24ComSerial(const char *ser_port)
{
    strncpy(sp, ser_port, 64);       
}


void RF24ComSerial::initialize(void)
{
    struct termios toptions;
    uint8_t buf[128];
    
    fd = open(sp, O_RDWR | O_NONBLOCK );
    if (fd == -1) 
        fatal(-1, "serialport_init: Unable to open port '%s'\n", sp);

    if (tcgetattr(fd, &toptions) < 0)
        fatal(-1, "serialport_init: Couldn't get term attributes\n");

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
        fatal(-1, "init_serialport: Couldn't set term attributes\n");

    sleep(2); 
    buf[0] = 4;
    buf[1] = 1;
    buf[2] = RF24REMOTE_CMD_VERSIONCHECK;
    buf[3] = 0;
    sendRequest(buf);
    getResponse(buf);
    if ((buf[0] != 4) || (buf[2] != RF24REMOTE_PROTOCOL_VERSION))
        fatal(-1, "serialport: wrong protocol received from device version\n");

}


void RF24ComSerial::sendRequest(uint8_t *buffer)
{

    uint8_t len = buffer[0];
    int n = write(fd, buffer, len);
    if( n!=len )
        fatal(-1, "serialport_write: couldn't write whole string\n");
    //tcflush(fd, TCIOFLUSH); // clear buffer
    return;
}

void RF24ComSerial::getResponse(uint8_t *buffer)
{
    char b[1]; // read expects an array, so we give it a 1-byte array
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

    return;
}




