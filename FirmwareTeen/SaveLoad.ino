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

/** @addtogroup GeneralUtil Save SysEx
 *  @{
 */

/**
 *  \file SaveLoad.ino
 *  \brief Configuration for Save/Load support
 */

SaveLoadClass::SaveLoadClass () {
    uint32_t baseaddr = PAGEBASEADDR; // Use first byte to store Page number
    for (int i = 0; i < 8; i++) {
        PageBase[i] = baseaddr;
        baseaddr += PageSize;
    }
    CurrentPage = 0; // Defaults to bank 0
    ResetReception ();
}

/**
 * \brief Set new Bank number for Save/Load and save current config
 * 
 * \param page Memory bank to save to
 * \return true if succesful
 */
bool SaveLoadClass::SetCurrentPage (int page) {
    if (page < 0 || page > MAXSAVEPAGES - 1) return false;
	theApp.theGlobalCfg.initPage = CurrentPage = page;
    // Save current config
    if (SaveCfg (page)) {
        //EEPROM.put (0, CurrentPage);
		theApp.theGlobalCfg.SaveCfg();
        return true;
    }

    DP("Error setting page");
    return false;
}

/**
 * \brief Load Initial Global data
 * 
 * \return true if succesful
 */
bool SaveLoadClass::LoadInitialConfig(void) {
    DP("Load Initial config");
    CurrentPage = 0;
    if(!theApp.theGlobalCfg.LoadCfg()){ // Error loading config, save a new one and two standard pages
		DP("Error loading. Saving std config");
        theApp.initControls();
        if( !SetCurrentPage(0)){
            DP("Error Saving std config");
        }
		return false;
		}
	CurrentPage = theApp.theGlobalCfg.initPage;
	return true;
}


/**
 * \brief Get Initial Page from EEPROM and load it
 * 
 * \return true if succesful
 */
bool SaveLoadClass::LoadInitialPage (void) {
    byte initBank;

	initBank = theApp.theGlobalCfg.initPage;
    //EEPROM.get (0, initBank);
    if (initBank < 0 || initBank > MAXSAVEPAGES - 1){
		return false;
	}
    CurrentPage = initBank;
	if (LoadCfg (initBank)) {
        //CurrentPage = initBank;
        return true;
    } else { // Incorrect Tag Data, Save current config in page
        theApp.initControls();
		SaveCfg(initBank);
	}
    return false;
}

/**
 * \brief Save Config to EEPROM
 * 
 * \param page Valid pages 1 and 2
 * \return true if succesful
 */
bool SaveLoadClass::SaveCfg (int page) {
    uint16_t i;
    int MemPointer;

    // Load address of selected page
    if (page == -1) page = CurrentPage; // No value = Current Bank
    if (page < 0 || page > MAXSAVEPAGES - 1){
        DP("Wrong page when saving");
        return false;
        }

    // Write data
    MemPointer = PageBase[page];
    // Mark the page as containing Cfg data
    EEPROM.put (MemPointer, (uint16_t)CFGDATATAG);
    // MemPointer += sizeof (uint16_t);
	// Reserved PAGEGENERALeeSize for general data
    MemPointer += PAGEGENERALeeSize;

    // Bank data
    for (i = 0; i < 9; i++) {
        MemPointer += CVControls[i].SaveCfg (MemPointer);
    }

    #ifdef PRINTDEBUG
    char outBuf[80];
	sprintf(outBuf, "Saved: %1d/%4lu/%4d", page, PageBase[page], MemPointer);
	Serial.println( outBuf);
    #endif
    #ifdef DEBUGMODE
    char outBuf2[80];
	sprintf(outBuf2, "Saved: %1d/%4lu/%4d", page, PageBase[page], MemPointer);
    SetMessageText (outBuf2);
    #endif

	CurrentPage= page;
	theApp.theGlobalCfg.SetPage(page);
  
    return true;
}

/**
 * \brief Load Config from EEPROM
 * 
 * \param page Memory block to load
 * \return true if succesful
 */
bool SaveLoadClass::LoadCfg (int page) {
    int i;
    uint16_t CfgDataCheck;
    int MemPointer;

    if (page == -1) page = CurrentPage; // No value = Current Bank
    // Load address of selected page
    if (page < 0 || page > MAXSAVEPAGES - 1) return false;
    // Load data index
    MemPointer = PageBase[page];
    // Check if the page contains Cfg data
    EEPROM.get (MemPointer, CfgDataCheck);
    if (CfgDataCheck != CFGDATATAG) {
        D(myMenu.setupPopup ("Incorrect config data", 5000, 0, 17));
        DP("Incorrect config data");
        return false;
    }
    // Reserved PAGEGENERALeeSize for general data
    MemPointer += PAGEGENERALeeSize;// sizeof (uint16_t);

    // Bank data
    for (i = 0; i < 9; i++) {
        MemPointer += CVControls[i].LoadCfg (MemPointer);
    }
	CurrentPage= page;
	theApp.theGlobalCfg.SetPage(page);

#ifdef DEBUGMODE
    char outBuf[80];
	sprintf(outBuf, "Load: %1d/%4lu/%4d", page, PageBase[page], MemPointer);
    SetMessageText (outBuf);
#endif
    return true;
}


/**
 * \brief Load SysEx info to EEPROM
 * 
 * \param page Block to save to
 * \return true if succesful
 */
bool SaveLoadClass::LoadSysExCfg (int page) {
    uint16_t *CfgDataCheck;
    int MemPointer;

    if (page == -1) page = CurrentPage; // No value = Current Bank
    // Load address of selected page
    if (page < 0 || page > MAXSAVEPAGES - 1) return false;
    // Decode and get CheckSum
    unsigned csum = calcCSum ();
    if (!csum || csum != msgCsum) {
        D(myMenu.setupPopup ("No valid SysEx", 5000, 0, 17));
        return false; // CheckSum not OK
    }
    // Check Cfg data
    CfgDataCheck = (uint16_t *)rcvData;
    if (*CfgDataCheck != CFGDATATAG) {
        return false;
    }
    // Load data in EEPROM
    MemPointer = PageBase[page];
    EEPROM.put (MemPointer, rcvData);
    // Load from EEPROM
    return LoadCfg (page);
}


/** @} */
