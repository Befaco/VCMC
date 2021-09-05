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
#define DP(x) Serial.println(x)		/// Shorthand for Deug serial print
#else
#define D(x)
#define DP(x)
#endif
#define USEREADINTERR       /// Define to use interrupts for port reading
#define READANGINT          /// Read analog ports using interrupts
#define FILTEREDANAGINPUT   /// Filter analog inputs
#ifdef DEBUGMODE
const int TIMERINTSERVICE = 250; /// Microseconds between call to read ports service function servicePorts()
const int TIMERENCSERVICE = 1100; /// Microseconds between call to read encoder service function
#else
const int TIMERINTSERVICE = 125; /// Microseconds between call to read ports service function servicePorts()
const int TIMERENCSERVICE = 1100; /// Microseconds between call to read encoder service function
#endif
#ifdef USB_MIDI_SERIAL2
#define CVTHING 1
#endif
#define CVTHING 1
//#define USEOSC
//#define USECONFIGOSC
//#define USEI2C
const uint8_t FREQMEASUREACTIVE = 0;

#ifdef CVTHING
	const uint8_t SCREEN_OFFSET = 0xFF; /// Screen offset (Adjust based on screen manufacturer, lot, ...)
#else
	const uint8_t SCREEN_OFFSET = 0x00; /// Screen offset (Adjust based on screen manufacturer, lot, ...)
#endif

#define GATEPULLUP 1	/// Digital input type
const uint8_t NUMBERMISSEDCLOCKS = 3; /// Number of missed clock before stoppig MIDI clock

// Analog INputs resolution
const uint8_t ANBITS = 16;				/// Bits read 8on ADC during AnalogRead
const int16_t ANRANGEMAX = 4095;		/// Maximum range for Input. On Teensy out of the 16 bits read first 4 are discarded.
const int16_t DEFMINDAC = 4095;	/// Default CV Minimum DAC
const int16_t DEFRANGEDAC = -4095; /// Default CV Range DAC
const float DefFaderfilter = 0.001;  //0.00060;								  /// Filter for faders inputs
const uint16_t DefActivityThreshold = 8;  //17; /// Activity detection for Faders
#ifdef CVTHING
const uint8_t PINENCA = 7;
const uint8_t PINENCB = 6;
const uint8_t PINENCBUT = 12;
const uint8_t PINDEMUXA = 2;
const uint8_t PINDEMUXB = 3;
const uint8_t PINDEMUXC = 4;
const uint8_t PINCVINPUT = 22;
const uint8_t PINSLIDERINPUT = 16;
const uint8_t PINAUXA = 16;
const uint8_t PINAUXB = 16;
const uint8_t PINOFFSET = 23;
const uint8_t PINBUT[] = { 15, 15, 15, 15, 15, 15, 15, 15, 15};
const uint8_t PINLED[] = { 16, 16, 16, 16, 16, 16, 16, 16, 16 };
const uint8_t OLED_DC = 9;
const uint8_t OLED_CS = 10;
const uint8_t OLED_RESET = 8;
const uint8_t MOSI_PIN = 11;
const uint8_t SCK_PIN = 14;
const uint8_t I2C_SDA = 18;
const uint8_t I2C_SCL = 19;
#else
///@{
/// Pin definitions
const uint8_t PINENCA = 7;
const uint8_t PINENCB = 6;
const uint8_t PINENCBUT = 12;
const uint8_t PINDEMUXA = 33;
const uint8_t PINDEMUXB = 24;
const uint8_t PINDEMUXC = 32;
const uint8_t PINCVINPUT = 36;  //15
const uint8_t PINSLIDERINPUT = 37; //16
const uint8_t PINAUXA = 18;
const uint8_t PINAUXB = 26;
const uint8_t PINOFFSET = 23;
const uint8_t PINBUT[] = { 2, 3, 4, 5, 25, 31, 27, 28, 0};
const uint8_t PINLED[] = {22, 21, 20, 19, 17, 16, 15, 13, 0 };//13, 13, 13 };
const uint8_t OLED_DC = 9;
const uint8_t OLED_CS = 10;
const uint8_t OLED_RESET = 8;
const uint8_t MOSI_PIN = 11;
const uint8_t SCK_PIN = 14;
const uint8_t I2C_SDA = 30;
const uint8_t I2C_SCL = 29;
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

/// Divider from input clacks to MIDI clicks
#define INPUTCLKDIVIDER 6
/// Calibration modes
typedef enum { NoCalMode, MatrixCalMode, SetRangeMode, TwoPointCalMode, FadersCalMode, SetClockMode, FreqMeasure } CalModes;

/// Screen refresh rates
const uint32_t REFRESH_RATE  = 30000;	  /// Refresh screen portion every REFRESH_RATE usec
const uint32_t REFRESH_RATE2 = 20000; /// Refresh text screen portion every REFRESH_RATE2 usec

#ifdef USEOSC
/// OSC definitions
const uint32_t OSCINTERVAL = 50000;
const uint16_t TRIMOSC = 5; //10
#endif


///@{
/** Names for the ports */
const int NUMPORTNAMES = 47;
const int NUMUSERNAMES = 15;
const int SIZEPORTNAMES = 6;
const char FIRSTNAMECHAR = 32;
const char LASTNAMECHAR = 127;
///@}


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
