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

/** @addtogroup MenuClass
* @{
*/

/**
 *  \file OLEDMenuConfig.ino
 *  \brief Defines functions for configuration screen  
 */

/**
 * \brief Show Ports configuration data
 * 
 */
void OLEDMenu::displayMenuConfig () {
    int posCursor = 17;

    theOLED->fillRect (POSXCARD, 0, MenuPosX - POSXCARD, 64, BLACK);
    // Visualization on General Cal
    if(GlobalCalEnabled){
        displayGenCal();
        return;
    } 

    // Standard Cards
    displayMenuBankHeader (true);

    InputPortCfg *SelCfg = GetPortCfg ();
    if (!SelCfg) {
        displayBankConfig (posCursor);
        return;
    }
    switch (PortSelected) {
    case 1:
        displayGateConfig (posCursor);
        break;
    case 2:
        displayCVConfig (posCursor);
        break;
    case 3:
        displayFaderConfig (posCursor);
        break;
    }
}


void OLEDMenu::displayGenCal(void){
    int posCursor = 8; //17;
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    // Fader Data
    for (int i = 0; i < NUMCHAN-1;i++){
        theOLED->print(CVControls[i].Slider.PortValue);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        }
    /* 
    // CV Data
    int TrimCV = CVControls[BankSelected].CVPort.TrimValue (-9999);
    theOLED->print (TrimCV);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->print (CVControls[BankSelected].CVPort.PortValue);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    // Fader Data
    int TrimFader = CVControls[BankSelected].Slider.TrimValue (-9999);
    theOLED->print (TrimFader); // CVControls[BankSelected].Slider.MIDIData);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->print (CVControls[BankSelected].Slider.PortValue);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8; */
}

/**
 * \brief Display in OLED the configuration for Bank (CV+Fader+Gate)
 * 
 * \param posCursor 
 */
void OLEDMenu::displayBankConfig (int posCursor) {

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->setTextColor (BLACK, WHITE);
    if (BankSelected == 8)
        theOLED->println (" Aux A ");
    else
        theOLED->println ("   CV   ");
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    PrintFunction(&(CVControls[BankSelected].CVPort.PortCfg));
    /*theOLED->print (PortFuncStr[CVControls[BankSelected].CVPort.PortCfg.MIDIfunction]);
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == CONTROLCHANGE)
        theOLED->println (CVControls[BankSelected].CVPort.PortCfg.ControllerNumber);
    */

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;

    // Fader
    theOLED->setTextColor (BLACK, WHITE);
    if (BankSelected == 8)
        theOLED->println (" Aux B ");
    else
        theOLED->println ("  Fader ");
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    PrintFunction(&(CVControls[BankSelected].Slider.PortCfg));
    /* theOLED->print (PortFuncStr[CVControls[BankSelected].Slider.PortCfg.MIDIfunction]);
    if (CVControls[BankSelected].Slider.PortCfg.MIDIfunction == CONTROLCHANGE)
        theOLED->println (CVControls[BankSelected].Slider.PortCfg.ControllerNumber); */

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;

    // Gate
    if (BankSelected == 8) return;
    theOLED->setTextColor (BLACK, WHITE);
    theOLED->println ("  Gate  ");
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    PrintGateFunction(&(CVControls[BankSelected].GateBut.PortCfg));
}

void OLEDMenu::PrintGateFunction(DigPortCfg *cfgSel)
{    
    theOLED->print (PortGateFuncStr[cfgSel->MIDIfunction]);
    switch (cfgSel->MIDIfunction) {
    case TRIGGER:
        theOLED->print (cfgSel->NoteToSend);
        break;
    case LATCH:
        theOLED->print (cfgSel->NoteToSend);
        break;
    case CCLATCH:
    case CCBUT:
        theOLED->print (cfgSel->ControllerNumber);
        theOLED->print ("#");
        theOLED->print (cfgSel->ControllerValue);
        break;
    case GATECLOCK:
    case GATESTARTSTOP:
    case GATEPAUSECONT:
    case GATETRIGSTARTSTOP:
    case GATETRIGPAUSECONT:
    case GATESTART:
    case GATECONTINUE:
    case GATESTOP:
    case GATEPANIC:
    case NODIGFUNC:
        break;
    }
}

void OLEDMenu::PrintFunction(AnInputPortCfg *cfgSel){
    theOLED->print (PortFuncStr[cfgSel->MIDIfunction]);

    if (cfgSel->MIDIfunction == CONTROLCHANGE||cfgSel->MIDIfunction == CC14BITS) {
        theOLED->println (cfgSel->ControllerNumber);
    } else
    {
        theOLED->println ("");
    }
}

/**
 * \brief Display in OLED the configuration for CV port
 * 
 * \param posCursor 
 */
