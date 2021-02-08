#ifndef __have__Chords_h__
#define __have__Chords_h___

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file Chords.h
 *  \brief Music Chords support
 */

const int8_t chordNotesDef[][9] = {
    {1, 0},

    {3, 0, 4, 7},
    {4, 0, 4, 7, 11},

    {3, 0, 3, 7},
    {3, 0, 3, 6},
    {3, 0, 4, 8},
    {3, 0, 5, 7},

    {4, 0, 4, 7, 10},
    {4, 0, 3, 7, 10},
    {4, 0, 3, 6, 9},
    {4, 0, 4, 8, 10},
    {4, 0, 3, 6, 10},

    {4, 0, 4, 7, 9},

    {2, 0, 7},
    {4, 0, 4, 8, 11},
    {4, 0, 5, 6, 7},
    {4, 0, 3, 7, 9},
    {5, 0, 4, 7, 11, 14},
    {5, 0, 3, 7, 10, 14},
    {7, 0, 4, 7, 11, 14, 18, 21},
    {7, 0, 3, 7, 10, 14, 17, 21},
    {8, 0, 1, 5, 6, 10, 12, 15, 17},
    {3, 0, 6, 10},
    {3, 0, 5, 8},
    {3, 0, 1, 6},
    {4, 0, 5, 7, 10},
    {4, 0, 3, 6, 11},
    {4, 0, 4, 6, 10},
    {4, 0, 3, 7, 11},
    {4, 0, 2, 4, 7},
    {5, 0, 4, 7, 10, 15},
    {5, 0, 4, 7, 10, 13},
    {5, 0, 4, 7, 10, 14},
    {5, 0, 7, 9, 13, 16},
    {5, 0, 8, 11, 16, 21},
    {5, 0, 4, 7, 11, 18},
    {5, 0, 4, 7, 9, 14},
    {5, 0, 4, 8, 11, 18},
    {5, 0, 3, 7, 9, 14},
    {5, 0, 4, 8, 10, 14},
    {5, 0, 4, 6, 10, 14},
    {5, 0, 4, 7, 9, 10},
    {5, 0, 5, 10, 15, 19},
    {6, 0, 4, 7, 10, 14, 18},
    {6, 0, 4, 7, 10, 14, 17},
    {6, 0, 1, 4, 5, 8, 9},
    {6, 0, 4, 7, 11, 14, 17},
    {6, 0, 3, 7, 10, 14, 17},
    {6, 0, 6, 10, 16, 21, 14},
    {6, 0, 1, 4, 6, 7, 10},
    {7, 0, 4, 7, 10, 14, 17, 21},

    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0}
};

static const char *const ChordNames[] = {
    "No Chord",

    "Major",
    "Major 7th",

    "Minor",
    "Diminished",
    "Augmented",
    "Suspended",

    "Dom 7th",
    "Minor 7th",
    "Dim 7th ",
    "Aug 7th",
    "HalfDim 7th",

    "Major 6th",

    "Power P5",
    "Aug major 7th",
    "Dream",
    "Minor 6th",
    "Major 9th",
    "Minor 9h",
    "Major 13th",
    "Minor 13th",
    "Magic",
    "Aug 6th italian",
    "Neapolitan",
    "Viennese trichord",
    "7th suspension four",
    "Dim major 7th",
    "Dom 7th flat five",
    "Minor major 7th",
    "Mu",
    "Dom 7th sharp nine",
    "Dom minor 9th",
    "Dom 9th",
    "Elektra",
    "Farben",
    "Lydian",
    "Major 6th ninth",
    "Major 7th sharp 11th",
    "Minor 6th ninth",
    "Ninth Aug. 5th",
    "Ninth flat 5th",
    "Seven six",
    "So What",
    "Aug 11th",
    "Dom 11th",
    "Hexachord",
    "Major 11th",
    "Minor 11th",
    "Mystic",
    "Petrushka",
    "Dom 13th",

    "Last Chord",
    "Default"
};

static const char *const LongChordNames[] = {
    "No Chord",

    "Major chord",
    "Major seventh chord",

    "Minor chord",
    "Diminished chord",
    "Augmented chord",
    "Suspended chord",

    "Dominant seventh chord",
    "Minor seventh chord",
    "Diminished seventh chord ",
    "Augmented seventh chord",
    "Half-diminished seventh chord",

    "Major sixth chord",

    "Power chord P5",
    "Augmented major seventh chord",
    "Dream chord",
    "Minor sixth chord",
    "Major ninth chord",
    "Minor ninth chord",
    "Major thirteenth chord",
    "Minor thirteenth chord",
    "Magic chord",
    "Augmented sixth chord italian",
    "Neapolitan chord",
    "Viennese trichord",
    "Seventh suspension four chord",
    "Diminished major seventh chord",
    "Dominant seventh flat five chord",
    "Minor major seventh chord",
    "Mu chord",
    "Dominant seventh sharp nine / Hendrix chord",
    "Dominant minor ninth",
    "Dominant ninth",
    "Elektra chord",
    "Farben chord",
    "Lydian chord",
    "Major sixth ninth chord",
    "Major seventh sharp eleventh chord",
    "Minor sixth ninth chord (6/9)",
    "Ninth augmented fifth chord",
    "Ninth flat fifth chord",
    "Seven six chord",
    "So What chord",
    "Augmented eleventh chord",
    "Dominant eleventh chord",
    "Hexachord Ode-to-Napoleon",
    "Major eleventh chord",
    "Minor eleventh chord",
    "Mystic chord",
    "Petrushka chord",
    "Dominant thirteenth chord",

    "Last Chord,do not use",
    "Default",
};

enum
{
    ONENOTECHORD = 0,

