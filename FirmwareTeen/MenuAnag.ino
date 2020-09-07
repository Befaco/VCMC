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

// Gate Mode. NOT IN USE?
MenuItem GateModelist[4] = {
    {"<-BACK", gotoMenuBanks, 1},
    {"GATE", SelectGateModeGate, 1},
    {"TRIGG", SelectGateModeTrigger, 1},
    {"Bank Function", NULL, 1}};
MenuList listGateMode(GateModelist, 4, ListLines);

// Name selection Menu
MenuItem NameList[] = {
    {"<-BACK", gotoMenuAnag, 1},
    {"SELECT NAME", SelectStdName, 1},
    {"PREFIX", SelectPrefix, 0},
    {"SUFIX", SelectSufix, 1},
    {"EDIT USER DEF", selectUserNames, 1},
    {"PORT NAME", NULL, 1}};
MenuList listName(NameList, 5, ListLines);


// CV configuration menu
MenuItem AnagList[] = {
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
    {"I2C Menu", selectI2CDevice, 1},
    {"I2C Ops", selectI2COp, 1},
    {" CV CONFIG ", NULL, 1}};
MenuList listAnag(AnagList, 16, ListLines);

// Fader configuration menu
MenuItem AnagList2[] = {
    {"<-BACK", gotoMenuBanks, 1},
    {"FUNCTION", SelectAnagFn2, 1},
    {"MIDI CH", SelectMIDIChan, 1},
    {"CC#", SelectController, 0}, // Shown only when Function CC
    {"NRPN MSB", SelectNRPNMSB, 1},
    {"NRPN LSB", SelectNRPNLSB, 1},
    {"AUTO OFF", AutoOffFn, 0},
    {"OUT RANGE", SetRangeMenu, 1},
    {"PORT NAME", SelectNameMenu, 1},
    {"I2C Device", selectI2CDevice, 1},
    {"I2C Ops", selectI2COp, 1},
    {"FADER CONFIG", NULL, 1}};
MenuList listAnag2(AnagList2, 11, ListLines);

MenuItem AnagFnList[] = {
    {"<-BACK", gotoMenuAnag, 1},
    {"V/OC TO NOTE", SelectTrig, 1},
    {"V/OC POLY", SelectTrig8Level, 1},
    {"CC", SelectCC, 1},
    {"VELOCITY", SelectVel, 1},
    {"PITCH BEND", SelectBend, 1},
    {"PROG CHANGE", SelectProgCha, 1},
    {"DIG FUNC", SelectAngDigFN, 1},
    {"ST/SP", SelectAnaSTSPFn, 0}, //Before was calling SelectAnagStartStop
    {"NOTES", SelectNoteDud, 1},
    {"NOTE GATE", SelectTrigLevel, 0},
    {"NRPN 7BIT", SelectNRPN7, 1},
    {"NRPN 14BITS", SelectNRPN14, 1},
    {"CC 14BITS", SelectCC14b, 1},
    {"NO FUNC", SelectNoFunc, 1},
    {"MIDI MAPPING", NULL, 1}
    };
MenuList listAnagFn(AnagFnList, 14, ListLines);

MenuItem AnagFnList2[] = {
    {"<-BACK", SelectFaderConfig, 1},
    {"V/OC TO NOTE", SelectTrig, 1},
    {"CC", SelectCC, 1},
    {"VELOCITY", SelectVel, 1},
    {"PITCH BEND", SelectBend, 1},
    {"PROG CHANGE", SelectProgCha, 1},
    {"DIG FUNC", SelectAngDigFN, 0},
    {"ST/SP", SelectAnaSTSPFn, 0}, //Before was calling SelectAnagStartStop
    {"NOTES", SelectNoteDud, 1},
    {"NOTE GATE", SelectTrigLevel, 0},
    {"NRPN 7BITS", SelectNRPN7, 1},
    {"NRPN 14BITS", SelectNRPN14, 1},
    {"CC 14BITS", SelectCC14b, 1},
    {"NO FUNC", SelectNoFunc, 1},
    {"MIDI MAPPING", NULL, 1}};
MenuList listAnagFn2(AnagFnList2, 14, ListLines);

MenuItem AnagDigFnList[8] = {
    {"<-BACK", gotoMenuAnag, 1},
    {"NOTE GATE", SelectAnagNote, 1},
    {"NOTE LATCH", SelectAnagLatchNote, 1},
    {"CC GATE", SelectAnagCC, 1},
    {"CC LATCH", SelectAnagLatchCC, 1},
    {"CLOCK", SelectAngClock, 1},
    {"ST/SP", SelectAnaSTSPFn, 1}, //Before was calling SelectAnagStartStop
    {"MIDI MAPPING", NULL, 1}};
MenuList listAnagDigFn(AnagDigFnList, 7, ListLines);

