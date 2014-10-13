#include <avr/io.h>

// IR sensor connected to PB4 = ATtiny85 physical pin 3
#define IRPIN_PIN      PINB
#define IR_PIN          2

#define MAXPULSE 5000 // max IR pulse length, default 5 milliseconds
#define NUMPULSES 100 // max IR pulse pairs to sample
#define RESOLUTION 2 // time between IR measurements


extern void ir_setup();

extern long getNextIR();
