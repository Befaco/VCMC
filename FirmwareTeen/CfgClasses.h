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

#ifndef CFGCLASSES_H_
#define CFGCLASSES_H_
/** @addtogroup ConfigGroup
  @{
*/

/**
    \file CfgClasses.h
    \brief Definition of enums and classes for Input configuration
*/


//////////////////////////////////////////
// Configuration classes Input Control
/// Global Configuration for VCMC module
class GlobalCfg {
  public:
    uint8_t initPage;
    int16_t InitMinDAC;   ///< Minimum default CV DAC value
    int16_t InitRangeDAC; ///< Minimum default CV DAC range
    int16_t FaderMinDAC;   ///< Minimum default Fader DAC value
    int16_t FaderRangeDAC; ///< Minimum default Fader DAC range
    int16_t AuxAMinDAC;   ///< Minimum default Aux A DAC value
    int16_t AuxARangeDAC; ///< Minimum default Aux A DAC range
    int16_t AuxBMinDAC;   ///< Minimum default Aux B DAC value
    int16_t AuxBRangeDAC; ///< Minimum default Aux B DAC range
    uint16_t AutoOff;      ///< Note Off after n milliseconds
    float       ClockDivider = 1.2;         ///< Clack divider: Divides the interval by this number for fine clock adjustment
    int8_t      ClockShift = 0;         ///< Clack multiplier: Multiplies(+) / Divides(-) by power of 2

    union
    {
      struct {
        bool EncoderDir: 1; ///< Encder Diretion default=0, reversed = 1)
        uint8_t Unused: 7;
      };
      uint8_t GenOptions1;
    };

    float filterFader = DefFaderfilter;
    uint8_t ActThrFader = DefActivityThreshold;

    char UserNames[NUMUSERNAMES][SIZEPORTNAMES + 1] = { { 0 } };
    ////////////////////////////////
    // TODO Global Config Candidates
    // uint16_t DACPoints[21];

    GlobalCfg ():
      initPage(0),
      InitMinDAC (ANRANGEMAX),
      InitRangeDAC (-ANRANGEMAX),
      FaderMinDAC (0),
      FaderRangeDAC (ANRANGEMAX),
      AuxAMinDAC (ANRANGEMAX),
      AuxARangeDAC(-ANRANGEMAX),
      AuxBMinDAC (ANRANGEMAX),
      AuxBRangeDAC(-ANRANGEMAX),
      AutoOff(1000),
      EncoderDir(1)
    {
      filterFader = DefFaderfilter;
      ActThrFader = DefActivityThreshold;
    }
    int SaveCfg();//int addr);
    int LoadCfg();//int addr);
    int SetPage(int page);
#ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
    void ReadCfgOSC(OSCMessage *pMsg);
#endif
    void SaveCfgSysEx (uint8_t par = 0);
    bool ReadCfgSysEx(byte *DecodedData, unsigned int decLen, uint8_t bPort);

};

//////////////////////////////////////////
// Configuration classes Input Control
/// Enumerate available Functions for each of the NUMCHAN InputControl
typedef enum ICFun_e {
  INDEP,   ///< Each Input acts independently
  SUM,     ///< Sum CV and Fader Inputs
  MULTIPL, ///< Multiply CV anf Fader inputs. If Fader is set to Percent, it will act as percentage of CV.
  NOTEMODE ///< Note Mode: CV is the pitch, Fader is the velocity and Gate is the trigger
} CtrlFunctions;

/// InputControl configuration
class InputCtrlCfg {
  public:
    CtrlFunctions Chanfunction; ///< Function selected for the input control
    uint8_t ScaleId;
    uint8_t ChordType;
    uint8_t InvDrop;
    union 
    {
        struct{
            uint16_t delayFix;
            uint16_t delayRnd;
        };
        uint32_t delayChord=0;
    };    

    InputCtrlCfg (): Chanfunction (INDEP) {}
    InputCtrlCfg (int chanF) {
      Chanfunction = (CtrlFunctions)chanF;
    }

    void SaveCfgSysEx (uint8_t par = 0, uint8_t chan = 0);
    bool ReadCfgSysEx(byte *DecodedData, unsigned int decLen);
};

//////////////////////////////////////////
// Configuration classes
// PERCENT =

