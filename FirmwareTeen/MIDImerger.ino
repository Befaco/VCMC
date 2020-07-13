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

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file MIDImerger.ino
 *  \brief MIDI Input/Output support
 */

/// Create instance for MIDI port
MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midi2);


MIDImerger::MIDImerger () {
    midiJack = &midi2;
    defaultMIDIData ();
}

/**
 * \brief Read ports and process incoming messages if needed
 * 
 * \return true when activity in any input
 * \return false 
 */
bool MIDImerger::poll (void) {
    bool activity = false;
    byte type = 0, channel, data1, data2;

    if (usbMIDI.read ()) {
        // Thru on A has already pushed the input message to out A.
        // Forward the message to out B as well.
        // get the USB MIDI message, defined by these 5 numbers (except SysEX)
        type = usbMIDI.getType ();
        channel = usbMIDI.getChannel ();
        data1 = usbMIDI.getData1 ();
        data2 = usbMIDI.getData2 ();
        // byte cable = usbMIDI.getCable();

        // forward this message the Serial MIDI OUT ports
        if (type != usbMIDI.SystemExclusive) {
            // Normal messages, first we must convert usbMIDI's type (an ordinary
            // byte) to the MIDI library's special MidiType.
            if( MIDIThru){
                midi::MidiType mtype = (midi::MidiType)type;
                midiJack->send (mtype, data1, data2, channel);
            }
        } else {
            // SysEx messages are special.  The message length is given in data1 & data2
            unsigned int SysExLength = data1 + data2 * 256;
            FlashAccess->ReceivedSysEx (SysExLength, usbMIDI.getSysExArray ());
            if( MIDIThru){
                midiJack->sendSysEx (SysExLength, usbMIDI.getSysExArray (), true);
            }
        }
        activity = true;
    }

    if (midiJack->read ()) {
        // Thru on B has already pushed the input message to out B.
        // Forward the message to out A as well.
        // get a MIDI IN1 (Serial) message
        type = midiJack->getType ();
        channel = midiJack->getChannel ();
        data1 = midiJack->getData1 ();
        data2 = midiJack->getData2 ();

        // forward the message to USB MIDI virtual cable #0
        if (type != midi::SystemExclusive) {
            // Normal messages, simply give the data to the usbMIDI.send()
            if( MIDIThru){
                usbMIDI.send (type, data1, data2, channel, 0);
            }
        } else {
            // SysEx messages are special.  The message length is given in data1 & data2
            unsigned int SysExLength = data1 + data2 * 256;
            FlashAccess->ReceivedSysEx (SysExLength, midiJack->getSysExArray ());
            if( MIDIThru){
                usbMIDI.sendSysEx (SysExLength, midiJack->getSysExArray (), true, 0);
            }
        }
        activity = true;
    }

    if (FlashAccess->IsSysExRecComplete () && MidiMerge.tagProcessSysex == 0) {
        if (MidiMerge.tagProcessSysex == 0) {
            myMenu.ClearArea ();
            myMenu.setCurrentMenu (&listLoadSysEx);
            MidiMerge.tagProcessSysex = 1;
        }
        // SetMessageText( "SysEx Config Received!");
        //&& myMenu.currentMenu!=&listLoadSysEx
    }

    switch (type) {
    case usbMIDI.Start:
    case usbMIDI.Continue:
        IsRunning = true;
        SetMessageText ("Start/Cont.");
        break;
    case usbMIDI.Stop:
        IsRunning = false;
        SetMessageText ("Stop");
        break;
    }

    return activity;
}


void MIDImerger::begin (void) {
    Serial1.begin(31250);
    // midiSerial->begin(MIDI_CHANNEL_OMNI);
    midiJack->begin (MIDI_CHANNEL_OMNI);
}


/**
 * \brief Set stored data to default values
 * 
 */
void MIDImerger::defaultMIDIData (void) {
    for (int i = 0; i < 16; i++) {
        PitchData[i] = 127;
        VelData[i] = 127;
    }
    IsRunning = true; // By default start active
}


/**
 * \brief Send Panic to all outputs
 * 
 */
void MIDImerger::Panic () {
    for (int i = 0; i < 16; i++) {
        // Send Note Off
        sendNoteOff (PitchData[i], 0, i + 1);
        // Send all Note Off message
        sendControlChange (123, 0, i + 1);
    }
    IsRunning = true;
}

/////////////////////////////////////////////////////////////
// Send MIDI messages to outputs
void MIDImerger::sendNoteOn (byte Note, byte Veloc, byte Chann) {
    PitchData[Chann - 1] = Note;
    // VelData[Chann-1]= Veloc;
    usbMIDI.sendNoteOn (Note, Veloc, Chann);
    midiJack->sendNoteOn (Note, Veloc, Chann);
}

void MIDImerger::sendNoteOff (byte Note, byte Veloc, byte Chann) {
    // VelData[Chann-1]= 0;
    usbMIDI.sendNoteOff (Note, Veloc, Chann);
    midiJack->sendNoteOff (Note, Veloc, Chann);
}

void MIDImerger::sendControlChange (byte Ctrler, byte Value, byte Chann) {
    usbMIDI.sendControlChange (Ctrler, Value, Chann);
    midiJack->sendControlChange (Ctrler, Value, Chann);
}

void MIDImerger::sendPitchBend (int Value, byte Chann) {
    usbMIDI.sendPitchBend (Value, Chann);
    midiJack->sendPitchBend (Value, Chann);
}

void MIDImerger::sendProgramChange (byte Value, byte Chann) {
    usbMIDI.sendProgramChange (Value, Chann);
    midiJack->sendProgramChange (Value, Chann);
}

void MIDImerger::sendRealTime (byte Value) {
    switch (Value) {
    case usbMIDI.Start:
    case usbMIDI.Continue:
        IsRunning = true;
        SetMessageText ("Start/Cont.");
        break;
    case usbMIDI.Stop:
        IsRunning = false;
        SetMessageText ("Stop");
        break;
    case usbMIDI.Clock:
        //if (!IsRunning) return; // Do not send clock if stopped
        break;
    }

    usbMIDI.sendRealTime (Value);
    midiJack->sendRealTime ((midi::MidiType)Value);
}

 void MIDImerger::endNrpn(uint8_t channel){
	usbMIDI.endNrpn (channel);
	midiJack->endNrpn (channel);
}
 void MIDImerger::sendNrpnValue(uint16_t value, uint8_t channel){
	usbMIDI.sendNrpnValue (value,channel);
	midiJack->sendNrpnValue (value,channel);
}
void MIDImerger::beginNrpn(uint16_t number, uint8_t channel){
 	usbMIDI.beginNrpn (number,channel);
	midiJack->beginNrpn (number,channel);
}

/*
midiJack->sendRealTime()
     Clock                 = 0xF8,
     Start                 = 0xFA,
     Continue              = 0xFB,
     Stop                  = 0xFC,

  usbMIDI.sendRealTime(usbMIDI.Clock);
  usbMIDI.sendRealTime(usbMIDI.Start);
  usbMIDI.sendRealTime(usbMIDI.Continue);
  usbMIDI.sendRealTime(usbMIDI.Stop);
usbMIDI.Clock	0xF8	System Real Time - Timing Clock
usbMIDI.Start	0xFA	System Real Time - Start
usbMIDI.Continue	0xFB	System Real Time - Continue
usbMIDI.Stop	0xFC	System Real Time - Stop
*/

/** @} */
