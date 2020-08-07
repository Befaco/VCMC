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

/** @addtogroup ConfigGroup
 * @{
*/
/**
 *  \file CfgClasses.ino 
  *  \brief Defines Configuration classes functions 
*/

/**
 * \brief Returns the name associated to the port 
 * 
 * \param name string to fill (should be able to contain SIZEPORTNAMES+3 characters) 
 * \return number of characters in string 
 */
uint8_t InputPortCfg::getName(char*name)
{
    uint8_t pos = 0;
    if(textPort == 0 || textPort > 200+NUMUSERNAMES-1){
        *name = 0;
        return 0;
    }
    if( charPrefix){ 
        name[pos] = charPrefix;
        pos++;}
    if(textPort){
        if(textPort>199){
            uint8_t userindex = textPort - 200;
            strcpy( name + pos, theApp.theGlobalCfg.UserNames[userindex]);
            pos += strlen(theApp.theGlobalCfg.UserNames[userindex]);               
        }
        else{
            strncpy( name + pos, PortNames[textPort], SIZEPORTNAMES);
            pos += strlen(PortNames[textPort]);
        }
    }
    if( charSufix){ 
        name[pos] = charSufix;
        pos++;
        }
    name[pos] = 0;
    return pos;
}


// Return min and max values depending on MIDI function
/**
 *  \brief Retreive limit values based on analog port configuration
 *  
 *  \param [in] minv Min value
 *  \param [in] maxv Max value

 */
void AnInputPortCfg::LimitValues (int &minv, int &maxv) {
    switch (MIDIfunction) {
    case ANAGTRIGGER:
    case ANAGLATCH:  
    case PITCHTRIG:
    case PITCH:
	case PITCHLEVEL:
	case PITCH8TRIG:
        if (getInputRange()==MINUSPLUS5V) {
            minv = -128;
            maxv = 256;
        } else if(getInputRange()==ZEROTO5V){
            minv = 0;
            maxv = 60;        
        }else {
            minv = 0;
            maxv = 127;
        }
        break;
    case VELOCITY:
    case CONTROLCHANGE:
    case ANAGCCBUT:
    case ANAGCCLATCH:
    case PROGRAMCHANGE:
    case ANAGCLOCK:
    case ANAGSTARTSTOP:
    case ANAGPAUSECONT:
    case ANAGTRIGSTARTSTOP:
    case ANAGTRIGPAUSECONT:
    case ANAGSTART:  
    case ANAGSTOP:
    case ANAGCONTINUE:
        if (getInputRange()==MINUSPLUS5V) {
            minv = -128;
            maxv = 256;
        } else {
            minv = 0;
            maxv = 127;
        }
        break;
    case CC14BITS:
        minv = 0;
        maxv = 16383;
        break;
    case PITCHBEND:
        minv = MIDI_PITCHBEND_MIN;
        maxv = MIDI_PITCHBEND_MAX;
        break;
    case PERCENT:
        minv = -100;
        maxv = 100;
        break;
    case ANAGFREEVALUE:
        minv = SHRT_MIN;
        maxv = SHRT_MAX;
        break;
	case ANAGNRPN7bits:
		minv = 0;
		maxv = 127;
        break;
	case ANAGNRPN14bits:
		minv = 0;
		maxv = 16383;
        break;
    }
}

/**
 * \brief Check if the config is a digital function
 * 
 * \return boolean 
 */
boolean AnInputPortCfg::IsDigitalFunc(void){
  switch (MIDIfunction) {
    case PITCHTRIG:
    case PITCH:
	case PITCH8TRIG:
    case VELOCITY:
    case CONTROLCHANGE:
    case CC14BITS:
    case PROGRAMCHANGE:
    case PITCHBEND:
    case PERCENT:
    case ANAGFREEVALUE:
	case ANAGNRPN7bits:
	case ANAGNRPN14bits:
        return false;
	case PITCHLEVEL:
    case ANAGCLOCK:
    case ANAGSTARTSTOP:
    case ANAGPAUSECONT:
    case ANAGTRIGSTARTSTOP:
    case ANAGTRIGPAUSECONT:
    case ANAGSTART:  
    case ANAGSTOP:
    case ANAGCONTINUE:
    case ANAGCCBUT:
    case ANAGCCLATCH:
    case ANAGTRIGGER:
    case ANAGLATCH:  
        return true;
	}
    return false;
}

