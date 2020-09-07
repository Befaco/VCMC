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


/** @addtogroup MenuClass
* @{
*/

/**
 *  \file OLEDMenu.ino
 *  \brief Defines Menu class functions for OLED screen 
 */


const int16_t POSXBARS = 62;
const int16_t BOXY = 11;
const int16_t BOXH = 64 - BOXY; //-8;
#ifdef CVTHING    
#define AUXBARSMODE 1 ///< 0= Standard mode 1= Do not show Aux
#else
#define AUXBARSMODE 0 ///< 0= Standard mode 1= Do not show Aux
#endif

MenuItem CurrentMenuItems[100];
MenuList CurrentMenuList (NULL, 0, ListCards);

/**
 * \brief Updates the selection based on current menu style
 * 
 * \return int 
 */
int OLEDMenu::updateSelection () {
    int lineMove = 0;

    // Read encoder
    valueEncoder -= encoder->getValue ();

    //valueEncoder = encoder->getValue ();

    // No Change in encoder
    if (valueEncoder == lastEncoder) return 0;

    if (valueEncoder > lastEncoder) {
        lineMove = 1;
        if (firstVisibleLine < currentMenu->getSize () - MaxLines) firstVisibleLine++;
        // if(currentItemIndex==currentMenu->getSize()-1) firstVisibleLine=0;
        if (currentMenu->Style == ListCards){
            #ifdef CVTHING
			if( BankSelected < 7) BankSelected++;
            #else
			if( BankSelected < 8) BankSelected++;
            #endif
            else lineMove = 0;
			}
    } else {
        lineMove = -1;
        if (firstVisibleLine > 0) firstVisibleLine--;
        // if( currentItemIndex == 0)
        //  if( currentMenu->getSize() > MaxLines) firstVisibleLine = currentMenu->getSize() - MaxLines;
        if (currentMenu->Style == ListCards){
            //BankSelected = (BankSelected > 0) ? BankSelected - 1 : 8;
			if(BankSelected > 0) BankSelected--;
			else lineMove = 0;
			}
    }

    lastEncoder = valueEncoder;
    if(lineMove){
        updatingText = true;
        updateText = -1;
    } 

    return lineMove;
}


/**
 * \brief Clears menu screen area
 * 
 */
void OLEDMenu::ClearArea () {
    theOLED->fillRect (MenuPosX, MenuPosY, /*MenuPosX+14*6*/ 128 - MenuPosX,
                      /*MenuPosY+MaxLines*8+9*/ 64 - MenuPosY, BLACK);
}

/**
 * \brief Updates states after the selection is made
 * 
 * \return boolean 
 */
boolean OLEDMenu::selectionMade () {

    // if( EncButton->fell())
    if (!EncButton->read ()) msDurationButton = EncButton->duration ();

    if (currentMenu->Style == ListCards && msDurationButton > 1000) {
        gotoGloblaMenu = true;
        msDurationButton = 0;
        return true;
    }

    if (EncButton->rose ()) {
        // if( currentMenu->Style==ListCards && msDurationButton>1000) gotoGloblaMenu = true; // Main Config
        EncButton->update ();
        msDurationButton = 0;
        return true;
    } else
        return false;
}

uint8_t OLEDMenu::FillNameMenuList()
{
    uint8_t posMenu = 0;
    // Add first User Nmaes
    posMenu = FillUserNameMenuList(false);
    // Add Standard Names
    for (int i = 1; i < NUMPORTNAMES + 1; i++)
    {
        CurrentMenuItems[posMenu].func = selectMenuPortName;
        CurrentMenuItems[posMenu].Status = i;
        strncpy(CurrentMenuItems[posMenu].text, PortNames[i], SIZEPORTNAMES);
        CurrentMenuItems[posMenu].text[SIZEPORTNAMES] = 0;
        posMenu++;
    }
    strcpy(CurrentMenuItems[posMenu].text, "Port Name"); // Copy menu title
    CurrentMenuList.Style = ListLines;
    return posMenu;
}

