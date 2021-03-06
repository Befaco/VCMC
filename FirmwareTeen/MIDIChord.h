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
#include "src/MidiThing/MIDITools/NoteEvent.h"

const size_t NOTESCHORD = 8; // Number of maximum notes in chord
const uint8_t defVel = 60;

//using NoteCallback  = void (*)(uint8_t  note, uint8_t  vel, uint8_t  chan);
using NoteCallback  = void (*)(NoteEvent* pEV);

/// Class support for Serial MIDI and USB MIDI
class MIDIChord
{
//public:
protected:
    union {
        struct {
        uint8_t Scale:4;      // Scale 0 is all notes iun scale
        uint8_t ScaleRoot:4;  // Scale root note: 0-11
        };
        uint8_t ScaleId = 0;
    };
    union {
        struct{
        uint8_t ChordType;  // Chord 0 is only 1 note without offset
        uint8_t InvDrop;
        };
        uint16_t ChordId=0;
    };
    uint8_t rootNote = 0;
    uint8_t Velocity = defVel;
    uint8_t MIDIChannel = 1;
    
    uint8_t NotesPlaying[NOTESCHORD+1] = {0};
    uint8_t isPlaying = 0; //< number of notes playing
    NoteCallback NoteOn = nullptr;
    NoteCallback NoteOff = nullptr;

    // delays in msecs
    union 
    {
        struct{
            uint16_t delayFix;
            uint16_t delayRnd;
        };
        uint32_t delayChord=0;
    };
    uint32_t noteOnTime = 0;

public:
    MIDIChord() : Scale(FULL_SCALE), ChordType(ONENOTECHORD), InvDrop(NO_INVDROP) {
        delayFix=0;
        delayRnd=0;
    }

    // Returns note adjusted to selected scale
    uint8_t adjustNoteToScale(uint8_t note);
    uint8_t playChord(void);
    uint8_t getChordToPlay(void);
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
    void setScaleRoot(uint8_t newroot) { ScaleRoot=newroot; }
    void setScaleId(uint8_t scid) { ScaleId = scid; }
    void setdelayFix(uint16_t nv) { delayFix = nv; }
    void setdelayRand(uint16_t nv) { delayRnd = nv; }
    // get func
    uint8_t getScale(void) { return Scale; }
    uint8_t getChordType(void) { return ChordType; }
    uint8_t getInvDrop(void) { return InvDrop; }
    //uint16_t getType(void) { return Type; }
    uint8_t getScaleId(void) { return ScaleId; }
    const int8_t *getInvTable(uint8_t chordtoPlay);
    uint8_t getScaleRoot(void) { return ScaleRoot; }
    uint16_t  getdelayFix(void) { return delayFix; }
    uint16_t  getdelayRand(void) { return delayRnd; }};

/** @} */

#endif // __have__MIDIChord_h__
