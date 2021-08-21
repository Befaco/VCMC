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
 *  \file InputPortDig.ino
 *  \brief Definition of functions related to Digital input ports
 */


bool DigitalPort::SetPort (byte PortN, byte Ledp) {
    // if( PortN < 0 || PortN > 32) return false;

    PortNumber = PortN;
    GateStatus = false;
    if (PortN == 0) return false;
    #if GATEPULLUP
    pinMode (PortN, INPUT_PULLUP);
    #else
    pinMode (PortN, INPUT_PULLDOWN);
    #endif

    LedPort = Ledp;
    if (Ledp) pinMode (Ledp, OUTPUT);

    return true;
}



/**
 *  \brief Read digital Port (with debouncing function if selected) and process clock signal
 *  
 *  \param [in] NewData will store Read data 
 *  \return true when data read has changed, false otherwise (debouncing for instance)
 *  
 *  \details This functions perform two main tasks \n
 *  1. Read the Digital port including debouncing \n
 *  2. Call InputPort::ProcessClock() when Clock function selected \n
 *  Call ClockReceived() when Clock function selected.
 *  See InputPort::ClockReceived() for more details
 */
bool DigitalPort::ReadPort (long &NewData) {
    if (PortNumber == 0) return false;
	if (PortCfg.MIDIfunction == NODIGFUNC) return false; // No Function, do nothing
	
    // Process MIDI clock
    if (PortCfg.MIDIfunction == GATECLOCK && IntervalMIDIClock && ClockCounter) {
		ProcessClock();
    }

    // Process Trigger
    if( trigTime && micros()>trigTime){
        trigTime = 0; // reset trigger
        GateStatus = trigStatus;
        NewData = GateStatus;
        // Lit LED based on gate status
        digitalWrite(LedPort, GateStatus);

        // Process Clock
        if (PortCfg.MIDIfunction == GATECLOCK)
            if (GateStatus)
                ClockReceived();
        return true;
    }

    // read the state of the switch into a local variable:
    unsigned reading = digitalRead (PortNumber);
    int newGate = 2;

    if (debounceGate) { // Debounce function if selected
        // check to see if you just pressed the button
        // (i.e. the input went from LOW to HIGH),  and you've waited
        // long enough since the last press to ignore any noise:
        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonState) {
            // reset the debouncing timer
            lastDebounceTime = micros ();
        }

        if ((micros () - lastDebounceTime) > debounceDelay) { //PortCfg.DelayGate) {
            // whatever the reading is at, it's been there for longer
            // than the debounce delay, so take it as the actual current state:
            newGate = reading;
        }
        lastButtonState = reading;
    } else
        newGate = reading;

    if (newGate == 2) return false; // No lecture (debouncing)

    // PortValue = reading;
    if (newGate != PortValue) { // Gate changed
        PortValue = newGate;
        #if GATEPULLUP
        newGate = (newGate == 0) ? 1 : 0; //  !newGate;
        #endif
        // Latch management: switch gate with edge up
        if (PortCfg.MIDIfunction == LATCH || PortCfg.MIDIfunction == CCLATCH) {
            if (newGate) {
                newGate = !GateStatus;
            } else
                return false; // Low status in latch mode = do nothing
        }
        
        // Prepare delay
        //if(newGate)
        setTrigger(PortCfg.DelayGate, newGate);
    }

    return false;
}


/**
 *  \brief  Every clock received, recalculate interval for MIDI Clock sent
 */
bool DigitalPort::ClockReceived (void) {
    if (InputPort::ClockReceived (getClockDivider (), getClockShift())) {
        return true;
    } else
        return false;
}



/**
 *  \brief  Set Blink for Led
 */
void DigitalPort::setBlink (unsigned long periodon, unsigned long periodoff, int times) {
    blinkPort.setBlink (periodon, periodoff, times, LedPort);
}





/**
 *  \brief Send MIDI Data. 
 *  
 *  \param [in] MidiData Value to send. Defult value (-9999) to send internally stored data 
 *  \param [in] GateStat Gate Status 
 *  
 *  \details This functions will send the MIDI message based on the port configuration \n
 *  
 */
void DigitalPort::SendMIDI (int MidiData, bool GateStat) {
	msecLastMIDISent = millis();

    if(MidiMerge.soloMode){ // Check if we are in solo Mode
        if( theApp.GetPort() != (InputPort*)this){
            DP("Solo mode: Digital message skip");
            return;
        } else
            DP("Solo mode: Solo Digital Port selected");
    }


    switch (PortCfg.MIDIfunction) {
    case TRIGGER:
    case LATCH:
    case CHORD:
        break;
    case CCLATCH:
    case CCBUT:
        if (GateStatus == true)
            MidiMerge.sendControlChange (PortCfg.ControllerNumber, PortCfg.ControllerValue, PortCfg.MIDIChannel);
        else
            MidiMerge.sendControlChange (PortCfg.ControllerNumber, 0, PortCfg.MIDIChannel);
        break;
    case GATECLOCK:
        break;
    case GATESTARTSTOP:
        if (GateStatus == true)
            MidiMerge.sendRealTime (usbMIDI.Start);
        else
            MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case GATEPAUSECONT:
        if (GateStatus == true)
            MidiMerge.sendRealTime (usbMIDI.Continue);
        else
            MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case GATETRIGSTARTSTOP:
        if (GateStatus == true) {
            if (!MidiMerge.IsRunning)
                MidiMerge.sendRealTime (usbMIDI.Start);
            else
                MidiMerge.sendRealTime (usbMIDI.Stop);
        }
        break;
    case GATETRIGPAUSECONT:
        if (GateStatus == true) {
            if (!MidiMerge.IsRunning)
                MidiMerge.sendRealTime (usbMIDI.Continue);
            else
                MidiMerge.sendRealTime (usbMIDI.Stop);
        }
        break;
    case GATESTART:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Start);
        break;
    case GATECONTINUE:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Continue);
        break;
    case GATESTOP:
        if (GateStatus == true) MidiMerge.sendRealTime (usbMIDI.Stop);
        break;
    case GATEPANIC:
        if (GateStatus == true) MidiMerge.Panic();
    case NODIGFUNC:
        break;
    }
}






/** @} */
