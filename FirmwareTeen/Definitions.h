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

#ifndef __have__Definitions_h__
#define __have__Definitions_h__

/** @addtogroup GeneralUtil 
 *  @{
 */

/**
 *  \file Definitions.h
 *  \brief Definitions and configuration flags 
 */


// Configuration flags
//#define DEBUGMODE  1  /// Define to use Debug screen and messages
#define PRINTDEBUG  /// Define to send debug messages n Serial port
#ifdef PRINTDEBUG
#define D(x) x		/// Shorthand for PRINTDEBUG
#else
#define D(x)
#endif
#define USEREADINTERR       /// Define to use interrupts for port reading
#define READANGINT          /// Read analog ports using interrupts
#define FILTEREDANAGINPUT   /// Filter analog inputs
#ifdef DEBUGMODE
const int TIMERINTSERVICE = 250; /// Microseconds between call to read ports service function servicePorts()
#else
const int TIMERINTSERVICE = 125; /// Microseconds between call to read ports service function servicePorts()
#endif
//#define USEOSC
//#define CVTHING 1

#ifdef CVTHING
	const uint8_t SCREEN_OFFSET = 0xFF; /// Screen offset (Adjust based on screen manufacturer, lot, ...)
#else
	const uint8_t SCREEN_OFFSET = 0x00; /// Screen offset (Adjust based on screen manufacturer, lot, ...)
#endif

#define GATEPULLUP 1	/// Digital input type
const uint8_t NUMBERMISSEDCLOCKS = 3; /// Number of missed clock before stoppig MIDI clock

// Analog INputs resolution
const uint8_t ANBITS = 16;				/// Bits read 8on ADC during AnalogRead
const int32_t ANRANGEMAX = 4095;		/// Maximum range for Input. On Teensy out of the 16 bits read first 4 are discarded.
const int32_t DEFMINDAC = ANRANGEMAX;	/// Default CV Minimum DAC
const int32_t DEFRANGEDAC = -ANRANGEMAX; /// Default CV Range DAC

#ifdef CVTHING
const byte PINENCA = 7;
const byte PINENCB = 6;
const byte PINENCBUT = 12;
const byte PINDEMUXA = 2;
const byte PINDEMUXB = 3;
const byte PINDEMUXC = 4;
const byte PINCVINPUT = 22;
const byte PINSLIDERINPUT = 16;
const byte PINAUXA = 16;
const byte PINAUXB = 16;
const byte PINOFFSET = 23;
const byte PINBUT[] = { 15, 15, 15, 15, 15, 15, 15, 15, 15};
const byte PINLED[] = { 16, 16, 16, 16, 16, 16, 16, 16, 16 };
const byte OLED_DC = 9;
const byte OLED_CS = 10;
const byte OLED_RESET = 8;
const byte MOSI_PIN = 11;
const byte SCK_PIN = 14;
#else
///@{
/// Pin definitions
const byte PINENCA = 7;
const byte PINENCB = 6;
const byte PINENCBUT = 12;
const byte PINDEMUXA = 33;
const byte PINDEMUXB = 24;
const byte PINDEMUXC = 32;
const byte PINCVINPUT = PIN_A12;  //15
const byte PINSLIDERINPUT = PIN_A13; //16
const byte PINAUXA = PIN_A4;
const byte PINAUXB = PIN_A15;
const byte PINOFFSET = 23;
const byte PINBUT[] = { 2, 3, 4, 5, 25, 31, 27, 28, 0};
const byte PINLED[] = {22, 21, 20, 19, 17, 16, 15, 13, 0 };//13, 13, 13 };
const byte OLED_DC = 9;
const byte OLED_CS = 10;
const byte OLED_RESET = 8;
const byte MOSI_PIN = 11;
const byte SCK_PIN = 14;
///@}
#endif

/// Number of ports in the VCMC module
const int NUMCHAN = 9;

///@{
/// Definitions for OLED screen
const int16_t POSXCARD = 0;
const int16_t COLUMNW = 8;
#define DRAWCOLUMNLINES 0
///@}

///@{
/** Names for the ports */
const int NUMPORTNAMES = 47;
const int NUMUSERNAMES = 15;
const int SIZEPORTNAMES = 6;
const char FIRSTNAMECHAR = 32;
const char LASTNAMECHAR = 127;
const char PortNames[][SIZEPORTNAMES+1] = {
	//"", "Volume", "Pan", "Balanc", "Other"
	"",
	"REVE_D", "REVE_T", "REVE_M", "DELAYT", "DELAYD", "DELAYM", 
	"NOISE ", "CUTOFF", "FEEDBK", "RESON ", "RING  ", "LFORAT",
	"LFOINI", "LFOSHA", "LFOMOD", "LFOTAR", "VCOOCT", "VCOSHA",
	"VCOFRE", "VCOTUN", "PORTAM", "BANK  ", "PROGCH", "MODUL ", 
	"PITCHB", "PAN   ", "VOL   ", "RELEAS", "ATTACK", "DECAY ",
	"SUSTAI", "RATE  ", "DEPTH ", "TIME  ", "INIT  ", "SHAPE ",
	"MOD   ", "TARGET", "BREATH", "BALANC", "EXPRES", "LEGATO",
	"ALLOFF", "OMNI  ", "PANIC ", "POLY  ", "MONO  "
	};