    MAJOR,
    MAJOR_7TH,

    MINOR,
    DIMINISHED,
    AUGMENTED,
    SUSPENDED,

    DOM_7TH,
    MINOR_7TH,
    DIM_7TH,
    AUG_7TH,
    HALFDIM_7TH,

    MAJOR_6TH,

    POWER_P5,
    AUG_MAJOR_7TH,
    DREAM,
    MINOR_6TH,
    MAJOR_NINTH,
    MINOR_NINTH,
    MAJOR_13TH,
    MINOR_13TH,
    MAGIC,
    AUG_6TH_ITALIAN,
    NEAPOLITAN,
    VIENNESE_TRICHORD,
    SEVTH_SUSPENSION_FOUR,
    DIM_MAJOR_7TH,
    DOM_7TH_FLAT_FIVE,
    MINOR_MAJOR_7TH,
    MU,
    DOM_7TH_SHARP_NINE,
    DOM_MINOR_NINTH,
    DOM_NINTH,
    ELEKTRA,
    FARBEN,
    LYDIAN,
    MAJOR_6TH_NINTH,
    MAJOR_7TH_SHARP_11TH,
    MINOR_6TH_NINTH,
    NINTH_AUG_FIFTH,
    NINTH_FLAT_FIFTH,
    SEVEN_SIX,
    SO_WHAT,
    AUG_11TH,
    DOM_11TH,
    HEXACHORD,
    MAJOR_11TH,
    MINOR_11TH,
    MYSTIC,
    PETRUSHKA,
    DOM_13TH,

    LASTCHORD,
    DEF_CHORD

};


// diatonic chords
const uint8_t diatonicNumberOfChords = 2;
const int8_t diatonicChords[][diatonicNumberOfChords][12] = {
    { // Lydian {1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7},
    {MAJOR, 0, MAJOR, 0, MINOR, 0, DIMINISHED, MAJOR, 0, MINOR, 0, MINOR},
    {MAJOR_7TH, 0, MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0, MINOR_7TH, 0, HALFDIM_7TH}
    },
    
    { // Major/Ionian {1, 0, 2, 0, 3, 4, 0, 5, 0, 6, 0, 7},
    {MAJOR, 0, MINOR, 0, MINOR, MAJOR, 0, MAJOR, 0, MINOR, 0, DIMINISHED},
    {MAJOR_7TH, 0, MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0, MINOR_7TH, 0, HALFDIM_7TH}
    },
    
    { // Mixolydian {1, 0, 2, 0, 3, 4, 0, 5, 0, 6, 7, 0}
    {MAJOR, 0, MINOR, 0, DIMINISHED, MAJOR, 0, MINOR, 0, MINOR, MAJOR, 0},
    {MAJOR_7TH, 0, MINOR_7TH, 0, HALFDIM_7TH, MAJOR_7TH, 0, MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0}
    },

    {// Dorian {1, 0, 2, 3, 0, 4, 0, 5, 0, 6, 7, 0}
    {MINOR, 0, MINOR, MAJOR, 0, MAJOR, 0, MINOR, 0, DIMINISHED, MAJOR,0},
    {MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0, MINOR_7TH, 0, HALFDIM_7TH, MAJOR_7TH,0}
    },

    {// Minor {1, 0, 2, 3, 0, 4, 0, 5, 6, 0, 7, 0}
    {MINOR, 0, DIMINISHED, MAJOR, 0, MINOR, 0, MINOR, MAJOR, 0, MAJOR, 0},
    {MINOR_7TH, 0, HALFDIM_7TH, MAJOR_7TH, 0, MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0}
    },

    {// Phrygian {1, 2, 0, 3, 0, 4, 0, 5, 6, 0, 7, 0}
    {MINOR, MAJOR, 0, MAJOR, 0, MINOR, 0, DIMINISHED, MAJOR, 0, MINOR, 0},
    {MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0, MINOR_7TH, 0, HALFDIM_7TH, MAJOR_7TH, 0, MINOR_7TH, 0}
    },

    {// Locrian {1, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7, 0}
    {DIMINISHED, MAJOR, 0, MINOR, 0, MINOR, MAJOR, 0, MAJOR, 0, MINOR, 0},
    {HALFDIM_7TH, MAJOR_7TH, 0, MINOR_7TH, 0, MINOR_7TH, MAJOR_7TH, 0, MAJOR_7TH, 0, MINOR_7TH, 0},
    }
};


const int8_t allDiatonicChords[][2][7][9] = {
    // Major/Ionian
    { 
        { // C Major Triad
        {3, 0, 4, 7},
        {3, 0, 3, 7},
        {3, 0, 3, 7},
        {3, 0, 4, 7},
        {3, 0, 4, 7},
        {3, 0, 3, 7},
        {3, 0, 3, 6}
        },
        { // Cmajor-7
        {4, 0, 4, 7, 11},
        {4, 0, 3, 7, 10},
        {4, 0, 3, 7, 10},
        {4, 0, 4, 7, 11},
        {4, 0, 4, 7, 10},
        {4, 0, 3, 7, 10},
        {4, 0, 3, 6, 10}
        },
    },
    // Minor
    { 
        { // C Minor Triad
        {3,0,3,7},
        {3,0,3,6},
        {3,0,4,7},
        {3,0,3,7},
        {3,0,3,7},
        {3,0,4,7},
        {3,0,4,7}
        },
        { // C Minor 7th
        {4,0,3,7,10},
        {4,0,3,6,10},
        {4,0,4,7,11},
        {4,0,3,7,10},
        {4,0,3,7,10},
        {4,0,4,7,11},
        {4,0,4,7,10}
        }
    }
};

/** @} */

#endif
