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
#ifndef __have__MIDImerger_h__
#define __have__MIDImerger_h__

#include "PrjIncludes.h"

/** @addtogroup MenuClassDefs
* @{
*/

/**
 *  \file MenuLists.h
 *  \brief Predefine Menu functions callbacks and menu items 
 */

//////////////////////////////////////////////
// Menu Definition and menu response functions
// Defined on MenuMain
extern MenuList list1;
extern MenuList listGlobal;
extern MenuList listGlobalCal;
extern MenuList listLoadSysEx;
extern MenuList ListBankSaveSel;
extern MenuList ListBankLoadSel;
extern MenuList ListFactReset;
// Defined on MenuBlock
extern MenuList listBanks;
extern MenuList listAux;
extern MenuList BankFnListMenu;
// Defined on MenuMode
extern MenuList StdNameList;
extern MenuList UserNameList;
extern MenuList listGateMode;
extern MenuList listAnag;
extern MenuList listAnag2;
extern MenuList listAnagFn;
extern MenuList listAnagFn2;
extern MenuList listAnaSTSPFn;
// Defined on MenuRange
extern MenuList listAnagRange;
extern MenuList InRangeListMenu;
extern MenuList listButFn;
extern MenuList listGatSTSPFn;
extern MenuList listGate;

// Defined on MenuMain
bool gotoMenuGlobalCfg();
bool gotoMenuGlobalCal();
bool gotoListFactReset();
bool FactoryReset();
bool CreditsScreen();
bool DoNothing();
bool ResetCPU();
bool FreqMeas();
// Defined on MenuBlock
bool gotoMenuBanks();
bool gotoMenuSettings();
bool SelectGateConfig();
bool SelectFaderConfig();
bool SelectCVConfig();
bool setActiveChord();
bool SelectChordType();
bool SelectScale();
bool SelectChordPreset();
bool selectRootScale();
bool SelectChordInv();
// Defined on MenuMode
bool SelectNameMenu();
bool selectMenuPortName();
bool editUserNames();
bool SelectGateModeGate();
bool gotoMenuGate();
bool gotoMenuAnag();
bool SelectAnagRange();
bool SelectAnagFn();
bool SelectAnagFn2();
bool SelectMIDIChan();
bool SelectController();
// Defined on MenuRange
bool SelectMin();
bool SelectMax();
bool SetLowLimit();
bool SetHighLimit();
bool SelectMinInput();
bool SelectMaxInput();
// Defined on MenuGate
bool SelectDelay();
bool SelectNoteNmbr();
bool SelectControllerValue();
// Defined on CalMenu
bool SetRangeMenu();
bool TwoPointsCal();
bool SetClockMenu();
bool CVTwoPointsCal();
bool CalibrateCV();
bool AuxATwoPointsCal();
bool AuxBTwoPointsCal();
bool FadersCal();
// Defined on setValues
bool LoadPreset();
// Defined on GrafUtil
void padvalue(long value, char *outBuf, int padPos);

#ifdef DEBUGMODE
unsigned long clicktimer2, clicktimer3,
	avgtimer2 = 500, avgtimer = 8000, maxtimer = 0, mintimer = 999999999;
int count = 0, count2 = 0, count3 = 0;
void beginDebugTimers(void);
void endDebugTimers(void);
#endif // DEBUGMODE

/** @} */

#endif // __have__MIDImerger_h__