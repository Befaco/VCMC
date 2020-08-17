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

struct I2CMessage {
    uint8_t Command;
    uint8_t Port;
    union
    {
        uint8_t data[2];
        uint16_t uiValue;
        int16_t iValue;
        //float fValue;
    };
};



/// Class for I2C I/O management
class I2Cmerger
{
private:
    bool I2CInput = true;
    bool I2COutput = true;
    bool I2CMaster = true;

public:
    union {
        uint8_t databuf[64]; // Raw data
        I2CMessage recMsg;  // Received message
    };
    int received=0;
    int ClientPort=0x66;

public:
    bool IsMaster() { return I2CMaster; }
    bool poll(void);
    void begin(void);
    void sendI2C();
    void readI2C();
    //void ProcessI2CMsg(I2CMessage *pMsg);
    void SendI2Cint(uint8_t model, uint8_t deviceIndex, uint8_t cmd, uint8_t devicePort, int16_t value);
    void SendI2Cint(uint8_t bank, uint8_t port, int16_t value);
};

    extern TwoWire *pWire;

#endif

/** @} */
