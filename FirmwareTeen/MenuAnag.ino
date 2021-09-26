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
 *  \file MenuAnag.ino
 *  \brief Defines Menu functions callbacks response for Analog Ports configuration 
 */


// Name selection Menu
DEF_MENULIST (listName, PORT NAME, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"SELECT NAME", SelectStdName, 1},
    {"PREFIX", SelectPrefix, 0},
    {"SUFIX", SelectSufix, 1},
    {"EDIT USER DEF", selectUserNames, 1}
)


// CV configuration menu
DEF_MENULIST (listAnag, CV CONFIG, ListLines,
    #ifdef CVTHING
    {"<-BACK", gotoMenuSettings, 1},
    #else
    {"<-BACK", gotoMenuBanks, 1},
    #endif
    {"FUNCTION", SelectAnagFn, 1},
    {"MIDI CH", SelectMIDIChan, 1},
    {"CC#", SelectController, 0}, // Shown only when Function CC
    {"CV DELAY", SelectDelay, 1}, //
    {"RANGES", SelectAnagRange, 1},
    {"CLOCK DIV", SelectAnagClockDiv, 0},
    {"CLOCK SHIFT", SelectAnagClockShift, 0}, // Shown only when Clock
    {"CLOCK SET", SetClockMenu, 1},           // Shown only when Clock
    {"NRPN MSB", SelectNRPNMSB, 1},
    {"NRPN LSB", SelectNRPNLSB, 1},
    {"AUTO OFF", AutoOffFn, 0},
    {"NRPN Val", SelectNRPNval, 1},
    {"PORT NAME", SelectNameMenu, 1},
    {"SCALE/CHORDS", SelectChordMenu, 0}
)

// Fader configuration menu
DEF_MENULIST (listAnag2, FADER CONFIG, ListLines,
    {"<-BACK", gotoMenuBanks, 1},
    {"FUNCTION", SelectAnagFn2, 1},
    {"MIDI CH", SelectMIDIChan, 1},
    {"CC#", SelectController, 0}, // Shown only when Function CC
    {"NRPN MSB", SelectNRPNMSB, 1},
    {"NRPN LSB", SelectNRPNLSB, 1},
    {"AUTO OFF", AutoOffFn, 0},
    {"OUT RANGE", SetRangeMenu, 1},
    {"PORT NAME", SelectNameMenu, 1}
)


DEF_MENULIST (AnagChordFn, CHORD FUNC, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"CHORD TYPE", SelectDefChord, 1},
    {"SCALE MODE", SelectDefScale, 1},
    {"SCALE ROOT", SelectScaleRoot, 1},
    {"INV/DROP", SelectInvDrop, 1},
    {"STRUM", SelectFixDelay, 1},
    {"HUMANIZE", SelectFixRand, 1}
)


DEF_MENULIST (HiResFn, HI-RES MIDI, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"NRPN 7BIT", SelectNRPN7, 1},
    {"NRPN 14BITS", SelectNRPN14, 1},
    {"CC 14BITS", SelectCC14b, 1}
)


DEF_MENULIST (AnagTransposeFn, TRANSPOSE, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"OCTAVE", SelectTransposeOctave, 1},
    {"SEMITONES", SelectTransposeSemitones, 1}
)


DEF_MENULIST (listAnagFn, MIDI MAPPING, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"CC", SelectCC, 1},
    {"V/OC TO NOTE", SelectTrig, 1},
    {"V/OC POLY", SelectTrig8Level, 1},
    {"NOTES", SelectNoteDud, 1},
    {"VELOCITY", SelectVel, 1},
    {"PITCH BEND", SelectBend, 1},
    {"PROG CHANGE", SelectProgCha, 1},
    {"CH PRESSURE", SelectAfterT, 1},
    {"DIG FUNC->", SelectAngDigFN, 1},
    {"CHORD FUNC->", SelectChordFN, 1},
    {"HiRES MIDI->", SelectHiresFN, 1},
    {"TRANSPOSE->", SelectTransposeFN, 1},
    {"NO FUNC", SelectNoFunc, 1}
)

DEF_MENULIST (listAnagFn2, MIDI MAPPING, ListLines,
    {"<-BACK", SelectFaderConfig, 1},
    {"CC", SelectCC, 1},
    {"V/OC TO NOTE", SelectTrig, 1},
    {"NOTES", SelectNoteDud, 1},
    {"VELOCITY", SelectVel, 1},
    {"PITCH BEND", SelectBend, 1},
    {"PROG CHANGE", SelectProgCha, 1},
    {"CH PRESSURE", SelectAfterT, 1},
    {"DIG FUNC->", SelectAngDigFN, 0},
    {"CHORD FUNC->", SelectChordFN, 1},
    {"HiRES MIDI->", SelectHiresFN, 1},
    {"TRANSPOSE->", SelectTransposeFN, 1},
    {"NO FUNC", SelectNoFunc, 1}
)

