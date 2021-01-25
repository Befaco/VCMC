// Simple Menu Class
//
// By Delta_G
//
// https://forum.arduino.cc/index.php?topic=353045.0
//

#include "MenuClass.h"

/** @addtogroup MenuClass
* @{
*/

/**
 *  \file MenuClass.cpp
 *  \brief Defines Menu base functions 
 */

MenuItem* MenuList::getItem(int aIndex) {
 // To make modulo work with negatives the way we want
 while (aIndex < 0) {
 aIndex += listSize;
 }
 return (aIndex==listSize)?&(menuItems[aIndex]):&(menuItems[aIndex % listSize]);
}

// Return total number of items
uint8_t MenuList::getSize() {
 return this->listSize;
}

// Return total number of active items
uint8_t MenuList::getActiveSize() {
	uint8_t ret =0;
	for(uint8_t i=0; i<listSize; i++)
		if( menuItems[i].Status) ret++;
 return ret;
}

MenuClass::MenuClass() {
 currentMenu = 0;
 currentItemIndex = 0;
 cancelFlag = false;
 runningFunction = false;
}

MenuClass::MenuClass(MenuList* aList) {
 currentMenu = aList;
 currentItemIndex = 0;
 cancelFlag = false;
 runningFunction = false;

}

bool MenuClass::runFunction(void) {
 Item_Function theFunc;

#ifdef USING_PROGMEM
 theFunc =
 (Item_Function) pgm_read_word(&(currentMenu->getItem(currentItemIndex)->func));
#else
 theFunc = currentMenu->getItem(currentItemIndex)->func;

#endif
 return theFunc();

}

void MenuClass::doMenu() {
	cancelFlag = checkForCancel();
	if (runningFunction) {
		runningFunction = !runFunction();
	} else {
		//menuHandler();
		cancelFlag = false;
		int selUp = updateSelection();
		if (selUp) {
			currentItemIndex += selUp;
			// Do not roll over
			currentItemIndex = (currentItemIndex<0)?0:currentItemIndex;
			currentItemIndex = (currentItemIndex>currentMenu->getSize()-1)?currentMenu->getSize()-1:currentItemIndex;
		/*	 //  Avoid negative numbers with the modulo
			//  to roll the menu over properly
			while (currentItemIndex < 0) {
				currentItemIndex += currentMenu->getSize();
			}
			currentItemIndex %= currentMenu->getSize();
		*/
		} else {   // only check for selection if menu not also updating
			if (selectionMade()) { // selection made, run item function once now
				if (!runFunction()) { // If item function returns false then it isn't done and we need to keep running it.
				runningFunction = true;
				return; // skip displaying the menu if we're going to run item function again.
				}
			}
		}
		displayMenu();
		}
}

void MenuClass::getText(char* aBuf, int aIndex) {
#ifdef USING_PROGMEM
 strcpy_P(aBuf, (currentMenu->getItem(aIndex)->text));
#else
 strcpy(aBuf, (currentMenu->getItem(aIndex)->text));
#endif
}

void MenuList::enableItem(int aIndex){
 getItem(aIndex)->Status=1;
}

void MenuList::disableItem(int aIndex){
 getItem(aIndex)->Status=0;
}

void MenuClass::enableItem(int aIndex){
 currentMenu->enableItem(aIndex);
}

void MenuClass::disableItem(int aIndex){
 currentMenu->disableItem(aIndex);
}

bool MenuClass::isEnabled(int aIndex)
{
	return currentMenu->getItem(aIndex)->Status;
}

bool MenuClass::checkForCancel() {
 return false;
}

void MenuClass::setCurrentMenu(MenuList* aMenu) {
 currentMenu = aMenu;
 currentItemIndex = 0;
}


/** @} */