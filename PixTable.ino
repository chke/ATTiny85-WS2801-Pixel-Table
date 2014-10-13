#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <EEPROM.h>
#include <string.h>
#include "ws2801.h"
#include "ir.h"

// used to set empty EEPROM locations
#define EEPROMblank 255   
// largest EEPROM cell ID (ATTiny85)
#define EEPROMmax 511
// EEPROM LAYOUT:
// |1. Byte|2. |3. |4.   |5.  |2 - 5|n.                 |
// |Mode   |Pos|Red|Green|Blue|next |EEPROMblank if Last|

// Modes
// Mode: 4 = Show Pixels


uint8_t loopVal = 0;
int EEPROMidx = 1;
uint8_t pixel_data[WS2801_BUFFER_LEN];


// Constants für showing Numbers (Letters later=) 
// Binary coded i.e. 1 | 8 (16. Bit unused)
// 15 10 05    010    111
// 14 09 04    110    101
// 13 08 03    010    111
// 12 07 02    010    111
// 11 06 01    010    101
static uint16_t const  PIX_1 = 9184;
static uint16_t const  PIX_2 = 24253;
static uint16_t const  PIX_3 = 18111;
static uint16_t const  PIX_4 = 13028;
static uint16_t const  PIX_5 = 30386;
static uint16_t const  PIX_6 = 15031;
static uint16_t const  PIX_7 = 17144;
static uint16_t const  PIX_8 = 32447;
static uint16_t const  PIX_9 = 13998;
static uint16_t const  PIX_0 = 14894;

void setup() {
  ws2801_setup();
  memset(pixel_data, 128, WS2801_BUFFER_LEN); // WS2801_BUFFER_LEN
  ws2801_show(pixel_data);
  ir_setup();
  
  // Init EEPROM if not initialized
  if (EEPROM.read(0) != 4) {
    EEPROM.write(0, 4);
    EEPROM.write(1, EEPROMblank);
  }

}

extern uint32_t Wheel(uint8_t WheelPos);

extern void rainbow(uint8_t wait);

extern void showRandomPixels(uint8_t num);

extern void blank();

extern void show_pixels();

extern void point(uint16_t p, uint8_t r, uint8_t g, uint8_t b);

extern void point(uint16_t p, uint32_t c);

extern void wait_ms(uint8_t ms);

extern void showWheel(uint8_t firstWheelPos);

void loop() {
  uint8_t irKey=getNextIntFormIr(); // Wait for an IR Code as int
  
  if (irKey==1) { // Key 1
    rainbow(200);
  } else if (irKey==2) { // Key 2
    showWheel(loopVal);
    loopVal = loopVal > 255 ? 0 : (loopVal + 20);
  } else if (irKey==3) { // Key 3
    //showNumber(loopVal++);
    loadPixels();
  } else if (irKey==4) { // Key 4 Set Pixel
    setPixels();
  } else if (irKey==5) {
    loopVal = 0;
    if (EEPROM.read(1) != EEPROMblank && EEPROMidx > 1) {
      EEPROM.write(1, EEPROMblank);
      EEPROMidx = 1;
    }
    blank();
  } else if (irKey==6) {
    showRandomPixels(10);
  } else if (irKey==7) {
    showRandomPixels(5);
  } else if (irKey==8) {
    loopVal = loopVal > 255 ? 0 : (loopVal + 20);
    
    showInnerPixels(loopVal, 255 - loopVal, 255 / loopVal);
  } else if (irKey==9) {
    loopVal = loopVal > 255 ? 0 : (loopVal + 20);
    showOuterPixels(loopVal, 255 - loopVal, 255 / loopVal);
  } else if (irKey==0) {
    printIR();
  } else {
    show_pixels();
  }
}


void showOuterPixels(uint8_t red, uint8_t green, uint8_t blue) {
  point(16, red, green, blue);
  point(17, red, green, blue);
  point(18, red, green, blue);
  
  point(23, red, green, blue);
  point(24, red, green, blue);
  point(25, red, green, blue);
  
  point(30, red, green, blue);
  point(31, red, green, blue);
  point(32, red, green, blue);
  show_pixels();
}

void showInnerPixels(uint8_t red, uint8_t green, uint8_t blue) {
  point(48, red, green, blue);
  point(42, loopVal, green, blue);
  point(0, loopVal, green, blue);
  point(6, loopVal, green, blue);
  show_pixels();
}