uint8_t OLEDMenu::FillUserNameMenuList(bool fillEmpty)
{
    uint8_t posMenu = (fillEmpty)?1:2;
    CurrentMenuItems[0].func = SelectNameMenu;
    CurrentMenuItems[0].Status = 0;
    strcpy(CurrentMenuItems[0].text, "<-BACK");
    if(!fillEmpty){
        CurrentMenuItems[1].func = selectMenuPortName;
        CurrentMenuItems[1].Status = 0;
        strcpy(CurrentMenuItems[1].text, "<no Name>");
    }
    // Create User names Menu list items
    for (int i = 0; i < NUMUSERNAMES; i++)
    {
        char *str = theApp.theGlobalCfg.UserNames[i];
        if(str[0]==0){
            if(!fillEmpty) continue;
            strncpy(CurrentMenuItems[posMenu].text, "<empty", SIZEPORTNAMES);
        } else
            strncpy(CurrentMenuItems[posMenu].text, str, SIZEPORTNAMES);
        if( fillEmpty) CurrentMenuItems[posMenu].func = editUserNames;
        else CurrentMenuItems[posMenu].func = selectMenuPortName;
        CurrentMenuItems[posMenu].Status = i+200;
        CurrentMenuItems[posMenu].text[SIZEPORTNAMES] = 0;
        posMenu++;
    }
    strcpy(CurrentMenuItems[posMenu].text, "User Name"); // Copy menu title
    CurrentMenuList.Style = ListLines;
    return posMenu;
}

/**
 * \brief Updates items list for new menu
 * 
 * \param aMenu 
 */
void OLEDMenu::setCurrentMenu (MenuList *aMenu) {
    uint8_t posMenu = 0;
    firstVisibleLine = 0;

    if(aMenu == &StdNameList){
        posMenu = FillNameMenuList();
    } else if(aMenu == &UserNameList){
        posMenu = FillUserNameMenuList(true);
    } else if(aMenu == &I2CDevList){
        posMenu = FillI2CDevList();
    } else if(aMenu == &I2COpList){
        posMenu = FillI2COpList();
    } else{
        // Create Menu with visible list items
        for (int i = 0; i < aMenu->getSize (); i++) {
            if (aMenu->getItem (i)->Status) {
                CurrentMenuItems[posMenu] = *aMenu->getItem (i);
                posMenu++;
            }
        }
        CurrentMenuItems[posMenu] = *aMenu->getItem (aMenu->getSize ()); // Copy menu title
        CurrentMenuList.Style = aMenu->Style;
    }
    // Create Menu List
    CurrentMenuList.menuItems = CurrentMenuItems;
    CurrentMenuList.listSize = posMenu;

    // Update text
    updatingText = true;
    updateText = -1;

    MenuClass::setCurrentMenu (&CurrentMenuList);
    // MenuClass::setCurrentMenu(aMenu);
}


/**
 * \brief Setup Text Popup Message
 * 
 * Pop up message with text msg during time milliseconds to show at pox, posy position and max width 
 * pixels wide width = 0 measn create multiline message to show complete msg text
 * 
 * \param msg text msg to show
 * \param time milliseconds to show
 * \param posx 
 * \param posy 
 * \param width max width pixels width = 0 create multiline message to show complete msg text
 * \return true 
 */
bool OLEDMenu::setupPopup (const char *msg, unsigned long time, uint16_t posx, uint16_t posy, uint16_t width) {
    showPopUp = true;
    strncpy (PopupMsg, msg, 255);
    PopupTime = time;
    PopupXpos = posx;
    PopupYpos = posy;
    PopupWidth = width;

    return true;
}

/**
 * \brief Setup Bitmap Popup Message
 * 
 * \param xp x Position
 * \param yp y Position
 * \param bmp b/w bitmap
 * \param xW x width of bitmap
 * \param yW y width of bitmap
 * \param Color Color for Bitmap
 */
void OLEDMenu::setupPopupBMP (int16_t xp, int16_t yp, const uint8_t *bmp, int16_t xW, int16_t yW, uint16_t Color) {
    PopupUseBMP = true;
    PopupBMPXpos = xp;
    PopupBMPYpos = yp;
    PopupBMPInfo = bmp;
    PopupBMPXWidth = xW;
    PopupBMPYWidth = yW;
    PopupBMPColor = Color;
}