///@}

// Include files
#include "PrjIncludes.h"

/// Analog Functions names
const char *PortFuncStr[] = {
	"V/Oct", "Note", "CC#", "Vel", "Prog", "Bend", "Perc",
	"Clock", "ST/SP", "ST/PA", "ST/SP", "ST/PA",
	"Free", "NRPN7", "NRPN14", "No Func",
	"Gate", "Start", "Stop", "Cont", "CC Tr", "CC Lt", "Trig","Latch"};
/// Digital Functions names
const char *PortGateFuncStr[] = {"Note", "Latc", "CC", "CC"/*"CC Lt#"*/, "Clock", "ST/SP", "CNT/SP", "ST/SP Lt", "CNT/SP Lt", "Start", "Stop", "Continue","Panic","No Func"};
/// MIDI Clock multiplier texts
const char *ClokShiftTxt[] = { "x32", "x16", "x08", "x04", "x02", "x01", "/02", "/04", "/08", "/16", "/32"};
/// MIDI Clock multiplier texts
const char *RangeInTxt[] = { "+10V", "Bip.", "+5V"};
/// Divider from input clacks to MIDI clicks
#define INPUTCLKDIVIDER 6
/// Calibration modes
typedef enum { NoCalMode, MatrixCalMode, SetRangeMode, TwoPointCalMode, FadersCalMode, SetClockMode } CalModes;

/// Screen refresh rates
const uint32_t REFRESH_RATE  = 30000;	  /// Refresh screen portion every REFRESH_RATE usec
const uint32_t REFRESH_RATE2 = 20000; /// Refresh text screen portion every REFRESH_RATE2 usec

#ifdef USEOSC
/// OSC definitions
const uint32_t OSCINTERVAL = 50000;
const uint16_t TRIMOSC = 5; //10
#endif

/// Credit text for splah screen
#ifdef CVTHING
const char CreditText[] =
	"                    "
	"                    "
	"                    "
	"     CV Thing       "
	"    v1 23 Jun       "
	"   www.befaco.org";
#else
const char CreditText[] =
	"                    "
	"                    "
	"                    "
	"    VCMC v1.1       "
	"    22 May  2020    "
	"   www.befaco.org";
#endif

/// Credits graph 128x24 pixels
const unsigned char mini_splash [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xE3, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xC7, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x8F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x3F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFF, 0xFE, 0x7F, 0xDF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC3, 0xFC, 0x00, 0xFF, 0x9F, 0xC7, 0xC0, 0x7E, 0x3F, 0x07, 0xF0,
0x0F, 0xC3, 0xFC, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x1F, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0E, 0x00, 0x1C, 0x00, 0xFF, 0x00, 0x7F, 0x80, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xC3, 0xFC, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xCF, 0xFC, 0x00, 0x3F, 0x01, 0xFE, 0x00, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xDF, 0xFC, 0x00, 0x1F, 0xC3, 0xFC, 0x00, 0xFF, 0xFF, 0xC7, 0xEF, 0xFC, 0x3F, 0xF7, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFC, 0x00, 0xFF, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x0F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/// Notes text names
char NotesNames [][5]= {
	"C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
	"C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1", 
	"C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0", 
	"C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1", 
	"C2", "C#2", "D2", "D#2", "E2", "F2", "G2", "F#2", "G#2", "A2", "A#2", "B2", 
	"C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", 
	"C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", 
	"C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", 
	"C6", "C#6", "D6", "E6", "D#6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6", 
	"C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7", "C7"
};

/** @} */

/** @defgroup Maingroup The Main Application
 *  Main app functionalty and integration for the different modules
*/

/** @defgroup GeneralUtil General Tools
*  @ingroup Maingroup
*  Classes and functions for general purposes, utilities,...
*/

/** @defgroup InputPortsGroup Input ports management
*  @ingroup Maingroup
*  Classes for reading, coverting, storing,... input ports.
*/

/** @defgroup ConfigGroup Configuration storage and management
*  @ingroup Maingroup InputPortsGroup
*  Classes for storing the configuration data and functions to access, store and convert this data
*/

/** @defgroup MenuClass Menu management
*  @ingroup Maingroup
*  Classes for managing the user interface.
*/

/** @defgroup MenuClassDefs Menu items definition and response functions for selections
*  @ingroup MenuClass
*  Menu items definition and response functions for selections.
*/

/** @defgroup OSC OSC/I2C Tools
*  @ingroup GeneralUtil
*  Classes and functions for OSC and I2C input/output
*/

/** @defgroup MIDI MIDI Tools
*  @ingroup GeneralUtil
*  Classes and functions for MIDI input/output
*/

/** @defgroup SysEx  SysEx Tools
*  @ingroup GeneralUtil
*  Classes and functions for SysEx input/output
*/

/** @defgroup Save Save/Load Tools
*  @ingroup GeneralUtil
*  Classes and functions for input/output to EEPROM
*/

#endif
