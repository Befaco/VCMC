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
/**
 *  \file FirmwareTeen.ino
 *  \brief Project main loop
 */

#include "PrjIncludes.h"
#include "MenuLists.h"

///////////////////////////////////////
//Variables definition
VCMCApp theApp; /// Main App variable

//////////////////////////////////////
// Global  declaration


//////////////////////////////////////
void setup () {
	//pau=micros();
    theApp.setup();
    DP("Init App");

}


/**
 *  \brief Main loop
 *
 *  \details Tasks performed in the main loop:\n
 *  . Update the encoder
 *  . Check for calibration timer
 *  . For each channel check changes and act on then. See InputControl::DoChanges()
 *  . Update the Menu
 *  . Update the display
 *  . Poll the MIDI channels
 *
 */
void loop () {

    // Read encoder button
    EncButton->update ();

    if (CalTimer) {
        if (CalTimer + 5000 < millis ()) {
            CalTimer = 0;
            AutoCal = false; // AutoCal to false after 50 seconds
        }
    }
	
    // React to changes on channels
    for (int i = 0; i < NUMCHAN; i++) {
        #ifdef USEREADINTERR
        noInterrupts ();
        #endif
        // React to changes
        CVControls[i].DoChanges ();
        #ifdef USEREADINTERR
        interrupts ();
        #endif
    }

    #ifdef DEBUGMODE
        beginDebugTimers ();
    #endif
    // Update the menu
    if (gotoGloblaMenu) {
        gotoMenuGlobalCfg ();
        if (durationButton == 0) {
            durationButton = 1;
        }
        myMenu.displayMenu ();
    } else
        myMenu.doMenu ();
    #ifdef DEBUGMODE
        endDebugTimers ();
    #endif
 
    // Refresh the screen every REFRESH_RATE usec
    if (micros () - myMenu.disptimer > REFRESH_RATE) {
        theOLED->display ();
        myMenu.disptimer = micros ();
    }

    // Refresh MIDI stream
    MidiMerge.poll ();
    #ifdef USEOSC
    theApp.OSCMerge.poll();
    #endif
    #ifdef USEI2C
    theApp.I2CMerge.poll();
    #endif
}
