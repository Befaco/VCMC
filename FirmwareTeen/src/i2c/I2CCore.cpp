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
#include "I2CCore.h"
#define PRINTDEBUG

/** @addtogroup I2C 
 *  @{
 */

/**
 *  \file I2CCore.ino
 *  \brief I2C I/O management functions
 */


#ifdef USEI2C

#ifdef PRINTDEBUG
#define D(x) x		/// Shorthand for PRINTDEBUG
#define DP(x) Serial.println(x)		/// Shorthand for Deug serial print
#else
#define D(x)
#define DP(x)
#endif


/// Define global I2C object
I2Ccore I2CCore;             ///< Control I2C inputs / outputs


bool I2Ccore::IsMaster(){
    return I2CMaster;
}

void I2Ccore::setMasterMode(bool isMast) { 
    I2CMaster = isMast;
    begin(pinSDA,pinSCL);
}

//////////////////////////////////////////////////
// Utils
void I2Ccore::begin(uint8_t I2C_SDA,uint8_t I2C_SCL)
{
    pinSDA = I2C_SDA;
    pinSCL = I2C_SCL;

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


///////////////////////////////////////////////////
// Slave I2C/ Follower

uint16_t I2Ccore::ProcessInputRequest()
{
    if( InMsg.Length==NOMSGLEN){
        return NOMSGLEN;
    }
    uint16_t retval = NOMSGLEN;
    if(ProcessInput){
        retval = ProcessInput(&InMsg);
        OutMsg.Fill( (uint8_t *)&retval, 2);
        }
    return retval;
}

const tele_op_t *I2Ccore::getTeleOp(uint16_t Comm){
    return tele_ops[Comm];
}

bool I2Ccore::TeleOpUseChanInfo(uint16_t Comm){
    if(Comm>=E_OP_JF_TR && Comm<=E_OP_JF_QT)
        return tele_opsJFChan[Comm - E_OP_JF_TR];

    return true;
}

#endif

/** @} */
