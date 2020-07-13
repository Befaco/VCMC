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
MenuItem DigFnList[] = {
    {"<-BACK", gotoMenuDig, 1},
    {"NOTE GATE", SelectButTrig, 1}, // Revise for Gate behaviour
    {"NOTE LATCH", SelectButLatch, 1},
    {"CC GATE", SelectButCC, 1},
    {"CC LATCH", SelectButCCLATCH, 1},
    {"CLOCK", SelectButClock, 1},
    {"ST/SP", SelectGatSTSPFn, 1}, // Antes SelectButStartStop
    {"PANIC", SelectGatePanic, 1},
    {"NO FUNCTION", SelectNoDifFunc, 1},
    {" GATE FUNCTION ", NULL, 1} // the algorithm
};
MenuList listButFn(DigFnList, 9, ListLines);

MenuItem STSPGatFnList[] = {
    {"<-BACK", gotoMenuDig, 1},
    {"ST/SP GATE", SelectButStartStop, 1},
    {"ST/SP LATCH", SelectLatchStartStop, 1},
    {"CONT/SP GATE", SelectButContStop, 1},
    {"CONT/SP LATCH", SelectLatchContStop, 1},
    {"START(ST)", SelectGateStart, 1},
    {"STOP(SP)", SelectGateStop, 1},
    {"CONTINUE", SelectGateCont, 1},
    {" ST/SP MODE ", NULL, 1}
};
MenuList listGatSTSPFn(STSPGatFnList, 8, ListLines);

// GATE Menu. Poner el valor del CC#
MenuItem GateList[13] = {
    {"<-BACK", gotoMenuBanks, 1},
    {"FUNCTION", SelectGateFn, 1},
    {"MIDI CH", SelectMIDIChan, 1},
    {"NOTE#", SelectNoteNmbr, 0},            // Shown only when Function Note
    {"CC#", SelectController, 0},            // Shown only when Function CC
    {"CC VALUE#", SelectControllerValue, 0}, // Shown only when Function CC
    {"CLOCK DIV", SelectClockDiv, 0},        // Shown only when Clock
    {"CLOCK SHIFT", SelectClockShift, 0},    // Shown only when Clock
    {"CLOCK SET", SetClockMenu, 1},          // Shown only when Clock
    {"PORT NAME", SelectNameMenu, 1},
    {"GATE CONFIG", NULL, 1}
};
MenuList listGate(GateList, 10, ListLines);

// Gate Cfg Mode ??? Only needed if menu needed. Might add CC latch and CC Gate in main menu
MenuItem SelectGateModeCfgList[4] = {
    {"<-BACK", gotoMenuDig, 1},
    {"NOTE", DoNothing, 1},
    {"CC", DoNothing, 1},
    {" GATE MODE ", NULL, 1}
};

/*****************************
 ******************************
 * Define the functions you want your menu to call
 * They can be blocking or non-blocking
 * They should take no arguments and return a boolean
 * true if the function is finished and doesn't want to run again
 * false if the function is not done and wants to be called again
 ******************************
 *****************************/

// NEW FUNCTIONS ADDED

// ST/SP Menu foir Gates
boolean SelectGatSTSPFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listGatSTSPFn);
    return true;
}

/*
boolean SelectGateModeCfg () {
    PortSelected = 2;
    myMenu.ClearArea ();
    myMenu.setCurrentMenu (&listAnag);

    return true;
}
*/

boolean SelectNoteNmbr()
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
boolean SelectOffset () {
    return EncoderchangeValue ("Offset:", ((AnInputPortCfg *)GetPortCfg ())->Offset, -16387, 16386, 6, 0, 45);
}

boolean SelectGain () {
    bool ret =
    EncoderchangeValue ("Ampl x100:", ((AnInputPortCfg *)GetPortCfg ())->Amp, -500, 500, 4, 0, 45);
    return ret;
}
*/

////////////////////////////////
// Gate Config Functions
boolean SelectGateFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listButFn);
    return true;
}

boolean SelectControllerValue()
{
    long val = GetPortCfg()->ControllerValue;
    bool ret = EncoderchangeValue("CC Value ", val, 0, 127, 3, 0, 45);
    GetPortCfg()->ControllerValue = val;
    return ret;
}

boolean SelectClockDiv()
{
    long val = ((DigPortCfg *)GetPortCfg())->ClockDivider;
    bool ret = EncoderchangeValue("Clock Div:", val, 1, 255, 3, 0, 45);
    ((DigPortCfg *)GetPortCfg())->ClockDivider = val;
    return ret;
}

boolean SelectClockShift()
{
    long val = ((DigPortCfg *)GetPortCfg())->ClockShift;
    bool ret = EncoderchangeValue("Clock Shift:", val, -120, 120, 4, 0, 45);
    ((DigPortCfg *)GetPortCfg())->ClockShift = val;
    return ret;
}

boolean SelectDelay()
{
    uint16_t *vdir;
    if( CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCHTRIG)
        vdir = &CVControls[BankSelected].GateBut.PortCfg.DelayGate; // debounceDelay;//
    else
        vdir = &CVControls[BankSelected].CVPort.PortCfg.DelayGate;
    long val = *vdir;
    bool ret = myMenu.EncoderselDigitLong("Delay ms:", val, 0, 1000, 4, 0, 45);
    *vdir = val;
    // debounceDelay= val;
    return ret;
}

boolean gotoMenuDig()
{
    SelectGateConfig();
    return true;
}

// MIDI Functions selection

boolean SelectNoDifFunc()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(NODIGFUNC);
    SelectGateConfig();
    return true;
}

boolean SelectButTrig()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(TRIGGER);
    if (!SelectNoteNmbr())
        return false;

    SelectGateConfig();
    return true;
}

boolean SelectButLatch()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(LATCH);
    if (!SelectNoteNmbr())
        return false;

    SelectGateConfig();
    return true;
}

boolean SelectButCC()
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

boolean SelectButCCLATCH()
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

boolean SelectButClock()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATECLOCK);
    if (!SetClockMenu())
        return false;

    SelectGateConfig();
    return true;
}

boolean SelectButStartStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTARTSTOP);
    SelectGateConfig();
    return true;
}

boolean SelectButContStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATEPAUSECONT);
    SelectGateConfig();
    return true;
}

boolean SelectLatchStartStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATETRIGSTARTSTOP);
    SelectGateConfig();
    return true;
}

boolean SelectLatchContStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATETRIGPAUSECONT);
    SelectGateConfig();
    return true;
}

boolean SelectGateStart()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTART);
    SelectGateConfig();
    return true;
}

boolean SelectGateCont()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATECONTINUE);
    SelectGateConfig();
    return true;
}

boolean SelectGateStop()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATESTOP);
    SelectGateConfig();
    return true;
}

boolean SelectGatePanic()
{
    ((DigPortCfg *)GetPortCfg())->SetMIDIFunc(GATEPANIC);
    SelectGateConfig();
    return true;
}

///////////////////////
long value = 100;
boolean fun1() { return EncoderchangeValue("", value, 0, 120, 3, 40, 45); }

/** @} */