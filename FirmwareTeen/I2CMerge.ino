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


bool I2Cmerger::IsMaster()
{
    return I2CCore.IsMaster();
}

void I2Cmerger::setMasterMode(bool isMast) { 
    theApp.theGlobalCfg.masterI2C = isMast;
    I2CCore.setMasterMode(isMast);
    begin();
}


///////////////////////////////////////////////////
// Master I2C/ Leader

void I2Cmerger::sendI2C () 
{
    static uint32_t microsI2C = 0;
    uint32_t current = micros();
    if (current - microsI2C > I2CINTERVAL D(&& Serial.availableForWrite () > 20) ) {
        microsI2C = current;
        uint16_t NewValue;
        static uint16_t OldValue[2][9]={0};
        static uint16_t OldGate[9]={0};
		
        for (int i = 0; i < 9; i++) {
            if( CVControls[i].Config.UseMIDII2C)
                continue; // Do not process bank in MIDI mode
            // Send port values
            NewValue = CVControls[i].CVPort.PortValue;
            if ( !CVControls[i].CVPort.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                (NewValue - OldValue[0][i] > TRIMI2C || OldValue[0][i] - NewValue > TRIMI2C)) 
                // Send Only when difference bigger than Trim I2C value
            {
                CVControls[i].CVPort.SendI2C(0,(~NewValue)<<2);
                OldValue[0][i] = NewValue;
            }
            NewValue = CVControls[i].Slider.PortValue;
            if (  !CVControls[i].Slider.PortCfg.UseMIDII2C && // If MIDI mode selected, do not send
                (NewValue - OldValue[1][i] > TRIMI2C || OldValue[1][i] - NewValue > TRIMI2C)) 
                // Send Only when difference bigger than Trim I2C value
            {
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
    /*
    //////////////////////////////////////////
    // Only for test purpose, poll another VCMC
    static uint8_t Bank = 1;
    static uint8_t Port = 2;
    static uint32_t prevMill = 0;
    const uint32_t INTERVALPOLLMASTER = 3000; 
    uint32_t actMill = millis();

    if( prevMill+INTERVALPOLLMASTER < actMill){
        prevMill = actMill;
        // request
        DP("poll I2C");
        uint16_t value = LeaderReceiveSimple(VCMC0, Bank, Port);
        Bank = (Bank == 8) ? 1 : Bank + 1;
        Port = (Port == 3) ? 3 : Port + 1;
        D(Serial.printf("Received %d\n", value));
    }
    /////////////////////////////////////////////
    */
}




//////////////////////////////////////////////////
// Utils

void I2Cmerger::InitDefControls(void)
{
    I2CCore.I2CDevices.addBaseDevices();
    // Test Just Friends
    //I2CDevices.InitDefault(JF_ADDR);
    // Test ER-301
    //I2CDevices.InitDefault(ER301_1);
}


void I2Cmerger::InitDefault(uint8_t Dev)
{
    if( !I2CCore.I2CDevices.getDevice(Dev)){
        DP("InitDefault Device not initialized");
        return;
    }
    // Reset values
    for (size_t i = 0; i < 8; i++)
    {
        CVControls[i].Config.CommI2C = E_NOI2CFUNC;
        CVControls[i].CVPort.PortCfg.CommI2C = E_NOI2CFUNC;
        CVControls[i].Slider.PortCfg.CommI2C = E_NOI2CFUNC;
        CVControls[i].GateBut.PortCfg.CommI2C = E_NOI2CFUNC;
        CVControls[i].Config.UseMIDII2C = false;
        CVControls[i].CVPort.PortCfg.UseMIDII2C = false;
        CVControls[i].Slider.PortCfg.UseMIDII2C = false;
        CVControls[i].GateBut.PortCfg.UseMIDII2C = false;
    }

    switch (Dev)
    {
    case NO_I2CDEVICE:
        break;
    case ER301_1:
        DP("Init Default ER-301 Config");
        for (size_t i = 0; i < 8; i++)
        {
            CVControls[i].GateBut.PortCfg.I2Cdev = Dev;
            CVControls[i].GateBut.PortCfg.CommI2C = E_OP_SC_TR;
            CVControls[i].GateBut.PortCfg.I2CChannel = i+1;
            CVControls[i].CVPort.PortCfg.I2Cdev = Dev;
            CVControls[i].CVPort.PortCfg.CommI2C = E_OP_SC_CV;
            CVControls[i].CVPort.PortCfg.I2CChannel = i+1;
            CVControls[i].Slider.PortCfg.I2Cdev = Dev;
            CVControls[i].Slider.PortCfg.CommI2C = E_OP_SC_CV_SLEW;
            CVControls[i].Slider.PortCfg.I2CChannel = i+1;
        }
        break;
    case JF_ADDR:
        DP("Init Default Just Friends Config");
        CVControls[0].Config.CommI2C = E_OP_JF_NOTE; // First Bank sends Note ON/Off
        CVControls[0].Config.UseMIDII2C = true;
        for (size_t i = 1; i < 7; i++)
        {
            CVControls[i].GateBut.PortCfg.I2Cdev = Dev;
            CVControls[i].GateBut.PortCfg.CommI2C = E_OP_JF_TR;
            CVControls[i].GateBut.PortCfg.I2CChannel = i;
            CVControls[i].Slider.PortCfg.I2Cdev = Dev;
            CVControls[i].Slider.PortCfg.CommI2C = E_OP_JF_VTR+i;
            CVControls[i].Slider.PortCfg.I2CChannel = i;
        }
        CVControls[7].GateBut.PortCfg.I2Cdev = Dev;
        CVControls[7].GateBut.PortCfg.CommI2C = E_OP_JF_TR;
        CVControls[7].GateBut.PortCfg.I2CChannel = 7;
        CVControls[7].GateBut.PortCfg.UseMIDII2C = true;
        CVControls[7].Slider.PortCfg.I2Cdev = Dev;
        CVControls[7].Slider.PortCfg.CommI2C = E_OP_JF_VOX;
        CVControls[7].Slider.PortCfg.I2CChannel = 7;
        CVControls[7].Slider.PortCfg.UseMIDII2C = true;
        break;
    case TELEXO:
        DP("Init Default TxO Config");
        for (size_t i = 0; i < 8; i++)
        {
            CVControls[i].GateBut.PortCfg.I2Cdev = Dev;
            CVControls[i].GateBut.PortCfg.CommI2C = E_OP_TO_TR;
            CVControls[i].GateBut.PortCfg.I2CChannel = i+1;
            CVControls[i].CVPort.PortCfg.I2Cdev = Dev;
            CVControls[i].CVPort.PortCfg.CommI2C = E_OP_TO_CV;
            CVControls[i].CVPort.PortCfg.I2CChannel = i+1;
            CVControls[i].Slider.PortCfg.I2Cdev = Dev;
            CVControls[i].Slider.PortCfg.CommI2C = E_OP_TO_CV_SLEW;
            CVControls[i].Slider.PortCfg.I2CChannel = i+1;
        }
        break;
    default:
        break;
    }
}


void I2Cmerger::begin(void)
{
    I2CCore.ProcessInput = I2Cmerger::ProcessInput;
    I2CCore.begin(I2C_SDA,I2C_SCL);
}



uint16_t I2Cmerger::ProcessInput(I2CMessage *pMsg)
{
    uint16_t retval = NOMSGLEN;
    if(pMsg->Bank==0){ // General Command
        return retval;
    } else if( pMsg->I2CCommand==0){ // Command for Bank
        return retval;
    } else { // Command for Port
        // TODO Process command
        // Send Port Command
        InputPort *pPort = theApp.GetPort(pMsg->Destination-1, pMsg->I2CCommand);
        retval = pPort->PortValue;
    }
    return retval;
}


bool I2Cmerger::poll(void)
{
    // Send I2C
    //if( I2COutput && IsMaster() && CalTimer==0) sendI2C ();
    if( I2CCore.IsOutputActive() && IsMaster() ) sendI2C ();
    // Receive I2C
    if( I2CCore.IsInputActive() && IsMaster()) readI2C (); 

    #ifdef PRINTDEBUG
    if( I2CCore.InMsg.Length !=NOMSGLEN && !IsMaster())
    {
        Serial.printf("Received %d:", I2CCore.InMsg.Length);
        //Serial.println((char*)InMsg.dataRaw);
        I2CCore.printI2CData(VCMC0, I2CCore.InMsg.dataRaw, I2CCore.InMsg.Length);
        I2CCore.InMsg.Length = NOMSGLEN;
    }
    #endif

    return true;
}


#endif

/** @} */
