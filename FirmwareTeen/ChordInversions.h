#ifndef __have__ChordInversions_h__
#define __have__ChordInversions_h__

/** @addtogroup MIDI
 *  @{
 */

/**
 *  \file ChordInversions.h
 *  \brief Music Chord Inversions definition
 */

enum
{
    DEF_INVDROP = 0,

    NO_INVDROP ,
    INV1_INVDROP,
    INV2_INVDROP,
    INV3_INVDROP,
    DROP2ROOT_INVDROP,
    DROP2INV1_INVDROP,
    DROP2INV2_INVDROP,
    DROP2INV3_INVDROP,
    DROP3ROOT_INVDROP,
    DROP3INV1_INVDROP,
    DROP3INV2_INVDROP,
    DROP3INV3_INVDROP,

    LAST_INVDROP
};

static const char *const InvDropShortNames[] =
{
    "Default",

    "Root",
    "1st Inv",
    "2nd Inv",
    "3rd Inv",
    "Drop 2 root",
    "Drop 2 Inv 1",
    "Drop 2 Inv 2",
    "Drop 2 Inv 3",
    "Drop 3 root",
    "Drop 3 Inv 1",
    "Drop 3 Inv 2",
    "Drop 3 Inv 3",

    "Invalid Inv"
};

const int8_t InversionDrop4[][4] __attribute__((unused)) = {
    {0, 0, 0, 0},    // Defauly == root

    {0, 0, 0, 0},    // root (close voicing)
    {12, 0, 0, 0},   // 1st inv
    {12, 12, 0, 0},  // 2nd inv
    {12, 12, 12, 0}, //{ 0, 0, 0, -12 },   // 3rd inv

    {0, 0, -12, 0},  // root drop 2
    {12, 0, 0, -12}, // 1st inv drop 2
    {0, 12, 0, 0},   // 2nd inv drop 2
    {12, 0, 12, 0},  // 3rd inv drop 2

    {0, -12, 0, 0},   // root drop 3
    {12, 0, -12, 0},  // 1st inv drop 3
    {12, 12, 0, -12}, // 2nd inv drop 3
    {0, 12, 12, 0}    // 3rd inv drop 3
};

const int8_t InversionDrop3[][3] __attribute__((unused)) = {
    {0, 0, 0}, //default = root

    {0, 0, 0}, //close voicing
    {12, 0, 0},
    {12, 12, 0},
    {0, 0, 0}, //close voicing

    {0, -12, 0}, // drop 2
    {12, 0, -12},
    {0, 12, 0},
    {0, -12, 0}, // drop 2

    {-12, 0, 0}, // drop 3
    {12, -12, 0},
    {12, 12, -12},
    {-12, 0, 0}, // drop 3

};

/** @} */

#endif
