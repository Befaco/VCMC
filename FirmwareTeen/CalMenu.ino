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

#include "PrjIncludes.h"

/** @addtogroup MenuClassDefs
* @{
*/

/**
 *  \file CalMenu.ino
 *  \brief Defines Menu functions callbacks response for calibration and clock configuration 
 */

DemuxAnalogPort *PortInUse = NULL;
Item_Function runFunc = NULL;

uint32_t calPoints[21];
bool pointChanged[21];
RangeConv *rangeData = NULL;
long PortData = -1;
int noteRec = -1;
bool InitCal = false;
int PointSelected = 0;
// Store values for cancel option
bool oldBipolar;
int16_t OldminD, OldRangeD;
int16_t DAC1 = 0, DAC2 = 0;
int16_t OldClipLow = 0, OldClipHigh = 120;
int16_t OldminMIDI,OldrangeMIDI;
uint8_t OldMIDIfunction;
	
byte MAXMENUSETRANGES = 7;
byte MAXMENUCLOCKRANGES = 4;//5;
byte MAXMENU2POINTS = 3;

CalModes calMode = NoCalMode;

uint8_t LastpointRec = 0;
static uint8_t RangeAutoCalPoint = 5;

long FadersData[8];
long MinFadersData[8];
long MaxFadersData[8];
int noteFadersRec[8];

// MenuItem CalMenuItems[25];
// MenuList CalMenuList(NULL, 0, ListCards);

// Base Calibration  functions
bool selectionCalMode () {
    if (EncButton->rose ()) {
        EncButton->update ();
        return true;
    } else
        return false;
}

int updateCalSelection (int maxval) {
    int lineMove = 0;

    // Read encoder
    valueEncoder -= encoder->getValue ();

    // No Change in encoder
    if (valueEncoder == lastEncoder) return 0;

    if (valueEncoder > lastEncoder) {
        lineMove = 1;
        PointSelected = (PointSelected < maxval) ? PointSelected + 1 : PointSelected;//0;
    } else {
        lineMove = -1;
        PointSelected = (PointSelected > 0) ? PointSelected - 1 : 0;//maxval;
    }

    lastEncoder = valueEncoder;
    return lineMove;
}


// Clears menu screen area
void ClearCalArea () {
    theOLED->fillRect (0, 0, 128, 64, BLACK); // Erase input area
    // theOLED->display();
}


bool InitCalibration (void) {
    byte UsePort = PortSelected;

	runFunc = NULL;
	
    // For Matrix Calibration use CV
    if (calMode == MatrixCalMode)
        UsePort = 2;
    else if (UsePort < 2 && calMode!=SetClockMode)
        return false; // return false if gate selected

    switch (UsePort) {
    case 1:
        PortInUse = (DemuxAnalogPort *)&CVControls[BankSelected].GateBut;
        break;
    case 2:
        PortInUse = &CVControls[BankSelected].CVPort;
        break;
    case 3:
        PortInUse = &CVControls[BankSelected].Slider;
        break;
    }
	if( UsePort != 1){
		oldBipolar = PortInUse->PortCfg.getInputRange();
		rangeData = &PortInUse->PortCfg.Ranges;
		rangeData->getDAC (OldminD, OldRangeD); // Store old DAC values
		OldClipLow = PortInUse->PortCfg.ClipLow;
		OldClipHigh = PortInUse->PortCfg.ClipHigh;
		rangeData->getMIDI(OldminMIDI,OldrangeMIDI);
	}
	if( calMode==TwoPointCalMode||calMode == MatrixCalMode){
//		int16_t minD, RangeD;
//		rangeData->getMIDI(minD,RangeD);
//		if( minD!=0 || RangeD!=120) return false; // Calibration only with 0-120 MIDI range
		OldMIDIfunction= PortInUse->PortCfg.MIDIfunction;
		PortInUse->PortCfg.SetMIDIFunc(PITCHTRIG); // Select Pitch trigger for calibration
	}	
	

    PointSelected = 1;
    PortData = -1;
    noteRec = -1;
    LastpointRec = 0;
    if (calMode == MatrixCalMode) {
        PortInUse->PortCfg.setInputRange(NOOFFSET);
        rangeData->SetDAC (INITMINDAC, INITRANGEDAC);
        for (int i = 0; i < 20; i++) {
            calPoints[i] = 0;//DACPoints[i]; // Store original points (for cancel purpose)
            pointChanged[i] = false;
        }
    }
    InitCal = true;

    return true;
}


