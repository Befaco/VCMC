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
#include "../../PrjIncludes.h"

/** @addtogroup I2C 
 *  @{
 */

/**
 *  \file I2CMerge.h
 *  \brief I2C I/O management 
 */



/// Class for specific application I2C I/O management
class I2Cmerger
{
public:
    uint8_t databuf[512];
    int received=0;
    int ClientPort=0x66;

    bool IsMaster();
    void setMasterMode(bool isMast);
    bool poll(void);
    void begin(void);
    void InitDefControls(void);
    void InitDefault(uint8_t Dev);
    void sendI2C();
    void readI2C();
    uint16_t ProcessInputRequest(void);
    static uint16_t ProcessInput(I2CMessage *pMsg);
};

#endif

/** @} */
