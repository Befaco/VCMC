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
#ifndef __have__OSCMerger_h__
#define __have__OSCMerger_h__
#include "Definitions.h"

/** @addtogroup OSC 
 *  @{
 */

/**
 *  \file OSCMerge.h
 *  \brief OSC I/O management 
 */

/// Class for OSC I/O management
class OSCmerger
{
private:
    int msgInSize = 0;
    OSCMessage msgIn;
    char charIn[260];

public:
    SLIPEncodedUSBSerial SLIPSerial; ///< OSC Serial
    uint8_t databuf[512];
    int received;
    int ClientPort;
    OSCMessage msgOut;

public:
    OSCmerger(): SLIPSerial(Serial),received(0),ClientPort(0x66) {};
    bool poll(void);
    void begin(void);
    void sendOSC();
    void readOSC();
    void ProcessOscMsg(OSCMessage *pMsg);
    void SendOSCMsgint(char *address, int value);
    void SendOSCMsgfloat(char *address, float value);

    SLIPEncodedUSBSerial *getSLIPSerial(void){ return &SLIPSerial; }
};

#endif

/** @} */
