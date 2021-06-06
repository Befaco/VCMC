
#ifndef _ArduinoTools_h
#define _ArduinoTools_h


#include <Arduino.h>
/* #include <Encoder.h>
int16_t getEncPos();
int16_t readEncoder(void);

#include <Bounce2.h>
extern Bounce EncButt;
inline Bounce *getEncButton(void) { return &EncButt; }
void initEncoderButton();
bool encButSelMade(void);
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "./Menu/SSD1306FastText.h"
const uint8_t charW = 6, charH = 8;
const uint8_t textH2 = 2;
extern SSD1306FastText *disp;    ///< Display
SSD1306FastText *getDisplay();
 */


#define debPrintf Serial.printf
#define debPrintln Serial.println
#define debPrint Serial.print

#define getMicros micros
/**
 * \brief Get the Clock value
 * 
 * \param newval new initial value for the clock
 * \return uint32_t clock in 1/10th milliseconds
 */
uint32_t getClock(uint32_t newval = 0xffff);

#endif
