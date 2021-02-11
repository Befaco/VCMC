#ifndef __have__Scales_h__
#define __have__Sclaes_h___

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file Scales.h
 *  \brief Music Scales support
 */


enum
{
    FULL_SCALE = 0,
    Lydian_SCALE,
    Ionian_SCALE,
    Mixolydian_SCALE,
    Dorian_SCALE,
    Aeolian_SCALE,
    Phrygian_SCALE,
    Locrian_SCALE,
    LASTSCALE,
    Mpenta_SCALE,
    mpenta_SCALE,
    DEF_SCALE
};

// Representation of notes in the full 12 notes scale.
// First scales are the seven western modes sorted from "Brighter" to "Darker"
// Please note tht the first (root note) have to be set to 1
const bool ScaleNotes[][12] = {
    {1, 2, 3, 4, 5, 6, 7, 7, 9,10,11,12},
    {1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7},
    {1, 0, 2, 0, 3, 4, 0, 5, 0, 6, 0, 7},
    {1, 0, 2, 0, 3, 4, 0, 5, 0, 6, 7, 0},
    {1, 0, 2, 3, 0, 4, 0, 5, 0, 6, 7, 0},
    {1, 0, 2, 3, 0, 4, 0, 5, 6, 0, 7, 0},
    {1, 2, 0, 3, 0, 4, 0, 5, 6, 0, 7, 0},
    {1, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // LAST SCALE
    {1, 0, 2, 0, 3, 0, 0, 4, 0, 5, 0, 0},
    {1, 0, 0, 2, 0, 3, 0, 4, 0, 0, 5, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

static const char * const ScaleShortNames[] =
    {
        "Full",
        "Lydian",
        "Major",
        "Mixol.",
        "Dorian",
        "Minor",
        "Phrygia",
        "Locrian",
        "Last",
        "M penta",
        "m penta",
        "Default",
};

static const char * const ScaleLongNames[] =
    {
        "Full Scale",
        "Lydian mode",
        "Major / Ionian mode",
        "Mixolydian mode",
        "Dorian mode",
        "Minor / Aeolian mode",
        "Phrygian mode",
        "Locrian mode",
        "Last, do not use",
        "Major pentatonic scale",
        "Minor pentatonic scale",
        "Default"
};



struct ScaleDef{
public:
    uint8_t ScaleID;
    const char *shortName;
    const char *longName;
    const bool *Notes;
};

#define DEF_SCALE(ID) \
    ScaleDef{ID, ScaleShortNames[ID], ScaleLongNames[ID], ScaleNotes[ID]}

const ScaleDef ScalesDefinition[] __attribute__((unused)) = {
    DEF_SCALE(FULL_SCALE),
    DEF_SCALE(Lydian_SCALE),
    DEF_SCALE(Ionian_SCALE),
    DEF_SCALE(Mixolydian_SCALE),
    DEF_SCALE(Dorian_SCALE),
    DEF_SCALE(Aeolian_SCALE),
    DEF_SCALE(Phrygian_SCALE),
    DEF_SCALE(Locrian_SCALE),
    DEF_SCALE(LASTSCALE),
    DEF_SCALE(Mpenta_SCALE),
    DEF_SCALE(mpenta_SCALE),
    DEF_SCALE(DEF_SCALE)
};

/*
#define DEF_SCALE(ID, shname, lgname) \
    ScaleDef{ID, shname, lgname, ScaleNotes[ID]}

const ScaleDef ScalesDefinition[] = {
    DEF_SCALE(FULL_SCALE,"Full", "Full Scale"),
    DEF_SCALE(Mpenta_SCALE,"M penta", "Major pentatonic scale"),
    DEF_SCALE(Ionian_SCALE,"Ionian", "Ionian mode or major"),
    DEF_SCALE(Mixolydian_SCALE,"Mixolydian", "Mixolydian mode"),
    DEF_SCALE(Lydian_SCALE,"Lydian", "Lydian mode"),
    DEF_SCALE(Dorian_SCALE,"Dorian", "Dorian mode"),
    DEF_SCALE(Aeolian_SCALE,"Aeolian", "Aeolian or natural minor"),
    DEF_SCALE(mpenta_SCALE,"m penta", "Minor pentatonic scale"),
    DEF_SCALE(Phrygian_SCALE,"Phrygian", "Phrygian mode"),
    DEF_SCALE(Locrian_SCALE,"Locrian", "Locrian mode"),
    DEF_SCALE(LASTSCALE,"Empty", "Empty Invalid Scale"),
};

 */



/** @} */

#endif
