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

/**
 *  \file I2CMerge.ino
 *  \brief I2C I/O management functions
 */


#ifdef USEI2C

#ifdef CVTHING
    TwoWire *pWire = &Wire;
#else
    TwoWire *pWire = &Wire1;
#endif



bool I2Cmerger::poll(void)
{
    // Send I2C
    if( I2COutput) sendI2C ();
    // Receive I2C
    if( I2CInput) readI2C (); 

    #ifdef PRINTDEBUG
    if( received )
    {
        Serial.printf("Received %d:",received);
        Serial.println((char*)databuf);
        received = 0;
    }
    #endif

    return true;
}


void I2Cmerger::sendI2C () 
{
    static uint32_t microsI2C = 0;
    uint32_t current = micros();
    if (current - microsI2C > I2CINTERVAL && Serial.availableForWrite () > 20) {
        microsI2C = current;
        uint16_t NewValue;
        static uint16_t OldValue[2][9];
        static uint16_t OldGate[9];
		
        for (int i = 0; i < 9; i++) {
            if( CVControls[i].Config.UseMIDII2C)
                continue; // Do not process bank in MIDI mode
            // Send port values
            NewValue = CVControls[i].CVPort.PortValue;
            if ( !CVControls[i].CVPort.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                (NewValue - OldValue[0][i] > TRIMI2C || OldValue[0][i] - NewValue > TRIMI2C)) {
                CVControls[i].CVPort.SendI2C(0,(~NewValue)<<2);
                OldValue[0][i] = NewValue;
            }
            NewValue = CVControls[i].Slider.PortValue;
            if (  !CVControls[i].Slider.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                (NewValue - OldValue[1][i] > TRIMI2C || OldValue[1][i] - NewValue > TRIMI2C)) {
                /* if( CVControls[i].Slider.PortCfg.CommI2C != E_NOI2CFUNC )
                    SendI2Cint ( i, FADERSLOT, (NewValue<<2)); */
                CVControls[i].Slider.SendI2C(0,NewValue);
                OldValue[1][i] = NewValue;
            }
            NewValue = CVControls[i].GateBut.GateStatus;
            if (  !CVControls[i].GateBut.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                    NewValue != OldGate[i]) {
                //if( CVControls[i].GateBut.PortCfg.CommI2C != E_NOI2CFUNC )
                //SendI2CMsgbool ( i, GATESLOT, NewValue);
                CVControls[i].GateBut.SendI2C(0,NewValue);
                OldGate[i] = NewValue;
            }
        }
		
    }
}


void I2Cmerger::readI2C () {
}

/* 
void I2Cmerger::ProcessI2CMsg (I2CMessage *pMsg) {
}
 */

void I2Cmerger::begin(void)
{
    // Begin I2C serial channel
    if( IsMaster()){
        // Setup for Master mode
        pWire->begin();//I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 400000);
        pWire->setSDA(I2C_SDA);
        pWire->setSCL(I2C_SCL);
    } else
    {
        // Setup for Slave mode
        pWire->begin(ClientPort);
        pWire->setSDA(I2C_SDA);
        pWire->setSCL(I2C_SCL);
        // register events
        pWire->onReceive(receiveEvent);
        pWire->onRequest(requestEvent);
    }

    DP("Init I2C Merger");
}

void I2Cmerger::SendI2Cint(uint8_t bank, uint8_t port, int16_t value)
{
    SendI2Cint(0x66, 0x11, bank*10+port, value); // Send to ER-301
}

void I2Cmerger::SendI2Cint(uint8_t model, uint8_t cmd, uint8_t devicePort, int16_t value)
{
    recMsg.Command = cmd;
    recMsg.Port = devicePort;
    recMsg.iValue = value;

    SendI2Cdata(model, databuf, 4);

    /* D(Serial.printf("Sent Int %d:%d,%d,%d,%d\n", value, 
        databuf[0], databuf[1], databuf[2], databuf[3])); */
}


void I2Cmerger::SendI2Cdata(uint8_t addr, uint8_t *data, uint8_t l)
{
    pWire->beginTransmission(addr);
    pWire->write(data, l);
    pWire->endTransmission();
    D(printData(addr, data, l));
}

void printData(uint8_t addr, uint8_t *data, uint8_t l)
{
    Serial.printf("Addr:%2X: ", addr);
    for (size_t i = 0; i < l; i++)
    {
        Serial.printf("%2x - ", data[i]);
    }
    Serial.println("EOM");
}

#endif

/** @} */
