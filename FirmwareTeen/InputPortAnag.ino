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
#include "PrjIncludes.h"

/** @addtogroup InputPortsGroup
* @{
*/

/**
 *  \file InputPortAnag.ino
 *  \brief Definition of functions related to Analog (CV and Fader) input ports
 */

/**
 *  \brief Set the port value and analog input
 *  
 *  \param [in] PortN Input port for analog reading
 *  
 */
bool AnalogPort::SetPort (byte PortN) {
    // if( PortN < 0 || PortN > 7) return false;

    PortNumber = PortN;
    pinMode (PortN, INPUT); // INPUT_ANALOG);
    PortValue = 0;
    return true;
}

/**
 *  \brief Set the port value to Demux and analog input
 *  
 *  \param [in] DemuxVal 3 bit Value to set in the demuc chip
 *  \param [in] PortN Input port for analog reading
 *  
 */
bool DemuxAnalogPort::SetPort (byte DemuxVal, byte PortN) {
    // Port to demux
    DemuxPort = DemuxVal;
    // call base class to setup analog input
    return AnalogPort::SetPort (PortN);
}



/**
 *  \brief  Every clock received, recalculate interval for MIDI Clock sent
 */
bool AnalogPort::ClockReceived (void) {
    if (InputPort::ClockReceived (getClockDivider (), getClockShift())) {
        return true;
    } else
        return false;
}



/**
 *  \brief Read analog Port make conversion and check analog clock
 *  
 *  \param [in] NewData will store Read data 
 *  \return true when data read and converted has changed
 *  
 *  \details This functions perform two main tasks \n
 *  1. Read the Analog port:\n
 *      1.1 Set the output for Bipolar input\n
 *      1.2 Read the analog input\n
 *      1.3 Filter the input values\n
 *      1.4 Convert data to MIDI as defined in the config data (see RangeConv::invConvert()\n
 *      1.5 Apply Amplification and Offset\n
 *  2. Send clock when ClockDivider is bigger than 1 (first clock is sent on InputPort::ClockReceived())\n
 *  It will check if the interval from last clock has passed and decrease the ClockCounter\n
 *  See InputPort::ClockReceived() for more details
 */
