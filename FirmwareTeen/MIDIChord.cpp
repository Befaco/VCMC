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
 *  \file MIDIChord.cpp
 *  \brief MIDI Chords support
 */


/**
 * \brief Set new scale and adjust rootNote accordingly.
 * 
 * \param newscale See Scales.h for valid scales
 * \return Returns new rootNote
 */
uint8_t MIDIChord::setScale(uint8_t newscale){
    if( newscale>=LASTSCALE) // Incorrect scale, no change
        return rootNote;
    if(isPlaying) noteoffChord();

    Scale = newscale;
    return setRootNote(rootNote); // Adjust rootNote to new scale and return new root
}

/**
 * \brief Select new chord (check if playing and send Notes off)
 * 
 * \param newchord See Chord.h for valid chord types
 */
void MIDIChord::setChord(uint8_t newchord){
    if( newchord>=LASTCHORD) // Incorrect scale, no change
        return;
    if(isPlaying) noteoffChord();

    ChordType = newchord;
}


/**
 * \brief Returns note adjusted to selected scale
 * 
 * \param note to adjust
 * \return note adjusted to current scale
 */
uint8_t MIDIChord::adjustNoteToScale(uint8_t note){
    uint8_t notepos = note % 12;
    
    for (size_t i = notepos; i > 0 && note >0; i--,note--)
    {
        if(ScalesDefinition[Scale].Notes[i]) return note; // Note in scale
    }
    //DP("Error in Scale"); // Code should not get to this position
    return note;
}


/**
 * \brief Set new root Note and play if selected. Returns new root note
 * 
 * \param newroot Note
 * \param playCh true will play the chord 
 * \return new root note
 */
uint8_t MIDIChord::setRootNote(uint8_t newroot, uint8_t vel, uint8_t chan, bool playCh){
    newroot = adjustNoteToScale(newroot);
    if(isPlaying) noteoffChord();
    rootNote = newroot;
    Velocity = vel;
    if(chan<17) MIDIChannel = chan;
    if(playCh) playChord();
    return rootNote;
}

uint8_t MIDIChord::playChord(void)
{
    if( !NoteOn) return rootNote; // No NoteOn defined

    const int8_t *chord = chordNotesDef[ChordType];

    if(isPlaying) noteoffChord();
    
    for (size_t i = 0; i < chord[0]; i++){
        NoteOn(rootNote + chord[i + 1], Velocity, MIDIChannel);
    }
    isPlaying = true;
    return rootNote;
}

uint8_t MIDIChord::noteoffChord(void)
{
    if( !NoteOff) return rootNote; // No NoteOff defined
    
    const int8_t *chord = chordNotesDef[ChordType];

    for (size_t i = 0; i < chord[0]; i++){
        NoteOff(rootNote + chord[i + 1], Velocity, MIDIChannel);
    }
    isPlaying = false;
    return rootNote;
}

/** @} */