/// MIDI Functions for analog ports
typedef enum IPFun_e {
  PITCHTRIG,         ///< Note Mode triggered by gate
  PITCH,             ///< Dude mode: Send note off/on for each input change
  CONTROLCHANGE,     ///< MIDI Control Change message
  VELOCITY,          ///< Velocity Mode. Will be used when sending a Note On on the same channel.
  PROGRAMCHANGE,     ///< MIDI Program Change message
  AFTERTOUCH,        ///< MIDI After Pressure message
  PITCHBEND,         ///< MIDI Pitch Bend message
  PERCENT,           ///< Set value to be between 0 and 100. This apply to the slider value only when the function of the Bank is Multiply
  ANAGCLOCK,         ///< Send MIDI clock
  ANAGSTARTSTOP,     ///< Start/Stop MIDI messages based on High /Low value of input
  ANAGPAUSECONT,     ///< Pause/Stop MIDI messages based on High /Low value of input
  ANAGTRIGSTARTSTOP, ///< Start/Stop MIDI messages triggered on High value of input
  ANAGTRIGPAUSECONT, ///< Pause/Stop MIDI messages triggered on High value of input
  ANAGFREEVALUE,
  ANAGNRPN7bits,     ///< Send 7 bits NRPN values
  ANAGNRPN14bits,    ///< Send 14 bits NRPN values
  NOANFFUNC,
  PITCHLEVEL,			///< Not in use in VCMC. On CVThing used to mark paired inputs on V/oct mode
  ANAGSTART,         ///< Send MIDI Start
  ANAGSTOP,          ///< Send MIDI Stop
  ANAGCONTINUE,      ///< Send MIDI Continue
  ANAGCCBUT,             ///< Send CC value on  High, set to 0 value on Low
  ANAGCCLATCH,           ///< Send CC value or 0 value on High value
  ANAGTRIGGER,           ///< CV as trigger
  ANAGLATCH,             ///< CV Latched trigger. flips output on High value
  PITCH8TRIG,         ///< Input 8 serves as gate for all other 7 CV with this function
  CC14BITS,

  SCALE_DEF,
  CHORDTYPE_DEF,
  CHORDINVERSION,
  SCALEROOT,
  CHORDDELAYFIX,
  CHORDDELAYRAND
} InputFunctions;

/// MIDI Functions for digital ports
typedef enum GFun_e {
  TRIGGER,           ///< Gate as trigger
  LATCH,             ///< Latched trigger. flips output on High value
  CCBUT,             ///< Send CC value on  High, set to 0 value on Low
  CCLATCH,           ///< Send CC value or 0 value on High value
  GATECLOCK,         ///< MIDI Clock
  GATESTARTSTOP,     ///< Start/Stop MIDI messages based on High /Low value of input
  GATEPAUSECONT,     ///< Pause/Stop MIDI messages based on High /Low value of input
  GATETRIGSTARTSTOP, ///< Start/Stop MIDI messages triggered on High value of input
  GATETRIGPAUSECONT, ///< Pause/Stop MIDI messages triggered on High value of input
  GATESTART,         ///< Send MIDI Start
  GATESTOP,          ///< Send MIDI Stop
  GATECONTINUE,      ///< Send MIDI Continue
  GATEPANIC,         ///< Send MIDI Panic
  NODIGFUNC,
  GATE8TRIG,         ///< On VCMC Gate 8 serves as gate for all other 7 CV
  CHORD              ///< Chord generation mode
} GateFunctions;



/// Base port configuration class
class InputPortCfg {
  public:
    byte        MIDIChannel = 1;        ///< MIDI channel for port messages
    uint8_t     ControllerNumber = 1;   ///< MIDI CC Controller number
    uint8_t     NoteToSend = 60;        ///< Note to send on Note mode
    uint8_t     ControllerValue = 40;   ///< MIDI CC Controller Value
    float       ClockDivider;           ///< Clack divider: Divides the interval by this number for fine clock adjustment
    int8_t      ClockShift = 0;         ///< Clack multiplier: Multiplies(+) / Divides(-) by power of 2
    uint8_t     charPrefix = 0;
    uint8_t     textPort = 0;
    uint8_t     charSufix = 0;
    uint16_t    DelayGate = 0;          ///< Delay in msecs for gate change (minimu time to accept new value). Used for debouncing.

#ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
    bool ReadCfgSysEx(byte *DecodedData, unsigned int decLen);
#endif
    InputPortCfg ():
      ClockDivider (1.0)
    {}
    InputPortCfg (byte MIDIChan, byte CCN, byte nSend = 60, byte ccVal = 40, float clkDiv = 1.0, int8_t clkSh = 0):
      MIDIChannel (MIDIChan),
      ControllerNumber (CCN),
      NoteToSend(nSend),
      ControllerValue(ccVal),
      ClockDivider (clkDiv),
      ClockShift (clkSh)
    {}
    uint8_t getName(char *name);
};