DEF_MENULIST (listAnagDigFn, MIDI MAPPING, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"NOTE GATE", SelectAnagNote, 1},
    {"NOTE LATCH", SelectAnagLatchNote, 1},
    {"CC GATE", SelectAnagCC, 1},
    {"CC LATCH", SelectAnagLatchCC, 1},
    {"CLOCK", SelectAngClock, 1},
    {"ST/SP", SelectAnaSTSPFn, 1}
)


DEF_MENULIST (listAnaSTSPFn, ST/SP MODE, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"ST/SP GATE", SelectAnagStartStop, 1},
    {"ST/SP LATCH", SelectAnagLatchStartStop, 1},
    {"CONT/SP GATE", SelectAnagContStop, 1},
    {"CONT/SP LATCH", SelectAnagLatchContStop, 1},
    {"START", SelectAnagStart, 1},
    {"STOP", SelectAnagStop, 1},
    {"CONTINUE", SelectAnagCont, 1}
)

DEF_MENULIST (ListAutoOffOptions, AUTO OFF, ListLines,
    {"<-BACK", gotoMenuAnag, 1},
    {"NO AUTO OFF", AutoOff0, 1},
    {"0500ms OFF", AutoOff0500, 1}, 
    {"1000ms OFF", AutoOff1000, 1}, 
    {"1500ms OFF", AutoOff1500, 1}, 
    {"3000ms OFF", AutoOff3000, 1}, 
    {"Edit AUTOOFF", AutoOffEdit, 1}
)

MenuItem StdNamesItems[] = {
    {"Name", selectMenuPortName, 1}};
MenuList StdNameList(StdNamesItems, 1, ListLines);



// Gate Mode. NOT IN USE?
DEF_MENULIST (listGateMode, Bank Function, ListLines,
    {"<-BACK", gotoMenuBanks, 1},
    {"GATE", SelectGateModeGate, 1},
    {"TRIGG", SelectGateModeTrigger, 1}
)

/*****************************
 ******************************
   Define the functions you want your menu to call
   They can be blocking or non-blocking
   They should take no arguments and return a bool
   true if the function is finished and doesn't want to run again
   false if the function is not done and wants to be called again
 ******************************
 *****************************/

//////////////////////////////////////////

bool SelectCVConfig()
{
    if (BankSelected == 8)
    {
        if (PortSelected < 2)
            return true;
    }
    else
        PortSelected = 2;

    if (CVControls[BankSelected].CVPort.PortCfg.IsDigitalFunc() ||
        CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCHTRIG)
        listAnag.enableItem(4);
    else
        listAnag.disableItem(4);

    listAnag.disableItem(6);
    listAnag.disableItem(7);
    listAnag.disableItem(8);
    listAnag.disableItem(9);
    listAnag.disableItem(10);
    listAnag.disableItem(11);
    listAnag.disableItem(12);
    // Show clock Div
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == ANAGCLOCK)
    {
        listAnag.enableItem(8);
    }
    // Show NRPN
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == ANAGNRPN14bits ||
        CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == ANAGNRPN7bits)
    {
        listAnag.enableItem(12);
        listAnag.enableItem(10);
        listAnag.enableItem(9);
    }
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCH)
    {
        listAnag.enableItem(11);
    }

    #ifdef CVTHING
    // Show Chord option for CVThing
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCHTRIG 
        || CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCH8TRIG){
        listAnag.enableItem(14);
    } else{
        listAnag.disableItem(14);
    }
    #endif


    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnag);
    return true;
}

////////////////////////////////////////////////
// Transpose functions
bool SelectTransposeFN()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&AnagTransposeFn);
    return true;
}

////////////////////////////////////////////////
// Scale / Chord functions
bool SelectChordFN()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&AnagChordFn);
    return true;
}

bool SelectHiresFN(){
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&HiResFn);
    return true;
}