////////////////////////////
// Display Menu Functions
/**
 * \brief Main Display Menu Functions
 * 
 */
void OLEDMenu::displayMenu () {
    long microsdisp = micros ();

    if( currentMenu->Style==ListCards){
        // Performance screen: Columns and info card
        // Update only every REFRESH_RATE microsec
        if (microsdisp - disptimer >= REFRESH_RATE) {
            displayMenuColumns ();
            // disptimer3 = microsdisp;
        }
        if (microsdisp - disptimer2 > REFRESH_RATE2) {
            displayMenuCards ();
            disptimer2 = microsdisp;
        }

    } else{
        if (microsdisp - disptimer3 >= REFRESH_RATE) {
            updatingText = true;
            updateText = -1;
            if( currentMenu->Style==ListLinesSimple)
                theOLED->fillRect (POSXCARD, 0, MenuPosX - POSXCARD, 64, BLACK);
            else{
                #ifdef DEBUGMODE
                displayMenuCards ();
                #else
                displayMenuConfig ();
                #endif
            }
            disptimer3 = microsdisp;
        }
        if( updatingText == true){
            displayMenuListLines ();
        }
    }

    // Show Popup messages
    if (showPopUp) {
        if (EncoderPopupMsg (PopupMsg, PopupTime, PopupXpos, PopupYpos, PopupWidth)) { // True = end showing popup
            showPopUp = false;
            PopupUseBMP = false;
        }
    }
}


/**
 * \brief  Display Drop list menu
 * 
 */
void OLEDMenu::displayMenuListLines () {
    char outBuf[15];
    int i = 0, j = 0;

	/* #ifdef DEBUGMODE
    beginDebugTimers ();
	#endif */
    if( updatingText == false) return;

    if( updateText == -1){
        // Display title
        theOLED->setTextSize (1);
        theOLED->setTextColor (WHITE, BLACK);
        strncpy (outBuf, currentMenu->menuItems[currentMenu->getSize ()].text, 14);
        // Pad with spaces
        for (j = strlen (outBuf); j < 14; j++) outBuf[j] = ' ';
        outBuf[14] = 0;
        theOLED->setCursor (MenuPosX, MenuPosY);
        theOLED->println (outBuf);
        theOLED->drawLine (MenuPosX, MenuPosY + 8, MenuPosX + 14 * 6, MenuPosY + 8, WHITE);
        //updateText++;
        updateText = firstVisibleLine;
    } else {
        theOLED->setTextSize (1);

        int limit = (MaxLines + firstVisibleLine < currentMenu->getSize ()) ? MaxLines + firstVisibleLine :
                                                                            currentMenu->getSize ();
        i= updateText;
        //for (i = firstVisibleLine; i < limit; i++) 
        //{
            // Current line in inverted mode
            if (i == currentItemIndex)
                theOLED->setTextColor (BLACK, WHITE);
            else
                theOLED->setTextColor (WHITE, BLACK);

            // use getText method to pull text out into a buffer you can print
            getText (outBuf, i);
            // Pad with spaces
            for (j = strlen (outBuf); j < 14; j++) outBuf[j] = ' ';
            outBuf[14] = 0;

            // Display menu element
            theOLED->setCursor (MenuPosX, MenuPosY + 9 + (i - firstVisibleLine) * 8);
            // theOLED->setCursor( MenuPosX, MenuPosY+9+(lineprint)*8);
            theOLED->println (outBuf);
        //}
        updateText++;
        //updateText = i;
        if(updateText== limit){ // End of Menu update
            updatingText = false;
            updateText = -1;
            }
    }
	/* #ifdef DEBUGMODE
	endDebugTimers ();
	#endif */
    //if (!showPopUp) theOLED->display ();
}

#ifdef DEBUGMODE
extern unsigned long  clicktimer2, clicktimer3,
    avgtimer2, avgtimer , maxtimer, mintimer;
extern int count, count2, count3;
//char Buf[80];
#endif

/**
 * \brief Performance Display one item at a time
 * 
 */
