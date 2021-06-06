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

#ifndef INPUTPORT_H_
#define INPUTPORT_H_
/** @addtogroup InputPortsGroup
* @{
*/

/**
 *  \file InputPort.h
 *  \brief Definition for Port classes InputPort, DigitalPort, AnalogPort and DemuxAnalogPort
 */

/// Default analog input change threshold
#define DEFTHERSHOLD 12


//////////////////////////////
// Input port management
/// Base class for input ports (digital, analog,...)
class InputPort {
    public:
    byte PortNumber;                 ///< Pin for input
    InputPortCfg PortCfg;
    uint16_t PortValue;              ///< Last value read from port
    int16_t MIDIData;                ///< Input data converted to MIDI
    int16_t LastSentMIDIData = -999; ///< Last data sent to MIDI port
    bool GateStatus;                 ///< Current status of Gate for Digital. Used for triggered values in Anag
	long msecLastMIDISent = 0;           ///< Time in msecs last MIDI message was sent
    // Clock vars
    unsigned long IntervalMIDIClock = 0, ///<
	  IntervalClock = 0,                   ///<
    CurrentClock = 0,                    ///<
    PreviousClock = 0,                   ///<
    PreviousMIDIClock = 0;               ///<
    byte ClockCounter = 0;               ///<
    // virtual int getClockDivider(void);
    // ResponsiveAnalogRead FilteredInterval( 0, false, 0.1);
    ResponsiveAnalogRead *FilteredInterval; ///<

    bool debounceGate;              ///< True to debounce gate
    bool lastButtonState;           ///< Previous reading from the input pin
    uint32_t lastDebounceTime; ///< Last time the output pin was toggled (in usecs)
    uint32_t debounceDelay = 0;  ///< the debounce time in usecs ; increase if the output flickers

    InputPort () {
        PortNumber = 0;
        PortValue = 0;
        MIDIData = 0;
        GateStatus = 0; //}
        FilteredInterval = new ResponsiveAnalogRead (0, false, 0.0001);
        FilteredInterval->setAnalogResolution (65535);
        FilteredInterval->setActivityThreshold(56.0);//256.0);//

        // Debounce digital inputs parameters 
        debounceDelay = 500;
        lastButtonState = LOW;
        lastDebounceTime = 99999;
        debounceGate = true;
    }
    ///< Set Port. TBD in derived classes
    bool SetPort (byte PortNumber) { return false; }
    ///< Read port. TBD in derived classes
    bool ReadPort (long &NewData) { return false; }
    // void SendMIDI(int MidiData=-9999, bool GateStat=false)=0;// {}
    bool ClockReceived (float clkDiv, int8_t clkShift);
	void ProcessClock(void);
    virtual InputPortCfg *getCfg() { return &PortCfg; }

    uint32_t trigTime = 0; //< Time set for trigger
    bool trigStatus = 0; //< Status to set after trigger
    // Trigger change in input (delay in msecs)
    uint32_t setTrigger( uint16_t del, bool newstatus)
    {
        trigTime = micros() + del*1000;
        trigStatus = newstatus;
        return trigTime;
    }
};

/// Class for digital ports (Gates)
class DigitalPort : public InputPort {
    public:
    DigPortCfg PortCfg; ///< Configuration data for the DigitalPort class

    byte LedPort = -1; ///< Pin Port for the Led
    Blinker blinkPort; ///< Used to blink led asssociated to the Gate

    void setBlink (unsigned long periodon, unsigned long periodoff, int times);
    DigitalPort () {
    }
    bool SetPort (byte PortN, byte Ledp);
    bool ReadPort (long &NewData);
    void SendMIDI (int MidiData = -9999, bool GateStat = false);
    ///< Return the member InputPortCfg::ClockDivider
    float getClockDivider (void) { return PortCfg.ClockDivider; }
    int8_t getClockShift (void) { return PortCfg.ClockShift; }
    bool ClockReceived (void);

    int SaveCfg (int addr);
    int LoadCfg (int addr);
    int parse(uint8_t type, uint8_t *buf, int buLen);
    int parseFunctionData(uint8_t *buf, int buLen);
    int fill(uint8_t type, uint8_t *buf, int buLen);
    int fillFunctionData(uint8_t *buf, int buLen);

    #ifdef USEI2C
    void SendI2C(int MidiData, bool GateStat); // Definition in I2CCommands.cpp
    #endif
    
    virtual InputPortCfg *getCfg() { return &PortCfg; }
};


/// Class for analog ports (CV and Faders)
class AnalogPort : public InputPort {
    private:

    public:
    bool typeSlider = false;
    AnInputPortCfg PortCfg; ///< Configuration info for the Port
    #ifdef FILTEREDANAGINPUT
    ResponsiveAnalogRead *FilteredInput; ///<  Filtered data for the port
    void setFilter(float filter, uint16_t Thres){
        FilteredInput->setSnapMultiplier (filter);
        FilteredInput->setActivityThreshold(Thres);
        }
    #endif
    int8_t adcNum = -1;
	
    AnalogPort () {
        #ifdef FILTEREDANAGINPUT
        FilteredInput =
        new ResponsiveAnalogRead (0, true, 0.01); // Filter parameters: nochange, true/false, 0.0 - 1.0
        FilteredInput->setAnalogResolution (ANRANGEMAX);
        FilteredInput->setActivityThreshold (2); // Filter parameter: 0 - ANRANGEMAX
        #endif
        MIDIData = 0;
    }
    bool SetPort (byte PortN);
    bool ReadPort (long &NewData);
    void SendMIDI (int MidiData = -9999, bool GateStat = false);
    int TrimValue (int DatatoTrim);
    ///< Return the member InputPortCfg::ClockDivider
    float getClockDivider (void) { return PortCfg.ClockDivider; }
    int8_t getClockShift (void) { return PortCfg.ClockShift; }
    bool ClockReceived (void);
    ///< Return true if input is bigger than half the range
    bool IsHigh (void) {
        int16_t minM, rangeM;
        PortCfg.Ranges.getMIDI (minM, rangeM);
        return (MIDIData > minM+rangeM / 3) ? true : false;
    }
    int SaveCfg (int addr);
    int LoadCfg (int addr);
    int parse(uint8_t type, uint8_t *buf, int buLen);
    int parseFunctionData(uint8_t *buf, int buLen);
    int fill(uint8_t type, uint8_t *buf, int buLen);
    int fillFunctionData(uint8_t *buf, int buLen);
    #ifdef USEI2C
    void SendI2C(int MidiData, bool GateStat); // Definition in I2CCommands.cpp
    #endif

    virtual InputPortCfg *getCfg() { return &PortCfg; }
};


/**
 *  \brief Class for Muxed analog ports (CV and Faders)
 *  Demux will use defines for pins: PINDEMUXA, PINDEMUXB, PINDEMUXC
 */
class DemuxAnalogPort : public AnalogPort {
    public:
    uint8_t DemuxPort; ///<  3 bit value to select in Demux 4051 or similar*/

    DemuxAnalogPort () { DemuxPort = 0; }
    bool SetPort (byte DemuxVal, byte PortN);
    bool ReadPort (long &NewData);
    void SetDemux (void);
    ///< Call base AnalogPort::SendMIDI() function
    void SendMIDI (int MidiData = -9999, bool GateStat = false) {
        AnalogPort::SendMIDI (MidiData, GateStat);
    }
};


/** @} */ 

#endif /* INPUTPORT_H_ */
