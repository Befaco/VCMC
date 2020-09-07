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
    {"I2CDev", selectMenuPortName, 1}};
MenuList I2CDevList(I2CDevItems, 1, ListLines);


MenuItem I2COpItems[] = {
    {"I2COp", selectMenuPortName, 1}};
MenuList I2COpList(I2COpItems, 1, ListLines);


bool selectI2CDevice(void){
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&I2CDevList);
    return true;
}

bool selectI2COp(void){
    InputPortCfg *cfg = GetPortCfg();
    uint8_t dev = cfg->I2Cdev;
    if(dev==NO_I2CDEVICE){
        DP("No Dev selected");
        return true; // No device selected on this port
    }
    myMenu.ClearArea();
    myMenu.setCurrentMenu(&I2COpList);
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
            D(Serial.printf("Selected %s/%d Op:%d\n", pDev->sName, pDev->I2Cid, pDev->firstOP));
            gotoMenuAnag();
            }
        else{ // No port selected, change all
            pCol->InitDefault(pDev->I2Cid);
            gotoMenuGlobalCfg();
            }
    }

    return true;
}


bool setI2COp(void){
    InputPortCfg *cfg = GetPortCfg();
    uint8_t dev = cfg->I2Cdev;
    if(dev == NO_I2CDEVICE) return true; // No I2C Dev
    I2CDevCollection *pCol = &theApp.I2CMerge.I2CDevices;
    I2CDevice *pDev = pCol->getDevice(dev);
    uint16_t op = myMenu.getItemStatus();
    if (op == 0xFF)
        op = E_NOI2CFUNC;
    else{ 
        op+= pDev->firstOP;
        D(Serial.printf("Selected %d, OP %d\n", pDev->firstOP, op));
       // D(Serial.printf("Selected %d, OP %s/%d\n", pDev->firstOP, tele_ops[op]->name, op));
        }
    if(cfg){ // Port selected
        cfg->CommI2C = op;
        gotoMenuAnag();
        }
    else{ // No port selected, change all
        //pCol->InitDefault(pDev->I2Cid);
        //gotoMenuGlobalCfg();
        }

    return true;
}


uint8_t OLEDMenu::FillI2CDevList(void)
{
    uint8_t posMenu = 0;
    I2CDevCollection *pCol = &theApp.I2CMerge.I2CDevices;
    I2CDevice *pDev = NULL;

    // Add Device Ops
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
    I2CDevice *pDev = pCol->getDevice(dev);
    if( pDev==NULL){
        D(Serial.printf("No Devu %d\n", dev));
        return 0;
    }
    D(Serial.printf("Menu %s/%d\n", pDev->sName, pDev->I2Cid));
    CurrentMenuItems[posMenu].func = setI2COp;
    CurrentMenuItems[posMenu].Status = 0xFF;
    strcpy(CurrentMenuItems[posMenu].text, "No Op");
    posMenu++;

    // Add Standard Names
    for (int i = pDev->firstOP; i < pDev->lastOP+1; i++)
    {
        if( tele_ops[i]==NULL){
            DP("No Op");
            continue;
        }
        D(Serial.printf("  Op %s/%d\n", tele_ops[i]->name, i));
        CurrentMenuItems[posMenu].func = setI2COp;
        CurrentMenuItems[posMenu].Status = i-pDev->firstOP;
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
