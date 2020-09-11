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

const tele_op_t *I2Cmerger::getTeleOp(uint16_t Comm){
    return tele_ops[Comm];
}

bool I2Cmerger::TeleOpUseChanInfo(uint16_t Comm){
    if(Comm>=E_OP_JF_TR && Comm<=E_OP_JF_QT)
        return tele_opsJFChan[Comm - E_OP_JF_TR];

    return true;
}

void AnalogPort::SendI2C (int MidiData, bool GateStat)
{
    int16_t toSend = 0;
    if(PortCfg.CommI2C==E_NOI2CFUNC)
        return;

    if(!PortCfg.UseMIDII2C){ // Raw data config
        toSend = (typeSlider) ? PortValue<<2 : (~PortValue)<<2;
    } else{
        if( PortCfg.MIDIfunction == CC14BITS ||
                PortCfg.MIDIfunction == PITCHBEND || 
                PortCfg.MIDIfunction == ANAGNRPN14bits)
            toSend = MidiData << 2; // 14 bits MIDI data
        else if( PortCfg.MIDIfunction == PITCHTRIG || 
                    PortCfg.MIDIfunction == PITCH8TRIG)
            toSend = MidiData / 120. * 0x3FF; // V/Oct MIDI data
        else 
            toSend = MidiData / 127. * 0x3FF; // 7 bits MIDI data
    }
    
    command_state_t Notedata;
    // Convert 14 to 16 bits
    Notedata.push(0x1FF);              // Send other velocity instead?
    Notedata.push(toSend);     // Send MidiData instead?
    if( theApp.I2CMerge.TeleOpUseChanInfo(PortCfg.CommI2C))
        Notedata.push(PortCfg.I2CChannel); // Selected channels

    theApp.I2CMerge.callOP(PortCfg.CommI2C, &Notedata);

    return;
}




void DigitalPort::SendI2C (int MidiData, bool GateStat)
{
command_state_t GateData;

    if(PortCfg.CommI2C==E_NOI2CFUNC)
        return;
    GateData.push(GateStatus); // High/Low
    if( theApp.I2CMerge.TeleOpUseChanInfo(PortCfg.CommI2C))
        GateData.push(PortCfg.I2CChannel); // Selected channels
    theApp.I2CMerge.callOP(PortCfg.CommI2C, &GateData);

    return;
}

#endif


void InputControl::sendNoteOn(byte Note, byte Veloc, byte Chann){
    MidiMerge.sendNoteOn(Note, Veloc, Chann);

    #ifdef USEI2C
    if(!Config.UseMIDII2C || (Config.CommI2C == E_NOI2CFUNC && !GateBut.PortCfg.UseMIDII2C))
        return; // Send only on MIDI mode
    // Review other conditions to send Note On / Off
    if( Config.CommI2C == E_NOI2CFUNC && GateBut.PortCfg.CommI2C == E_NOI2CFUNC ){
        return;
    }

    // Send Note On/Off as Note plus velocity
    command_state_t Notedata;
    // Convert 14 to 16 bits
    Notedata.push(Veloc / 127. * 0x3FF);//Slider.PortValue<<2);
    Notedata.push(Note / 120. * 0x3FF);//(~CVPort.PortValue)<<2);
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
    if(!Config.UseMIDII2C || (Config.CommI2C == E_NOI2CFUNC && !GateBut.PortCfg.UseMIDII2C))
        return; // Send only on MIDI mode
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
