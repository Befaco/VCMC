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
/** @addtogroup GeneralUtil Save SysEx
 *  @{
 */

/**
 *  \file SaveLoad.h
 *  \brief Configuration for Save/Load support
 *  
 *  Memory format:
 *  Pos		        Size	        Comment
 *  0		        2		        Config Data Marker
 *  2		        1		        Last saved page. Used for auto loading at start up
 *  ...		        		        DAC init values for CV, Fader and Aux
 *  150		        PageSize        First Page first byte
 *  150+PageSize*i	PageSize		Second Page first byte
 *
 *  Page format: (970 bytes)
 *  Pos		Size	Comment
 *  0		2		Config Data Marker
 *  16		BANKeeSize		Bank 1
 *  16+		BANKeeSize		Bank 2
 *  ...
 *  16+*7		BANKeeSize		Bank 8
 *  16+*8		BANKeeSize		Bank Aux
 *  
 *  Bank format: (BANKeeSize bytes)
 *  Pos Size                Comment
 *  0   BANKGENERALeeSize   Bank Function
 *  +   DIGITALeeSize       Gate Config
 *  +	ANALOGeeSize		CV Config
 *  +	ANALOGeeSize		Fader Config
 */
#ifndef __SAVELOAD_H
#define __SAVELOAD_H

#define VERSAVE 16 // Change version when saved data version changes
#define CFGDATATAG \
    9898 + VERSAVE // Pre-defined value to check if data in EEPROM is valid Configuration data
// EEPROM Save/load config
#define GLOBALeeSize 150
#define DIGITALeeSize 8 // min 7
#define ANALOGeeSize 24  // min 23
#define ANALOGFADeeSize 14  // min 13
#define BANKGENERALeeSize 2
#define BANKeeSize (BANKGENERALeeSize+DIGITALeeSize+ANALOGeeSize+ANALOGFADeeSize)  //48 bytes min 45
#define PAGEGENERALeeSize 4 // min 2

#define PageSize \
    (PAGEGENERALeeSize+BANKeeSize*9+ANALOGeeSize-ANALOGFADeeSize) 
// Calculate page size: 4+48*8+2+8+24+24=446 
#define PAGEBASEADDR GLOBALeeSize // Start position for first bank
#define MAXSAVEPAGES 4

/// Save and Load to EEPROM and SysEx
class SaveLoadClass {
    public:
    uint32_t PageBase[8];
    byte CurrentPage = 0;
    byte rcvSysEx[PageSize * 8 / 7 + 8]; // Memory for encoded messages received via SysEx
    byte rcvData[PageSize]; // Memory for decoded messages received via SysEx
    unsigned nDecoded = 0; // Number of decoded bytes
    uint8_t nPacketsRec = 0;
    unsigned long ulPacketID = 0;
    unsigned long nBytesRec = 0;
    uint8_t PacketsRec[50];
    byte nPacketsTotal = 0;
    byte msgCsum;

    SaveLoadClass ();
    bool SaveCfg (int page = -1);
    bool LoadCfg (int page = -1);
    bool SetCurrentPage (int page);
    bool LoadInitialPage (void);
    bool LoadInitialConfig (void);
	
    bool LoadSysExCfg (int page = -1);

    bool SaveCfgSysEx (int page = -1);
    bool SaveCfgSingleSysEx(byte *pSource, uint16_t uLengthSource, byte bSlot, byte bType);
    bool SaveCfgSysEx (byte *pSource, uint16_t uLengthSource);
    bool ReceivedSysEx (unsigned SysExLength, const byte *RecArray);
    bool ReceivedSingleSysEx(unsigned SysExLength, const byte *RecArray);
    void ResetReception(void)
    {
        ulPacketID = 0;
        nPacketsRec = 0;
        nBytesRec = 0;
        nPacketsTotal = 0;
        msgCsum = 0;
        nDecoded = 0;
        for (unsigned i = 0; i < 50; i++) PacketsRec[i] = 0;
    }
    bool IsSysExRecComplete (void) {
        return (nPacketsRec != 0 && nPacketsRec == nPacketsTotal) ? true : false;
    }
    bool decodeSysEx (void) {
        if (!IsSysExRecComplete ()) return false;
        nDecoded = midi::decodeSysEx ((const byte *)rcvSysEx + 2, rcvData, nBytesRec - 2);
        return true;
    }
    byte calcCSum (void) {
        if (!decodeSysEx ()) return 0; // Error
        byte csum = 0;
        csum = rcvData[0];
        for (unsigned i = 1; i < nDecoded; i++) csum ^= rcvData[i];
        csum &= 0x7F; // Strip to MIDI standard
        return csum;
    }
    //	bool SaveCfgSysEx(int page);
    //	bool LoadCfgSysEx(int page);
};

#endif /* __SAVELOAD_H */

/** @} */
