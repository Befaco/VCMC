// Simple Menu Class
//
// By Delta_G
//
// https://forum.arduino.cc/index.php?topic=353045.0
//
#ifndef MENUCLASS_H_
#define MENUCLASS_H_

//#define USING_PROGMEM

#include "Arduino.h"

/** @addtogroup MenuClass
* @{
*/

/**
 *  \file MenuClass.h
 *  \brief Defines Base Menu class
 */

typedef bool (*Item_Function)();

enum ItemStatus : uint8_t
{
	ITEM_DIS = 0,
	ITEM_EN = 1,
	ITEM_LINK
};


const uint8_t MENUITEMTEXTSIZE=16;
/*const */typedef struct MenuItem_t {
 char text[MENUITEMTEXTSIZE];
 Item_Function func;
 byte Status; // Use for enable/disable option. This function is not used in the base MenuList class and should be implemented on derived classes
} MenuItem;


enum MenuListStyles{
	ListLines, // Classical menu with several options shown at the same time
	ListLinesSimple, // Classical menu with several options shown at the same time, no info
	ListCards  // This type will show only one item
} ;

#define DEF_MENULIST(name, title, style, ItList...) \
	MenuItem name##List[]{                          \
		ItList,                                     \
		{#title, NULL, ITEM_EN}};                   \
	MenuList name(name##List, sizeof(name##List) / sizeof(MenuItem) - 1, style);

class MenuList {
private:
public:
 MenuItem *menuItems;
 uint8_t listSize;
 MenuListStyles Style;
 MenuItem* getItem(int aIndex);
 MenuList(MenuItem* aList, uint8_t aSize, MenuListStyles sStyle) :
 menuItems(aList), listSize(aSize), Style(sStyle) {}
 uint8_t getSize();
 uint8_t getActiveSize();
 void enableItem(int);
 void disableItem(int);
};

class MenuClass {
private:

 bool cancelFlag;
 bool runningFunction;
public:
 MenuList *currentMenu;
 int currentItemIndex;

 MenuClass();
 MenuClass(MenuList* aList);

 void doMenu();
 void setCurrentMenu(MenuList*);
 void getText(char*, int);
 void enableItem(int);
 void disableItem(int);
 bool isEnabled(int);
 bool runFunction(void);
 virtual void displayMenu() = 0;
 virtual bool checkForCancel();
 virtual int updateSelection() = 0;
 virtual bool selectionMade() = 0;
 byte getItemStatus() { return currentMenu->getItem(currentItemIndex)->Status; };
 void setCurrentItem(uint8_t itnum);
};

/** @} */

#endif /* MENUCLASS_H_ */