bool SelectChannel(int inic)
{
    static long val = -1;
    AnInputPortCfg *pPort = ((AnInputPortCfg *)GetPortCfg());
    if( val == -1) // Init value
        val = pPort->DestCtrl+inic;
    bool ret = EncoderchangeValue("Channel:", val, inic, 8, 1, 00, 45);
    
    if( ret){ // End selection
        MidiMerge.VelData[pPort->MIDIChannel - 1] = 127; //Reset MIDI channel velocity
        pPort->DestCtrl = val - inic;
        val = -1;
        }

    return ret;
}

bool SelectDefChord()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CHORDTYPE_DEF);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}

bool SelectDefScale()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(SCALE_DEF);

    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}


bool SelectInvDrop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CHORDINVERSION);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}


bool SelectScaleRoot()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(SCALEROOT);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}


bool SelectFixDelay()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CHORDDELAYFIX);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}


bool SelectFixRand()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CHORDDELAYRAND);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectChannel())
            return false; // Select MSB
        else
            SetValState = 0;
    }

    return gotoMenuAnag();
}


//////////////////////
// Transposition config
bool SelectTransposeOctave(){
    long val = ((AnInputPortCfg *)GetPortCfg())->TransposeOctave;
    bool ret = EncoderchangeValue("OCTAVE:", val, -10, 10, 3, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->TransposeOctave = val;
    return ret;
    return true;
}

bool SelectTransposeSemitones(){
    long val = ((AnInputPortCfg *)GetPortCfg())->TransposeSemitones;
    bool ret = EncoderchangeValue("SEMITONES:", val, -12, 12, 4, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->TransposeSemitones = val;
    return ret;
    return true;
}

//////////////////////
// Port name selection
bool SelectNameMenu()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listName);
    return true;
}


bool selectUserNames()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&UserNameList);
    return true;
}

bool editUserNames()
{
    static uint8_t idx = 0;

    if (SetValState == 0){
        idx=  myMenu.getItemStatus();
        if(idx<200 || idx>200+NUMUSERNAMES)
            return true; // Cancel: no User Name selected
        GetPortCfg()->textPort = idx;
        idx -= 200;
        SetValState = idx+1;
    }
    if (SetValState > 0)
    {
        if (!theApp.vMenu.EncoderselDigitStr("Name ", theApp.theGlobalCfg.UserNames[idx], SIZEPORTNAMES, 0, 45))
            return false; // Editing name
        SetValState = 0;
    }

    return SelectNameMenu();
}

bool selectMenuPortName()
{
    //GetPortCfg()->textPort = myMenu.currentItemIndex+1;
    GetPortCfg()->textPort = myMenu.getItemStatus();

    return SelectNameMenu();
}

bool SelectStdName()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&StdNameList);
    return true;
}

bool SelectPrefix()
{
    char str[2] = " ";
    char act = GetPortCfg()->charPrefix;
    if(act) str[0] = act;
    else    str[0] = ' ';
    bool ret = theApp.vMenu.EncoderselDigitStr("Prefix ", str, 1, 0, 45);
    if(str[0]==' ') str[0] = 0;
    GetPortCfg()->charPrefix = str[0];

    if( ret) 
        return SelectNameMenu();
    else
        return false;
}

bool SelectSufix()
{
    char str[2] = " ";
    char act = GetPortCfg()->charSufix;
    if(act) str[0] = act;
    else    str[0] = ' ';
    bool ret = theApp.vMenu.EncoderselDigitStr("Sufix ", str, 1, 0, 45);
    if(str[0]==' ') str[0] = 0;
    GetPortCfg()->charSufix = str[0];

    if( ret) 
        return SelectNameMenu();
    else
        return false;
}


// GATE Mode configuration
bool SelectGateModeGate()
{
    // TODO
    //Sends Note on on rise and note off on fall
    return true;
}

bool SelectGateModeTrigger()
{
    // TODO
    //Sends Note on on rise and note off ~3 milliseconds after
    return true;
}

// Back to Gate function Menu
bool gotoMenuGate()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnag);
    return true;
}
////////////////////////////////
// CV/Fader configuration menu

bool AutoOffFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&ListAutoOffOptions);
    return true;
}


bool AutoOff0(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 0;
    gotoMenuAnag();
    return true;
}

bool AutoOff0500(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 500;
    gotoMenuAnag();
    return true;    
}

bool AutoOff1000(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 1000;
    gotoMenuAnag();
    return true;
}

bool AutoOff1500(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 1500;
    gotoMenuAnag();
    return true;
}

bool AutoOff3000(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 3000;
    gotoMenuAnag();
    return true;
}

bool AutoOffEdit()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->AutoOff; // theApp.theGlobalCfg.AutoOff;
    bool ret = EncoderchangeValue("Auto Off ms:", val, 0, 5000, 4, 00, 45);
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = val;

    if(ret) gotoMenuAnag();
    return ret;
}


