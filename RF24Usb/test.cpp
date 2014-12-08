#include <stdio.h>
#include "RF24Usb.h"

void print_ascii(char *s)
{
    uint8_t c;
    while (c=*s++)
        printf("%02X", c);
    printf("\n");
}


int main(void)
{
     
    //const char *test = "\x08\x06\x07gabcdef\x02\x07\x00";
    const char *test = "\x07\x02\x04\x00";
    char o[32];
    RF24Usb sc(1,2);

    sc.write("abcd", 4);   
    print_ascii(sc.to_send);
}
