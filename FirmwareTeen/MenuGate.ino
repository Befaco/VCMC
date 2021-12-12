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
 *  \file MenuGate.ino
 *  \brief Defines Menu functions callbacks response for Gate configuration 
 */

/*****************************
 ******************************
 * Setup the menu as an array of MenuItem
 * Create a MenuList and an instance of your
 * menu class
 ******************************
 *****************************/
// Create Menu lists
// Gate Function config.
DEF_MENULIST (listButFn,  GATE FUNCTION , ListLines,
    {"<-BACK", gotoMenuDig, 1},
    {"NOTE GATE", SelectButTrig, 1}, // Revise for Gate behaviour
    {"NOTE LATCH", SelectButLatch, 1},
    {"CC GATE", SelectButCC, 1},
    {"CC LATCH", SelectButCCLATCH, 1},
    {"CLOCK", SelectButClock, 1},
    {"ST/SP", SelectGatSTSPFn, 1}, // Antes SelectButStartStop
    {"CHORD", SelectChordPreset, 0},
    {"PANIC", SelectGatePanic, 1},
    {"NO FUNCTION", SelectNoDifFunc, 1}
)

DEF_MENULIST (listGatSTSPFn,  ST/SP MODE , ListLines,
    {"<-BACK", gotoMenuDig, 1},
    {"ST/SP GATE", SelectButStartStop, 1},
    {"ST/SP LATCH", SelectLatchStartStop, 1},
    {"CONT/SP GATE", SelectButContStop, 1},
    {"CONT/SP LATCH", SelectLatchContStop, 1},
    {"START(ST)", SelectGateStart, 1},
    {"STOP(SP)", SelectGateStop, 1},
    {"CONTINUE", SelectGateCont, 1}
)

// GATE Menu. Poner el valor del CC#
DEF_MENULIST (listGate, GATE CONFIG, ListLines,
    {"<-BACK", gotoMenuBanks, 1},
    {"FUNCTION", SelectGateFn, 1},
    {"MIDI CH", SelectMIDIChan, 1},
    {"NOTE#", SelectNoteNmbr, 0},            // Shown only when Function Note
    {"CC#", SelectController, 0},            // Shown only when Function CC
    {"CC VALUE#", SelectControllerValue, 0}, // Shown only when Function CC
    {"CLOCK DIV", SelectClockDiv, 0},        // Shown only when Clock
    {"CLOCK SHIFT", SelectClockShift, 0},    // Shown only when Clock
    {"CLOCK SET", SetClockMenu, 1},          // Shown only when Clock
    {"PORT NAME", SelectNameMenu, 1}
)

// Gate Cfg Mode ??? Only needed if menu needed. Might add CC latch and CC Gate in main menu
MenuItem SelectGateModeCfgList[4] = {
    {"<-BACK", gotoMenuDig, 1},
    {"NOTE", DoNothing, 1},
    {"CC", DoNothing, 1},
    {" GATE MODE ", NULL, 1}};

/*****************************
 ******************************
 * Define the functions you want your menu to call
 * They can be blocking or non-blocking
 * They should take no arguments and return a bool
 * true if the function is finished and doesn't want to run again
 * false if the function is not done and wants to be called again
 ******************************
 *****************************/

// NEW FUNCTIONS ADDED

// ST/SP Menu foir Gates
bool SelectGatSTSPFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listGatSTSPFn);
    return true;
}


bool SelectNoteNmbr()
{
    long val = GetPortCfg()->NoteToSend;
    //long val = CVControls[BankSelected].GateBut.PortCfg.NoteToSend;
    bool ret = EncoderchangeValue("Note ", val, 0, 119, 3, 0, 45);
    //CVControls[BankSelected].GateBut.PortCfg.NoteToSend = val;
    GetPortCfg()->NoteToSend = val;
    return ret;
}

// END NEW FUNCTIONS ADDED

// DELETE
/*
bool SelectOffset () {
    return EncoderchangeValue ("Offset:", ((AnInputPortCfg *)GetPortCfg ())->Offset, -16387, 16386, 6, 0, 45);
}

bool SelectGain () {
    bool ret =
    EncoderchangeValue ("Ampl x100:", ((AnInputPortCfg *)GetPortCfg ())->Amp, -500, 500, 4, 0, 45);
    return ret;
}
*/

