// To give your project a unique name, this code must be
// placed into a .c file (its own tab).  It can not be in
// a .cpp file or your main sketch (the .ino file).

#include "usb_names.h"
//#define CVTHING

// Edit these lines to create your own name.  The length must
// match the number of characters in your custom name.
#ifdef CVTHING
        #define MIDI_NAME   {'C','V','T','h','i','n','g'}
        #define MIDI_NAME_LEN  7
#else
        #define MIDI_NAME   {'V','C','M','C'}
        #define MIDI_NAME_LEN  4
#endif

// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};