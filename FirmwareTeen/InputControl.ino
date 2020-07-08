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
#include "Definitions.h"

/** @addtogroup InputPortsGroup
* @{
*/
/**
 *  \file InputControl.ino
 *  \brief Functions definition for Input Control class 
 */

/**
 *  \brief Helper function to initialize the Controller number
 *
 *  \param [in] CtrlNum Port number for the module
 *  \return 
 *
 *  \details 
 */
bool InputControl::SetControlNumber (byte CtrlNum) {
    ControlNumber = CtrlNum;
    SetPort (CtrlNum, CtrlNum, PINBUT[CtrlNum], PINLED[CtrlNum]);
    return true;
}

/**
 *  \brief Initialization function to set port values
 *
 *  \param [in] PortCV   port number for CV
 *  \param [in] PortSlid port number for Slider
 *  \param [in] PortGate port number for Gate
 *  \param [in] Ledp     port number for led
 *  \return TRUE if succesful
 *
 *  \details Calls child CV,Slider and gate SetPort functions.\n
 *	Defines custom filter parameters for Faders
 */
bool InputControl::SetPort (byte PortCV, byte PortSlid, byte PortGate, byte Ledp) {

    if (ControlNumber == 8) { // Aux inputs
        CVPort.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxAMinDAC, theApp.theGlobalCfg.AuxARangeDAC);
        Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxBMinDAC, theApp.theGlobalCfg.AuxBRangeDAC);
        return CVPort.SetPort (PortCV, PINAUXA) & Slider.SetPort (PortSlid, PINAUXB) &
               GateBut.SetPort (0, 0);
    } else { // CV and sliders
        // CV port uses reversed values
        //CVPort.PortCfg.Ranges.setMultiPointMode (true); // Use MultiPoint for CV
        // CVPort.PortCfg.Ranges.SetDAC( 4148, -4145);  // Hack for current values in DAC
        CVPort.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.InitMinDAC, theApp.theGlobalCfg.InitRangeDAC);
        Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.FaderMinDAC, theApp.theGlobalCfg.FaderRangeDAC);
        #ifdef FILTEREDANAGINPUT
        // Set parameters for FilteredInput
        Slider.FilteredInput->setSnapMultiplier (0.0005);
        Slider.FilteredInput->setActivityThreshold (12);
        //Slider.FilteredInput->enableEdgeSnap();
        #endif
    }

    return CVPort.SetPort (PortCV, PINCVINPUT) & Slider.SetPort (PortSlid, PINSLIDERINPUT) &
           GateBut.SetPort (PortGate, Ledp);
}


/**
 *  \brief Read selected ports and store  data change
 *  
 *  \param [in] readDig True to read digital port (Gate)
 *  \param [in] readCV True to read CV port
 *  \param [in] readFader True to read Fader port
 *  \return TRUE if data has changed
 *  
 *  \details 
 *  Read each of the port with the function input\n
 *  See individual port read functions for more details
 */
bool InputControl::ReadPorts (bool readDig, bool readCV, bool readFader) {
    long CVData;
    bool changed;

    #ifndef CVTHING
    long SlidData, GateSt;
    if (readDig) Gatechanged |= GateBut.ReadPort (GateSt);
    if (readFader) Slidchanged |= Slider.ReadPort (SlidData);
    #endif
    if (readCV) CVchanged |= CVPort.ReadPort (CVData);

    changed = CVchanged | Slidchanged | Gatechanged;

    return changed;
}

/**
 *  \brief Read  ports and react on data change
 *
 *  \return TRUE if data has changed
 *
 *  \details Execute functions for each input control module
 */
bool InputControl::DoChanges (void) {
    bool changed = CVchanged | Slidchanged | Gatechanged;
    if (!servicingPorts)
        if (changed) {
            OnDataChange ();
            CVchanged = Slidchanged = Gatechanged = 0;
        }

    return changed;
}

