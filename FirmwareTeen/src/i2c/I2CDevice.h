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

#ifndef USEI2C // If you arrived here, I2C is desired
#define USEI2C
#endif 

#define LASTDEVICE 0xFF
#define NO_I2CDEVICE 0
const uint16_t E_NOI2CFUNC = 0x3FF;


class I2CDevice{
public:
    uint8_t I2Cid;  // I2C device number as defined in ii.h
    uint16_t firstOP; // First OP for device as defined in op_enum.h
    uint16_t lastOP; // Last OP for device as defined in op_enum.h
    const char *sName; // Text string for device
    I2CDevice():
        I2Cid(0), firstOP(0), lastOP(0), sName(NULL) 
        {}
    I2CDevice(uint8_t id, uint16_t first, uint16_t last, const char *name):
        I2Cid(id), firstOP(first), lastOP(last), sName(name) 
        {}
};

class I2CDevCollection{
private:
    uint8_t countDev = 0;
    uint8_t mapDev[256] = {0}; // TODO  CHECK if useful
    I2CDevice *pDevices[256] = {NULL};
public:
    uint8_t addDevice(I2CDevice *pDev) { 
        mapDev[countDev] = pDev->I2Cid;
        countDev++;
        pDevices[pDev->I2Cid] = pDev;
        //D(Serial.printf("Device %s:%d added\n", pDev->sName, pDev->I2Cid));
        return pDev->I2Cid;
    }
    
    void addBaseDevices(void);
    uint8_t getCount(void) { return countDev; }
    I2CDevice *getDeviceAtPos(uint8_t pos) { // Based on Device position on list
        if(pos<countDev) return pDevices[mapDev[pos]];
        else return NULL;
        }
    I2CDevice *getDevice(uint8_t opID) { // Based on Device I2C operator
        return pDevices[opID];
        }
};

extern I2CDevice listDevices[];


/**@}*/

#endif // I2CDevice_H_

