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

/** @addtogroup InputPortsGroup
* @{
*/

/**
 *  \file InputPortAnagSaveLoad.ino
 *  \brief Definition of functions related to Analog (CV and Fader) Save/Load
 */

//////////////////////////////////
// Save / Load functions

/**
 *  \brief Save Port configuration to EEPROM. 
 *  
 *  \param [in] addr address to store the data 
 *  \return number of bytes written to memory
 *  
 */
int AnalogPort::SaveCfg (int addr) {
    
    uint8_t buf[sizeof(PortCfg)];
    int lenMem = fill(DUMPGENERAL, buf, sizeof(PortCfg));
    eeprom_write_block(buf, (void *)addr, lenMem);
    int fixLen = (typeSlider) ? ANALOGFADeeSize : ANALOGeeSize;
    /* int MemPointer = addr;
    EEPROM.put (MemPointer, PortCfg);
    MemPointer += sizeof (PortCfg);
    lenMem = MemPointer-addr */

    #ifdef PRINTDEBUG
	Serial.print( "Saved Analog ");
	Serial.print( PortNumber);
	Serial.print( ": ");
	Serial.print( addr);
	Serial.print( "/");
	Serial.print( lenMem);
	Serial.print( "/");
	Serial.println( fixLen);
    #endif
    return fixLen;//MemPointer - addr;
}

/**
 *  \brief Load Port configuration from EEPROM. 
 *  
 *  \param [in] addr address to read data from
 *  \return number of bytes read from memory 
 */
int AnalogPort::LoadCfg (int addr) {

    uint8_t buf[sizeof(PortCfg)];
    eeprom_read_block(buf, (const void *)addr, sizeof(PortCfg));
    parse(DUMPGENERAL, buf, sizeof(PortCfg));
    int fixLen = (typeSlider) ? ANALOGFADeeSize : ANALOGeeSize;

    /* int MemPointer = addr;
    EEPROM.get (MemPointer, PortCfg);
    MemPointer += sizeof (PortCfg); */

    return fixLen;
}



/**
 * \brief fill the class with information from buffer
 * 
 * \param type Type of transfer (See @ref VCMCSysExType)
 * \param buf Pointer to content
 * \param buLen Max length
 * \return int bytes read
 */
int AnalogPort::parse (uint8_t type, uint8_t *buf, int buLen) {
    int Initbuf = (int)buf;
    int cont;
    SysExDataStruct *pDataStr;
    int16_t minMIDI, rMIDI;
    int32_t minDAC, rDAC;

    if(typeSlider){
        cont = sizeof(DATA_Fader) / sizeof(SysExDataStruct);
        pDataStr = DATA_Fader;
    }else{
        cont = sizeof(DATA_Analog) / sizeof(SysExDataStruct);
        pDataStr = DATA_Analog;
    }
    switch (type)
    {
    case DUMPGENERAL: // Dump directly buffer to class
        for (int i = 1; i < cont;i++){
            buf += parse(pDataStr[i].ParID, buf, 
                        sizeTypes[pDataStr[i].Type]);
            //D(Serial.printf("Digital Par %2d mem %d/%1d\n", pDataStr[i].ParID,
            //                buf, sizeTypes[pDataStr[i].Type]));
        }
        return (int)buf-Initbuf;
        //LentoCopy = sizeof(PortCfg);
        //memcpy(&PortCfg, buf, LentoCopy);
        //break;
    case PAR_MIDIChannel:
        PortCfg.MIDIChannel = *buf;
        return buLen;
    case PAR_PortName:
        PortCfg.textPort = *buf;
        return buLen;
    case PAR_NameSufix:
        PortCfg.charSufix = *buf;
        return buLen;
    case PAR_DelayGate:
        PortCfg.DelayGate = *((uint16_t*)buf);
        return buLen;
    case PAR_Function:
        PortCfg.MIDIfunction = *buf;
        return buLen;
    case PAR_Range_minMIDI:
        PortCfg.Ranges.getMIDI(minMIDI, rMIDI);
        PortCfg.Ranges.SetMIDI(*((int16_t *)buf),rMIDI);
        return buLen;
    case PAR_Range_rangeMIDI:
        PortCfg.Ranges.getMIDI(minMIDI, rMIDI);
        PortCfg.Ranges.SetMIDI(minMIDI,*((int16_t *)buf));
        return buLen;
    case PAR_Range_minDAC:
        PortCfg.Ranges.getDAC(minDAC, rDAC);
        PortCfg.Ranges.SetDAC(*((int32_t *)buf),rDAC);
        return buLen;
    case PAR_Range_rangeDAC:
        PortCfg.Ranges.getDAC(minDAC, rDAC);
        PortCfg.Ranges.SetDAC(minDAC, *((int32_t *)buf));
        return buLen;
    case PAR_ClipLow:
        PortCfg.ClipLow = *((int16_t*)buf);
        return buLen;
    case PAR_ClipHigh:
        PortCfg.ClipHigh = *((int16_t*)buf);
        return buLen;
    case PAR_FunctionData:
        return parseFunctionData( buf, buLen);
    case PAR_Option1:
        PortCfg.Options1 = *buf;
    default:
        return 0;
    }
    return 0;
}


