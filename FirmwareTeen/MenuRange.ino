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
/** @addtogroup MenuClassDefs
* @{
*/

/**
 *  \file MenuRange.ino
 *  \brief Defines Menu functions callbacks response for Input/Output Range configuration 
 */

// CV/Fader configuration menu. SOBRAN ULTIMAS OPCIONES??
MenuItem AnagRangeList[14] = {
	{"<-BACK", gotoMenuAnag, 1}, 
	{"IN RANGE", InRange, 1}, 
	{"OUT RANGE", SetRangeMenu, 1}, 
	{"CALIBRATION", TwoPointsCal, 1}, 
	{"MIDI Limits", SetLimitMIDI, 0}, 
	{"Low Limit", SetLowLimit, 0}, 
	{"High Limit", SetHighLimit, 0}, 
	{"MIDI Range", SetRangeMIDI, 0}, 
	{"Min. MIDI", SelectMin, 0}, 
	{"Max. MIDI", SelectMax, 0}, 
	{"DAC Range", SetRangeDAC, 0}, 
	{"Min. DAC", SelectMinInput, 0}, 
	{"Max. DAC", SelectMaxInput, 0}, 
	{"  RANGES  ", NULL, 1}};
MenuList listAnagRange(AnagRangeList, 13, ListLines);

// Input Range
MenuItem InRangeList[5] = {
	{"<-BACK", SelectAnagRange, 1}, 
	{"0/+10V", SetRange10, 1}, 
	{"0/+5V", SetRange5, 1}, 
	{"-5/+5V", SetRangeBipolar, 1}, 
	{"INPUT RANGE", NULL, 1}};
MenuList InRangeListMenu(InRangeList, 4, ListLines);

/*****************************
 ******************************
 * Define the functions you want your menu to call
 * They can be blocking or non-blocking
 * They should take no arguments and return a bool
 * true if the function is finished and doesn't want to run again
 * false if the function is not done and wants to be called again
 ******************************
 *****************************/

////////////////////////////////
// Ranges configuration
byte MultiSel = 0;

// MIDI Limits
bool SetLimitMIDI()
{
	switch (MultiSel)
	{
	case 0:
		if (!SetLowLimit())
			return false;
		MultiSel = 1;
		break;
	case 1:
		if (!SetHighLimit())
			return false;
		MultiSel = 0;
		return true;
	}
	return false;
}

bool SetLowLimit()
{
	int16_t minv = 0, maxv = 127;
	long val = ((AnInputPortCfg *)GetPortCfg())->ClipLow;
	((AnInputPortCfg *)GetPortCfg())->LimitValues(minv, maxv);
	bool ret = EncoderchangeValue("Clip Low:", val, minv, maxv, 5, 00, 45);
	((AnInputPortCfg *)GetPortCfg())->ClipLow = val;
	return ret;
}

bool SetHighLimit()
{
	int16_t minv = 0, maxv = 127;
	long val = ((AnInputPortCfg *)GetPortCfg())->ClipHigh;
	((AnInputPortCfg *)GetPortCfg())->LimitValues(minv, maxv);
	bool ret = EncoderchangeValue("Clip High:", val, minv, maxv, 5, 00, 45);
	((AnInputPortCfg *)GetPortCfg())->ClipHigh = val;
	return ret;
}

// DAC Ranges
bool SetRangeDAC()
{
	switch (MultiSel)
	{
	case 0:
		if (!SelectMinInput())
			return false;
		MultiSel = 1;
		break;
	case 1:
		if (!SelectMaxInput())
			return false;
		MultiSel = 0;
		return true;
	}
	return false;
}

bool SelectMinInput()
{
	int32_t min, range;
	((AnInputPortCfg *)GetPortCfg())->Ranges.getDAC(min, range);
	long val = min;
	bool ret = EncoderchangeValue("Min:", val, -16387, 16386, 6, 0, 45); // *** TODO ***
	((AnInputPortCfg *)GetPortCfg())->Ranges.SetDAC(val, range);
	return ret;
}

bool SelectMaxInput()
{
	int32_t min, range;
	((AnInputPortCfg *)GetPortCfg())->Ranges.getDAC(min, range);
	long max = min + range;
	bool ret = EncoderchangeValue("Max:", max, -16387, 16386, 6, 0, 45); // *** TODO ***
	((AnInputPortCfg *)GetPortCfg())->Ranges.SetDAC(min, max - min);
	return ret;
}

// MIDI Ranges
bool SetRangeMIDI()
{
	switch (MultiSel)
	{
	case 0:
		if (!SelectMin())
			return false;
		MultiSel = 1;
		break;
	case 1:
		if (!SelectMax())
			return false;
		MultiSel = 0;
		return true;
	}
	return false;
}