////////////////////////////////
// Gate Config Functions
bool SelectGateFn()
{
    //if (BankSelected != 7) listButFn.disableItem(7);

    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listButFn);
    return true;
}

bool SelectControllerValue()
{
    long val = GetPortCfg()->ControllerValue;
    bool ret = EncoderchangeValue("CC Value ", val, 0, 127, 3, 0, 45);
    GetPortCfg()->ControllerValue = val;
    return ret;
}

bool SelectClockDiv()
{
    long val = ((DigPortCfg *)GetPortCfg())->ClockDivider;
    bool ret = EncoderchangeValue("Clock Div:", val, 1, 255, 3, 0, 45);
    GlobalCfg.ClockDivider =
        ((DigPortCfg *)GetPortCfg())->ClockDivider = val;
    return ret;
}

bool SelectClockShift()
{
    long val = ((DigPortCfg *)GetPortCfg())->ClockShift;
    bool ret = EncoderchangeValue("Clock Shift:", val, -120, 120, 4, 0, 45);
    GlobalCfg.ClockShift =
        ((DigPortCfg *)GetPortCfg())->ClockShift = val;
    return ret;
}

bool SelectDelay()
{
    uint16_t *vdir;
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCHTRIG)
        vdir = &CVControls[BankSelected].GateBut.PortCfg.DelayGate; 
    else
        vdir = &CVControls[BankSelected].CVPort.PortCfg.DelayGate;
    long val = *vdir;
    bool ret = myMenu.EncoderselDigitLong("Delay us:", val, 0, 2000, 4, 0, 45);
    *vdir = val;
    // debounceDelay= val;
    return ret;
}

bool gotoMenuDig()
{
    SelectGateConfig();
    return true;
}

// MIDI Functions selection

bool SelectNoDifFunc()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(NODIGFUNC);
    SelectGateConfig();
    return true;
}

bool SelectButTrig()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(TRIGGER);
    if (!SelectNoteNmbr())
        return false;

    SelectGateConfig();
    return true;
}

bool SelectButLatch()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(LATCH);
    if (!SelectNoteNmbr())
        return false;

    SelectGateConfig();
    return true;
}

bool SelectButCC()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(CCBUT);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectController())
            return false; // Select CC number
        else
            SetValState++;
        return false;
    }
    if (SetValState == 2)
    {
        if (!SelectControllerValue())
            return false; // Select CC value
        else
            SetValState = 0;
    }

    SelectGateConfig();
    return true;
}

bool SelectButCCLATCH()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(CCLATCH);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectController())
            return false; // Select CC number
        else
            SetValState++;
        return false;
    }
    if (SetValState == 2)
    {
        if (!SelectControllerValue())
            return false; // Select CC value
        else
            SetValState = 0;
    }

    SelectGateConfig();
    return true;
}

bool SelectButClock()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATECLOCK);
    if (!SetClockMenu())
        return false;

    SelectGateConfig();
    return true;
}

bool SelectButStartStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTARTSTOP);
    SelectGateConfig();
    return true;
}

bool SelectButContStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATEPAUSECONT);
    SelectGateConfig();
    return true;
}

bool SelectLatchStartStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATETRIGSTARTSTOP);
    SelectGateConfig();
    return true;
}

bool SelectLatchContStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATETRIGPAUSECONT);
    SelectGateConfig();
    return true;
}

bool SelectGateStart()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTART);
    SelectGateConfig();
    return true;
}

bool SelectGateCont()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATECONTINUE);
    SelectGateConfig();
    return true;
}

bool SelectGateStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTOP);
    SelectGateConfig();
    return true;
}

/*
bool SelectChordTr()
{
    CVControls[BankSelected].CVPort.PortCfg.SetMIDIFunc(PITCHTRIG);
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(CHORDTRIG);
    SelectChordType();
    return true;
}
*/

bool SelectGatePanic()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATEPANIC);
    SelectGateConfig();
    return true;
}

///////////////////////
long value = 100;
bool fun1() { return EncoderchangeValue("", value, 0, 120, 3, 40, 45); }

/** @} */