/**
 *  \brief Read  ports and react on data change
 *
 *  \return TRUE if data has changed
 *
 *  \details Main loop function for each input control module, see individual port read functions for more details 
 *	@note Deprecated, use InputControl::ReadPorts(bool readDig, bool readCV, bool readFader) instead
 */
bool InputControl::ReadPorts (bool onlyDig) {
    long CVData, SlidData, GateSt;
    bool changed;

    if (servicingPorts) {
        if (!onlyDig) {
            CVchanged |= CVPort.ReadPort (CVData);
        }
        Gatechanged |= GateBut.ReadPort (GateSt);
    } else
        Slidchanged |= Slider.ReadPort (SlidData); // Service Fader out of interrupts

    changed = CVchanged | Slidchanged | Gatechanged;

    // if( !onlyDig) // Do not process during digital port reading
    if (!servicingPorts)
        if (changed) {
            OnDataChange ();
            CVchanged = Slidchanged = Gatechanged = 0;
        }

    return changed;
}


/**
 *  \brief Provides functionality when inputs or gate change
 *
 *  \details Core function for actions based on inputs changes
 */
void InputControl::OnDataChange (void) {
    int SendData = DataCalc ();
    bool GateStat = GetGateState ();

    // Blink LED
    // GateBut.setBlink(100, 100, 1);

    // Gate change management
    if (Gatechanged) {
        if (Config.Chanfunction == NOTEMODE) { // In NOTEMODE send NoteOn/NoteOff once per trigger
            if (GateStat == true) { // Send Note On
                byte datatosend = CVPort.TrimValue (CVPort.MIDIData);
                if( GateBut.MIDIData){ // sendNoteOff previous note
					MidiMerge.sendNoteOff (GateBut.MIDIData, 0, CVPort.PortCfg.MIDIChannel);
					GateBut.MIDIData=0;}
                GateBut.MIDIData = datatosend; // Store Note in CVPort at gate activation
                MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[CVPort.PortCfg.MIDIChannel - 1],
                                      CVPort.PortCfg.MIDIChannel);
                // GateBut.setBlink(100, 100, 1);
            } else { // Send Note off
                MidiMerge.sendNoteOff (GateBut.MIDIData, 0, CVPort.PortCfg.MIDIChannel);
				GateBut.MIDIData=0;
            }
        } 
        else if (GateBut.PortCfg.MIDIfunction == TRIGGER || GateBut.PortCfg.MIDIfunction == LATCH) { // In TRIGGER or LATCH send NoteOn/NoteOff with every change on inputs
            if (GateStat == true) { // Send Note On	
				// Independ channels
				boolean played = false;
				if (Config.Chanfunction != INDEP) {
                    // If SUM or MULTIP send result through CV channel if it has the trigger option
                    if (CVPort.PortCfg.MIDIfunction == TRIGGER) {
                        byte datatosend = CVPort.TrimValue (SendData);
						if( CVPort.LastSentMIDIData>0){ // sendNoteOff previous note
							MidiMerge.sendNoteOff (GateBut.MIDIData, 0, CVPort.PortCfg.MIDIChannel);
							CVPort.LastSentMIDIData=-999;}
						CVPort.LastSentMIDIData = datatosend;
                        MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[CVPort.PortCfg.MIDIChannel - 1],
                                              CVPort.PortCfg.MIDIChannel);
						played = true;
                        // GateBut.setBlink(100, 100, 1);
                    }
                } else {

                    if (CVPort.PortCfg.MIDIfunction == PITCHTRIG) { // CV triggered notes
                        byte datatosend = CVPort.TrimValue (CVPort.MIDIData);
						if( CVPort.LastSentMIDIData>0){ // sendNoteOff previous note
							MidiMerge.sendNoteOff (CVPort.LastSentMIDIData, 0, CVPort.PortCfg.MIDIChannel);
							CVPort.LastSentMIDIData=-999;}
						CVPort.LastSentMIDIData= datatosend; // Store Note in CVPort at gate activation
                        MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[CVPort.PortCfg.MIDIChannel - 1],
                                              CVPort.PortCfg.MIDIChannel);
						played = true;
                    }
					if (Slider.PortCfg.MIDIfunction == PITCHTRIG) { // Fader triggered notes
                        byte datatosend = Slider.TrimValue (Slider.MIDIData);
						if( Slider.LastSentMIDIData>0){ // sendNoteOff previous note
							MidiMerge.sendNoteOff (Slider.LastSentMIDIData, 0, Slider.PortCfg.MIDIChannel);
							Slider.LastSentMIDIData=-999;}
                        Slider.LastSentMIDIData = datatosend; // Store Note in Slider at gate activation
                        MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[Slider.PortCfg.MIDIChannel - 1],
                                              Slider.PortCfg.MIDIChannel);
						played = true;
                    } 
                }
				if(!played){ // Gate play notes
					byte datatosend = GateBut.PortCfg.NoteToSend;
					if( GateBut.MIDIData){ // sendNoteOff previous note
						MidiMerge.sendNoteOff (GateBut.MIDIData, 0, GateBut.PortCfg.MIDIChannel);
						GateBut.MIDIData=0;}
					GateBut.MIDIData = datatosend; // Store Note in CVPort at gate activation
					MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[GateBut.PortCfg.MIDIChannel - 1], GateBut.PortCfg.MIDIChannel);
					}

            } 
            else { // Send Note Off
				boolean played = false;
                // If SUM or MULTIP send result through CV channel if it has the trigger option
                if (Config.Chanfunction != INDEP) {
                    if (CVPort.PortCfg.MIDIfunction == TRIGGER){
                        MidiMerge.sendNoteOff (CVPort.LastSentMIDIData, 0, CVPort.PortCfg.MIDIChannel);
						CVPort.LastSentMIDIData=0;
						played = true;
						}
                }
                // Independ channels
                else {
                    if (CVPort.PortCfg.MIDIfunction == PITCHTRIG){
						played = true;
                        MidiMerge.sendNoteOff (CVPort.LastSentMIDIData, 0, CVPort.PortCfg.MIDIChannel);
						CVPort.LastSentMIDIData=-999;}
                    if (Slider.PortCfg.MIDIfunction == PITCHTRIG){
						played = true;
                        MidiMerge.sendNoteOff (Slider.LastSentMIDIData, 0, Slider.PortCfg.MIDIChannel);
						Slider.LastSentMIDIData=-999;}
                }
				if(!played) {
					MidiMerge.sendNoteOff (GateBut.MIDIData, 0, GateBut.PortCfg.MIDIChannel);
					GateBut.MIDIData=0;}
            }
        } 
        else 
            GateBut.SendMIDI ();
    }
    // Apply data calculation based on selected function
    // In case of operations performed, send calculated data using CVport
    if (CVchanged || Slidchanged) {
        switch (Config.Chanfunction) {
        case NOTEMODE:
            break;
        case INDEP:
            // Do not send Notes CV changes on NOTEMODE
            if (CVchanged) {
                #ifdef CVTHING
                if (CVPort.PortCfg.MIDIfunction == PITCHLEVEL) { // send Trigger to paired CV
                    if( ControlNumber<1)
                        break;
                    CVControls[ControlNumber - 1].GateBut.GateStatus = CVPort.IsHigh();
                    CVControls[ControlNumber - 1].Gatechanged = true;
                    CVControls[ControlNumber - 1].OnDataChange();
                    CVControls[ControlNumber - 1].Gatechanged = false;
                    return;
                }
                #endif
                CVPort.SendMIDI (-9999, GateStat);
            }
            if (Slidchanged) {
                /*if (Slider.PortCfg.MIDIfunction == ANAGCLOCK) {
                    if (Slider.IsHigh ())
                        if (Slider.GateStatus == 0) {
                            Slider.GateStatus = 1;
                            Slider.ClockReceived ();
                        } // Transition 0->1
                } else*/
                    Slider.SendMIDI (-9999, GateStat);
            }
            break;
        case SUM:
        case MULTIPL:
            CVPort.SendMIDI (SendData, GateStat);
            // GateBut.setBlink(100, 100, 1);
        }
    }
}