////////////////////////////////////////
// Auto calibration
bool AutoCalCV () {

    if (calMode == TwoPointCalMode) {
        if (TwoPointsCal ()) {
            calMode = MatrixCalMode;
            InitCal = true;
        } else
            return false;
    }
    return CalibrateCV ();
}


////////////////////////////////////////
// CV matrix calibration

// Display Calibration screen
void displayCalibrationCV () {
    char outBuf[17];
    int posx = 0, posy = 0;
    int16_t minD, rangeD;

    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy);
    theOLED->print ("Channel:");
    theOLED->print (BankSelected + 1);
    theOLED->print (" Point:");
    if (PointSelected) {
        theOLED->print (0.5 * PointSelected, 1);
        theOLED->println ("V");
    } else
        theOLED->println ("END");

    // Print values
    for (int i = 0, pCursor = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++, pCursor++) {
            if (PointSelected == i * 4 + j)
                theOLED->setTextColor (BLACK, WHITE);
            else
                theOLED->setTextColor (WHITE, BLACK);
            theOLED->setCursor (posx + j * 6 * 5, posy + i * 9 + 10);
            if (i + j == 0)
                theOLED->println (" END");
            else {
               // padvalue (DACPoints[pCursor], outBuf, 4);
                theOLED->println (outBuf);
                if (pointChanged[pCursor])
                    theOLED->drawFastHLine (posx + j * 6 * 5, posy + i * 9 + 14, 24, WHITE);
            }
        }
    }
    // Rect around input area
    // theOLED->drawRect(posx,posy,strlen(title)*6+padPos*12+3,18,WHITE);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (0, 55);
    theOLED->print ("In: ");
    theOLED->print (noteRec);
    theOLED->print ("/");
    theOLED->print (PortData);
    theOLED->print ("/");
    rangeData->getDAC (minD, rangeD);
    theOLED->print ((float)(PortData - minD) / rangeD * 10.0, 2);
    theOLED->println ("V");

    theOLED->display ();
}


bool CalibrateCV () 
{
    calMode = MatrixCalMode;

    // Init Calibration
    if (!InitCal)
        if (!InitCalibration ()) return true; // If Init fails, abort

    // Read CV Port
    // if( CVControls[BankSelected].CVPort.ReadPort( PortData))
    PortInUse->ReadPort (PortData);
    noteRec = PortInUse->MIDIData;

    updateCalSelection (19); // Update Selection
    displayCalibrationCV (); // Display values

    // If button pressed (LOW input), finish
    if (!selectionCalMode ()) {
        uint8_t pointRec = IsCalPoint (PortData);
        if (!AutoCal || pointRec == 0) return false;
        if (pointRec == LastpointRec) return false;
        LastpointRec = PointSelected = pointRec;
    }
    if (PointSelected == 0) { // END Selected
        ClearCalArea (); // Clear area used
        CVControls[BankSelected].CVPort.PortCfg.setInputRange(oldBipolar);
        rangeData->SetDAC (OldminD, OldRangeD); // Restore old DAC values
        InitCal = false;
        calMode = NoCalMode;
        // TODO Check here if OK or Cancel
        return true;
    } else { // Accept input in point selected
        // TODO Check conditions for point acceptance here
        //DACPoints[PointSelected] = PortData;
        pointChanged[PointSelected] = true;
        PointSelected = (PointSelected < 19) ? PointSelected + 1 : 0; // Step to next point
    }

    return false;
}