bool AnalogPort::ReadPort (long &NewData) {
    /*#ifdef DEBUGMODE
    //if(PortNumber==PINCVINPUT) 
        beginDebugTimers ();
    #endif*/
    // Process MIDI clock
    if (PortCfg.MIDIfunction == ANAGCLOCK && IntervalMIDIClock && ClockCounter) {
		ProcessClock();
    }

    int readMidi=MIDIData;
    // Set Bipolar input
    if (PortCfg.getInputRange()==MINUSPLUS5V)
        digitalWrite (PINOFFSET, HIGH);
    else
        digitalWrite (PINOFFSET, LOW);
    // Read input
    /* uint16_t reading = 0; // Test CV inputs
    if(PortCfg.ControllerNumber<21) reading = myMenu.disptimer / 50 / PortCfg.ControllerNumber;
    if(PortCfg.ControllerNumber==21) reading = ((myMenu.disptimer%5000000)<500000)?0:4095<<4;
     */
    uint16_t reading = (uint16_t)adc->analogRead (PortNumber);//,adcNum);

    if (ANBITS > 12) reading >>= 4; // Filter first 4 bits
    if( PortCfg.IsDigitalFunc())
    {
        //unsigned reading = digitalRead (PortNumber);
        int16_t mindac,rangedac;
        PortCfg.Ranges.getDAC(mindac,rangedac);
        // Read as digital input
        bool readGate = GateStatus;        
        if( reading < 4095*2/3-100){ //mindac-rangedac/3
            reading= PortCfg.Ranges.getMaxDAC();
            readGate = true;
            //1readMidi = 1;
        }  else if(reading > 4095*2/3+100){
            reading = mindac;
            readGate = false;
        }

        int newGate = 2;
        if (debounceGate) { // Debounce function if selected
            // check to see if you just pressed the button
            // (i.e. the input went from LOW to HIGH),  and you've waited
            // long enough since the last press to ignore any noise:
            // If the switch changed, due to noise or pressing:
            if (readGate != lastButtonState) {
                // reset the debouncing timer
                lastDebounceTime = micros ();
            }
            if ((micros () - lastDebounceTime) > debounceDelay) { //PortCfg.DelayGate) {
                // whatever the reading is at, it's been there for longer
                // than the debounce delay, so take it as the actual current state:
                newGate = readGate;
            }
            lastButtonState = readGate;
        } else
            newGate = readGate;
        if (newGate == 2) return false; // No lecture (debouncing)


        NewData= reading;
        if (reading != PortValue) { // Gate changed
            PortValue = reading;
            // Latch management: switch gate with edge up
            if (PortCfg.MIDIfunction == ANAGCCLATCH || PortCfg.MIDIfunction == ANAGLATCH) {
                if (newGate) {
                    GateStatus = !GateStatus;
                    newGate = GateStatus;
                }  else
                    return false;
            } else
                GateStatus = newGate;

            readMidi = (newGate)?PortCfg.Ranges.getMaxMIDI():0;
        }
       // NewData = PortValue = reading;
    } else 
    {
        // Use filter for the input
        #ifdef FILTEREDANAGINPUT
        FilteredInput->update (reading);
        reading = FilteredInput->getValue ();
        #endif
        if( reading!=PortValue){
            // Convert 12 bit input to MIDI values
            readMidi = PortCfg.Ranges.invConvert (reading);
        }
        NewData = PortValue = reading;
    }

    /*#ifdef DEBUGMODE
    //if(PortNumber==PINCVINPUT) 
        endDebugTimers ();
    #endif*/
    if (readMidi != MIDIData) {
        // Store new value
        MIDIData = readMidi;
		// Check if clock Received 
		if (PortCfg.MIDIfunction == ANAGCLOCK)
			if( IsHigh()) ClockReceived();
        return true;
    }

	// Check Auto off for Note in Dude mode
	if( msecLastMIDISent && PortCfg.AutoOff &&
		millis()-msecLastMIDISent > PortCfg.AutoOff && PortCfg.MIDIfunction==PITCH){
		// Note off previous note
		MidiMerge.sendNoteOff( LastSentMIDIData, 0, PortCfg.MIDIChannel);
		LastSentMIDIData = -999;
		msecLastMIDISent = 0;
		return false;
	}
    return false;
}



/**
 *  \brief Read multiplexed analog input
 *  
 *  \param [in] NewData will store Read data 
 *  \return true when data read and converted has changed 
 */
bool DemuxAnalogPort::ReadPort (long &NewData) {
    SetDemux ();
    // read the analog value
    return AnalogPort::ReadPort (NewData);
}

/**
 *  \brief Set Demux pins
 *  
 */
void DemuxAnalogPort::SetDemux (void) {
    // setup demux ports
    digitalWrite (PINDEMUXA, DemuxPort & 1);
    digitalWrite (PINDEMUXB, DemuxPort >> 1 & 1);
    digitalWrite (PINDEMUXC, DemuxPort >> 2 & 1);
}



/**
 *  \brief Send MIDI Data. 
 *  
 *  \param [in] MidiData Value to send. Defult value (-9999) to send internally stored data 
 *  \param [in] GateStat Gate Status 
 *  
 *  \details This functions will send the MIDI message based on the port configuration \n
 *  
 *  Before sending the data, it is trimmed to valid MIDI limits. See  AnalogPort::TrimValue()
 */