/**
 *  \brief Provides the calculation in case Sum or Multiplication is selected
 *
 *  \return calculated value or -9999 if no calculation required
 *
 *  \details Limits data output based on selected function (e.g. 0 to 127 for most MIDI functions)
 */
int InputControl::DataCalc (void) {
    int outdata = -9999;

    switch (Config.Chanfunction) {
    case NOTEMODE: //*** Review
    case INDEP:
        return -9999;
    case SUM:
        outdata = (CVPort.MIDIData + Slider.MIDIData);
		break;
    case MULTIPL:
        // Overflow due to result bigger than int is not managed
        // When the slider is set to percent, scale value down by 100
        if (Slider.PortCfg.MIDIfunction == PERCENT)
            outdata = ((long)CVPort.MIDIData * Slider.MIDIData) / 100;
        else
            outdata = ((long)CVPort.MIDIData * Slider.MIDIData);
    }
	byte bBip = CVPort.PortCfg.RangeBipolar;
	CVPort.PortCfg.RangeBipolar= NOOFFSET;
	outdata = CVPort.TrimValue(outdata);
    CVPort.PortCfg.RangeBipolar= bBip;
    
    return outdata; // CVPort.TrimValue(outdata);
}

/**
 *  \brief Return State of the gate/button
 *
 *  \return State of Gate
 *
 *  \details This function will return HIGH for activation of the gate and LOW for idle state
 */