void OLEDMenu::displayCVConfig (int posCursor) {
    AnInputPortCfg *SelCfg = (AnInputPortCfg*)GetPortCfg ();

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->setTextColor (BLACK, WHITE);
    //theOLED->print ("MIDI CH");
    theOLED->print ("CHAN.:");
    theOLED->setTextColor (WHITE, BLACK);
    //theOLED->setCursor (POSXCARD + 0, posCursor);
    //posCursor += 8;
    theOLED->print(" ");
    theOLED->println (SelCfg->MIDIChannel);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    char name[24];
    if( SelCfg->getName(name)){
        // Print Name
        theOLED->print(name);
    } else {
        theOLED->setTextColor (BLACK, WHITE);
        theOLED->print ("Rg.");
        theOLED->setTextColor (WHITE, BLACK);
        theOLED->println (RangeInTxt[SelCfg->getInputRange()]);
    }

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->setTextColor (BLACK, WHITE);
    theOLED->print ("FUNCTION");
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    PrintFunction(SelCfg);

    switch (((AnInputPortCfg *)SelCfg)->MIDIfunction) {
    case ANAGCLOCK:
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->setTextColor (BLACK, WHITE);
        theOLED->println ("  Div  ");
        theOLED->setTextColor (WHITE, BLACK);
		if( ((AnInputPortCfg *)SelCfg)->ClockShift >-6 && ((AnInputPortCfg *)SelCfg)->ClockShift <6){
			theOLED->print(ClokShiftTxt[((AnInputPortCfg *)SelCfg)->ClockShift+5]);
			}
		theOLED->print ("-");
        theOLED->print (((AnInputPortCfg *)SelCfg)->ClockDivider);
        /*theOLED->println ("");
        theOLED->print ("Div: ");
        theOLED->println (((AnInputPortCfg *)SelCfg)->ClockDivider);*/
        break;
	case ANAGNRPN7bits:
	case ANAGNRPN14bits:
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
	    theOLED->setTextColor (BLACK, WHITE);
        theOLED->print("MSB:");
        theOLED->setTextColor (WHITE, BLACK);
		theOLED->println (((AnInputPortCfg *)SelCfg)->NRPNparMSB);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
	    theOLED->setTextColor (BLACK, WHITE);
		theOLED->print("LSB:");
        theOLED->setTextColor (WHITE, BLACK);
		theOLED->println (((AnInputPortCfg *)SelCfg)->NRPNparLSB);
		break;
    }
}


/**
 * \brief Display in OLED the configuration for Fader port
 * 
 * \param posCursor 
 */
void OLEDMenu::displayFaderConfig (int posCursor) { displayCVConfig (posCursor); }

/**
 * \brief Display in OLED the configuration for Gate port
 * 
 * \param posCursor 
 */
void OLEDMenu::displayGateConfig (int posCursor) {
    DigPortCfg *SelCfg = &CVControls[BankSelected].GateBut.PortCfg;
/*	
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->print (PortGateFuncStr[SelCfg->MIDIfunction]);
*/
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->setTextColor (BLACK, WHITE);
    //theOLED->print ("MIDI CH");
    theOLED->print ("CHAN.:");
    theOLED->setTextColor (WHITE, BLACK);
    //theOLED->setCursor (POSXCARD + 0, posCursor);
    //posCursor += 8;
    theOLED->print(" ");
    theOLED->println (SelCfg->MIDIChannel);


    char name[24];
    if( SelCfg->getName(name)){
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        // Print Name
        theOLED->print(name);
    }
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->setTextColor (BLACK, WHITE);
    theOLED->print ("FUNCTION");
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    theOLED->print (PortGateFuncStr[SelCfg->MIDIfunction]);
	
    switch (SelCfg->MIDIfunction) {
    case TRIGGER:
        break;
    case LATCH:
        break;
    case CCLATCH:
    case CCBUT:
        theOLED->print (SelCfg->ControllerNumber);
        theOLED->print ("#");
        theOLED->print (SelCfg->ControllerValue);
        break;
    case GATECLOCK:
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->setTextColor (BLACK, WHITE);
        theOLED->println ("  Div  ");
        theOLED->setTextColor (WHITE, BLACK);
		if( SelCfg->ClockShift >-6 && SelCfg->ClockShift <6){
			theOLED->print(ClokShiftTxt[SelCfg->ClockShift+5]);
			}
		theOLED->print ("-");
        theOLED->print (SelCfg->ClockDivider);
        break;
    case GATESTARTSTOP:
    case GATEPAUSECONT:
    case GATETRIGSTARTSTOP:
    case GATETRIGPAUSECONT:
    case GATESTART:
    case GATECONTINUE:
    case GATESTOP:
    case GATEPANIC:
    case NODIGFUNC:
        break;
    }
}



/** @} */