uint8_t IsCalPoint (uint16_t DACpoint) {
    if (calMode == TwoPointCalMode) {
        if (DACpoint < DAC1 + RangeAutoCalPoint * 20 && DACpoint > DAC1 - RangeAutoCalPoint * 20)
            return 2; // Point 1
        if (DACpoint < DAC2 + RangeAutoCalPoint * 20 && DACpoint > DAC2 - RangeAutoCalPoint * 20)
            return 3; // Point 2
        int16_t DACMid = PortInUse->PortCfg.Ranges.Convert (60);
        if (DACpoint < DACMid + RangeAutoCalPoint * 20 && DACpoint > DACMid - RangeAutoCalPoint * 20)
            return 1; // End
    } else {
        /*
        for (int i = 1; i < 20; i++)
            if (DACpoint < DACPoints[i] + RangeAutoCalPoint && DACpoint > DACPoints[i] - RangeAutoCalPoint)
                return i;
        */
    }
    return 0;
}


/////////////////////////////////////////////////////////
// Two points calibration

void displayTwoPoints () {
    //char outBuf[17];
    int posx = 0, posy = 0;
    int16_t minD, rangeD;
    rangeData->getDAC (minD, rangeD);

    //bool Is5V = (rangeD > 0) ? rangeD < INITRANGEDAC / 2 + 100 : rangeD > INITRANGEDAC / 2 - 100;
    bool Is5V = (PortInUse->PortCfg.getInputRange() == ZEROTO5V);

    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy);
    theOLED->print ("Ch:");
    if( BankSelected>7){
		if( PortSelected == 2) theOLED->print ("Aux A");
		else  theOLED->print ("Aux B");
		}
	else{
		theOLED->print (BankSelected + 1);
		if (PortSelected == 2)
			theOLED->print ("  CV ");
		else
			theOLED->print (" FAD ");
		}
    if (PointSelected == 0)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->print (" CANCEL ");
    if (PointSelected == 1)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->println ("END");

    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy + 12+5);
    theOLED->println ("POINT MIDI  NOTE");

    // Print values
    if (PointSelected == 2)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy + 22+5);
    if( Is5V){
        theOLED->print (" 1V    12   C-1 ");//" 2V    24    C0 "); 
    } else{
        theOLED->print (" 2V    24    C0 "); 
    }
    //padvalue (DAC1, outBuf, 5);
    //theOLED->println (outBuf);
    if (PointSelected == 3)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy + 32+5);
    if( Is5V){
        theOLED->print (" 4V    48    C2 ");//" 8V    96    C6 ");
    } else{
        theOLED->print (" 8V    96    C6 ");
    }

    theOLED->setTextColor (WHITE, BLACK);

    // Rect around input area
    // theOLED->drawRect(posx,posy,strlen(title)*6+padPos*12+3,18,WHITE);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (0, 55);
    theOLED->print ("In: ");
    if( noteRec>0) theOLED->print (NotesNames[noteRec]);
	//theOLED->print (noteRec);
    //theOLED->print ("/");
    //theOLED->print (PortData);
    theOLED->print (" / ");
    if(Is5V)
        theOLED->print ((float)(PortData - minD) / rangeD * 5.0, 2);
    else    
        theOLED->print ((float)(PortData - minD) / rangeD * 10.0, 2);
    theOLED->println ("V");

    theOLED->display ();
}

