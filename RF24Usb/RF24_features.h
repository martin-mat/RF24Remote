#ifndef _RF24FEATURES
#define _RF24FEATURES

#define delay(ms) {for(int __i=0; __i<ms; __i++) _delay_us(1000);}
#define delayMicroseconds(us) {for(int __i=0; __i<us; __i+=10) _delay_us(10);}
#define NO_MILLIS
#define MILLIS_INTERVAL 10

#endif


