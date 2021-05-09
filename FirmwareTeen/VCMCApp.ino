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

//#include "src/MidiThing/MIDITools/NoteEvent.h"
// Callbacks
void sendNoteOn(NoteEvent *pEv){
    if(!pEv->timestamp)
        MidiMerge.sendNoteOn( pEv->pitch, pEv->velocity, pEv->chann);
    else{
        NoteEvent *pnew = theApp.eventList.push_back();
        if (!pnew) return;
        // Check if note on event present on the list
        for( NoteEvent*pNon= theApp.eventList.front(); /* pNon &&  */pNon!=pnew; ){
            if(pEv->chann==pNon->chann && pEv->pitch==pNon->pitch ){
                // Remove previous events
                NoteEvent *pn = (NoteEvent *)pNon->next();
                theApp.eventList.remove(pNon);
                pNon = pn;
                MidiMerge.sendNoteOff( pEv->pitch, pEv->velocity, pEv->chann); // Note off previous
                // break;
            } else
                pNon = (NoteEvent *)pNon->next();
        }
        pnew->fill(pEv);
        //theApp.eventList.print("Note on added ");
        }
}

void sendNoteOff(NoteEvent *pEv){
    if(!pEv->timestamp)
        MidiMerge.sendNoteOff( pEv->pitch, pEv->velocity, pEv->chann);
    else{
        if(!pEv->pitch){
            DP("No pitch");
            return;
        }
        NoteEvent *pnew = theApp.eventList.push_back();
        if(!pnew)
            return;  // List full
        // Check if note on event present on the list
        for( NoteEvent*pNon= theApp.eventList.front(); pNon!=pnew;/* pNon; */ pNon = (NoteEvent *)pNon->next()){
            if(pEv->chann==pNon->chann && pEv->pitch==pNon->pitch ){
                pEv->timestamp = pNon->timestamp + pEv->timestamp; // Add delay to NoteOn event
                break;
            }
        }
        if(pEv->timestamp < getClock())
            pEv->timestamp = 0;
        pnew->fill(pEv);
        pnew->velocity = 0; // mark as note off
        //pnew->print("Note off: ");
        //theApp.eventList.print("notes in list: ");
        }
}


/**@}*/