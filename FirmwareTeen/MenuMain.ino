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
 *  \file MenuMain.ino
 *  \brief Defines Menu functions callbacks response for Main/General configuration 
 */

// Main screen menu placeholder (This menu do not show )
MenuItem firstList[3] = {
    {"Channel Cfg", gotoMenuBanks, 1},
    {"Global Cfg", gotoMenuGlobalCfg, 0},
    {"Channel Select", NULL, 0} // causa-efecto
};
MenuList list1(firstList, 2, ListCards); // (Nombre lista, numero items, Tipo de item)

// global config menu. enters Button+encoder
MenuItem GlobalconfigList[] = {
    {"<-BACK", gotoMenuSettings, 1},
    {"PANIC", PanicFn, 1},
    {"FACTORY RST", gotoListFactReset, 1},
    {"GLOBAL CV CAL", CVTwoPointsCal, 1},
    {"DIM SCREEN", SetScreenLight, 0},
    {"SAVE CONF", gotoListBankSaveSel, 1}, //SelectSavePage, 1}
    {"LOAD CONF", gotoListBankLoadSel, 1}, //SelectLoadPage, 1}
    {"CREDITS", CreditsScreen, 1},         //Show a screen with firmware version and credits
    {"FACTORY CAL", gotoMenuGlobalCal, 0},
    {"User Names", changeUserNames, 0},
    {" GLOBAL CFG ", NULL, 1}};
MenuList listGlobal(GlobalconfigList, 10, ListLinesSimple);

MenuItem GlobalCalList[] = {
    {"<-BACK", gotoMenuSettings, 1},
    {"MATRIX CAL", CalibrateCV, 0},       //Global config?
    {"CV GLOBAL CAL", CVTwoPointsCal, 1}, //Global config?
    {"FADER CAL", makeFadersCal, 1},
    {"FADER FILT", SelectFaderFilter, 1},
    {"FADER THR", SelectFaderThres, 0},
    {"AUX A CAL", AuxATwoPointsCal, 1},
    {"AUX B CAL", AuxBTwoPointsCal, 1},
    {"GLOBAL CAL", NULL, 1}};
MenuList listGlobalCal(GlobalCalList, 8, ListLines);//ListLinesSimple);

// Received SysEx Menu
MenuItem RecSysExList[3] = {
    {"DISCARD", gotoMenuSettings, 1},
    {"LOAD SYSEX", SelectSysExLoadPage, 1},
    {"RECEIVED SYSEX", NULL, 1}};
MenuList listLoadSysEx(RecSysExList, 2, ListLinesSimple);

// Save Config Menu
MenuItem BankSaveSelList[] = {
    {"CANCEL", gotoMenuGlobalCfg, 1},
    {"USER 1", SaveSlot1, 1},
    {"USER 2", SaveSlot2, 1},
    {"USER 3", SaveSlot3, 1},
    {"USER 4", SaveSlot4, 1},
    {"SYSEX", SaveViaSysEx, 1},
    {"SAVE CONFIG", NULL, 1}};
MenuList ListBankSaveSel(BankSaveSelList, 6, ListLinesSimple);

// Load Config Menu
MenuItem BankLoadSelList[] = {
    {"CANCEL", gotoMenuGlobalCfg, 1},
    {"USER 1", LoadSlot1, 1},
    {"USER 2", LoadSlot2, 1},
    {"USER 3", LoadSlot3, 1},
    {"USER 4", LoadSlot4, 1},
    {"PRESETS", LoadPreset, 1},
    {"LOAD CONFIG", NULL, 1}};
MenuList ListBankLoadSel(BankLoadSelList, 6, ListLinesSimple);

// Factory Reset Menu
MenuItem FactResetList[3] = {
    {"CANCEL", gotoMenuGlobalCfg, 1},
    {"RESET", FactoryReset, 1},
    {"FACTORY RESET", NULL, 1}};
MenuList ListFactReset(FactResetList, 2, ListLinesSimple);

MenuItem UserNamesItems[] = {
    {"Name", selectMenuPortName, 1}};
MenuList UserNameList(UserNamesItems, 1, ListLines);


/*****************************
 ******************************
 * Define the functions you want your menu to call
 * They can be blocking or non-blocking
 * They should take no arguments and return a boolean
 * true if the function is finished and doesn't want to run again
 * false if the function is not done and wants to be called again
 ******************************
 *****************************/
boolean DoNothing()
{
    return true;
}

////////////////////////////////
// User Names
bool changeUserNames()
{
    bool ret = true;
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&UserNameList);
    return true;
    return ret;
}

//Main Menu entry function
boolean gotoMenuGlobalCfg()
{
    fullCVDisplay = false;
    if (AutoCal)
    {
        listGlobal.enableItem(8);
        GlobalCalEnabled = true;
    }
    else
    {
        listGlobal.disableItem(8);
        //GlobalCalEnabled = false;
    }
    if (durationButton != 1)
        myMenu.ClearArea();
    myMenu.setCurrentMenu(&listGlobal);
    if (!EncButton->rose())
        return false;
    EncButton->update();
    gotoGloblaMenu = false;
    return true;
}

boolean gotoMenuGlobalCal()
{
    #ifdef CVTHING
    listGlobalCal.disableItem(3);
    listGlobalCal.disableItem(4);
    listGlobalCal.disableItem(5);
    #endif
    
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&listGlobalCal);
    return true;
}

