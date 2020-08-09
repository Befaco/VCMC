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
#ifndef __have__MIDIMerger_h__
#define __have__MIDIMerger_h__

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file MIDImerger.h
 *  \brief MIDI Input/Output support
 */

/// Class support for Serial MIDI and USB MIDI
class MIDImerger
{
public:
    byte PitchData[16];                            ///< MIDI channel data for Note
    byte VelData[16];                              ///< MIDI channel data for velocity
    byte IsRunning = true;                         ///< True if start/contine real time message is received
    int tagProcessSysex = 0;                       ///< Mark Multi SysEx reception

    MIDImerger();
    bool poll(void);
    void begin(void);
    void defaultMIDIData();
    void Panic();
    void setMIDIThru(bool setT) { MIDIThru = setT; }
    // MIDI functions
    void sendNoteOn(byte Note, byte Veloc, byte Chann);
    void sendNoteOff(byte Note, byte Veloc, byte Chann);
    void sendControlChange(byte Ctrler, byte Value, byte Chann);
    void sendPitchBend(int Value, byte Chann);
    void sendProgramChange(byte program, byte channel);
    void sendRealTime(byte Value);
    void endNrpn(uint8_t channel);
    void sendNrpnValue(uint16_t value, uint8_t channel);
    void beginNrpn(uint16_t number, uint8_t channel);
private:
    midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> *midiJack; ///< MIDI Interface pointers
    bool MIDIThru = false; 
};

/** @} */

#endif // __have__MIDIMerger_h__