/// Two point calibration entry function
bool TwoPointsCal () {
    calMode = TwoPointCalMode;

    // Init Calibration
    if (!InitCal)
        if (!InitCalibration ()) {
            myMenu.setupPopup ("Error:Set Note Mode", 2000, 0, 17);
            return true; // If Init fails, abort
        }
    // Get the 2 calibration points
    int16_t minD, rangeD;
	rangeData->getDAC(minD, rangeD);
    // bool Is5V = (rangeD > 0) ? rangeD < INITRANGEDAC / 2 + 100 : rangeD > INITRANGEDAC / 2 - 100;
    bool Is5V = (PortInUse->PortCfg.getInputRange() == ZEROTO5V);


    if( Is5V){
        DAC1 = PortInUse->PortCfg.Ranges.Convert (12);//24);
        DAC2 = PortInUse->PortCfg.Ranges.Convert (48);//96);
    } else{
        DAC1 = PortInUse->PortCfg.Ranges.Convert (24);
        DAC2 = PortInUse->PortCfg.Ranges.Convert (96);
    }

    // Read Port
    //PortInUse->ReadPort (PortData);
	PortData = PortInUse->PortValue;
    noteRec = PortInUse->MIDIData;

    updateCalSelection (MAXMENU2POINTS); // Update Selection
    displayTwoPoints (); // Display values

    // If button pressed (LOW input), finish
    if (!selectionCalMode ()) {
        uint8_t pointRec = IsCalPoint (PortData);
        if (!AutoCal || pointRec == 0) return false;
        if (pointRec == LastpointRec) return false;
        LastpointRec = PointSelected = pointRec;
    }
    if (PointSelected < 2) { // Cancel or END Selected
        // TODO Check here if OK or Cancel
        ClearCalArea (); // Clear area used
		PortInUse->PortCfg.SetMIDIFunc(OldMIDIfunction);
        if (PointSelected == 0) // Cancel calibration, revert to previous values
            rangeData->SetDAC (OldminD, OldRangeD); // Restore old DAC values
        InitCal = false;
        calMode = NoCalMode;
        return true;
    } else { // Accept input in point selected
        //int16_t minD, rangeD;
        if (PointSelected == 2) DAC1 = PortData;
        if (PointSelected == 3) DAC2 = PortData;
        if( Is5V){
            minD = DAC1 - (DAC2 - DAC1) * 1.0 / 3; //2.0 / 6;
            rangeD = (DAC2 - DAC1) * 5.0 / 3;//6;
        } else {
            minD = DAC1 - (DAC2 - DAC1) * 2.0 / 6.0;
            rangeD = (DAC2 - DAC1) * 10.0 / 6.0;
        }
        rangeData->SetDAC (minD, rangeD);
    }

    return false;
}


bool CVTwoPointsCal ()
{
    PortSelected = 2; // Select CV
	BankSelected = 0; // Calibrate on Bank 1

    if (!TwoPointsCal ()) return false; // Wait for calibration to end
    if (PointSelected) { // OK pressed
		int16_t minD, rangeD;
		rangeData->getDAC(minD, rangeD);
		for(int i=0; i<8; i++){
			CVControls[i].CVPort.PortCfg.Ranges.SetDAC (minD, rangeD);
		}
		if( GlobalCalEnabled){
			// Save Global Config
			GlobalCfg.InitMinDAC = minD;
			GlobalCfg.InitRangeDAC = rangeD;
			FlashAccess->SetCurrentPage( 0);
			FlashAccess->SetCurrentPage( 1);
			GlobalCfg.SaveCfg();
			}
    }
    return true;
}

bool AuxATwoPointsCal () 
{
    PortSelected = 2; // Select CV
	BankSelected = 8; // Calibrate on Aux
	if (!AuxTwoPointsCal ()) return false; // Wait for calibration to end
    if (PointSelected) { // OK pressed
		int16_t minD, rangeD;
		// Store on second Aux
		rangeData->getDAC(minD, rangeD);
		CVControls[8].CVPort.PortCfg.Ranges.SetDAC (minD, rangeD);
		// Save Global Config
		GlobalCfg.AuxAMinDAC = minD;
		GlobalCfg.AuxARangeDAC = rangeD;
		FlashAccess->SetCurrentPage( 0);
		FlashAccess->SetCurrentPage( 1);
		GlobalCfg.SaveCfg();
    }
    return true;
}

bool AuxBTwoPointsCal () 
{
    PortSelected = 3; // Select CV
	BankSelected = 8; // Calibrate on Aux
	if (!AuxTwoPointsCal ()) return false; // Wait for calibration to end
    if (PointSelected) { // OK pressed
		int16_t minD, rangeD;
		// Store on second Aux
		rangeData->getDAC(minD, rangeD);
		CVControls[8].Slider.PortCfg.Ranges.SetDAC (minD, rangeD);
		// Save Global Config
		GlobalCfg.AuxBMinDAC = minD;
		GlobalCfg.AuxBRangeDAC = rangeD;
		FlashAccess->SetCurrentPage( 0);
		FlashAccess->SetCurrentPage( 1);
		GlobalCfg.SaveCfg();
    }
    return true;
}