void OLEDMenu::displayMenuCards () {
    int posCursor = 17;
    char name[24];

    theOLED->fillRect (0, 0, POSXBARS - COLUMNW, 64, BLACK);
    displayMenuBankHeader (false);


#ifndef DEBUGMODE
    int TrimCV;
    TrimCV = CVControls[BankSelected].CVPort.TrimValue (-9999);
    // Status of analog inputs
    // CV Data
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;

    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;
    if (CVControls[BankSelected].CVPort.PortCfg.textPort) // Has name
    {
        // Print Name
        CVControls[BankSelected].CVPort.PortCfg.getName(name);
        theOLED->print(name);
    } else{
        PrintFunction(&(CVControls[BankSelected].CVPort.PortCfg));
    }
    theOLED->setCursor (POSXCARD + 8, posCursor);
    posCursor += 8;
    theOLED->print (TrimCV);
    if (CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCHTRIG ||
        CVControls[BankSelected].CVPort.PortCfg.MIDIfunction == PITCH8TRIG) {
        if (TrimCV > 0) {
            theOLED->print (" ");
            theOLED->print (NotesNames[TrimCV]);
        }
    }
    theOLED->setCursor (POSXCARD + 0, posCursor);
    posCursor += 8;

    #ifndef CVTHING
    // Fader Data
    int TrimFader = CVControls[BankSelected].Slider.TrimValue (-9999);

    if( CVControls[BankSelected].Config.Chanfunction != INDEP) {
        if (BankSelected == 8) {
            theOLED->print ("AuxB ");
        } else {
            theOLED->print ("FAD ");
        }
    } else {
        if (CVControls[BankSelected].Slider.PortCfg.textPort) // Has name
        {
            // Print Name
            CVControls[BankSelected].Slider.PortCfg.getName(name);
            theOLED->print(name);
        } else{
                PrintFunction(&(CVControls[BankSelected].Slider.PortCfg));
        }
        theOLED->setCursor (POSXCARD + 8, posCursor);
        posCursor += 8;
    }
    theOLED->println (TrimFader); // CVControls[BankSelected].Slider.MIDIData);
    //posCursor += 8;
    if( CVControls[BankSelected].Config.Chanfunction != INDEP){
        // Calculated Data
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        switch (CVControls[BankSelected].Config.Chanfunction) {
        case INDEP:
            theOLED->print ("");
            break;
        case SUM:
            theOLED->print ("SUM ");
            break;
        case NOTEMODE:
            theOLED->print ("NOTE ");
            break;
        case MULTIPL:
            theOLED->print ("ATTE ");
            break;
        }
        int SendData = CVControls[BankSelected].DataCalc ();
        if (SendData != -9999){
            SendData = CVControls[BankSelected].CVPort.TrimValue(SendData);
            theOLED->println (SendData);
        }
    }    

    // Gate Data
    if ( BankSelected < 8) {
        // Gate title
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        if (CVControls[BankSelected].GateBut.PortCfg.textPort) // Has name
        {
            // Print Name
            CVControls[BankSelected].GateBut.PortCfg.getName(name);
            theOLED->print(name);
        } else{
            PrintGateFunction(&(CVControls[BankSelected].GateBut.PortCfg));
            //theOLED->print(PortGateFuncStr[CVControls[BankSelected].GateBut.PortCfg.MIDIfunction]);
        }
        // Gate status
        if (CVControls[BankSelected].GetGateState ())
            theOLED->fillCircle (POSXCARD + 15 + 4*8, posCursor-5, 3, WHITE);
        else {
            theOLED->fillCircle (POSXCARD + 15 + 4*8, posCursor-5, 3, BLACK);
            theOLED->drawCircle (POSXCARD + 15 + 4*8, posCursor-5, 3, WHITE);
        }
    }

    #endif
////////////////////////////////////////////////////////
// Debug information
#else
    if (currentMenu->Style != ListLines) {
        // refresh cycle
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("min:");
        theOLED->println (mintimer);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("max:");
        theOLED->println (maxtimer);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("Av1:");
        theOLED->println (avgtimer);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("C1:");
        theOLED->println (count2);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("Av2:");
        theOLED->println (avgtimer2);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
     } else {
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("CV: ");
        theOLED->println (CVControls[BankSelected].CVPort.MIDIData);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("CV: ");
        theOLED->println (CVControls[BankSelected].CVPort.PortValue);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("FAD:");
        theOLED->println (CVControls[BankSelected].Slider.MIDIData);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
        theOLED->print ("FAD:");
        theOLED->println (CVControls[BankSelected].Slider.PortValue);
        theOLED->setCursor (POSXCARD + 0, posCursor);
        posCursor += 8;
    } 
#endif
#ifdef DEBUGMODE
    // Message line
    if (MessageTimer) {
        theOLED->setCursor (POSXCARD + 0, 57);
        if (millis () < InitMessageTime + MessageTimer) {
            theOLED->println (LastMessage);
        } else { // Clear message
            MessageTimer = 0;
            theOLED->println ("                    ");
        }
    }
#endif
    // theOLED->display();
}


