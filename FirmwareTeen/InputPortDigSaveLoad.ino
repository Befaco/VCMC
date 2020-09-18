
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
 *  \file InputPortDigSaveLoad.ino
 *  \brief Definition of functions related to Digital input ports Save and Load
 */



//////////////////////////////////
// Save / Load functions


/**
 *  \brief Save Port configuration to EEPROM. 
 *  
 *  \param [in] addr address to store the data 
 *  \return number of bytes written to memory 
 */
int DigitalPort::SaveCfg (int addr) {

    uint8_t buf[sizeof(PortCfg)];
    int lenMem = fill(DUMPGENERAL, buf, sizeof(PortCfg));
    eeprom_write_block(buf, (void *)addr, lenMem);

    //int MemPointer = addr;
    //EEPROM.put (MemPointer, PortCfg);
    //MemPointer += sizeof (PortCfg);
    #ifdef PRINTDEBUG
	Serial.print( "Saved Digital ");
	Serial.print( PortNumber);
	Serial.print( ": ");
	Serial.print( addr);
	Serial.print( "/");
	Serial.print( lenMem);
	Serial.print( "/");
	Serial.println( DIGITALeeSize);
    #endif
    return DIGITALeeSize;//MemPointer - addr;
}


/**
 *  \brief Load Port configuration from EEPROM. 
 *  
 *  \param [in] addr address to read data from
 *  \return number of bytes read from memory 
 */
int DigitalPort::LoadCfg (int addr) {

    uint8_t buf[sizeof(PortCfg)];
    eeprom_read_block(buf, (const void *)addr, sizeof(PortCfg));
    parse(DUMPGENERAL, buf, sizeof(PortCfg));
    //int MemPointer = addr;
    //EEPROM.get (MemPointer, PortCfg);
    //MemPointer += sizeof (PortCfg);

    return DIGITALeeSize; //MemPointer - addr;
}


/**
 * \brief fill the class with information from buffer
 * 
 * \param type Type of transfer (See @ref VCMCSysExType)
 * \param buf Pointer to content
 * \param buLen Max length
 * \return int bytes read
 */
int DigitalPort::parse (uint8_t type, uint8_t *buf, int buLen) {
    int Initbuf = (int)buf;
    int cont = sizeof(DATA_Digital)/sizeof(SysExDataStruct);

    switch (type)
    {
    case DUMPGENERAL: // Dump directly buffer to class
        for (int i = 1; i < cont;i++){
            buf += parse(DATA_Digital[i].ParID, buf, 
                        sizeTypes[DATA_Digital[i].Type]);
            //D(Serial.printf("Digital Par %2d mem %d/%1d\n", DATA_Digital[i].ParID,
            //                buf, sizeTypes[DATA_Digital[i].Type]));
        }
        return (int)buf-Initbuf;
        //LentoCopy = sizeof(PortCfg);
        //memcpy(&PortCfg, buf, LentoCopy);
        //break;
    case PAR_GateMIDIChannel:
        PortCfg.MIDIChannel = *buf;
        return buLen;
    case PAR_GatePortName:
        PortCfg.textPort = *buf;
        return buLen;
    case PAR_GateNameSufix:
        PortCfg.charSufix = *buf;
        return buLen;
    case PAR_GateDelayGate:
        PortCfg.DelayGate = *buf;
        return buLen;
    case PAR_GateFunction:
        PortCfg.MIDIfunction = *buf;
        return buLen;
    case PAR_GateOptionsI2C:
        PortCfg.I2COptionsInputPort = *((uint32_t*)buf);
        return buLen;
    case PAR_GateFunctionData:
        return parseFunctionData( buf, buLen);
    default:
        return 0;
    }
    return 0;
}


int DigitalPort::parseFunctionData(uint8_t *buf, int buLen){

    if(buLen<2){
        DP("Incorrect Length parsing parameter");
        return 0;}

    switch(PortCfg.MIDIfunction){
        case TRIGGER:
        case LATCH:
            PortCfg.NoteToSend = *buf;
            return buLen;
        case CCBUT:
        case CCLATCH:
            PortCfg.ControllerNumber = buf[0];
            PortCfg.ControllerValue = buf[1];
            return buLen;
        case GATECLOCK:
            // get clock info from General config
            PortCfg.ClockDivider = theApp.theGlobalCfg.ClockDivider;
            PortCfg.ClockShift = theApp.theGlobalCfg.ClockShift;
            return buLen;
        case GATESTARTSTOP:
        case GATEPAUSECONT:
        case GATETRIGSTARTSTOP:
        case GATETRIGPAUSECONT:
        case GATESTART:
        case GATESTOP:
        case GATECONTINUE:
        case GATEPANIC:
        case NODIGFUNC:
        case GATE8TRIG:        
        default:
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
int DigitalPort::fill (uint8_t type, uint8_t *buf, int buLen) {
    int Initbuf = (int)buf;
    int cont = sizeof(DATA_Digital)/sizeof(SysExDataStruct);

    switch (type)
    {
    case DUMPGENERAL: // Dump directly buffer to class
        for (int i = 1; i < cont;i++){
            buf += fill(DATA_Digital[i].ParID, buf, 
                        sizeTypes[DATA_Digital[i].Type]);
            //D(Serial.printf("Digital Par %2d mem %d/%1d\n", DATA_Digital[i].ParID,
            //                buf, sizeTypes[DATA_Digital[i].Type]));
        }
        return (int)buf-Initbuf;
        //LentoCopy = sizeof(PortCfg);
        //memcpy( buf, &PortCfg, LentoCopy);
        break;
    case PAR_GateMIDIChannel:
        *buf = PortCfg.MIDIChannel;
        return buLen;
    case PAR_GatePortName:
        *buf = PortCfg.textPort;
        return buLen;
    case PAR_GateNameSufix:
        *buf = PortCfg.charSufix;
        return buLen;
    case PAR_GateDelayGate:
        *buf = PortCfg.DelayGate;
        return buLen;
    case PAR_GateFunction:
        *buf = PortCfg.MIDIfunction;
        return buLen;
    case PAR_GateOptionsI2C:
        *((uint32_t*)buf) = PortCfg.I2COptionsInputPort;
        return buLen;
    case PAR_GateFunctionData:
        return fillFunctionData( buf, buLen);
    default:
        D(Serial.printf("Parameter Not recognized %d\n", type));
        return 0;
    }
    return 0;
}


int DigitalPort::fillFunctionData(uint8_t *buf, int buLen){

    if(buLen<2){
        D(Serial.printf("Incorrect Length parsing parameter %d\n", buLen));
        return 0;}

    switch(PortCfg.MIDIfunction){
        case TRIGGER:
        case LATCH:
            *buf = PortCfg.NoteToSend;
            return buLen;
        case CCBUT:
        case CCLATCH:
            buf[0] = PortCfg.ControllerNumber;
            buf[1] = PortCfg.ControllerValue;
            return buLen;
        case GATECLOCK:
            //  Do nothing, clock info should already be on General config
        case GATESTARTSTOP:
        case GATEPAUSECONT:
        case GATETRIGSTARTSTOP:
        case GATETRIGPAUSECONT:
        case GATESTART:
        case GATESTOP:
        case GATECONTINUE:
        case GATEPANIC:
        case NODIGFUNC:
        case GATE8TRIG:
        default:
            return buLen;
    }
    return 0;
}



/** @} */