// Copyright 2019 Sergio Retamero.
//
// Author: Sergio Retamero (sergio.retamero@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//

#ifndef __have__PrjIncludes_h__
#define __have__PrjIncludes_h__

/** @addtogroup GeneralUtil 
 *  @{
 */

/**
 *  \file PrjIncludes.h
 *  \brief Includes for the project come here 
 */
#include <Arduino.h>
#include "Definitions.h"

#include <Adafruit_GFX.h>
#ifdef ST3375SCR
#include <ST7735_t3.h>
#define display() updateScreen()
#define dim(dimStatus) setCursor(0,0)
// Color definitions
#define BLACK ST7735_BLACK
#define WHITE ST7735_WHITE
#else
#include <Adafruit_SSD1306.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <limits.h>
#include <MIDI.h>
#include <EEPROM.h>
#include <Bounce2.h>
#include "src/encoder-master/ClickEncoder.h"

#include <ADC.h>
#include <ResponsiveAnalogRead.h>


#include "DefStrings.h"

// Code abreviations
//#define INITMINDAC getInitMinDAC()
//#define INITRANGEDAC getInitRangeDAC()
#define INITMINDAC theApp.getInitMinDAC()
#define INITRANGEDAC theApp.getInitRangeDAC()
#define adc theApp.getADC()					// ADC management object
#define BankSelected theApp.byBankSelected  // Channel selected in performance screen
#define PortSelected theApp.byPortSelected  // Port selected in Menu (0= No port selected, 1= Gate, 2= CV, 3= Fader, )
#define encoder theApp.getEncoder()			// Encoder
#define theOLED theApp.getDisplay()			// Display
#define FlashAccess theApp.getFlashAccess() // Flash Load /Save
#define myMenu theApp.vMenu
#define fullCVDisplay myMenu.fullDisplay
#define gotoGloblaMenu myMenu.gotoGMenu
#define gotoGlobalCal myMenu.gotoGCal
#define SetMessageText myMenu.SetMessage
#define MidiMerge theApp.MidiMerg
#define CVControls theApp.Controls
#define lastEncoder theApp.lastEnc
#define valueEncoder theApp.valueEnc
#define EncButton theApp.getEncButton()
#define durationButton myMenu.msDurationButton
#define GetPortCfg theApp.GetPortConfig
#define EncoderchangeValue myMenu.EncoderChangeLong
#define EncChangeVal myMenu.EncoderChangeVal
#define EncChangeValF myMenu.EncoderChangeValFloat
#define SetValState myMenu.MultiValState
#define AutoCal myMenu.FactoryCal
#define CalTimer myMenu.FactoryCalTimer
#define GlobalCalEnabled myMenu.GlobalCalEn

#ifdef USEOSC
// OSC
#include "SLIPEncodedUSBSerial.h"
#include <OSCMessage.h>
#include "OSCMerge.h"
#endif
#ifdef USEI2C

extern void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l);
#include "src/ops/ops.h"
#include "vcmcop.h"
#include "src/ops/I2CDevice.h"
#include "src/ops/I2CCore.h"
#include "I2CMerge.h"
#endif

#include "MIDIChord.h"

#include "Blinker.h"
#include "MultiPointConv.h"
#include "SysExDef.h"
#include "MenuClass.h"
#include "OLEDMenu.h"
#include "CfgClasses.h"
#include "InputPort.h"
#include "InputControl.h"
#include "SaveLoad.h"
#include "MIDImerger.h"
#include "VCMCApp.h"
#include "MenuLists.h"
#include "ParTypeMap.h"
#include "ParTypeMap.h"

/** @} */


#endif
