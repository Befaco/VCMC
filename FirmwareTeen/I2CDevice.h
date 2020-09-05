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

#ifndef I2CDevice_H_
#define I2CDevice_H_

/** @addtogroup I2C
* @{
*/

/**
 *  \file I2CDevice.h
 *  \brief 
 */

class I2CDevice{
public:
    uint8_t I2Cid;
    uint16_t firstOP;
    uint16_t lastOP;
    const char *sName;
    I2CDevice():
        I2Cid(0), firstOP(0), lastOP(0), sName(NULL) 
        {}
    I2CDevice(uint8_t id, uint16_t first, uint16_t last, const char *name):
        I2Cid(id), firstOP(first), lastOP(last), sName(name) 
        {}
};

#define LASTDEVICE 0xFF
extern I2CDevice listDevices[];


/**@}*/

#endif // I2CDevice_H_