int AnalogPort::parseFunctionData(uint8_t *buf, int buLen){

    if(buLen<2){
        DP("Incorrect Length parsing parameter");
        return 0;}

    switch(PortCfg.MIDIfunction){
        case PITCH: // Dude mode
            PortCfg.AutoOff = *((uint16_t *)buf);
            return buLen;
        case CC14BITS:
        case CONTROLCHANGE:
            PortCfg.ControllerNumber = *buf;
            return buLen;
        case ANAGCLOCK:
            // get clock info from General config
            PortCfg.ClockDivider = theApp.theGlobalCfg.ClockDivider;
            PortCfg.ClockShift = theApp.theGlobalCfg.ClockShift;
            return buLen;
        case ANAGCCBUT:
        case ANAGCCLATCH:
            PortCfg.ControllerNumber = buf[0];
            PortCfg.ControllerValue = buf[1];
            return buLen;
        case ANAGLATCH:
        case ANAGTRIGGER:
            PortCfg.NoteToSend = *buf;
            return buLen;
        case ANAGNRPN7bits:
        case ANAGNRPN14bits:
            PortCfg.NRPNparLSB = buf[0];
            PortCfg.NRPNparMSB = buf[1];
            return buLen;
        case PITCHTRIG:
        case VELOCITY:
        case PROGRAMCHANGE:
        case PITCHBEND:
        case PERCENT:
        case ANAGSTARTSTOP:
        case ANAGPAUSECONT:
        case ANAGTRIGSTARTSTOP:
        case ANAGTRIGPAUSECONT:
        case ANAGFREEVALUE:
        case ANAGSTART:
        case ANAGSTOP:
        case ANAGCONTINUE:
        case NOANFFUNC:
        case PITCHLEVEL:
        case PITCH8TRIG:
            return buLen;
    }
    return 0;
}

/**
 * \brief fill the buffer with information from class
 * 
 * \param type Type of transfer (See @ref VCMCSysExType)
 * \param buf Pointer to content
 * \param buLen Max length
 * \return int bytes read
 */