bool AuxTwoPointsCal () {

    if (!TwoPointsCal ()) return false; // Wait for calibration to end
 /*   if (PointSelected) { // OK pressed
        rangeData->ResetCalPoints ();
		int16_t minD, rangeD;
		// Store on second Aux
		rangeData->getDAC(minD, rangeD);
		CVControls[8].Slider.PortCfg.Ranges.SetDAC (minD, rangeD);
		// Save Global Config
		GlobalCfg.AuxMinDAC = minD;
		GlobalCfg.AuxRangeDAC = rangeD;
		GlobalCfg.SaveCfg();
    }*/
    return true;
}


/////////////////////////////////////////////////////////
// Fader calibration
bool FadersCal () 
{
    calMode = FadersCalMode;

    // Init Calibration
    if (!InitCal)
        if (!InitFadersCal ()) {
            myMenu.setupPopup ("Cal. failed", 5000, 0, 17);
            return true; // If Init fails, abort
        }

    // Read Ports
    for (int i = 0; i < 8; i++) {
        //CVControls[i].Slider.ReadPort (FadersData[i]);
		FadersData[i] = CVControls[i].Slider.PortValue;
        MinFadersData[i] = (FadersData[i] < MinFadersData[i]) ? FadersData[i] : MinFadersData[i];
        MaxFadersData[i] = (FadersData[i] > MaxFadersData[i]) ? FadersData[i] : MaxFadersData[i];
        noteFadersRec[i] = CVControls[i].Slider.MIDIData;
    }
    updateCalSelection (2); // Update Selection: 8 faders, cancel, OK
    displayFadersCal (); // Display values

    // If button pressed (LOW input), finish
    if (!selectionCalMode ()) {
        return false;
    }
    if (PointSelected < 2) { // Cancel or END Selected
        // TODO Check here if OK or Cancel
        ClearCalArea (); // Clear area used
		int16_t GlobalminD=0, GlobalrangeD=0;
        if (PointSelected == 1) {
            for (int i = 0; i < 8; i++){
				GlobalminD+=MinFadersData[i]/8; GlobalrangeD+=(MaxFadersData[i] - MinFadersData[i])/8;
                CVControls[i].Slider.PortCfg.Ranges.SetDAC (MinFadersData[i],
                                                            MaxFadersData[i] - MinFadersData[i]); // Store new DAC values
				}
        }
        InitCal = false;
        calMode = NoCalMode;
		GlobalCfg.FaderMinDAC = GlobalminD;
		GlobalCfg.FaderRangeDAC = GlobalrangeD;
		FlashAccess->SetCurrentPage( 0);
		FlashAccess->SetCurrentPage( 1);
		GlobalCfg.SaveCfg();
        return true;
    }

    return false;
}

void displayFadersCal () {
    int posx = 0, posy = 0;
    char outBuf[17];

    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setCursor (posx, posy);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->print ("Fader cal.");
    if (PointSelected == 0)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->print (" Cancel ");
    if (PointSelected == 1)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->println ("END");

    // Print values
    theOLED->setCursor (posx, posy + 10);
    theOLED->setTextColor (BLACK, WHITE);
    theOLED->println ("  1    2    3    4  ");
    theOLED->setCursor (posx, posy + 37);
    theOLED->println ("  5    6    7    8  ");
    theOLED->setTextColor (WHITE, BLACK);
    for (int i = 0; i < 8; i++) {
        theOLED->setCursor (posx + (i % 4) * 6 * 5, posy + 19 + i / 4 * 27);
        padvalue (MinFadersData[i], outBuf, 4);
        theOLED->print (outBuf);
        theOLED->setCursor (posx + (i % 4) * 6 * 5, posy + 28 + i / 4 * 27);
        padvalue (MaxFadersData[i], outBuf, 4);
        theOLED->print (outBuf);
    }


    theOLED->display ();
}


