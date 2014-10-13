/**
* Based on TinyPCRemote (removed array for irCodes)
* By Nathan Chantrell http://nathan.chantrell.net
* Receives infrared codes and emulates a USB keyboard for remote control
* https://github.com/nathanchantrell/TinyPCRemote
*
*/

#include "ir.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

uint8_t currentpulse = 0; // index for pulses

void ir_setup() {
  pinMode(IR_PIN, INPUT); // Make sure IR pin is set as input
}


// IR receive code
long getNextIR() {  
  currentpulse = 0;
  uint32_t irCode = 0;
  uint16_t highpulse, lowpulse;  // temporary storage timing
  while (currentpulse < 32) {
    highpulse = lowpulse = 0; // start out with no pulse length 
    
    while (IRPIN_PIN & _BV(IR_PIN)) { // got a high pulse
      highpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return 0;
      }
    }
    
    while (! (IRPIN_PIN & _BV(IR_PIN))) { // got a low pulse
      lowpulse++; 
      delayMicroseconds(RESOLUTION);
      if (((lowpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
        return 0; 
      }
    }
    if (currentpulse < 32) {
      if((highpulse * RESOLUTION)>0&&(highpulse * RESOLUTION)<500) {
        irCode|=0; 
      } else {
        irCode|=1;
      }
    }
    if (currentpulse < 31) {
      irCode=irCode<<1;
    }
    currentpulse++;
  }
  return irCode;
}

