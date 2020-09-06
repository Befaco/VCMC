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

/** @addtogroup MenuClassDefs
* @{
*/

/**
 *  \file MenuI2C.ino
 *  \brief 
 */


MenuItem I2CDevItems[] = {
    {"Name", selectMenuPortName, 1}};
MenuList I2CDevList(I2CDevItems, 1, ListLines);


bool selectI2CDevice(void){
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&I2CDevList);
    return true;
}


bool setI2CDevice(void){
    I2CDevCollection *pCol = &theApp.I2CMerge.I2CDevices;
    I2CDevice *pDev = pCol->getDeviceAtPos(myMenu.getItemStatus() - 1);
    InputPortCfg *cfg = GetPortCfg();

    if( pDev){
        if(cfg){ // Port selected
            cfg->I2Cdev= pDev->I2Cid;
            cfg->CommI2C = pDev->firstOP;
            }
        else{ // No port selected, change all
            pCol->InitDefault(pDev->I2Cid);
            gotoMenuGlobalCfg();
            }
    }

    return true;
}


uint8_t OLEDMenu::FillI2CDevList(void)
{
    uint8_t posMenu = 0;
    I2CDevCollection *pCol = &theApp.I2CMerge.I2CDevices;
    I2CDevice *pDev = NULL;
    // Add Standard Names
    for (int i = 1; i < pCol->getCount() + 1; i++)
    {
        pDev = pCol->getDeviceAtPos(i - 1);
        CurrentMenuItems[posMenu].func = setI2CDevice;
        CurrentMenuItems[posMenu].Status = i;
        strcpy(CurrentMenuItems[posMenu].text, pDev->sName);
        //strncpy(CurrentMenuItems[posMenu].text, pDev->sName, SIZEPORTNAMES);
        //CurrentMenuItems[posMenu].text[SIZEPORTNAMES] = 0;
        posMenu++;
    }
    strcpy(CurrentMenuItems[posMenu].text, "I2C Device"); // Copy menu title
    CurrentMenuList.Style = ListLines;

    return posMenu;
}

uint8_t OLEDMenu::FillI2COpList(void)
{
    uint8_t posMenu = 0;
    I2CDevCollection *pCol = &theApp.I2CMerge.I2CDevices;
    uint8_t dev = GetPortCfg()->I2Cdev;
    I2CDevice *pDev = pCol->getDeviceAtPos(dev);

    // Add Standard Names
    
    for (int i = pDev->firstOP; i < pDev->lastOP-pDev->firstOP; i++)
    {
        CurrentMenuItems[posMenu].func = setI2CDevice;
        CurrentMenuItems[posMenu].Status = i;
        strcpy(CurrentMenuItems[posMenu].text, tele_ops[i]->name);
        //strncpy(CurrentMenuItems[posMenu].text, pDev->sName, SIZEPORTNAMES);
        //CurrentMenuItems[posMenu].text[SIZEPORTNAMES] = 0;
        posMenu++;
    }
    strcpy(CurrentMenuItems[posMenu].text, "I2C Op"); // Copy menu title
    CurrentMenuList.Style = ListLines;

    return posMenu;
}

/** @} */