MenuItem STSPAnaFnList[] = {
    {"<-BACK", gotoMenuAnag, 1},
    {"ST/SP GATE", SelectAnagStartStop, 1},
    {"ST/SP LATCH", SelectAnagLatchStartStop, 1},
    {"CONT/SP GATE", SelectAnagContStop, 1},
    {"CONT/SP LATCH", SelectAnagLatchContStop, 1},
    {"START", SelectAnagStart, 1},
    {"STOP", SelectAnagStop, 1},
    {"CONTINUE", SelectAnagCont, 1},
    {"ST/SP MODE", NULL, 1}};
MenuList listAnaSTSPFn(STSPAnaFnList, 8, ListLines);

MenuItem AutoOffOptionsList[8] = {
    {"<-BACK", gotoMenuAnag, 1},
    {"NO AUTO OFF", AutoOff0, 1},
    {"0500ms OFF", AutoOff0500, 1}, 
    {"1000ms OFF", AutoOff1000, 1}, 
    {"1500ms OFF", AutoOff1500, 1}, 
    {"3000ms OFF", AutoOff3000, 1}, 
    {"Edit AUTOOFF", AutoOffEdit, 1}, 
    {"AUTO OFF", NULL, 1}};
MenuList ListAutoOffOptions(AutoOffOptionsList, 7, ListLinesSimple);

MenuItem StdNamesItems[] = {
    {"Name", selectMenuPortName, 1}};
MenuList StdNameList(StdNamesItems, 1, ListLines);

/*****************************
 ******************************
   Define the functions you want your menu to call
   They can be blocking or non-blocking
   They should take no arguments and return a boolean
   true if the function is finished and doesn't want to run again
   false if the function is not done and wants to be called again
 ******************************
 *****************************/

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
boolean SelectGateModeGate()
{
    // TODO
    //Sends Note on on rise and note off on fall
    return true;
}

boolean SelectGateModeTrigger()
{
    // TODO
    //Sends Note on on rise and note off ~3 milliseconds after
    return true;
}

// Back to Gate function Menu
boolean gotoMenuGate()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnag);
    return true;
}
////////////////////////////////
// CV/Fader configuration menu

boolean AutoOffFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&ListAutoOffOptions);
    return true;
}


boolean AutoOff0(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 0;
    gotoMenuAnag();
    return true;
}

boolean AutoOff0500(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 500;
    gotoMenuAnag();
    return true;    
}

boolean AutoOff1000(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 1000;
    gotoMenuAnag();
    return true;
}

boolean AutoOff1500(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 1500;
    gotoMenuAnag();
    return true;
}

boolean AutoOff3000(){
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = 3000;
    gotoMenuAnag();
    return true;
}

boolean AutoOffEdit()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->AutoOff; // theApp.theGlobalCfg.AutoOff;
    bool ret = EncoderchangeValue("Auto Off ms:", val, 0, 5000, 4, 00, 45);
    ((AnInputPortCfg *)GetPortCfg())->AutoOff = val;

    if(ret) gotoMenuAnag();
    return ret;
}


boolean SelectController()
{
    long val = GetPortCfg()->ControllerNumber;
    bool ret = EncoderchangeValue("Controller ", val, 0, 127, 3, 0, 45);
    GetPortCfg()->ControllerNumber = val;
    return ret;
}

// Analog options menu

// Back to CV config
boolean gotoMenuAnag()
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
boolean SelectAnagRange()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagRange);
    return true;
}

////////////////////////
// Analog functions menu
boolean SelectAnagFn()
{
    /*
    listAnagFn.disableItem(6+1);
    listAnagFn.disableItem(7+1);
    if(BankSelected==8){
        listAnagFn.enableItem(6+1);
        listAnagFn.enableItem(7+1);        
    }
    */
   #ifdef CVTHING
    listAnagFn.disableItem(1);
    //listAnagFn.disableItem(9+1);
    if(BankSelected%2==1){
        //listAnagFn.enableItem(9+1);
    } else
    {
        listAnagFn.enableItem(1);
    }
    if( BankSelected ==7)
      listAnagFn.disableItem(2);    
    #endif

    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagFn);
    return true;
}

boolean SelectAnagFn2()
{
    listAnagFn2.disableItem(6);
    listAnagFn2.disableItem(7);
    if(BankSelected==8){
        listAnagFn2.enableItem(6);
        listAnagFn2.enableItem(7);        
    }
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagFn2);
    return true;
}

boolean SelectMIDIChan()
{
    long val = GetPortCfg()->MIDIChannel; //((AnInputPortCfg*)GetPortCfg())->MIDIChannel;
    bool ret = EncoderchangeValue("MIDI Channel:", val, 1, 16, 3, 00, 45);
    ((AnInputPortCfg *)GetPortCfg())->MIDIChannel = val;
    //GetPortCfg()->MIDIChannel = val;
    return ret;
}

