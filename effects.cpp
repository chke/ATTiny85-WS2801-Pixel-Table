/**
* Some effects for WS2801 strands/strips
* Based on https://github.com/myelin/myelin-matrix-display/tree/master/attiny85_strand_tester
* Changed to display the correct pixels on the Table
*
* Copyright (C) 2012 Phillip Pearson
* You may use this code under a CC Attribution license.
*
* ... except for Color() and Wheel(), which are from the Adafruit_WS2081 library's "strandtest" example:
* https://github.com/adafruit/Adafruit-WS2801-Library
* Written by Limor Fried/Ladyada for Adafruit Industries.
* BSD license, all text above must be included in any redistribution
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "ws2801.h"

extern uint8_t* pixel_data;

void show() {
  ws2801_show(pixel_data);
}

#define delay _delay_ms

#define randbyte() ((uint8_t)random())

static long random(long a, long b) {
  return (random() % (b - a)) + a;
}

void wait_ms(uint8_t ms) {
  delay(ms);
}

// Calculates the real Table position, since the pixels on the table are placed in this order
uint16_t calcTablePos(uint16_t p) {
  return p - ((1-(p%7)%2) * (p%7 / 2)) + ((p%7)%2 * (7 - (p%7) - (p%7)/2 - 1));
}

void show_pixels() {
  ws2801_show(pixel_data);
}

void point(uint16_t p, uint8_t r, uint8_t g, uint8_t b) {
  p = calcTablePos(p);
  
  pixel_data[p * 3] = r;
  pixel_data[p * 3 + 1] = g;
  pixel_data[p * 3 + 2] = b;
}

void point(uint16_t p, uint32_t c) {
  p = calcTablePos(p);
  
  pixel_data[p * 3] = (uint8_t)(c >> 16);
  pixel_data[p * 3 + 1] = (uint8_t)(c >> 8);
  pixel_data[p * 3 + 2] = (uint8_t)c;
}

static void point(unsigned char x, unsigned char y, uint32_t c) {
  if (y & 1) {
    x = WIDTH - x; // odd numbered line - need to flip x
  }
  point((int)y * WIDTH + (int)x, c);
}

void blank() {
  for (int i = 0; i < N_PIXELS; ++i) point(i, 0);
}

// Create a 24 bit color value from R,G,B
uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(uint8_t WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 0, 255 - WheelPos * 3);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, WheelPos * 3, 0);
  } else {
   WheelPos -= 170;
   return Color(0, 255 - WheelPos * 3, WheelPos * 3);
  }
}

void showWheel(uint8_t firstWheelPos) {
  for (int i=0; i < N_PIXELS; i++) {
    point(i, Wheel( (i + firstWheelPos) % 255));
  }
  show();
}

void set_color(int r, int g, int b) {
  for (int i = 0; i < N_PIXELS; ++i) point(i, r, g, b);
}

int random_color() {
  return Color(randbyte(), randbyte(), randbyte());
}

void showRandomPixels(uint8_t num) {
  for (int i = 0; i < num; i++) {
    point(randbyte() % N_PIXELS, Wheel((randbyte()) % 255));
  }
}

void rainbow(uint8_t wait) {
  int i, j;

  for (j=0; j < 256; j++) { // 3 cycles of all 256 colors in the wheel
    for (i=0; i < N_PIXELS; i++) {
      point(i, Wheel( (i + j) % 255));
    }
    show(); // write all the pixels out
    delay(wait);
  }
}


