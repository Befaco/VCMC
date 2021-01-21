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

#ifndef InputControl_H_
#define InputControl_H_

/** @addtogroup InputPortsGroup
* @{
*/

/**
 *  \file InputControl.h
 *  \brief Definition for class InputControl to aggregate one CV, Fader and Gate inputs 
 */

/////////////////////////////////////////
// Main class
/** 
 *  \brief InputControl class provides the core functionaly for each block of CV, Fader and Gate
 *  The function InputControl stores the data and define the functionaly for each of the 8 blocks
 *  and the Auxiliar ports in the VCMC.
 *  In addition to call each of the ports, defines the functionality of the interaction between them
 *  i.e. CV and Gate working together to send Note On / Note off
 */
class InputControl {
    public:
    byte ControlNumber; /**< \brief Control Number in the rack. Valid numbers 0 to NUMCHAN-1  */
    DemuxAnalogPort CVPort; /**< \brief Control the CV port*/
    DemuxAnalogPort Slider; /**< \brief Control the Fader port*/
    DigitalPort GateBut; /**< \brief Control the Gate port*/
    InputCtrlCfg Config; /**< \brief Var to store the configuration for the Input Controls*/
    MIDIChord Chord;
    
    #ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
	  void ReadCfgOSC(OSCMessage *pMsg);
    #endif
    void SaveCfgSysEx (uint8_t par=0);
    bool ReadCfgSysEx(VCMCSysExPacket *SysExPacket, byte *DecodedData, unsigned int decLen);
    
    // Mark when data on the controls changed
    bool CVchanged, /**< \brief True when the CV port has changed from last call to function DoChanges*/
		Slidchanged, /**< \brief True when the Fader port has changed from last call to function DoChanges*/
		Gatechanged; /**< \brief True when the Gate port has changed from last call to function DoChanges*/

    InputControl () {
        ControlNumber = -1;
        CVchanged = Slidchanged = Gatechanged = false;
    }
    bool SetControlNumber (byte CtrlNum);
    bool SetPort (byte PortCV, byte PortSlid, byte PortGate, byte Ledp);
    bool ReadPorts (bool onlyDig = false);
    bool ReadPorts (bool readDig, bool readCV, bool readFader);
    bool DoChanges (void);
    void OnDataChange (void);
    void ProcessGateNotes(void);
    void ProcessGateChord(void);
    void SendNoteOn(byte controlNumber, InputPort &port, byte chan, int datatosend, bool chord=true);
    void SendLastNoteOff(byte controlNumber, InputPort& port, byte chan, bool chord=true);
    int DataCalc (void);
    bool GetGateState ();
    void Blink ();
    int SaveCfg (int addr);
    int LoadCfg (int addr);
};

/**@}*/

#endif // InputControl_H_