bool InputControl::GetGateState () { return GateBut.GateStatus; }


/**
 *  \brief change status of the Blink if neccessary
 *
 *  \return State of Gate
 *
 *  \details This function will return HIGH for activation of the gate and LOW for idle state
 */
void InputControl::Blink () { GateBut.blinkPort.playBlink (); }


/**
 *  \brief Save Control configuration to EEPROM
 *
 *  \param [in] addr address to save to
 *  \return number of bytes written
 *
 *  \details Will save Gate, CV and Fader configs
 */
int InputControl::SaveCfg (int addr) {
    int MemPointer = addr;

    // Bank function
    EEPROM.put (MemPointer, (uint8_t)Config.Chanfunction);
    MemPointer += sizeof (uint8_t);
    // Reserved BANKGENERALeeSize for general data
    MemPointer += BANKGENERALeeSize-(MemPointer-addr);

    MemPointer += GateBut.SaveCfg (MemPointer);
    MemPointer += CVPort.SaveCfg (MemPointer);
    MemPointer += Slider.SaveCfg (MemPointer);
    #ifdef PRINTDEBUG
	Serial.print( "Saved InputControl ");
	Serial.print( ControlNumber);
	Serial.print( ": ");
	Serial.print( addr);
	Serial.print( "/");
	Serial.print( MemPointer - addr);
	Serial.print( "/");
	Serial.println( MemPointer);
    #endif
    #ifdef USEOSC
	char msgTxt[120];
    sprintf (msgTxt, "/VCMC/Config/%d", ControlNumber + 1);
    SaveCfgOSC (msgTxt);
    #endif

    return BANKeeSize;//MemPointer - addr;
}

/**
 *  \brief Load Control configuration from EEPROM
 *
 *  \param [in] addr address to load from
 *  \return number of bytes read
 *
 *  \details Will load Gate, CV and Fader configs
 */
int InputControl::LoadCfg (int addr) {
    int MemPointer = addr;

    Config.Chanfunction = (CtrlFunctions)EEPROM.read (MemPointer);
    MemPointer += sizeof (uint8_t);
    // Reserved BANKGENERALeeSize for general data
    MemPointer += BANKGENERALeeSize-(MemPointer-addr);
	
    MemPointer += GateBut.LoadCfg (MemPointer);
    MemPointer += CVPort.LoadCfg (MemPointer);
    MemPointer += Slider.LoadCfg (MemPointer);

    return BANKeeSize;//MemPointer - addr;
}


/** @} */ 