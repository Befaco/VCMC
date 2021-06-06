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
    DROP3ROOT_INVDROP,
    DROP3INV1_INVDROP,
    DROP3INV2_INVDROP,
    DROP3INV3_INVDROP,
    
    DROP2ROOT_INVDROP,
    DROP2INV1_INVDROP,
    DROP2INV2_INVDROP,
    DROP2INV3_INVDROP,

    NO_INVDROP,
    INV1_INVDROP,
    INV2_INVDROP,
    INV3_INVDROP,

    OPEN3ROOT_INVDROP,
    OPEN3INV1_INVDROP,
    OPEN3INV2_INVDROP,
    OPEN3INV3_INVDROP,
    
    OPEN2ROOT_INVDROP,
    OPEN2INV1_INVDROP,
    OPEN2INV2_INVDROP,
    OPEN2INV3_INVDROP,

    LAST_INVDROP,
    DEF_INVDROP
};

static const char *const InvDropShortNames[] =
{
    "Dr3 root",
    "Dr3 1Inv",
    "Dr3 2Inv",
    "Dr3 3Inv",

    "Dr2 root",
    "Dr2 1Inv",
    "Dr2 2Inv",
    "Dr2 3Inv",

    "Root",
    "1st Inv",
    "2nd Inv",
    "3rd Inv",
    
    "Op3 root",
    "Op3 1Inv",
    "Op3 2Inv",
    "Op3 3Inv",

    "Op2 root",
    "Op2 1Inv",
    "Op2 2Inv",
    "Op2 3Inv",

    "Invalid Inv",
    "Default"
};

const int8_t InversionDrop4[][4] __attribute__((unused)) = {
    {0, 0, -12, 0},  // root drop 2
    {12, 0, 0, -12}, // 1st inv drop 2
    {0, 12, 0, 0},   // 2nd inv drop 2
    {12, 0, 12, 0},  // 3rd inv drop 2

    {0, -12, 0, 0},   // root drop 3
    {12, 0, -12, 0},  // 1st inv drop 3
    {12, 12, 0, -12}, // 2nd inv drop 3
    {0, 12, 12, 0},    // 3rd inv drop 3

    {0, 0, 0, 0},    // root (close voicing)
    {12, 0, 0, 0},   // 1st inv
    {12, 12, 0, 0},  // 2nd inv
    {12, 12, 12, 0}, //{ 0, 0, 0, -12 },   // 3rd inv

    {0, 12, 0, 0},   // root open 3
    {12, 0, 12, 0},  // 1st inv open 3
    {12, 12, 0, 12}, // 2nd inv open 3
    {24, 12, 12, 0},    // 3rd inv open 3

    {0, 0, 12, 0},  // root open 2
    {12, 0, 0, 12}, // 1st inv open 2
    {24, 12, 0, 0},   // 2nd inv open 2
    {12, 24, 12, 0},  // 3rd inv open 2

    {0, 0, 0, 0},    // Invalid, do not use
    {0, 0, 0, 0}    // Defauly == root
};

const int8_t InversionDrop3[][3] __attribute__((unused)) = {
    {0, -12, 0}, // drop 2
    {12, 0, -12},
    {0, 12, 0},
    {12, 0, 12}, // drop 2

    {-12, 0, 0}, // drop 3
    {12, -12, 0},
    {12, 12, -12},
    {0, 12, 12}, // drop 3

    {0, 0, 0}, //close voicing
    {12, 0, 0},
    {12, 12, 0},
    {12, 12, 12}, //close voicing

    {12, 0, 0}, // open 3 (TODO check sense for triads)
    {12, 12, 0},
    {12, 12, 12},
    {24, 12, 12}, // open 3

    {0, 12, 0}, // open 2
    {12, 0, 12},
    {24, 12, 0},
    {12, 24, 12}, // open 2

    {0, 0, 0}, //Invalid, do noy use
    {0, 0, 0}  //default = root
};

/** @} */

#endif
