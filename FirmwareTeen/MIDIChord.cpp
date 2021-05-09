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

    Scale = newscale;
    // Adjust rootNote to new scale and return new root 
    return adjustNoteToScale(rootNote);
    //return setRootNote(rootNote,Velocity,MIDIChannel,isPlaying); // Will stop playing current chord and play new one
}

/**
 * \brief Select new chord (check if playing and send Notes off)
 * 
 * \param newchord See Chord.h for valid chord types
 */
void MIDIChord::setChord(uint8_t newchord){
    if( newchord>=LASTCHORD) // Incorrect scale, no change
        return;
    //bool wasPlaying = isPlaying;
    //if(isPlaying) noteoffChord();

    ChordType = newchord;
    //if(wasPlaying) // Play new chord it it was playing
    //    setRootNote(rootNote, Velocity, MIDIChannel, true);
}


void MIDIChord::setInvDrop(uint8_t newInv){ 
    if(newInv>=LAST_INVDROP)
        return;
    //bool wasPlaying = isPlaying;
    //if(isPlaying) noteoffChord();

    InvDrop = newInv;
    //if(wasPlaying) // Play new chord it it was playing
    //    setRootNote(rootNote, Velocity, MIDIChannel, true);
}


/**
 * \brief Returns note adjusted to selected scale
 * 
 * \param note to adjust
 * \return note adjusted to current scale
 */
uint8_t MIDIChord::adjustNoteToScale(uint8_t note){
    // Get note octave position and shift based on scale root
    uint8_t notepos = (note < 12) ? note + 12 : note; // For notes 0-11 chose next octave
    notepos = (notepos - getScaleRoot()) % 12;
    uint8_t scaletoUse = (Scale == DEF_SCALE) ? theApp.DefaultChord.getScale() : Scale;
    uint8_t retVal = note;
    //D(Serial.printf("Scale (%s-%s), recv note %d/%d", 
    //    NotesNamesScale[getScaleRoot()], ScaleShortNames[scaletoUse], note, notepos));
    for (size_t i = notepos; i >= 0 && note >0; i--,note--)
    {
        if(ScalesDefinition[scaletoUse].Notes[i]){ break; }
    }
    retVal=note; // Note in scale
    //D(Serial.printf(": %d\n",retVal));
    return retVal;
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

const int8_t*  MIDIChord::getInvTable(uint8_t chordtoPlay)
{
    const int8_t *pretVal = nullptr;

    uint8_t invtoPlay = (InvDrop == DEF_CHORD) ? theApp.DefaultChord.getInvDrop() : InvDrop;
    if(chordNotesDef[chordtoPlay][0]==4){
        pretVal = InversionDrop4[invtoPlay];
        //D(Serial.printf("4 Notes Chord, Inversion %d\n",invtoPlay));
        }
    else if(chordNotesDef[chordtoPlay][0]==3){
        pretVal = InversionDrop3[invtoPlay];
        //D(Serial.printf("3 Notes Chord, Inversion %d\n",invtoPlay));
        }

    return pretVal;
}


uint8_t MIDIChord::getChordToPlay(void)
{
    uint8_t retVal = (ChordType == DEF_CHORD) ? theApp.DefaultChord.getChordType() : ChordType;
    uint8_t scale = getScale();
    if( retVal>0 && scale>FULL_SCALE){ // Check diatonic chords
        if(retVal>diatonicNumberOfChords) retVal = 1; // If chord is not defined, use triads
        //D(Serial.printf("In %d,%d,%d: Out %d\n", scale, retVal-1, rootNote%12, diatonicChords[scale-1][retVal-1][rootNote%12]));
        uint8_t root = (rootNote < 12) ? rootNote + 12 : rootNote;
        retVal = diatonicChords[scale-1][retVal-1][(root-getScaleRoot())%12];
        if(!retVal)
            retVal = LASTCHORD; // Selected chord is not in scale
    }
    //D(Serial.printf("Root note %d Chord selected: %s\n", rootNote, LongChordNames[retVal]));
    return retVal;
}


uint8_t MIDIChord::playChord(void)
{
    if( !NoteOn) return rootNote; // No NoteOn defined

    uint8_t chordtoPlay = getChordToPlay();
    const int8_t *chord = chordNotesDef[chordtoPlay];

    if(isPlaying) noteoffChord();

    noteOnTime = getClock();
    int8_t inv = 0;
    const int8_t *invTable = getInvTable(chordtoPlay);
    uint32_t del = 0;
    for (int i = 0; i < chord[0]; i++){
        if(invTable){
            inv = invTable[i];
            //DP(inv);
            }
        int16_t playnote = rootNote + chord[i + 1] + inv;
        playnote = (playnote < 0) ? 0 : playnote;
        playnote = (playnote > 120) ? 120 : playnote;
        NotesPlaying[i] = playnote;
        NoteEvent ev;
        
        ev.fill(MIDIChannel, playnote, Velocity, noteOnTime + del);
        NoteOn(&ev);//playnote, Velocity, MIDIChannel);
        del += delayFix * 10;
        del += (delayRnd) ? (rand() % delayRnd) * 10 : 0;
    }
    isPlaying = chord[0];
    return rootNote;
}

uint8_t MIDIChord::noteoffChord(void)
{
    if( !NoteOff) return rootNote; // No NoteOff defined
    
    //uint8_t chordtoPlay = (ChordType == DEF_CHORD) ? theApp.DefaultChord.getChordType() : ChordType;
    //const int8_t *chord = chordNotesDef[chordtoPlay];

    NoteEvent ev;
    for (int i = 0; i < isPlaying; i++){
        ev.fill(MIDIChannel, NotesPlaying[i], Velocity, getClock()-noteOnTime);
        NoteOff(&ev);//NotesPlaying[i] , Velocity, MIDIChannel);
    }
    isPlaying = 0;
    return rootNote;
}

/** @} */
