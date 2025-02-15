#ifndef ESP32_APP__LED_MATRIX
#define ESP32_APP__LED_MATRIX

// code from: https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA

#include <Arduino.h>
#include "xtensa/core-macros.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include <FastLED.h>

#define BAUD_RATE 115200 // serial debug port baud rate

// HUB75E pinout
// R1 | G1
// B1 | GND
// R2 | G2
// B2 | E
//  A | B
//  C | D
// CLK| LAT
// OE | GND

#define BL1 25
#define BL2 14
#define G1 27
#define G2 13
#define R1 26
#define R2 12

#define CH_A 23
#define CH_B 19
#define CH_C 5
#define CH_D 17
#define CH_E 32 // assign to any available pin if using panels with 1/32 scan
#define CLK 16
#define LAT 4
#define OE 15

// Configure for your panel(s) as appropriate!
// #define PIN_E 5
#define PIN_E 32
#define PANEL_WIDTH 128
#define PANEL_HEIGHT 64 // Panel height of 64 will required PIN_E to be defined.

#define PANELS_NUMBER 1 // Number of chained panels, if just a single panel, obviously set to 1

#define PANE_WIDTH PANEL_WIDTH *PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT
#define NUM_LEDS PANE_WIDTH *PANE_HEIGHT

MatrixPanel_I2S_DMA *matrix = nullptr;

// patten change delay
#define PATTERN_DELAY 2000

uint16_t time_counter = 0, cycles = 0, fps = 0;
unsigned long fps_timer;

// gradient buffer
CRGB *ledbuff;
//

unsigned long t1, t2, s1 = 0, s2 = 0, s3 = 0;
uint32_t ccount1, ccount2;

uint8_t color1 = 0, color2 = 0, color3 = 0;
uint16_t x, y;

const char *str = "* ESP32 I2S DMA *";

void led_matrix_buffclear(CRGB *buf)
{
  memset(buf, 0x00, NUM_LEDS * sizeof(CRGB)); // flush buffer to black
}

void led_matrix_setup(uint8_t bightness)
{

  Serial.begin(BAUD_RATE);
  Serial.println("Starting pattern test...");

  HUB75_I2S_CFG::i2s_pins _pins = {R1, G1, BL1, R2, G2, BL2, CH_A, CH_B, CH_C, CH_D, CH_E, LAT, OE, CLK};
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANELS_NUMBER, _pins);

  mxconfig.driver = HUB75_I2S_CFG::FM6126A; // for panels using FM6126A chips ICN2038S

  matrix = new MatrixPanel_I2S_DMA(mxconfig);
  matrix->begin();
  matrix->setBrightness8(bightness);

  ledbuff = (CRGB *)malloc(NUM_LEDS * sizeof(CRGB)); // allocate buffer for some tests
  led_matrix_buffclear(ledbuff);

  // matrix->fillScreenRGB888(255, 0, 0);
  // delay(2000);
  matrix->clearScreen();
  matrix->setTextSize(1);     // size 1 == 8 pixels high
  matrix->setTextWrap(false); // Don't wrap at end of line - will do ourselves

  // matrix->setCursor(5, 5);
  // const char *str = "Ready";
  // matrix->print(str);
}

void led_matrix_clearScreen()
{
  matrix->clearScreen();
}

void led_matrix_write(int16_t x, int16_t y, char *str)
{
  matrix->setCursor(x, y);  
  matrix->print(str);
}

#endif