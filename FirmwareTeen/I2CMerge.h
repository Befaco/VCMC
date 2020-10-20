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
#ifndef __have__I2CMerger_h__
#define __have__I2CMerger_h__
#include "PrjIncludes.h"

/** @addtogroup I2C 
 *  @{
 */

/**
 *  \file I2CMerge.h
 *  \brief I2C I/O management 
 */


class I2CMessage {
public:
    union{
    struct{
        uint8_t I2CCommand;
        union{
            uint8_t Bank : 4;
            uint8_t Port : 4;
            uint8_t Destination;
        };
        int16_t iValue;
        uint8_t additionaldata[MAXI2CMSGLEN-4];
    };
    uint8_t dataRaw[MAXI2CMSGLEN];
    uint16_t data16Raw[MAXI2CMSGLEN/2];
    };
    uint16_t Length=NOMSGLEN; // Use 0xFFFF to mark empty message
public:
    uint16_t Fill(uint8_t *pDest, uint16_t len) { 
        memcpy(dataRaw, pDest, 2);
        Length = len;
        return len;
    }
};



/// Class for I2C I/O management
class I2Cmerger
{
private:
    bool I2CInput = true;
    bool I2COutput = true;
    //bool I2CMaster = false;

public:
    I2CMessage OutMsg;  // Message to send
    I2CMessage InMsg;  // Received message

    int ClientPort=VCMC0;
    I2CDevCollection I2CDevices;
public:
    bool IsMaster();
    void setMasterMode(bool isMast);
    bool poll(void);
    void begin(void);
    void InitDefControls(void);
    void sendI2C();
    void readI2C();
    uint16_t ProcessInputRequest(void);
    
    // Functions for Leader and Follower Mode
    void ReadI2Cdata(int count);
    static void printI2CData(uint8_t addr, uint8_t *data, uint8_t l);

    // Functions for Master/Leader Mode
    //void scanforI2Cclients();
    void SendI2CdataLeader(uint8_t addr, uint8_t *data, uint8_t l);
    uint16_t LeaderReceiveSimple( uint8_t addr, uint8_t Bank, uint8_t Port);
    void ReadI2CLeader(uint8_t addr, uint8_t *data, uint8_t l);

    // Ops 
    const tele_op_t *getTeleOp(uint16_t Comm); // Definition in I2CCommands.cpp
    bool TeleOpUseChanInfo(uint16_t Comm);   // Definition in I2CCommands.cpp
    void callOP(uint16_t Comm, command_state_t *cs){
        callOP(getTeleOp(Comm), cs);
    }
    void callOP(const tele_op_t *pOp, command_state_t *cs){
        if(pOp)
            pOp->get(NULL, NULL, NULL, cs);
    }
};

extern TwoWire *pWire;
extern void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l);
extern void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l);

#endif

/** @} */
