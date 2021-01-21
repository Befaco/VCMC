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
#ifndef __have__MIDIChord_h__
#define __have__MIDIChord_h___

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file MIDIChord.h
 *  \brief MIDI Chords support
 */

#include "Scales.h"
#include "Chords.h"
#include "ChordInversions.h"

const size_t NOTESCHORD = 8; // Number of maximum notes in chord
const uint8_t defVel = 60;

using NoteCallback  = void (*)(uint8_t  note, uint8_t  vel, uint8_t  chan);

/// Class support for Serial MIDI and USB MIDI
class MIDIChord
{
//public:
protected:
    union {
        struct {
        uint8_t Scale;      // Scale 0 is all notes iun scale
        uint8_t ChordType;  // Chord 0 is only 1 note without offset
        uint8_t InvDrop;
        };
        uint32_t Type=0;
    };
    uint8_t rootNote = 0;
    uint8_t Velocity = defVel;
    uint8_t MIDIChannel = 1;
    // uint8_t Notes[NOTESCHORD+1] = {0};
    bool isPlaying = false;
    NoteCallback NoteOn = nullptr;
    NoteCallback NoteOff = nullptr;

public:
    MIDIChord() : Scale(DEF_SCALE), ChordType(DEF_CHORD), InvDrop(DEF_INVDROP) {}

    // Returns note adjusted to selected scale
    uint8_t adjustNoteToScale(uint8_t note);
    uint8_t playChord(void);
    uint8_t noteoffChord(void);
    // Set new scale and adjust rootNote accordingly. Returns new rootNote
    uint8_t setScale(uint8_t newscale);
    // Select new chord (check if playing and send Notes off)
    void setChord(uint8_t newchord); 
    // Set new root Note and play if selected. Returns new root note
    uint8_t setRootNote(uint8_t newroot, uint8_t vel =defVel, uint8_t chan=0xff, bool playChord=false);
    void setCallbacks(NoteCallback non, NoteCallback nof) { 
        NoteOn = non;
        NoteOff = nof;
    }
    void setInvDrop(uint8_t newInv);
    // get func
    uint8_t getScale(void) { return Scale; }
    uint8_t getChordType(void) { return ChordType; }
    uint8_t getInvDrop(void) { return InvDrop; }
    uint16_t getType(void) { return Type; }
    const int8_t *getInvTable(uint8_t chordtoPlay);
};

/** @} */

#endif // __have__MIDIChord_h__
