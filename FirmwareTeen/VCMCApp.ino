// Copyright 2019 Sergio Retamero.
//
// Author: Sergio Retamero (sergio.retamero@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
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

/** @ingroup Maingroup
 * @{
*/

/**
 * \file VCMCApp.ino
 * \brief Main App functions and interrupt handling
 * 
 */

volatile bool servicingPorts = false;   /// Flag true when Interruput handling service is reading the input ports
volatile int AnagPortService = 0;       /// Port to service inside the Interrupt reading function
volatile int countCycles = 0;           /// Cycles to wait for fader slider reading frequency feature

/**
 * \brief Service function for encoder
 */ 
void timerIsr() { encoder->service(); }

/**
 *  \brief Interrupt service to read ports
 *
 *  \details Every cycle will read all digital ports and one analog CV. Fader input is read every 8 complete CV cycles
 */
void servicePorts () {

    if (servicingPorts) return;
    servicingPorts = true;

    CVControls[AnagPortService].CVPort.SetDemux (); // Prepare demux for next reading
    // Read all the gates
    #ifndef CVTHING
    for (int i = 0; i < NUMCHAN-1; i++) {
        CVControls[i].ReadPorts (true, false, false);
        CVControls[i].Blink ();
    }
    // Read Aux
    if( CVControls[8].CVPort.PortCfg.IsDigitalFunc() || countCycles==3)
		CVControls[8].ReadPorts (false, true, false);
    if( CVControls[8].Slider.PortCfg.IsDigitalFunc() || countCycles==6)
		CVControls[8].ReadPorts (false, false, true);
    #endif
    // Read CV and Faders
	if( countCycles==0){
        #ifndef CVTHING
		adc->analogRead (PIN_A13);
		adc->analogRead (PIN_A13);
		CVControls[AnagPortService].ReadPorts (false, false, true);
        #endif
	} else if(countCycles!=3 && countCycles!=6)
		CVControls[AnagPortService].ReadPorts (false, true, false);

	if( AnagPortService==7) countCycles++;
	if( countCycles>8) countCycles=0;
	
    AnagPortService++;
    if (AnagPortService >= NUMCHAN - 1) AnagPortService = 0;

    CVControls[AnagPortService].CVPort.SetDemux (); // Prepare demux for next reading
    
    servicingPorts = false;
}



/**
 *  \brief Helper function to get the currently selected Config port
 *
 *  \return Currently selected config port pointer
 *
 */
InputPortCfg *VCMCApp::GetPortConfig (void) {
    InputPortCfg *SelCfg = NULL;

    switch (byPortSelected) {
    case 1:
        SelCfg = &Controls[byBankSelected].GateBut.PortCfg;
        break;
    case 2:
        SelCfg = &Controls[byBankSelected].CVPort.PortCfg;
        break;
    case 3:
        SelCfg = &Controls[byBankSelected].Slider.PortCfg;
        break;
        //default: return NULL;
    }
    return SelCfg;
}


// Calbbacks
void sendNoteOn(uint8_t  note, uint8_t  vel, uint8_t  chan){
    MidiMerge.sendNoteOn( note,   vel, chan);
}
void sendNoteOff(uint8_t  note, uint8_t  vel, uint8_t  chan){
    MidiMerge.sendNoteOff( note,   vel, chan);
}


/**@}*/