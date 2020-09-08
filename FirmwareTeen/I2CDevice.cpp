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

void I2CDevCollection::InitDefault(uint8_t Dev)
{
    if( !pDevices[Dev]){
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



#endif

/** @} */