bool SelectController()
{
    long val = GetPortCfg()->ControllerNumber;
    bool ret = EncoderchangeValue("Controller ", val, 0, 127, 3, 0, 45);
    GetPortCfg()->ControllerNumber = val;
    return ret;
}

// Analog options menu

// Back to CV config
bool gotoMenuAnag()
{

    if (PortSelected == 2)
        SelectCVConfig();
    else if (PortSelected == 3)
        SelectFaderConfig();
    else if (PortSelected == 1)
        SelectGateConfig();
    return true;
}

// Analog Range menu
bool SelectAnagRange()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagRange);
    return true;
}

////////////////////////
// Analog functions menu
bool SelectAnagFn()
{
   #ifdef CVTHING
    // Disable V/Oct in even ports
    if(BankSelected%2==1){
        listAnagFn.disableItem(2);
    } else
    {
        listAnagFn.enableItem(2);
    }
    // Disable V/Oct Poly in ports 8
    if( BankSelected ==7)
      listAnagFn.disableItem(2);  
    #endif

    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagFn);
    return true;
}

bool SelectAnagFn2()
{
    // Enable Dig Functions for AuxB
    listAnagFn2.disableItem(8);
    //listAnagFn2.disableItem(7);
    if(BankSelected==8){
        listAnagFn2.enableItem(8);
        //listAnagFn2.enableItem(7);        
    }
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagFn2);
    return true;
}


bool SelectMIDIChan()
{
    long val = GetPortCfg()->MIDIChannel; //((AnInputPortCfg*)GetPortCfg())->MIDIChannel;
    bool ret = EncoderchangeValue("MIDI Channel:", val, 1, 16, 3, 00, 45);
    ((AnInputPortCfg *)GetPortCfg())->MIDIChannel = val;
    //GetPortCfg()->MIDIChannel = val;
    return ret;
}

bool SelectTrig()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCHTRIG);
    CVControls[BankSelected].GateBut.PortCfg.MIDIfunction = TRIGGER;
    #ifdef CVTHING
    if(BankSelected%2==0){
        CVControls[BankSelected+1].CVPort.PortCfg.SetMIDIFunc(PITCHLEVEL);
        CVControls[BankSelected+1].GateBut.PortCfg.MIDIfunction = TRIGGER;
    }
    // PITCH8TRIG
    #endif

    return gotoMenuAnag();
}



bool SelectTrig8Level()
{
    //if( BankSelected == 7)
    //    return true; // Not applicable for Bank 8
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCH8TRIG);
    #ifdef CVTHING
        CVControls[BankSelected].GateBut.PortCfg.MIDIfunction = TRIGGER;
        CVControls[7].CVPort.PortCfg.SetMIDIFunc(PITCHLEVEL);
        CVControls[7].GateBut.PortCfg.MIDIfunction = TRIGGER;
    // PITCH8TRIG
    #else
        CVControls[7].GateBut.PortCfg.MIDIfunction = GATE8TRIG;
    #endif
    return gotoMenuAnag();
}


bool SelectTrigLevel()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCHLEVEL);
    CVControls[BankSelected].GateBut.PortCfg.MIDIfunction = TRIGGER;

    return gotoMenuAnag();
}




bool SelectNoteDud()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCH);
    //return gotoMenuAnag();

    if (!AutoOffFn())
        return false;
    return true;

}

/*bool SelectNRPN7 () {
    ((AnInputPortCfg *)GetPortCfg ())->SetMIDIFunc (ANAGNRPN7bits);
  if ( !SelectNRPNMSB()) return false;
  //return gotoMenuAnag();
  return true;
}
bool SelectNRPN14 () {
    ((AnInputPortCfg *)GetPortCfg ())->SetMIDIFunc (ANAGNRPN14bits);
  if ( !SelectNRPNMSB()) return false;
  //return gotoMenuAnag();
  return true;
}
*/

bool SelectNRPN7()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGNRPN7bits);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectNRPNMSB())
            return false; // Select MSB
        else
            SetValState++;
        return false;
    }
    if (SetValState == 2)
    {
        if (!SelectNRPNLSB())
            return false; // Select LSB
        else
            SetValState = 0;
    }

    gotoMenuAnag();
    return true;
}

