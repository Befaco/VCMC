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
#ifndef __have__VCMCApp_h__
#define __have__VCMCApp_h__
#include "PrjIncludes.h"

/** @addtogroup Maingroup 
 *  @{
 */
/** @file VCMCApp.h
 * \brief Main App declarations
 */

//#define DEPR_CONS
/// Main Class for the VCMC Application
class VCMCApp
{
public:
    byte byBankSelected;            ///< Channel selected in performance screen
    byte byPortSelected;            ///< Port selected in Menu (0= No port selected, 1= Gate, 2= CV, 3= Fader, )
    InputControl Controls[NUMCHAN]; ///< Main Input controls variable. Will be used to store configuration and functionaly for each 8 input controls and Aux inputs
    uint16_t lastEnc = -1, valueEnc = 30000;

    /** @name Modules integration
     *  Accesors for the different modules integrated in the main application.  
     */
    ///@{
public:
    GlobalCfg theGlobalCfg;         ///< Global configuration data
    OLEDMenu vMenu;                 ///< Menu to control the OLED display using quad encoder
    MIDImerger MidiMerg;            ///< Control both serial and USB MIDI outputs
//private:
    ADC *padc;                ///< ADC management object
    IntervalTimer PortsTimer; ///< Timer for port reading
    IntervalTimer myTimer;    ///< Service timer for encoder
    ClickEncoder *pEncoder;   ///< Encoder handling
    #ifdef DEPR_CONS
        Adafruit_SSD1306 disp;    ///< Display
    #else
        Adafruit_SSD1306 *disp;    ///< Display
    #endif
    SaveLoadClass FlashAcc; ///< Save/Load to EEPROM, SYSEX and OSC
    Bounce *pEncButt;       ///< Encoder Button handler
    ///@}

public:
    VCMCApp() : byBankSelected(1), byPortSelected(0)
    #ifdef DEPR_CONS
        , disp(OLED_DC, OLED_RESET, OLED_CS)
    #endif
    {
        padc = new ADC(); // adc object;
        pEncoder = new ClickEncoder(PINENCB, PINENCA, -1, 4);
        pEncButt = new Bounce();

    #ifdef DEPR_CONS
    #else
        SPI.begin();
        // Moce SCK from pin 13 to pin 14
        SPI.setSCK(14);
        disp= new Adafruit_SSD1306(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);
    #endif
    }
    void setup(void);
    void ApplyFilters(){
        for (int i = 0; i < NUMCHAN-1;i++){
            Controls[i].Slider.setFilter(theGlobalCfg.filterFader, theGlobalCfg.ActThrFader);
        }
    }

    ADC *getADC() { return padc; }
    byte getBankSelected() { return byBankSelected; }
    byte getPortSelected() { return byPortSelected; }
    ClickEncoder *getEncoder(void) { return pEncoder; }
    Bounce *getEncButton(void) { return pEncButt; }
    #ifdef USEOSC
    OSCmerger OSCMerge;             ///< Control sending OSC through Serial USB and I2C outputs
    SLIPEncodedUSBSerial *getSLIPSerial(void){ return OSCMerge.getSLIPSerial(); }
    #endif
    #ifdef USEI2C
    I2Cmerger I2CMerge;             ///< Control I2C inputs / outputs
    #endif
    int16_t getInitMinDAC() { return theGlobalCfg.InitMinDAC; }     ///< Return the global config minimum DAC value
    int16_t getInitRangeDAC() { return theGlobalCfg.InitRangeDAC; } ///< Return the global config minimum DAC range
    int16_t getInitAuxAMinDAC() { return theGlobalCfg.AuxAMinDAC; }     ///< Return the global config minimum DAC value for Aux A
    int16_t getInitAuxARangeDAC() { return theGlobalCfg.AuxARangeDAC; } ///< Return the global config minimum DAC range for Aux A
    int16_t getInitAuxBMinDAC() { return theGlobalCfg.AuxBMinDAC; }     ///< Return the global config minimum DAC value for Aux B
    int16_t getInitAuxBRangeDAC() { return theGlobalCfg.AuxBRangeDAC; } ///< Return the global config minimum DAC range for Aux B
    SaveLoadClass *getFlashAccess() { return &FlashAcc; }
    Adafruit_SSD1306 *getDisplay() { 
        #ifdef DEPR_CONS
            return &disp;
        #else
            return disp;
        #endif
     }
    InputPortCfg *GetPortConfig(void);

    void ProcessSysExCommand(VCMCSysExPacket *SysExPacket); ///< Process Global SysEx commands

private:
    void initControls(void);
    void initdisplay(void);
    void initEncoder(void);
    void initADC(void);
};

// Interrput handling definitions
extern volatile bool servicingPorts;
void timerIsr();
void servicePorts();
// Main object definition
extern VCMCApp theApp;

/** @} */ // end of Maingroup


#endif // __have__VCMCApp_h__