bool InitFadersCal () {

    for (int i = 0; i < 8; i++) {
        MinFadersData[i] = 2048;
        MaxFadersData[i] = 2048;
    }
    PointSelected = 1;
    PortData = -1;
    noteRec = -1;
    LastpointRec = 0;

    InitCal = true;

    return true;
}


/////////////////////////////////////////////////////////
// Set Range Mode
Item_Function SetFuns[] = { SelectMin,    SelectMax,      SetLowLimit,
                            SetHighLimit, SelectMinInput, SelectMaxInput };
bool SetRangeMenu () {

    calMode = SetRangeMode;

    // Init Calibration
    if (!InitCal)
        if (!InitCalibration ()) return true; // If Init fails, abort

    // Running set function?
    if (runFunc) {
        if (runFunc ()) runFunc = NULL;
        return false;
    }
    // Read Port
    PortInUse->ReadPort (PortData);
    noteRec = PortInUse->MIDIData;

    updateCalSelection (MAXMENUSETRANGES-2); // Update Selection
    displaySetRangeMenu (); // Display values

    // If button pressed (LOW input), finish
    if (!selectionCalMode ())
        return false;
    else {
        if (PointSelected < 2) { // Cancel or END Selected
            ClearCalArea (); // Clear area used
			if( PointSelected == 0){ // Cancel, return to old values
				PortInUse->PortCfg.ClipLow = OldClipLow;
				PortInUse->PortCfg.ClipHigh = OldClipHigh;
				rangeData->SetDAC(OldminD, OldRangeD); // Restore old DAC values
				rangeData->SetMIDI(OldminMIDI,OldrangeMIDI);
			}
            InitCal = false;
            calMode = NoCalMode;
            return true;
        } else { // Accept input in point selected
            runFunc = SetFuns[PointSelected - 2];
        }
    }

    return false;
}


void displaySetRangeMenu () {
    char outBuf[17];
    int posx = 0, posy = 0;
    //int16_t minD, rangeD;

    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy);
    theOLED->print ("Ch:");
    theOLED->print (BankSelected + 1);
    if (PortSelected == 2)
        theOLED->print ("  CV ");
    else
        theOLED->print (" FAD ");
    if (PointSelected == 0)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->print (" CANCEL ");
    if (PointSelected == 1)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->println ("END");

    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy + 16);
    theOLED->println ("     RANGE  TRIM");

    // Print values
    for (int i = 0, pCursor = 0; i < 2/*3*/; i++) {
        for (int j = 0; j < 2; j++, pCursor++) {
			theOLED->setTextColor (WHITE, BLACK);
            if(i==0){
				if (j) theOLED->print ("MAX ");
				else theOLED->print ("MIN ");}
            if (PointSelected == i * 2 + j + 2)
                theOLED->setTextColor (BLACK, WHITE);
            else
                theOLED->setTextColor (WHITE, BLACK);
            theOLED->setCursor (posx + i * 6 * 7+ 4*7, posy + 24 + j * 10);
            getSetRangeValue (outBuf, 2 + i * 2 + j);
            theOLED->println (outBuf);
        }
    }
/*
    // Rect around input area
    // theOLED->drawRect(posx,posy,strlen(title)*6+padPos*12+3,18,WHITE);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (0, 55);
    theOLED->print ("In: ");
    theOLED->print (noteRec);
    theOLED->print ("/");
    theOLED->print (PortData);
    theOLED->print ("/");
    rangeData->getDAC (minD, rangeD);
    theOLED->print ((float)(PortData - minD) / rangeD * 10.0, 2);
    theOLED->println ("V");
*/
    theOLED->display ();
}

