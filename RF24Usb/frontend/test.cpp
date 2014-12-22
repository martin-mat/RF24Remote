#include "RF24Usb.h"

RF24Usb rf(1,2);
char teststr[]={12, 0};
char dest[256];
uint8_t ln;

int main(void)
{   
    rf.parse(IPAR, teststr);
    rf.store(OPAR, dest, ln);
}