/*
boolean SelectGateMode() {
  PortSelected=2;
  myMenu.ClearArea();
  myMenu.setCurrentMenu(&listGateMode);	
  return true;
}*/

/////////////////////////////////////////////
/// Global Config
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

boolean SelectFaderFilter()
{
    long val = theApp.theGlobalCfg.filterFader*100000;
    bool ret = myMenu.EncoderselDigitLong("Filter: 0.", val, 0, 999999, 5, 0, 45);
    theApp.theGlobalCfg.filterFader = val/100000.0;
    if( ret)
        theApp.ApplyFilters();
    return ret;
}

boolean SelectFaderThres()
{
    long val = theApp.theGlobalCfg.ActThrFader;
    bool ret = EncoderchangeValue("Thresh:", val, 0, 256, 4, 0, 45);
    theApp.theGlobalCfg.ActThrFader = val;
    if( ret)
        theApp.ApplyFilters();
    return ret;
}




boolean PanicFn()
{
    // MIDI Panic
    MidiMerge.Panic();
    return true;
}


boolean makeFadersCal()
{
    // Fader calibration
    return FadersCal();
}

boolean gotoListFactReset()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&ListFactReset);
    return true;
}

boolean ResetCPU(){
    CPU_RESTART
    return true;
}

boolean FactoryReset()
{
    // Panic
    MidiMerge.Panic();
    // Reset
    EEPROM.write(2, 100); // Prevent auto loading Page config at re start
    ResetCPU();
    return true;
}

boolean SelectClockRes()
{
    //Send 24, 12 or 6 midi clicks per pulse received.
    // possible via a numeric field or need a menu?
    return true;
}

boolean dimStatus = false;
boolean SetScreenLight()
{
    //Set screen luminosity
    dimStatus = !dimStatus;
    theOLED->dim(dimStatus);
    return true;
}

/**
 * \brief Show the credits screen
 * 
 * It will also reset the timer for Factory calibration menu
 * 
 * \return boolean 
 */
boolean CreditsScreen()
{
    AutoCal = true;
    CalTimer = millis();
    //Show firmware version and credits.
    myMenu.setupPopupBMP(0, 3, mini_splash, 128, 24, 1);
    myMenu.setupPopup(CreditText, 3000, 0, 0, 127); //17, myMenu.MenuPosX-1);

    gotoMenuSettings();

    return true;
}

//////////////////////////////////////////////////
// Save and Load functions

boolean gotoListBankSaveSel()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&ListBankSaveSel);
    return true;
}

boolean gotoListBankLoadSel()
{
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&ListBankLoadSel);
    return true;
}

boolean SaveSlotCurrent(){
    if (!FlashAccess->SetCurrentPage(FlashAccess->CurrentPage))
    {
        SetMessageText("Error Save");
    }
    gotoMenuGlobalCfg();
    return true;

}
boolean SaveSlot1()
{
    FlashAccess->CurrentPage = 0;
    return SaveSlotCurrent();
}

boolean SaveSlot2()
{
    FlashAccess->CurrentPage = 1;
    return SaveSlotCurrent();
}

boolean SaveSlot3()
{
    FlashAccess->CurrentPage = 2;
    return SaveSlotCurrent();
}

boolean SaveSlot4()
{
    FlashAccess->CurrentPage = 3;
    return SaveSlotCurrent();
}

boolean SaveViaSysEx()
{
    FlashAccess->SaveCfgSysEx ();
    return true;
}

boolean LoadSlotCurrentPage(){
    if (!FlashAccess->LoadCfg())
    {
        SetMessageText("Error Load");
    }
    gotoMenuGlobalCfg();
    return true;
}

boolean LoadSlot1()
{
    FlashAccess->CurrentPage = 0;
    return LoadSlotCurrentPage();
}

boolean LoadSlot2()
{
    FlashAccess->CurrentPage = 1;
    return LoadSlotCurrentPage();
}

boolean LoadSlot3()
{
    FlashAccess->CurrentPage = 2;
    return LoadSlotCurrentPage();
}

boolean LoadSlot4()
{
    FlashAccess->CurrentPage = 3;
    return LoadSlotCurrentPage();
}

boolean SelectSavePage()
{
    long val = FlashAccess->CurrentPage + 1;
    bool ret = EncoderchangeValue("Save Page", val, 1, MAXSAVEPAGES, 3, 0, 45);
    FlashAccess->CurrentPage = val - 1;
    if (!ret)
        return false;
    if (!FlashAccess->SetCurrentPage(val))
    {
        SetMessageText("Error Save");
    }
    return true;
}

boolean SelectLoadPage()
{
    long val = FlashAccess->CurrentPage + 1;
    bool ret = EncoderchangeValue("Load Page", val, 1, MAXSAVEPAGES, 3, 0, 45);
    FlashAccess->CurrentPage = val - 1;
    if (!ret)
        return false;
    if (!FlashAccess->LoadCfg())
    {
        SetMessageText("Error Load");
    }
    return true;
}

boolean SelectSysExLoadPage()
{
    long val = FlashAccess->CurrentPage + 1;
    bool ret = EncoderchangeValue("Load Page", val, 1, MAXSAVEPAGES, 3, 0, 45);
    FlashAccess->CurrentPage = val - 1;
    if (!ret)
        return false;
    if (!FlashAccess->LoadSysExCfg())
    {
        SetMessageText("Error Load");
    }
    else
    {
        MidiMerge.tagProcessSysex = 2;
        gotoMenuSettings();
    }
    return true;
}

/** @} */