void AnalogPort::SendMIDI (int MidiData, bool GateStat) {
    int SendData;

    if(MidiMerge.soloMode){ // Check if we are in solo Mode
        if( theApp.GetPort() != (InputPort*)this){
            DP("Solo mode: Analog message skip");
            return;
        } else
            DP("Solo mode: Solo Analg Port selected");
    }

    if (MidiData == -9999)
        SendData = TrimValue (MIDIData); // Use internal value
    else
        SendData = TrimValue (MidiData); // Use passed value

    if (SendData == LastSentMIDIData) return; // Do not send the same data twice
	
    switch (PortCfg.MIDIfunction) {
	case PITCHTRIG:
	case PITCHLEVEL:
	case PITCH8TRIG:
        return;
    case PITCH:
        // Note off previous note
        //MidiMerge.sendNoteOff( MidiMerge.PitchData[PortCfg.MIDIChannel-1], 0, PortCfg.MIDIChannel);
		MidiMerge.sendNoteOff( LastSentMIDIData, 0, PortCfg.MIDIChannel);
        MidiMerge.PitchData[PortCfg.MIDIChannel-1] = SendData;
        // Send new Note
        MidiMerge.sendNoteOn( SendData, MidiMerge.VelData[PortCfg.MIDIChannel-1], PortCfg.MIDIChannel);
		break;
    case VELOCITY:
        if(!PortCfg.DestCtrl)   // Destination = 0 send to channel info
            MidiMerge.VelData[PortCfg.MIDIChannel - 1] = SendData; 
        else    // Send to destination gate port
            CVControls[PortCfg.DestCtrl - 1].GateBut.velToPlay = SendData;
        break; // do not send MIDI, gate button or pitch change will trigger sending data
    case CC14BITS:
        if(PortCfg.ControllerNumber>32)
            break; // Only for CC smalleer than 32
        // Send the input 12 bits: 7 MSB in CC Controller NUmber and the 5 LSB on CC Number+32
        if( !typeSlider) SendData = (~PortValue)&0x0fff;
        else SendData = (PortValue)&0x0fff;
        MidiMerge.sendControlChange (PortCfg.ControllerNumber, (SendData>>5), PortCfg.MIDIChannel);
        MidiMerge.sendControlChange (PortCfg.ControllerNumber+32, (SendData & 0x01f)<<2, PortCfg.MIDIChannel);
        break;
    case CONTROLCHANGE:
        MidiMerge.sendControlChange (PortCfg.ControllerNumber, SendData, PortCfg.MIDIChannel);
        break;
    case PROGRAMCHANGE:
        MidiMerge.sendProgramChange (SendData, PortCfg.MIDIChannel);
        break;
    case AFTERTOUCH:
        MidiMerge.sendAfterTouch (SendData, PortCfg.MIDIChannel);
        break;
    case PITCHBEND:
        MidiMerge.sendPitchBend (SendData, PortCfg.MIDIChannel);
        break;
    case PERCENT:
        break;
    case ANAGCLOCK:
        break;
    /* case ANAGSTARTSTOP:
        if (IsHigh ()) {
            if (GateStatus == 0) {
                MidiMerge.sendRealTime (usbMIDI.Start);
                GateStatus = 1;
            }
        } else if (GateStatus == 1) {
            MidiMerge.sendRealTime (usbMIDI.Stop);
            GateStatus = 0;
        }
        break;*/
    case NOANFFUNC:
		break;
    case ANAGSTARTSTOP:
        if (GateStatus == true)
            MidiMerge.sendRealTime (usbMIDI.Start);
        else
            MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case ANAGPAUSECONT:
        if (GateStatus == true)
            MidiMerge.sendRealTime (usbMIDI.Continue);
        else
            MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case ANAGTRIGSTARTSTOP:
        if (GateStatus == true) {
            if (!MidiMerge.IsRunning)
                MidiMerge.sendRealTime (usbMIDI.Start);
            else
                MidiMerge.sendRealTime (usbMIDI.Stop);
        }
        break;
    case ANAGTRIGPAUSECONT:
        if (GateStatus == true) {
            if (!MidiMerge.IsRunning)
                MidiMerge.sendRealTime (usbMIDI.Continue);
            else
                MidiMerge.sendRealTime (usbMIDI.Stop);
        }
        break;
    case ANAGSTART:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Start);
        break;
    case ANAGCONTINUE:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Continue);
        break;
    case ANAGSTOP:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case ANAGNRPN7bits:
		MidiMerge.beginNrpn( (PortCfg.NRPNparMSB<<7)+PortCfg.NRPNparLSB, PortCfg.MIDIChannel);
		MidiMerge.sendControlChange (6, SendData, PortCfg.MIDIChannel );
		MidiMerge.endNrpn( PortCfg.MIDIChannel);
        break;
	case ANAGNRPN14bits:
		MidiMerge.beginNrpn( (PortCfg.NRPNparMSB<<7)+PortCfg.NRPNparLSB, PortCfg.MIDIChannel);
		MidiMerge.sendNrpnValue ( SendData, PortCfg.MIDIChannel );
		MidiMerge.endNrpn( PortCfg.MIDIChannel);
        break;
    case ANAGCCLATCH:
    case ANAGCCBUT:
        if (GateStatus == true)
            MidiMerge.sendControlChange (PortCfg.ControllerNumber, PortCfg.ControllerValue, PortCfg.MIDIChannel);
        else
            MidiMerge.sendControlChange (PortCfg.ControllerNumber, 0, PortCfg.MIDIChannel);
        break;
    case ANAGLATCH:
    case ANAGTRIGGER:/*
        if (GateStatus == true)
        {
            // Send new Note
            MidiMerge.sendNoteOn( PortCfg.NoteToSend, MidiMerge.VelData[PortCfg.MIDIChannel-1], PortCfg.MIDIChannel);
            MidiMerge.PitchData[PortCfg.MIDIChannel-1] = SendData;
        }
        else
        {
		    MidiMerge.sendNoteOff( PortCfg.NoteToSend, 0, PortCfg.MIDIChannel);            
            MidiMerge.PitchData[PortCfg.MIDIChannel-1] = 0;
        } */
        break;
    case CHORDTYPE_DEF:
        theApp.Controls[PortCfg.DestCtrl].Chord.setChord(SendData);
        //theApp.DefaultChord.setChord(MIDIData);
        break;
    case SCALE_DEF:
        theApp.Controls[PortCfg.DestCtrl].Chord.setScale(SendData);
        //theApp.DefaultChord.setScale(MIDIData);
        break;
    case CHORDINVERSION:
        theApp.Controls[PortCfg.DestCtrl].Chord.setInvDrop(SendData);
        /* theApp.DefaultChord.setInvDrop(MIDIData);
        // Set also the Chord Inversion for control if not setup in default inversion
        if(theApp.Controls[PortNumber].Chord.getInvDrop()!= DEF_INVDROP)
            theApp.Controls[PortNumber].Chord.setInvDrop(MIDIData); */
        break; 
    case SCALEROOT:
        theApp.Controls[PortCfg.DestCtrl].Chord.setScaleRoot(SendData);
        break;
    case CHORDDELAYFIX:
        theApp.Controls[PortCfg.DestCtrl].Chord.setdelayFix(SendData);
        break;
    case CHORDDELAYRAND:
        theApp.Controls[PortCfg.DestCtrl].Chord.setdelayRand(SendData);
        break;
    }
	// Store current data
    LastSentMIDIData = SendData;
	msecLastMIDISent = millis();
}


/**
 *  \brief Trim value to valid MIDI range. 
 *  
 *  \param [in] DatatoTrim Value to Trim. Defult value (-9999) to send internally stored data 
 *  
 *  \details This functions will trim the MIDI message and apply the clipping limits based on the port configuration \n
 *  
 */
int AnalogPort::TrimValue (int DatatoTrim) {
    int returnValue;
    int16_t minv = 0, maxv = 127;

    if (DatatoTrim == -9999) DatatoTrim = MIDIData;

    returnValue = DatatoTrim;

    // apply transposition
    returnValue += (12 * PortCfg.TransposeOctave) + PortCfg.TransposeSemitones;

    PortCfg.LimitValues (minv, maxv);
    // Limit Data to MIDI limits
    //returnValue = (DatatoTrim < minv) ? minv : (DatatoTrim > maxv) ? maxv : DatatoTrim;
    // Check Clipping
    returnValue = (returnValue < PortCfg.ClipLow) ?
                  PortCfg.ClipLow :
                  (returnValue > PortCfg.ClipHigh) ? PortCfg.ClipHigh : returnValue;
    return returnValue;
}




/** @} */