bool SelectNRPN14()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGNRPN14bits);
    if (SetValState == 0)
        SetValState++;
    if (SetValState == 1)
    {
        if (!SelectNRPNMSB())
            return false; // Select MSB
        else
            SetValState++;
        return false;
    }
    if (SetValState == 2)
    {
        if (!SelectNRPNLSB())
            return false; // Select LSB
        else
            SetValState = 0;
    }

    gotoMenuAnag();
    return true;
}


bool SelectVel()
{
    if (SetValState == 0){ // Init
        ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(VELOCITY);
        SetValState++;
        }
    if (SetValState == 1)
    {
        //if( PortSelected!= 2) // Select destination only for CV
        //    return gotoMenuAnag();
        if (!SelectChannel(0)){
            return false;
            }
        else
            SetValState = 0;
        uint8_t dest = ((AnInputPortCfg *)GetPortCfg())->DestCtrl;
        if(dest){ // Set destination reference to velocity channel
            CVControls[dest - 1].GateBut.velOrigPort = BankSelected + 1;
            }
    }

    return gotoMenuAnag();
}

bool SelectCC()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CONTROLCHANGE);
    if (!SelectController())
        return false;
    return gotoMenuAnag();
}

bool SelectCC14b()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CC14BITS);
    if (!SelectController())
        return false;
    return gotoMenuAnag();
}


bool SelectBend()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCHBEND);
    return gotoMenuAnag();
}

bool SelectProgCha()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PROGRAMCHANGE);
    return gotoMenuAnag();
}

bool SelectAfterT()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(AFTERTOUCH);
    return gotoMenuAnag();
}

bool SelectPercent()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PERCENT);
    return gotoMenuAnag();
}

bool SelectAngClock()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCLOCK);
    if (!SetClockMenu())
        return false;

    return gotoMenuAnag();
}

bool SelectNoFunc()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(NOANFFUNC);
    return gotoMenuAnag();
}

// Modify Clock parameters
bool SelectAnagClockDiv()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->ClockDivider;
    bool ret = EncoderchangeValue("Clock Div:", val, 1, 255, 3, 0, 45);
    theApp.theGlobalCfg.ClockDivider= 
        ((AnInputPortCfg *)GetPortCfg())->ClockDivider = val;
    return ret;
}

bool SelectAnagClockShift()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->ClockShift;
    bool ret = EncoderchangeValue("Clock Shift:", val, -120, 120, 4, 0, 45);
    theApp.theGlobalCfg.ClockShift =
        ((AnInputPortCfg *)GetPortCfg())->ClockShift = val;
    return ret;
}

// Modify NRPN channel
bool SelectNRPNMSB()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB;
    bool ret = EncoderchangeValue("NRPN MSB:", val, 0, 127, 3, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB = val;
    return ret;
}

bool SelectNRPNLSB()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB;
    bool ret = EncoderchangeValue("NRPN LSB:", val, 0, 127, 3, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB = val;
    return ret;
}

bool SelectNRPNval()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB*128+((AnInputPortCfg *)GetPortCfg())->NRPNparLSB;
    bool ret = myMenu.EncoderselDigitLong("NRPN:", val, 0, 16383, 5, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB = val / 128;
    ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB = val%128;
    return ret;

}

/////////////////////////////////////////////
// Analog Ch Digital Func
bool SelectAngDigFN()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagDigFn);
    return true;
}

bool SelectAnagNote()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGGER);

    if (!SelectNoteNmbr())
        return false;

    return gotoMenuAnag();
}

bool SelectAnagLatchNote()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGLATCH);
    
    if (!SelectNoteNmbr())
        return false;

    return gotoMenuAnag();
}

bool SelectAnagCC()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCCBUT);

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

    //return gotoMenuAnag();
    return true;
}

bool SelectAnagLatchCC()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCCLATCH);

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

    return gotoMenuAnag();
}


/////////////////////////////////////////////
// STart Stop Func
// NEW ST SP functions menu for analogue menu
bool SelectAnaSTSPFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnaSTSPFn);
    return true;
}

bool SelectAnagStartStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTARTSTOP);
    return gotoMenuAnag();
}

bool SelectAnagContStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGPAUSECONT);
    return gotoMenuAnag();
}

bool SelectAnagLatchStartStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGSTARTSTOP);
    return gotoMenuAnag();
}

bool SelectAnagLatchContStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGPAUSECONT);
    return gotoMenuAnag();
}

bool SelectAnagStart()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTART);
    return gotoMenuAnag();
}

bool SelectAnagCont()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCONTINUE);
    return gotoMenuAnag();
}

bool SelectAnagStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTOP);
    return gotoMenuAnag();
}

/** @} */