#define SHOWGATECIRCLE 0
/**
 * \brief Show Header Info for Bank Selected
 * 
 * \param ShowFunc True to show channel function
 */
void OLEDMenu::displayMenuBankHeader (bool ShowFunc) {
    theOLED->fillRect (POSXCARD, 0, 54 - 8, 64, BLACK);
    // Display Channel selected
    theOLED->setTextSize (2);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->setCursor (POSXCARD, 0);
    if (BankSelected < 8)
        theOLED->println (BankSelected + 1);
    else{
		if( PortSelected == 2)
			theOLED->println ("A");
		else if(PortSelected == 3)
			theOLED->println ("B");
		else theOLED->println ("Aux");
		}
    theOLED->setTextSize (1);

    // Status of digital inputs
    // Display Gate status
    if (SHOWGATECIRCLE && BankSelected < 8) {
        if (CVControls[BankSelected].GetGateState ())
            theOLED->fillCircle (POSXCARD + 12 + 5, 6, 5, WHITE);
        else {
            theOLED->fillCircle (POSXCARD + 12 + 5, 6, 5, BLACK);
            theOLED->drawCircle (POSXCARD + 12 + 5, 6, 5, WHITE);
        }
    }
    // Bank function
    if (!ShowFunc || BankSelected>7) return;
    theOLED->setCursor (POSXCARD + 20 - 5, 8);
    switch (CVControls[BankSelected].Config.Chanfunction) {
    case INDEP:
        theOLED->println ("Indep");
        break;
    case SUM:
        theOLED->println ("Sum");
        break;
    case NOTEMODE:
        theOLED->println ("Note");
        break;
    case MULTIPL:
        theOLED->println ("Atten");
        break;
    }
}


/**
 * \brief Display columns for the performance screen
 * 
 */