void getSetRangeValue (char *Buf, int val) {
    int16_t minM, rangeM;
    int16_t minD, rangeD;
    PortInUse->PortCfg.Ranges.getMIDI (minM, rangeM);
    PortInUse->PortCfg.Ranges.getDAC (minD, rangeD);
    switch (val) {
    case 0:
        strcpy (Buf, "Cancel");
        break;
    case 1:
        strcpy (Buf, "END");
        break;
    case 2:
        padvalue (minM, Buf, 5);
        break;
    case 3:
        padvalue (minM + rangeM, Buf, 5);
        break;
    case 4:
        padvalue (PortInUse->PortCfg.ClipLow, Buf, 5);
        break;
    case 5:
        padvalue (PortInUse->PortCfg.ClipHigh, Buf, 5);
        break;
    case 6:
        padvalue (minD, Buf, 5);
        break;
    case 7:
        padvalue (minD + rangeD, Buf, 5);
        break;
    }
}


/////////////////////////////////////////////////////////
// Set Range Mode
Item_Function SetClockFuns[] = {ChangeClockShift, ChangeClockBPM}; // ChangeClockDiv

void getClkValues(float &ClkDiv, int8_t &ClkShift, unsigned long &IntMIDIClk){
    if(PortSelected>1){
		ClkDiv= PortInUse->getClockDivider();
		ClkShift= PortInUse->getClockShift();
        IntMIDIClk = PortInUse->IntervalMIDIClock;
		//ClacksPM= 60000.0/(PortInUse->IntervalClock/1000.0);
	} else { // Gate selecetd
		DigitalPort *PortUsed = (DigitalPort*) PortInUse; 
		ClkDiv= PortUsed->getClockDivider();
		ClkShift= PortUsed->getClockShift();
        IntMIDIClk = PortUsed->IntervalMIDIClock;
        /* MIDIBPM = roundf(MIDIBPM);
        PortUsed->IntervalMIDIClock= 60000.0/(MIDIBPM/1000.0)/24.0; */
        //ClacksPM= 60000.0/(PortUsed->IntervalClock/1000.0);
	}

}

bool SetClockMenu ()
{

    calMode = SetClockMode;

    // Init Calibration
    if (!InitCal)
        if (!InitCalibration ()) return true; // If Init fails, abort

    // Running set function?
    if (runFunc) {
        if (runFunc ()) runFunc = NULL;
        return false;
    }

    updateCalSelection (MAXMENUCLOCKRANGES-2); // Update Selection
    displaySetClockMenu (0); // Display values

    // If button pressed (LOW input), finish
    if (!selectionCalMode ())
        return false;
    else {
        if (PointSelected < 2) { // Cancel or END Selected
            // TODO Check here if OK or Cancel
            ClearCalArea (); // Clear area used
            InitCal = false;
            calMode = NoCalMode;
            float ClkDiv;
	        int8_t ClkShift;
            unsigned long IntMIDIClk;
            getClkValues(ClkDiv, ClkShift, IntMIDIClk);
            GlobalCfg.ClockShift = ClkShift;
            GlobalCfg.ClockDivider = ClkDiv;
            return true;
        } else { // Accept input in point selected
            runFunc = SetClockFuns[PointSelected - 2];
        }
    }

    return false;
}