/// Set new MIDI Function
void AnInputPortCfg::SetMIDIFunc (uint8_t Func) {
    MIDIfunction = Func;

    switch (MIDIfunction) {
    case ANAGTRIGGER:
    case ANAGLATCH:  
    case PITCHTRIG:
	case PITCHLEVEL:
	case PITCH8TRIG:
    case PITCH:
        if (getInputRange()==MINUSPLUS5V) { // Set 10 Octaves range (+5/-5 Oct)
        #ifndef CVTHING
            Ranges.SetMIDI (-60, 120);
            ClipLow = -120;//60;
            ClipHigh = 120;//60;
        #else
            Ranges.SetMIDI (0, 120);
            ClipLow = 0;
            ClipHigh = 120;
        #endif
        } else if(getInputRange()==ZEROTO5V){
            Ranges.SetMIDI (0, 60);
            ClipLow = 0;
            ClipHigh = 120;    
        } else { // Set 10 Octaves range
            Ranges.SetMIDI (0, 120);
            ClipLow = 0;
            ClipHigh = 120;
        }
        break;
    case VELOCITY:
    case CONTROLCHANGE:
    case ANAGCCBUT:
    case ANAGCCLATCH:
    case PROGRAMCHANGE:
    case ANAGCLOCK:
    case ANAGSTARTSTOP:
    case ANAGPAUSECONT:
    case ANAGTRIGSTARTSTOP:
    case ANAGTRIGPAUSECONT:
    case ANAGSTART:  
    case ANAGSTOP:
    case ANAGCONTINUE:
        if (getInputRange()==MINUSPLUS5V) { // Set 10 Octaves range (+5/-5 Oct)
        #ifndef CVTHING
            Ranges.SetMIDI (0, 127);
            ClipLow = -127;//64;
            ClipHigh = 127;//64;
        #else
            Ranges.SetMIDI (0, 127);
            ClipLow = 0;
            ClipHigh = 127;
        #endif
        } else { // Set standard MIDI range 0 to 127
            Ranges.SetMIDI (0, 127);
            ClipLow = 0;
            ClipHigh = 127;
        }
        break;
    case CC14BITS:
        // Set extended 14 bits MIDI range 0 to MIDI_PITCHBEND_MAX
        Ranges.SetMIDI (0, 16383);
        ClipLow = 0;
        ClipHigh = 16383;
        break;
    case PITCHBEND:
        // Set extended Pitch Bend MIDI range -MIDI_PITCHBEND_MIN to MIDI_PITCHBEND_MAX
        Ranges.SetMIDI (MIDI_PITCHBEND_MIN, MIDI_PITCHBEND_MAX - MIDI_PITCHBEND_MIN);
        ClipLow = MIDI_PITCHBEND_MIN;
        ClipHigh = MIDI_PITCHBEND_MAX;
        break;
    case PERCENT:
        // Set to percentage range -100 to 100
        Ranges.SetMIDI (0, 100);
        ClipLow = 0;
        ClipHigh = 100;
        break;
    case ANAGFREEVALUE:
        Ranges.SetMIDI (SHRT_MIN, SHRT_MAX);
        ClipLow = SHRT_MIN;
        ClipHigh = SHRT_MAX;
        break;
	case ANAGNRPN7bits:
            Ranges.SetMIDI (0, 127);
            ClipLow = 0;
            ClipHigh = 127;
        break;
	case ANAGNRPN14bits:
            Ranges.SetMIDI (0, 16383);
            ClipLow = 0;
            ClipHigh = 16383;
        break;    
	}
}

/// Set new MIDI Function
void DigPortCfg::SetMIDIFunc (uint8_t Func) { MIDIfunction = Func; }
/*
/// Return the global config minimum DAC value
int32_t getInitMinDAC () { return theApp.theGlobalCfg.InitMinDAC; }
/// Return the global config minimum DAC range
int32_t getInitRangeDAC () { return theApp.theGlobalCfg.InitRangeDAC; }
*/
/**
 *  \brief Save Page  to EEPROM
 *
 *  \param [in] page to save
 *
 *  \details 
 */
int GlobalCfg::SetPage(int page){
    int MemPointer = 0;//addr;
	initPage = page;
	
    // Mark the mem as containing Cfg data
    EEPROM.put (MemPointer, (uint16_t)CFGDATATAG);
    MemPointer += sizeof (uint16_t);    
	EEPROM.put (MemPointer, initPage);
    MemPointer += sizeof (initPage);

    return MemPointer;
}


/**
 *  \brief Save Control configuration to EEPROM
 *
 *  \param [in] addr address to save to
 *  \return number of bytes written
 *
 *  \details Will save Global config
 */