void OLEDMenu::displayMenuColumns () {

    int i = 0;
    long barhCV, barhF;
    #ifdef CVTHING    
    int16_t barW = 5;
    #else
    int16_t barW = 3;
    #endif

    int InitX = (AUXBARSMODE == 1) ? POSXBARS : POSXBARS - COLUMNW;
    int ColumnsX = (AUXBARSMODE == 1) ? 8 : 9;

    // Erase previous data
    theOLED->fillRect (InitX, 0, COLUMNW * ColumnsX + 1, BOXH + BOXY, BLACK);
    // Draw external box
    theOLED->drawRect (InitX, BOXY, COLUMNW * ColumnsX + 2, BOXH, WHITE);

    for (i = 0; i < 8; i++) {
        #if DRAWCOLUMNLINES == 1
        // External frame
        theOLED->drawRect ((uint16_t)InitX + i * COLUMNW, BOXY, COLUMNW + 1, BOXH, WHITE);
        // Header frame
        theOLED->drawRect ((uint16_t)InitX + i * COLUMNW, 0, COLUMNW + 1, BOXY, WHITE);
        #endif
        // Display Gate status
        //if (CVControls[i].GetGateState ()) theOLED->setTextColor (BLACK, WHITE);

        // Header text
        theOLED->setCursor ((uint16_t)InitX + i * COLUMNW + 1, 1);
        theOLED->println (i + 1);
        theOLED->setTextColor (WHITE, BLACK);

        // Display CV and Fader bar
        // Fader takes full length
        barhF = (int32_t) (
        (float)BOXH * CVControls[i].Slider.PortCfg.Ranges.PercentScale (CVControls[i].Slider.MIDIData));
        if (barhF > BOXH) barhF = BOXH;
        if (barhF < 0) barhF = 0;
        #ifndef CVTHING
        theOLED->drawRect ((uint16_t)InitX + i * COLUMNW + 5, (uint16_t)BOXY + BOXH - barhF - 1, barW,
                          (uint16_t)barhF, WHITE);
        #endif
        barhCV = (int32_t) (
        (float)BOXH * CVControls[i].CVPort.PortCfg.Ranges.PercentScale (CVControls[i].CVPort.MIDIData));
        // if( barhCV > BOXH) barhCV= BOXH;
        // theOLED->fillRect( (uint16_t)InitX+i*COLUMNW+1, (uint16_t)BOXY+BOXH-barhCV-1, 3, (uint16_t)barhCV, WHITE);

        if (CVControls[i].Config.Chanfunction == SUM) {
            int16_t minD, rgeMID;
            CVControls[i].CVPort.PortCfg.Ranges.getMIDI (minD, rgeMID);
            if (minD < 0) barhCV = barhCV - BOXH / 2;

            if (CVControls[i].CVPort.MIDIData > 0) {
                if (barhCV + barhF > BOXH) barhCV = BOXH - barhF;
                theOLED->fillRect ((uint16_t)InitX + i * COLUMNW + 1,
                                  (uint16_t)BOXY + BOXH - barhCV - barhF - 1, barW, (uint16_t)barhCV, WHITE);
            } else if (-barhCV > barhF)
                barhCV = -barhF;
            theOLED->fillRect ((uint16_t)InitX + i * COLUMNW + 1, (uint16_t)BOXY + BOXH - barhF - 1,
                              barW, (uint16_t)-barhCV, WHITE);
        } else
            theOLED->fillRect ((uint16_t)InitX + i * COLUMNW + 1, (uint16_t)BOXY + BOXH - barhCV - 1,
                              barW, (uint16_t)barhCV, WHITE);
    }
    #ifndef CVTHING
    // Display Aux
    if (AUXBARSMODE == 1) {
        /*
        barhCV = COLUMNW * 8 - ((int32_t)COLUMNW * 8 * CVControls[8].CVPort.PortValue) / ANRANGEMAX;
        theOLED->drawRect (InitX, BOXY + BOXH, (uint16_t)barhCV, 3, WHITE);
        barhF = COLUMNW * 8 - ((int32_t)COLUMNW * 8 * CVControls[8].Slider.PortValue) / ANRANGEMAX;
        theOLED->drawRect (InitX, BOXY + BOXH + 4, (uint16_t)barhF, 3, WHITE);*/
    } else {
        // Header text
        theOLED->setCursor ((uint16_t)InitX + 8 * COLUMNW + 1, 1);
        theOLED->println ("A");
        barhCV = (int32_t) (
        (float)BOXH * CVControls[i].CVPort.PortCfg.Ranges.PercentScale (CVControls[i].CVPort.MIDIData));
        //(BOXH)-((int32_t)(BOXH)*CVControls[i].CVPort.PortValue)/ANRANGEMAX;
        theOLED->fillRect ((uint16_t)InitX + 8 * COLUMNW + 1, (uint16_t)BOXY + BOXH - barhCV - 1, barW,
                          (uint16_t)barhCV, WHITE);
        barhF = (int32_t) (
        (float)BOXH * CVControls[i].Slider.PortCfg.Ranges.PercentScale (CVControls[i].Slider.MIDIData));
        // (BOXH)-((int32_t)(BOXH)*CVControls[i].Slider.PortValue)/ANRANGEMAX;
        theOLED->drawRect ((uint16_t)InitX + 8 * COLUMNW + 5, (uint16_t)BOXY + BOXH - barhF - 1, barW,
                          (uint16_t)barhF, WHITE);
    }
    #endif
}

/** @} */