void loadPixels() {
  uint8_t pos;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  EEPROMidx = 1;
  pos = EEPROM.read(EEPROMidx);
  for (;EEPROMidx < EEPROMmax && pos != EEPROMblank;EEPROMidx=EEPROMidx+4) {
    // If next pixel is stored
    red = EEPROM.read(EEPROMidx + 1);
    green = EEPROM.read(EEPROMidx + 2);
    blue = EEPROM.read(EEPROMidx + 3);
    point(pos, red, green, blue);
    pos = EEPROM.read(EEPROMidx + 4); // Read next pos to check if blank
  }
  show_pixels();
}

void printIR() {
  wait_ms(100);
  unsigned long irCode = getNextIR();
  for (int i = 0; i < 10; i++) {
    showNumber(irCode % 10);
    wait_ms(500);
    blank();
    show_pixels();
    wait_ms(200);
    irCode = irCode / 10;
  }
}

void showNumber(uint8_t num) {
  uint16_t toShow = 0;
  if (num == 1) {
    toShow = PIX_1;
  } else if (num == 2) {
    toShow = PIX_2;
  } else if (num == 3) {
    toShow = PIX_3;
  } else if (num == 4) {
    toShow = PIX_4;
  } else if (num == 5) {
    toShow = PIX_5;
  } else if (num == 6) {
    toShow = PIX_6;
  } else if (num == 7) {
    toShow = PIX_7;
  } else if (num == 8) {
    toShow = PIX_8;
  } else if (num == 9) {
    toShow = PIX_9;
  } else if (num == 0) {
    toShow = PIX_0;
  }
  
  for (int i = 4; i >= 1; --i) {
    
    for (int j = 5; j >= 1; --j) {
      if (toShow & 1 == 1) {
        point(i * 7 + j, 80, 80, 80);
      } else {
        point(i * 7 + j, 0, 0, 0);
      }
      toShow = toShow >> 1;
    }
  }
  show_pixels();
}

void setPixels() {
  uint8_t posX;
  uint8_t posY;
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  
  wait_ms(100);
  
  // Pos of pixel
  posX = waitForNextIntFormIr();
  wait_ms(100);
  posY = waitForNextIntFormIr();
  wait_ms(100);
  
  point(posX + posY * WIDTH, 255, 0, 0);
  show_pixels();
  
  // red
  red = readNextColor();
  
  point(posX + posY * WIDTH, 0, 255, 0);
  show_pixels();
  wait_ms(100);
  
  
  // Green
  green = readNextColor();
  
  point(posX + posY * WIDTH, 0, 0, 255);
  show_pixels();
  wait_ms(100);
  
  // Blue
  blue = readNextColor();
  
  // Store Color to EEPROM
  EEPROM.write(EEPROMidx++, posX + posY * WIDTH);
  EEPROM.write(EEPROMidx++, red);
  EEPROM.write(EEPROMidx++, green);
  EEPROM.write(EEPROMidx++, blue);
  EEPROM.write(EEPROMidx, EEPROMblank);
  point(posX + posY * WIDTH, red, green, blue);
  show_pixels();
  wait_ms(100);
}

uint8_t readNextColor() {
  uint8_t color = waitForNextIntFormIr();
  
  wait_ms(100);
  color = color * 10 + waitForNextIntFormIr();
  
  wait_ms(100);
  
  color = color * 10 + waitForNextIntFormIr();
  
  if (color > 255 || color < 0) {
    color = 255;
  }
  return color;
}

uint8_t waitForNextIntFormIr() {
  uint8_t irVal = getNextIntFormIr();
  return irVal;
}

uint8_t getNextIntFormIr() {
  unsigned long irCode = getNextIR();
  if (irCode==3225406515 || irCode==3718405219) { // 1
    return 1;
  } else if (irCode==3225404985 || irCode==3718404135) { // 2
    return 2;
  } else if (irCode==3225411233 || irCode==3718412295) { // 3
    return 3;
  } else if (irCode==3225404475 || irCode==3718401075) { // 4
    return 4;
  } else if (irCode==3225407025 || irCode==3718409235) { // 5
    return 5;
  } else if (irCode==3225409193 || irCode==3718405155) { // 6
    return 6;
  } else if (irCode==3225407663 || irCode==3718413315) { // 7
    return 7;
  } else if (irCode==3225408173 || irCode==3718401585) { // 8
    return 8;
  } else if (irCode==3225408683 || irCode==3718409745) { // 9
    return 9;
  } else if (irCode==3225410085 || irCode==3718400055) { // 0
    return 0;
  } else {
    return 10;
  }
}


int main() {
  setup();
  blank();
  while (1) loop();
}