boolean SelectTrig()
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



boolean SelectTrig8Level()
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


boolean SelectTrigLevel()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCHLEVEL);
    CVControls[BankSelected].GateBut.PortCfg.MIDIfunction = TRIGGER;

    return gotoMenuAnag();
}




boolean SelectNoteDud()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCH);
    //return gotoMenuAnag();

    if (!AutoOffFn())
        return false;
    return true;

}

/*boolean SelectNRPN7 () {
    ((AnInputPortCfg *)GetPortCfg ())->SetMIDIFunc (ANAGNRPN7bits);
  if ( !SelectNRPNMSB()) return false;
  //return gotoMenuAnag();
  return true;
}
boolean SelectNRPN14 () {
    ((AnInputPortCfg *)GetPortCfg ())->SetMIDIFunc (ANAGNRPN14bits);
  if ( !SelectNRPNMSB()) return false;
  //return gotoMenuAnag();
  return true;
}
*/

boolean SelectNRPN7()
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

boolean SelectNRPN14()
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


boolean SelectVel()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(VELOCITY);
    return gotoMenuAnag();
}

boolean SelectCC()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CONTROLCHANGE);
    if (!SelectController())
        return false;
    return gotoMenuAnag();
}

boolean SelectCC14b()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(CC14BITS);
    if (!SelectController())
        return false;
    return gotoMenuAnag();
}


boolean SelectBend()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PITCHBEND);
    return gotoMenuAnag();
}

boolean SelectProgCha()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PROGRAMCHANGE);
    return gotoMenuAnag();
}

boolean SelectPercent()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(PERCENT);
    return gotoMenuAnag();
}

boolean SelectAngClock()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCLOCK);
    if (!SetClockMenu())
        return false;

    return gotoMenuAnag();
}

boolean SelectNoFunc()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(NOANFFUNC);
    return gotoMenuAnag();
}

// Modify Clock parameters
boolean SelectAnagClockDiv()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->ClockDivider;
    bool ret = EncoderchangeValue("Clock Div:", val, 1, 255, 3, 0, 45);
    theApp.theGlobalCfg.ClockDivider= 
        ((AnInputPortCfg *)GetPortCfg())->ClockDivider = val;
    return ret;
}

boolean SelectAnagClockShift()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->ClockShift;
    bool ret = EncoderchangeValue("Clock Shift:", val, -120, 120, 4, 0, 45);
    theApp.theGlobalCfg.ClockShift =
        ((AnInputPortCfg *)GetPortCfg())->ClockShift = val;
    return ret;
}

// Modify NRPN channel
boolean SelectNRPNMSB()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB;
    bool ret = EncoderchangeValue("NRPN MSB:", val, 0, 127, 3, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB = val;
    return ret;
}

boolean SelectNRPNLSB()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB;
    bool ret = EncoderchangeValue("NRPN LSB:", val, 0, 127, 3, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB = val;
    return ret;
}

boolean SelectNRPNval()
{
    long val = ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB*128+((AnInputPortCfg *)GetPortCfg())->NRPNparLSB;
    bool ret = myMenu.EncoderselDigitLong("NRPN:", val, 0, 16383, 5, 0, 45);
    ((AnInputPortCfg *)GetPortCfg())->NRPNparMSB = val / 128;
    ((AnInputPortCfg *)GetPortCfg())->NRPNparLSB = val%128;
    return ret;

}

/////////////////////////////////////////////
// Analog Ch Digital Func
boolean SelectAngDigFN()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnagDigFn);
    return true;
}

boolean SelectAnagNote()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGGER);

    if (!SelectNoteNmbr())
        return false;

    return gotoMenuAnag();
}

boolean SelectAnagLatchNote()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGLATCH);
    
    if (!SelectNoteNmbr())
        return false;

    return gotoMenuAnag();
}

boolean SelectAnagCC()
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

boolean SelectAnagLatchCC()
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
boolean SelectAnaSTSPFn()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listAnaSTSPFn);
    return true;
}

boolean SelectAnagStartStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTARTSTOP);
    return gotoMenuAnag();
}

boolean SelectAnagContStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGPAUSECONT);
    return gotoMenuAnag();
}

boolean SelectAnagLatchStartStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGSTARTSTOP);
    return gotoMenuAnag();
}

boolean SelectAnagLatchContStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGTRIGPAUSECONT);
    return gotoMenuAnag();
}

boolean SelectAnagStart()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTART);
    return gotoMenuAnag();
}

boolean SelectAnagCont()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGCONTINUE);
    return gotoMenuAnag();
}

boolean SelectAnagStop()
{
    ((AnInputPortCfg *)GetPortCfg())->SetMIDIFunc(ANAGSTOP);
    return gotoMenuAnag();
}

/** @} */
