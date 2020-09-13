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


///////////////////////////////////////////////////
// Master I2C/ Leader

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
                CVControls[i].Slider.SendI2C(0,NewValue);
                OldValue[1][i] = NewValue;
            }
            NewValue = CVControls[i].GateBut.GateStatus;
            if (  !CVControls[i].GateBut.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                    NewValue != OldGate[i]) {
                CVControls[i].GateBut.SendI2C(0,NewValue);
                OldGate[i] = NewValue;
            }
        }
		
    }
}


void I2Cmerger::readI2C () {
    // Only for test purpose, poll another VCMC
    static uint8_t Bank = 0;
    static uint8_t Port = 0;
    static uint32_t prevMill = 0;
    const uint32_t INTERVALPOLLMASTER = 3000; 
    uint32_t actMill = millis();

    if( prevMill+INTERVALPOLLMASTER < actMill){
        prevMill = actMill;
        // request
        DP("poll I2C");
        uint16_t value = LeaderReceiveSimple(VCMC0, Bank, Port);
        Bank = (Bank == 8) ? 0 : Bank + 1;
        Port = (Port == 3) ? 0 : Port + 1;
        D(Serial.printf("Received %d\n", value));
    }
}


///////////////////////////////////////////////////
// Slave I2C/ Follower

uint16_t I2Cmerger::ProcessInputRequest()
{
    if( InMsg.Length==NOMSGLEN){
        return NOMSGLEN;
    }
    uint16_t retval = NOMSGLEN;
    if(InMsg.Bank==0){ // General Command
        return retval;
    } else if( InMsg.I2CCommand==0){ // Command for Bank
        return retval;
    } else { // Command for Port
        // TODO Process command
        // Send Port Command
        InputPort *pPort = theApp.GetPort(InMsg.Bank - 1, InMsg.Port);
        retval = OutMsg.Fill( (uint8_t *)&pPort->PortValue, 2);
    }

    return retval;
}


//////////////////////////////////////////////////
// Utils

void I2Cmerger::InitDefControls(void)
{
    I2CDevices.addBaseDevices();
    // Test Just Friends
    //I2CDevices.InitDefault(JF_ADDR);
    // Test ER-301
    //I2CDevices.InitDefault(ER301_1);
}

void I2Cmerger::begin(void)
{
    // Begin I2C serial channel
    if( IsMaster()){
        // Setup for Master mode
        pWire->begin();//I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_INT, 400000);
        pWire->setSDA(I2C_SDA);
        pWire->setSCL(I2C_SCL);
        DP("Init Leader I2C Merger");
    } else
    {
        // Setup for Slave mode
        pWire->begin(ClientPort);
        pWire->setSDA(I2C_SDA);
        pWire->setSCL(I2C_SCL);
        // register events
        pWire->onReceive(receiveEvent);
        pWire->onRequest(requestEvent);
        DP("Init Follower I2C Merger");
    }
}

bool I2Cmerger::poll(void)
{
    // Send I2C
    if( I2COutput && IsMaster() && CalTimer==0) sendI2C ();
    // Receive I2C
    if( I2CInput && IsMaster()) readI2C (); 

    #ifdef PRINTDEBUG
    if( InMsg.Length !=NOMSGLEN && !IsMaster())
    {
        Serial.printf("Received %d:",InMsg.Length);
        //Serial.println((char*)InMsg.dataRaw);
        printI2CData(VCMC0, InMsg.dataRaw, InMsg.Length);
        InMsg.Length = NOMSGLEN;
    }
    #endif

    return true;
}

#endif

/** @} */
