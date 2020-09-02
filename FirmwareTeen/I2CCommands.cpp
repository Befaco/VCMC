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


/** @addtogroup I2C 
 *  @{
 */

#ifdef USEI2C

void I2Cmerger::InitDefControls(void)
{
    CVControls[0].Config.CommI2C = E_OP_JF_NOTE; // First Bank sends Note ON/Off
    for (size_t i = 1; i < 7; i++)
    {
        CVControls[i].GateBut.PortCfg.CommI2C = E_OP_JF_TR;
        CVControls[i].GateBut.PortCfg.I2CChannel = i;
        CVControls[i].Slider.PortCfg.CommI2C = E_OP_JF_VOX;
        CVControls[i].Slider.PortCfg.I2CChannel = i;
    }
    
}


void AnalogPort::SendI2C (int MidiData, bool GateStat)
{
command_state_t Notedata;

    switch(PortCfg.CommI2C){
        case E_NOI2CFUNC:
            return;
        case E_OP_JF_VOX:
            // Convert 14 to 16 bits
            Notedata.push(0x1FF); // Send other velocity instead?
            Notedata.push(PortValue<<2); // Send MidiData instead?
            Notedata.push(PortCfg.I2CChannel); // Selected channels
            theApp.I2CMerge.callOP(&op_JF_VOX, &Notedata);
        break;
    }
    return;
}




void DigitalPort::SendI2C (int MidiData, bool GateStat)
{
command_state_t GateData;

    // Send Trigger
    switch(PortCfg.CommI2C){
        case E_NOI2CFUNC:
            return;
        case E_OP_JF_TR:
            GateData.push(GateStatus); // High/Low
            GateData.push(PortCfg.I2CChannel); // Selected channels
            theApp.I2CMerge.callOP(&op_JF_TR, &GateData);
            break;
    }
    return;
}

#endif


void InputControl::sendNoteOn(byte Note, byte Veloc, byte Chann){
    int16_t chan = 0;
    MidiMerge.sendNoteOn(Note, Veloc, Chann);

    #ifdef USEI2C
    // Review other conditions to send Note On / Off
    if( Config.CommI2C == E_NOI2CFUNC && GateBut.PortCfg.CommI2C == E_NOI2CFUNC ){
        return;
    }

    // Send Note On/Off as Note plus velocity
    command_state_t Notedata;
    // Convert 14 to 16 bits
    Notedata.push(Veloc * 0x3FF / 127.);//Slider.PortValue<<2);
    Notedata.push(Note * 0x3FF / 120.);//(~CVPort.PortValue)<<2);
    theApp.I2CMerge.callOP(&op_JF_NOTE, &Notedata);

    // Send Trigger
    command_state_t GateData;
    GateData.push(1); // High
    GateData.push(0); // All channels
    theApp.I2CMerge.callOP(&op_JF_TR, &GateData);
    #endif
}


void InputControl::sendNoteOff(byte Note, byte Veloc, byte Chann){
    MidiMerge.sendNoteOff(Note, Veloc, Chann);

    #ifdef USEI2C
    if( Config.CommI2C == E_NOI2CFUNC && GateBut.PortCfg.CommI2C == E_NOI2CFUNC ){
        return;
    }
    
    // Send Trigger
    command_state_t GateData;
    GateData.push(0); // Low
    GateData.push(0); // All channels
    theApp.I2CMerge.callOP(&op_JF_TR, &GateData);
    #endif
}

/** @} */