bool editingPar = true;
void displaySetClockMenu (long val) {
static long disptim=0;
 
    char outBuf[27];
    int posx = 0, posy = 0;
    float MIDIBPM;
    //float ClacksPM;
    float ClkDiv;
	int8_t ClkShift;
    unsigned long IntMIDIClk;

    getClkValues(ClkDiv, ClkShift, IntMIDIClk);

    MIDIBPM = 60000.0/(IntMIDIClk/1000.0)/24.0;
	
    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy);
    theOLED->print ("Ch:");
    theOLED->print (BankSelected + 1);
	switch( PortSelected){
	case 1: theOLED->print (" GATE "); break;
	case 2: theOLED->print ("  CV  "); break;
	case 3: theOLED->print (" FAD  "); break;
	}

    if (PointSelected == 0)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->print ("Cancel");
    if (PointSelected == 1)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->println (" END");

    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (posx, posy + 12);
    theOLED->println ("Mult.     MIDI BPM");
    // Print values
    theOLED->setCursor (posx, posy + 20);
 	theOLED->setTextSize (2);
    if (PointSelected == 2 && editingPar) theOLED->setTextColor (BLACK, WHITE);
    else theOLED->setTextColor (WHITE, BLACK);
	if( ClkShift >-6 && ClkShift <6){
		//theOLED->print ("  ");
		theOLED->print (ClokShiftTxt[ClkShift+5]);
		}
    if (PointSelected == 2 && !editingPar){
        int16_t  x1, y1;
        uint16_t w, h;
        theOLED->getTextBounds(ClokShiftTxt[ClkShift+5], posx, posy + 20, &x1, &y1, &w, &h);
        theOLED->drawRect(x1, y1, w, h,WHITE);
        }
	theOLED->setTextColor (WHITE, BLACK);
    theOLED->print("  ");

	sprintf( outBuf, "%3.1f", MIDIBPM);

    if (PointSelected == 3 && editingPar) theOLED->setTextColor (BLACK, WHITE);
    else theOLED->setTextColor (WHITE, BLACK);

    theOLED->println (outBuf);
    if (PointSelected == 3 && !editingPar){
        int16_t  x1, y1;
        uint16_t w, h;
        theOLED->getTextBounds(outBuf, posx+12*5, posy + 20, &x1, &y1, &w, &h);
        theOLED->drawRect(x1, y1, w, h,WHITE);
        }
	theOLED->setTextSize (1);
    theOLED->setTextColor (WHITE, BLACK);
    
    /* theOLED->setCursor (posx, posy + 38);
    theOLED->println ("(msecs) MIDI Clock");	
	sprintf( outBuf, "  %6.3f", IntMIDIClk/1000.0);
    theOLED->print (outBuf); */

    /* theOLED->setCursor (posx, posy + 38);
    theOLED->println ("Fine      Input/min");	
	sprintf( outBuf, "  %1.3f ", ClkDiv);
    //if (PointSelected == 3) theOLED->setTextColor (BLACK, WHITE);
    //else theOLED->setTextColor (WHITE, BLACK);
    theOLED->print (outBuf);
	theOLED->setTextColor (WHITE, BLACK);
	theOLED->print ("    ");
	//sprintf( outBuf, "	%2d ", ClkShift );
	sprintf( outBuf, "%3.1f", ClacksPM);
    theOLED->print (outBuf); */
	
	if (micros() - disptim >= REFRESH_RATE) {
		theOLED->display ();
        disptim = micros();
        }
}

bool ChangeClockDiv () {
    float *ck= &(((InputPortCfg *)GetPortCfg())->ClockDivider);
    float val = *ck;

    editingPar = EncChangeValF (val, 0, 2, 0.001, displaySetClockMenu);
    GlobalCfg.ClockDivider = *ck = val;

    return editingPar;//ret;
}


bool ChangeClockBPM () {
    float *ClkDiv;
    unsigned long *NewInterval;
    
    if(PortSelected>1){
        ClkDiv = &(PortInUse->PortCfg.ClockDivider);
        NewInterval= &(PortInUse->IntervalMIDIClock);
        }
    else{
	    DigitalPort *PortUsed = (DigitalPort*) PortInUse;
        ClkDiv = &(PortUsed->PortCfg.ClockDivider);
        NewInterval= &(PortUsed->IntervalMIDIClock);
        }
    
    float MIDIBPM = 60000.0/(*NewInterval/1000.0)/24.0;
    MIDIBPM = roundf(MIDIBPM*1)/1;
    float prevBPM = MIDIBPM;

    editingPar = EncChangeValF (MIDIBPM, 0, 500, 1.0, displaySetClockMenu);

    *NewInterval= 60000.0/(MIDIBPM/1000.0)/24.0;
    GlobalCfg.ClockDivider= *ClkDiv = MIDIBPM / prevBPM * (*ClkDiv);

    return editingPar;//ret;
}


bool ChangeClockShift () {
    int8_t *ClkShift;
    ClkShift = &(((InputPortCfg *)GetPortCfg())->ClockShift);

    float val = *ClkShift;

    editingPar = EncChangeValF (val, -5, 5, 1.0, displaySetClockMenu);
    GlobalCfg.ClockShift = *ClkShift = val;
    return editingPar;//ret;
}

/** @} */