int GlobalCfg::SaveCfg (/*int addr*/) 
{
    int MemPointer = 0;//addr;

    // Mark the mem as containing Cfg data
    MemPointer += SetPage(initPage);
	
    EEPROM.put (MemPointer, InitMinDAC);
    MemPointer += sizeof (InitMinDAC);
    EEPROM.put (MemPointer, InitRangeDAC);
    MemPointer += sizeof (InitRangeDAC);

    EEPROM.put (MemPointer, FaderMinDAC);
    MemPointer += sizeof (FaderMinDAC);
    EEPROM.put (MemPointer, FaderRangeDAC);
    MemPointer += sizeof (FaderRangeDAC);

    EEPROM.put (MemPointer, AuxAMinDAC);
    MemPointer += sizeof (AuxAMinDAC);
    EEPROM.put (MemPointer, AuxARangeDAC);
    MemPointer += sizeof (AuxARangeDAC);

    EEPROM.put (MemPointer, AuxBMinDAC);
    MemPointer += sizeof (AuxBMinDAC);
    EEPROM.put (MemPointer, AuxBRangeDAC);
    MemPointer += sizeof (AuxBRangeDAC);

    EEPROM.put (MemPointer, UserNames);
    MemPointer += sizeof (UserNames);

    EEPROM.put (MemPointer, ClockDivider);
    MemPointer += sizeof (ClockDivider);
    EEPROM.put (MemPointer, ClockShift);
    MemPointer += sizeof (ClockShift);


#ifdef PRINTDEBUG
	Serial.print( "Saved Global ");
	Serial.print( initPage);
	Serial.print( ": ");
	Serial.print( InitMinDAC);
	Serial.print( "/");
	Serial.print( InitRangeDAC);
	Serial.print( "/");	
	Serial.print( FaderMinDAC);
	Serial.print( "/");
	Serial.print( FaderRangeDAC);
	Serial.print( "/");	
	Serial.print( AuxAMinDAC);
	Serial.print( "/");
	Serial.print( AuxARangeDAC);
	Serial.print( "/");	
	Serial.print( AuxBMinDAC);
	Serial.print( "/");
	Serial.print( AuxBRangeDAC);
	Serial.print( "/");	
    Serial.print( ClockDivider);
	Serial.print( "/");
	Serial.print( ClockShift);
	Serial.print( "/");
    Serial.println( MemPointer);
#endif
#ifdef USECONFIGOSC
	char msgTxt[120];
    sprintf (msgTxt, "/VCMC/Config/Global");
    SaveCfgOSC (msgTxt);
#endif
    return GLOBALeeSize;//MemPointer - addr;
}


/**
 *  \brief Load Control configuration from EEPROM
 *
 *  \param [in] addr address to load from
 *  \return number of bytes read
 *
 *  \details Will load Global config
 */
int GlobalCfg::LoadCfg (/*int addr*/) {
    int MemPointer = 0;//addr;
	uint16_t CfgDataCheck;

    EEPROM.get (MemPointer, CfgDataCheck);
    if (CfgDataCheck != CFGDATATAG) {
        //myMenu.setupPopup ("Incorrect config data", 5000, 0, 17);
		DP( "Error Loading Global Config");
        return 0;
    }
	MemPointer += sizeof (uint16_t);  

    EEPROM.get (MemPointer, initPage);
    if (initPage < 0 || initPage > MAXSAVEPAGES - 1){
		initPage=-1;
		//return 0;
		}
    MemPointer += sizeof (initPage);

    EEPROM.get (MemPointer, InitMinDAC);
    MemPointer += sizeof (InitMinDAC);
    EEPROM.get (MemPointer, InitRangeDAC);
    MemPointer += sizeof (InitRangeDAC);

    EEPROM.get (MemPointer, FaderMinDAC);
    MemPointer += sizeof (FaderMinDAC);
    EEPROM.get (MemPointer, FaderRangeDAC);
    MemPointer += sizeof (FaderRangeDAC);

    EEPROM.get (MemPointer, AuxAMinDAC);
    MemPointer += sizeof (AuxAMinDAC);
    EEPROM.get (MemPointer, AuxARangeDAC);
    MemPointer += sizeof (AuxARangeDAC);

    EEPROM.get (MemPointer, AuxBMinDAC);
    MemPointer += sizeof (AuxBMinDAC);
    EEPROM.get (MemPointer, AuxBRangeDAC);
    MemPointer += sizeof (AuxBRangeDAC);
 
    EEPROM.get (MemPointer, UserNames);
    MemPointer += sizeof (UserNames);

    EEPROM.get (MemPointer, ClockDivider);
    MemPointer += sizeof (ClockDivider);
    EEPROM.get (MemPointer, ClockShift);
    MemPointer += sizeof (ClockShift);

#ifdef PRINTDEBUG
	Serial.print( "Load Global ");
	Serial.print( initPage);
	Serial.print( ": ");
	Serial.print( InitMinDAC);
	Serial.print( "/");
	Serial.print( InitRangeDAC);
	Serial.print( "/");	
	Serial.print( FaderMinDAC);
	Serial.print( "/");
	Serial.print( FaderRangeDAC);
	Serial.print( "/");	
	Serial.print( AuxAMinDAC);
	Serial.print( "/");
	Serial.print( AuxARangeDAC);
	Serial.print( "/");	
	Serial.print( AuxBMinDAC);
	Serial.print( "/");
	Serial.print( AuxBRangeDAC);
	Serial.print( "/");
    Serial.print( ClockDivider);
	Serial.print( "/");
	Serial.print( ClockShift);
	Serial.print( "/");
    Serial.println( MemPointer);
#endif 
	return GLOBALeeSize;//MemPointer - addr;
}



/**@}*/

