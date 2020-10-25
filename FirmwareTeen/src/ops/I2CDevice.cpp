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
//#include "PrjIncludes.h"
#include <stdint.h>
#include <stddef.h>
//#include "Definitions.h"
#include "I2CDevice.h"
#include "ops.h"

/** @addtogroup I2C 
 *  @{
 */

#ifdef USEI2C

I2CDevice listDevices[] = {
    {NO_I2CDEVICE,  E_NOI2CFUNC, E_NOI2CFUNC, "No Device"},
    
    {WW,            E_OP_WW_PRESET, E_OP_WW_MUTEB,  "whitewhale"},
    {MP,            E_OP_MP_PRESET, E_OP_MP_STOP,   "meadowphysics"},
    {ES,            E_OP_ES_PRESET, E_OP_ES_MAGIC,  "earthsea"},
    {ORCA,          E_OP_OR_TRK, E_OP_OR_CVB,       "orca"},
    {II_ANSIBLE_ADDR, E_OP_ANS_G_LED, E_OP_ARP_ER,  "ansible"},
    {JF_ADDR,       E_OP_JF_TR, E_OP_JF_QT,         "justfriends"},
    {WS_ADDR,       E_OP_WS_PLAY, E_OP_WS_LOOP,     "W/"},
    {TELEXO,        E_OP_TO_TR, E_OP_TO_CV_RESET,   "telexO"},
    {TELEXI,        E_OP_TI_PARAM, E_OP_TI_PRM_INIT,"telexI"},
    {ER301_1,       E_OP_SC_TR, E_OP_SC_TR_P,       "ER301"},
    {MATRIXARCHATE, E_OP_MA_SELECT, E_OP_MA_PCLR,   "matrixarchate"},
    {DISTING_EX_1,  E_OP_EX, E_OP_EX_TV,            "disting ex"},

    {LASTDEVICE, E_MOD__LENGTH, E_MOD__LENGTH, "Last Device"}
};

void I2CDevCollection::addBaseDevices(void)
{
    int i = 0;
    while (listDevices[i].I2Cid!=LASTDEVICE)
    {
        addDevice(&listDevices[i]);
        i++;
    }
}


#endif

/** @} */