enum
{
  NOOFFSET,
  MINUSPLUS5V,
  ZEROTO5V
};


/// Analog port configuration class
class AnInputPortCfg : public InputPortCfg {
  public:
    union
    {
      struct {
        uint8_t MIDIfunction: 6; ///< MIDI function as defined on InputFunctions enumerator
        uint8_t RangeBipolar: 2; ///< Apply -5Volts pffset to input amplifier: 0 No Offset, 1: -5/5 V, 2:
        //bool IsParamFunction: 1;  ///< When set, use the parameter change function
      };
      uint8_t Options1;
    };

    union
    {
      struct {
//          uint8_t RangeBipolar: 2; ///< Apply -5Volts pffset to input amplifier: 0 No Offset, 1: -5/5 V, 2:
          uint8_t DestPort : 2;      ///< Port to change parameter: Gate/Fader/CV
          uint8_t DestCtrl : 4;     /// Control number: 0-8 + 9=Aux
      };
      uint8_t Options2=0;
    };

    RangeConv Ranges;      ///< Configuration for ADC and MIDI with conversion functions
    int16_t ClipLow;   ///< Clip MIDI to this minimum value
    int16_t ClipHigh;        ///< Clip MIDI to this maximum value
    uint8_t NRPNparMSB;
    uint8_t NRPNparLSB;
    uint16_t AutoOff = 1000;    ///< Note Off after n milliseconds
    int8_t TransposeOctave;     ///< Transpose V/Oct upwards by octaves      (-10 -> 10 octaves)
    int8_t TransposeSemitones;  ///< Transpose V/Oct up or down by semitones (-12 -> +12 semitones)
#ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
    void ReadCfgOSC(OSCMessage *pMsg);
    void ReadBaseCfgOSC(OSCMessage *pMsg);
#endif
    void SaveCfgSysEx (uint8_t par = 0, uint8_t chan = 0, uint8_t slot = 0);
    bool ReadCfgSysEx(byte *DecodedData, unsigned int decLen);

    AnInputPortCfg ():
      MIDIfunction (PITCHTRIG),
      RangeBipolar(NOOFFSET),
      ClipLow(0), ClipHigh(120),
      NRPNparMSB(0x7f), NRPNparLSB(0x7f),
      AutoOff(1000)
    {}
    AnInputPortCfg (byte MIDIChan, byte CCN, uint8_t MIDIf,
                    byte RangeBip = 0, uint8_t NRPNMSB = 0x7f , uint8_t NRPNLSB = 0x7f,
                    int16_t ClipL = 0, int16_t ClipH = 120, float clkDiv = 1.0, int8_t clkSh = 0, byte ccVal = 0, byte nSend = 60):
      InputPortCfg( MIDIChan, CCN, nSend, ccVal, clkDiv, clkSh ),
      MIDIfunction (PITCHTRIG),
      RangeBipolar(RangeBip),
      NRPNparMSB(NRPNMSB), NRPNparLSB(NRPNLSB),
      AutoOff(1000)
    {
      SetMIDIFunc(MIDIf);
      ClipLow = ClipL; ClipHigh = ClipH;
    }
    void SetMIDIFunc (uint8_t Func);
    void LimitValues (int16_t &minv, int16_t &maxv);
    bool IsDigitalFunc(void);
    uint8_t getInputRange() {
      return RangeBipolar;
    }
    void setInputRange(uint8_t InRange) {
      RangeBipolar = InRange;
    }
};

/// Digital port configuration class
class DigPortCfg : public InputPortCfg {
  public:
    uint8_t MIDIfunction; ///< MIDI function as defined on GateFunctions enumerator

#ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
    void ReadCfgOSC(OSCMessage *pMsg);
    void ReadBaseCfgOSC(OSCMessage *pMsg);
#endif
    void SaveCfgSysEx (uint8_t par = 0, uint8_t chan = 0, uint8_t slot = 0);
    bool ReadCfgSysEx(byte *DecodedData, unsigned int decLen);
    void SetMIDIFunc (uint8_t Func);
    DigPortCfg () :
      MIDIfunction (TRIGGER)
    {} // LATCH;}
    DigPortCfg (byte MIDIChan, byte CCN, float clkDiv, int8_t clkSh,
                uint8_t MIDIf, byte ccVal, byte nSend):
      InputPortCfg( MIDIChan, CCN, nSend, ccVal, clkDiv, clkSh ),
      MIDIfunction(MIDIf)
    {
      SetMIDIFunc(MIDIf);
    }
};


/**@}*/

#endif /* CFGCLASSES_H_ */
