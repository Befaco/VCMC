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
#ifndef __have__OLEDMenu_h__
#define __have__OLEDMenu_h__

/** @addtogroup MenuClass
* @{
*/

/**
 *  \file OLEDMenu.h
 *  \brief Defines Menu class for OLED screen 
 */

class DigPortCfg;
class AnInputPortCfg; //class predeclaration
/**
 * \brief Menu Class to handle encoder and OLED screen
 * 
 */
class OLEDMenu : public MenuClass
{
public:
    int MaxLines; ///< Max lines to visulize
    int firstVisibleLine;
    int MenuPosX, MenuPosY;

    bool showPopUp = false;
    char PopupMsg[256];
    unsigned long PopupTime = 0;
    uint16_t PopupXpos, PopupYpos;
    uint16_t PopupWidth = 0;
    bool PopupUseBMP = false;
    int16_t PopupBMPXpos;
    int16_t PopupBMPYpos;
    const uint8_t *PopupBMPInfo;
    int16_t PopupBMPXWidth;
    uint16_t PopupBMPYWidth;
    byte PopupBMPColor;
    uint8_t fullDisplay; ///< Used to mark when menu pane should go full screen
    bool gotoGMenu;
    bool gotoGCal;

    // Message info
    char LastMessage[255] = "";
    unsigned long MessageTimer = 0;
    unsigned long InitMessageTime = 0;

    //Display refresh
    unsigned long disptimer = 0, disptimer2 = 0, disptimer3 = 0;

    unsigned long msDurationButton = 0; ///< Duration button is pressed in msecs
    uint8_t MultiValState = 0;          ///< Used for multi - selection menus

    // Calibration menus
    bool FactoryCal = false;         ///< Factory calibration flag
    unsigned long FactoryCalTimer = 0;  ///< Factory calibration timer
    bool GlobalCalEn = false;        ///< Global calibration flag

    // Digit selection option
    byte digSel=255;

    // Progressive update
    bool updatingText = false;
    int8_t updateText = -1;

public:
    bool setupPopup(const char *msg, unsigned long time, uint16_t posx, uint16_t posy, uint16_t width = 0);
    void setupPopupBMP(int16_t xp, int16_t yp, const uint8_t *bmp, int16_t xW, int16_t yW, uint16_t Color);

    int updateSelection();
    bool selectionMade();
    void setCurrentMenu(MenuList *aMenu);
    void setCurrentMenu(
        const char *title, const char * const*listStr, 
        uint8_t numitems, Item_Function fun, uint8_t initItem=0xff, uint8_t startItem=0);
    uint8_t FillNameMenuList();
    uint8_t FillUserNameMenuList(bool fillEmpty=true);
    uint8_t FillI2CDevList(void); // Definition on I2CDevice.cpp
    uint8_t FillI2COpList(void);  // Definition on I2CDevice.cpp

    void displayMenu();
    void displayMenuListLines();
    void displayMenuCards();
    void displayMenuColumns();
    void displayMenuConfig();
    void displayMenuBankHeader(bool ShowFunc);
    void ClearArea();
    void PrintFunction(AnInputPortCfg *selCfg);
    void PrintGateFunction(DigPortCfg *cfgSel);
    void displayGenCal(void);
    void displayBankConfig(int posCursor);
    void displayCVConfig(int posCursor);
    void displayFaderConfig(int posCursor);
    void displayGateConfig(int posCursor);
    OLEDMenu()
    {
        MaxLines = 7;
        firstVisibleLine = 0;
        MenuPosX = 54;
        MenuPosY = 0;
        fullDisplay = true;
        gotoGMenu = false;
        gotoGCal = false;
    }; // default constructor

    // Graf Utils
    unsigned long Inittime = 0;

    void SetMessage(const char *msg, unsigned long milTime = 10000)
    {
        strncpy(LastMessage, msg, 20);
        MessageTimer = milTime;
        InitMessageTime = millis();
    }
    bool EncoderPopupMsg(const char *title, unsigned long milTime, uint16_t posx, uint16_t posy, uint8_t xwidth = 0);
    bool EncoderChangeLong(const char *title, long &value, long minv, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy, void (*DispFunc)(char *outb, long val,uint16_t padpos)=nullptr);
    bool EncoderselDigitLong(const char *title, long &value, long minv, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy);
    
    bool EncoderChangeVal(long &value, long minv, long maxv, float inc, void (*DispFunc)(long val));
    bool EncoderselDigitVal(long &value, long minv, long maxv, uint16_t padPos, char *outBuf);
    bool EncoderSelectStr(const char *title, long &value, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy);

    bool EncoderChangeValFloat(float &value, float minv, float maxv, float inc, void (*DispFunc)(long val));

    bool EncoderselDigitStr(const char *title, char *str, uint16_t padPos, uint16_t posx, uint16_t posy);
    bool EncoderChangeStr(const char *title, char *str, uint16_t padPos, uint16_t posx, uint16_t posy);
    bool EncoderselStrChar(char *str, uint16_t padPos);

    void printStrInput(char *outBuf, long vale, uint16_t padPos);
    void ClearInputArea(const char *title, uint16_t padPos, uint16_t posx, uint16_t posy);
    void DrawInputArea(const char *title, uint16_t padPos, uint16_t posx, uint16_t posy);
};

extern MenuItem CurrentMenuItems[];
extern MenuList CurrentMenuList;

/** @} */
  
#endif //__have__OLEDMenu_h__