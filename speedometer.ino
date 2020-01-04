// Dina Pham, Nhi Trinh
// BIM174 Final Project - color coded bike-light speedometer
// 11/13/2019
//
// **need to install following libraries:
// u8glib
// http://wiki.sunfounder.cc/index.php?title=OLED-SSD1306_Module
// https://github.com/olikraus/u8glib/wiki
//
// Adafruit NeoPixel
// https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation
//
// inspiration taken from:
// arduino bike speedometer w serial.print()
// by Amanda Ghassaei 2012
// https://www.instructables.com/id/Arduino-Bike-Speedometer/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 */

#include <Wire.h>                             // I2C
#include "U8glib.h"                           // OLED library (SSD1306)
#include <Adafruit_NeoPixel.h>                // NeoPixels LED library

// bike speed
// max_speed = 35mph = 616 in/s
// max rps = max_speed/circumference

#define reedPin 10                            // pin connected to reed switch
#define ledPin 6                              // pin connected to LED NeoPixels
#define ledCount 30                           // NeoPixel LED count

int reedVal;                                  // stores high value when reed switch is magnetized
int maxReedCounter = 100;                     // min time (in ms) of one rotation (for debouncing)
int reedCounter;

long timer;                                   // time between one full rotation (in ms)
float prevMph;                                // stores previous mph value for determining when bike is slowing down
float mph;                                    // stores mph for OLED display
float radius = 13;                            // tire radius (in inches)
float circumference;                          // circumference = pi*2*r

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C OLED constructor call
char temp[10];                                // for printing mph value as a string in OLED

// NeoPixel strip object
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRB + NEO_KHZ800);
// NeoPixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void colorWipe(uint32_t color, int wait) {   // color input is strip.Color(R, G, B) 0-255
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);           // set pixel color (in RAM) one a time (pixels are not cleared between colors)
    strip.show();                            // update strip
    delay(wait);                             // delay in ms
  }
}

void setup(){
  strip.begin();                             // initialize NeoPixel strip object
  strip.show();                              // turn off all pixels 
  strip.setBrightness(50);                   // set brightness to about 1/5 (max = 255)
  
  u8g.setFont(u8g_font_unifont);
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);                  // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);                    // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);                    // NeoPixels on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(reedPin, INPUT);                   // set reed switch pin as input
  
  //----------TIMER SETUP----------- 
  // the timer interrupt allows precise timed measurements of the reed switch
  // for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli(); //stop interrupts

  // set timer1 interrupt at 1 kHz
  TCCR1A = 0;                                // set entire TCCR1A register to 0
  TCCR1B = 0;                                // same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1 khz increments
  OCR1A = 1999;                              // = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei(); // allow interrupts
  //---------END TIMER SETUP---------
  
  Serial.begin(9600);                         // for debugging
}

ISR(TIMER1_COMPA_vect) {                      // interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reedPin);             // get val of reed switch
  if (reedVal){                               // if reed switch is closed
    if (reedCounter == 0){                    // min time between pulses has passed
      prevMph = mph;                          // set current mph as previous mph to determine when bike is slowing down     
      mph = (56.8*float(circumference))/float(timer); // calculate miles per hour
      timer = 0;                              // reset timer
      reedCounter = maxReedCounter;           // reset reedCounter
    } else {
      if (reedCounter > 0){                   // don't let reedCounter go negative
        reedCounter -= 1;                     // decrement reedCounter
      }
    }
  } else { // if reed switch is open
    if (reedCounter > 0){                     // don't let reedCounter go negative
      reedCounter -= 1;                       // decrement reedCounter
    }
  }
  
  if (timer > 2000){
    mph = 0;                                  // if no new pulses from reed switch in 2 sec--tire is still, set mph/prevMph to 0
    prevMph = 0;
  } else {
    timer += 1;                               // increment timer
  } 
}

void displayMPH() {
  // OLED picture loop
  u8g.firstPage();  
  do {
      u8g.drawStr(0, 22, itoa(mph, temp, 10)); // position of lower right pixel of string (origin is upper left corner) (0, 22) is centered font
  } while( u8g.nextPage() );

  Serial.print(prevMph);                      // print to Serial Monitor
  Serial.print("\t");
  Serial.println(mph);                        
  Serial.println("");
}

void loop() {
  if ((mph <= 5) & (mph < prevMph)) {
    colorWipe(strip.Color(200, 225, 0), 5);  // yellow = slowing down (and drops below 6 mph)
  } else if (mph == 0) {
    colorWipe(strip.Color(255, 0, 0), 5);    // red = stop
  } else {
    colorWipe(strip.Color(0, 255, 0), 5);    // green = go
  }
  
  displayMPH();                              // print mph to OLED/Serial Monitor
}