bool SelectMin()
{
	int16_t minv = 0, maxv = 127;
	int16_t min, range;
	((AnInputPortCfg *)GetPortCfg())->Ranges.getMIDI(min, range);
	long val = min;
	((AnInputPortCfg *)GetPortCfg())->LimitValues(minv, maxv);
	bool ret = EncoderchangeValue("Min MIDI:", val, minv, maxv, 5, 0, 45); // *** TODO ***
	((AnInputPortCfg *)GetPortCfg())->Ranges.SetMIDI(val, range);
	return ret;
}

bool SelectMax()
{
	int16_t minv = 0, maxv = 127;
	int16_t min, range;
	((AnInputPortCfg *)GetPortCfg())->Ranges.getMIDI(min, range);
	long max = min + range;
	((AnInputPortCfg *)GetPortCfg())->LimitValues(minv, maxv);
	bool ret = EncoderchangeValue("Max MIDI:", max, minv, maxv, 5, 0, 45); // *** TODO ***
	((AnInputPortCfg *)GetPortCfg())->Ranges.SetMIDI(min, max - min);
	return ret;
}

////////////////////////////////
// Input range selection

bool InRange()
{
	if (BankSelected == 8)
		InRangeListMenu.disableItem(3);
	else
		InRangeListMenu.enableItem(3);
	myMenu.ClearArea();
	myMenu.setCurrentMenu(&InRangeListMenu);
	return true;
}

bool SetRange10()
{
	// CV port uses reversed values
	//0 - 10v  -> ANRANGEMAX-0 -> minMIDI-m
	//Set pin6 to LOW to read input
	//digitalWrite( PINOFFSET, LOW);
	if (BankSelected < 8 && PortSelected != 2)
		return true; // Fader selected: Do nothing

	AnInputPortCfg *Cfg = &CVControls[BankSelected].CVPort.PortCfg;

	if( BankSelected > 7 && PortSelected==3)
		Cfg = &CVControls[BankSelected].Slider.PortCfg;
	
	int32_t MinDAC = INITMINDAC;
	int32_t MidRange = INITRANGEDAC;
	Cfg->Ranges.getDAC(MinDAC,MidRange);
	if( Cfg->getInputRange() == ZEROTO5V) // If it was in 0-5 v double the range
		MidRange = MidRange * 2;

	Cfg->setInputRange(NOOFFSET);
	Cfg->Ranges.SetDAC(MinDAC, MidRange);

	Cfg->SetMIDIFunc( Cfg->MIDIfunction ); // Reset MIDI Function
	myMenu.ClearArea();
	myMenu.setCurrentMenu(&listAnagRange);

	return true;
}

bool SetRange5()
{
	//Set input range to 0 - 5
	//0 - 5v  -> 0-ANRANGEMAX -> 0 -127
	//if( PortSelected!=1) return true; // Only for CV Port
	if (BankSelected < 8 && PortSelected != 2)
		return true; // Fader selected: Do nothing

	AnInputPortCfg *Cfg = &CVControls[BankSelected].CVPort.PortCfg;

	if( BankSelected > 7 && PortSelected==3)  // Aux B 
		Cfg = &CVControls[BankSelected].Slider.PortCfg;
	
	int32_t MinDAC;
	int32_t MidRange;
	Cfg->Ranges.getDAC(MinDAC,MidRange);
	if( Cfg->getInputRange() != ZEROTO5V) // If it was 0-10 v halve the range
		MidRange = MidRange / 2;

	Cfg->setInputRange(ZEROTO5V);
	Cfg->Ranges.SetDAC(MinDAC, MidRange);

	Cfg->SetMIDIFunc( Cfg->MIDIfunction ); // Reset MIDI Function
	myMenu.ClearArea();
	myMenu.setCurrentMenu(&listAnagRange);
	return true;
}

bool SetRangeBipolar()
{
	//Set input range to -5 +5
	//Set pin6 to HIGH
	//-5 +5v  -> 0v=2045 5v=4096
	//Negative voltages = 0
	//Useful when in sum mode.
	//digitalWrite( PINOFFSET, HIGH);

	if (BankSelected < 8 && PortSelected != 2)
		return true; // Fader selected: Do nothing

	AnInputPortCfg *Cfg = &CVControls[BankSelected].CVPort.PortCfg;

	if( BankSelected > 7 && PortSelected==3)  // Aux B 
		Cfg = &CVControls[BankSelected].Slider.PortCfg;
	
	int32_t MinDAC;
	int32_t MidRange;
	Cfg->Ranges.getDAC(MinDAC,MidRange);
	if( Cfg->getInputRange() == ZEROTO5V) // If it was 0-10 v halve the range
		MidRange = MidRange * 2;

	Cfg->setInputRange(MINUSPLUS5V);
	Cfg->Ranges.SetDAC(MinDAC, MidRange);

	Cfg->SetMIDIFunc( Cfg->MIDIfunction ); // Reset MIDI Function

	myMenu.ClearArea();
	myMenu.setCurrentMenu(&listAnagRange);
	return true;
}

/** @} */