int AnalogPort::fill (uint8_t type, uint8_t *buf, int buLen) {
    int Initbuf = (int)buf;
    int cont;
    SysExDataStruct *pDataStr;
    int16_t minMIDI, rMIDI;
    int32_t minDAC, rDAC;

    if(typeSlider){
        cont = sizeof(DATA_Fader) / sizeof(SysExDataStruct);
        pDataStr = DATA_Fader;
    }else{
        cont = sizeof(DATA_Analog) / sizeof(SysExDataStruct);
        pDataStr = DATA_Analog;
    }
    switch (type)
    {
    case DUMPGENERAL: // Dump directly buffer to class
        for (int i = 1; i < cont;i++){
            buf += fill(pDataStr[i].ParID, buf, 
                        sizeTypes[pDataStr[i].Type]);
            //D(Serial.printf("Analog Par %2d mem %d/%1d\n", pDataStr[i].ParID,
            //                buf, sizeTypes[pDataStr[i].Type]));
        }
        return (int)buf-Initbuf;
        //LentoCopy = sizeof(PortCfg);
        //memcpy(&PortCfg, buf, LentoCopy);
        //break;
    case PAR_MIDIChannel:
        *buf = PortCfg.MIDIChannel;
        return buLen;
    case PAR_PortName:
        *buf = PortCfg.textPort;
        return buLen;
    case PAR_NameSufix:
        *buf = PortCfg.charSufix;
        return buLen;
    case PAR_DelayGate:
        *((uint16_t*)buf) = PortCfg.DelayGate;
        return buLen;
    case PAR_Function:
        *buf = PortCfg.MIDIfunction;
        return buLen;
    case PAR_Range_minMIDI:
        PortCfg.Ranges.getMIDI(minMIDI, rMIDI);
        *((int16_t *)buf) = minMIDI;
        return buLen;
    case PAR_Range_rangeMIDI:
        PortCfg.Ranges.getMIDI(minMIDI, rMIDI);
        *((int16_t *)buf) = rMIDI;
        return buLen;
    case PAR_Range_minDAC:
        PortCfg.Ranges.getDAC(minDAC, rDAC);
        *((int32_t *)buf) = minDAC;
        return buLen;
    case PAR_Range_rangeDAC:
        PortCfg.Ranges.getDAC(minDAC, rDAC);
        *((int32_t *)buf) = rDAC;
        return buLen;
    case PAR_ClipLow:
        *((int16_t*)buf) = PortCfg.ClipLow;
        return buLen;
    case PAR_ClipHigh:
        *((int16_t*)buf) = PortCfg.ClipHigh;
        return buLen;
    case PAR_FunctionData:
        return fillFunctionData( buf, buLen);
    case PAR_Option1:
        *buf = PortCfg.Options1;
    default:
        return 0;
    }
    return 0;
}


int AnalogPort::fillFunctionData(uint8_t *buf, int buLen){

    if(buLen<2){
        DP("Incorrect Length filling parameter");
        return 0;}

    switch(PortCfg.MIDIfunction){
        case PITCH: // Dude mode
            *((uint16_t *)buf) = PortCfg.AutoOff;
            return buLen;
        case CC14BITS:
        case CONTROLCHANGE:
            *buf = PortCfg.ControllerNumber;
            return buLen;
        case ANAGCCBUT:
        case ANAGCCLATCH:
            buf[0] = PortCfg.ControllerNumber;
            buf[1] = PortCfg.ControllerValue;
            return buLen;
        case ANAGLATCH:
        case ANAGTRIGGER:
            *buf = PortCfg.NoteToSend;
            return buLen;
        case ANAGNRPN7bits:
        case ANAGNRPN14bits:
            buf[0] = PortCfg.NRPNparLSB;
            buf[1] = PortCfg.NRPNparMSB;
            return buLen;
        case ANAGCLOCK:
            //  Do nothing, clock info should already be on General config
        case PITCHTRIG:
        case VELOCITY:
        case PROGRAMCHANGE:
        case PITCHBEND:
        case PERCENT:
        case ANAGSTARTSTOP:
        case ANAGPAUSECONT:
        case ANAGTRIGSTARTSTOP:
        case ANAGTRIGPAUSECONT:
        case ANAGFREEVALUE:
        case ANAGSTART:
        case ANAGSTOP:
        case ANAGCONTINUE:
        case NOANFFUNC:
        case PITCHLEVEL:
        case PITCH8TRIG:
            return buLen;
    }
    return 0;
}

/** @} */
