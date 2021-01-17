// Copyright 2019 Sergio Retamero.
//
// Author: Sergio Retamero (sergio.retamero@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
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
 *  \file GrafUtil.ino
 *  \brief Defines Helper functions for OLED screen user interface 
 */

/**
 * \brief Change a float value with the encoder 
 * 
 * \param value Value to modify
 * \param minv Minimum value
 * \param maxv Maximum value
 * \param inc Increment per encoder step
 * \param DispFunc Function to call to display the value
 * \return bool 
 */
bool OLEDMenu::EncoderChangeValFloat(float &value, float minv, float maxv, float inc, void (*DispFunc)(long val))
{
	if (DispFunc)
		DispFunc(value);

	valueEncoder -= encoder->getValue();

	//valueEncoder = encoder->getValue();
	if (valueEncoder != lastEncoder)
	{
		// Update value
		//value+= (long)valueEncoder-(long)lastEncoder;
		value += (valueEncoder > lastEncoder) ? inc : -inc;
		if (value < minv)
			value = minv;
		else if (value > maxv)
			value = maxv;
		lastEncoder = valueEncoder;
		return false;
	}
	// If button pressed (LOW input), finish
	if (!EncButton->rose())
		return false;
	else
	{
		return true;
	}
}

/**
 * \brief Change a long value with the encoder 
 * 
 * \param value Value to modify
 * \param minv Minimum value
 * \param maxv Maximum value
 * \param inc Increment per encoder step
 * \param DispFunc Function to call to display the value
 * \return bool 
 */
bool OLEDMenu::EncoderChangeVal(long &value, long minv, long maxv, float inc, void (*DispFunc)(long val))
{
	if (DispFunc)
		DispFunc(value);

	valueEncoder -= encoder->getValue();

	//valueEncoder = encoder->getValue();
	if (valueEncoder != lastEncoder)
	{
		// Update value
		value += (valueEncoder-lastEncoder)*inc;
		if (value < minv)
			value = minv;
		else if (value > maxv)
			value = maxv;
		lastEncoder = valueEncoder;
		return false;
	}
	// If button pressed (LOW input), finish
	if (!EncButton->rose())
		return false;
	else
	{
		return true;
	}
}

/**
 * \brief Change a long values digits individually with the encoder. Recommended to adjust bigger numbers.
 * 
 * \param value Value to modify
 * \param minv Minimum value
 * \param maxv Maximum value
 * \param padPos Number of digits to show
 * \param outBuf character array for the output of value
 * \return bool 
 */
bool OLEDMenu::EncoderselDigitVal(long &value, long minv, long maxv, uint16_t padPos, char *outBuf)
{
	valueEncoder += encoder->getValue();
	char selNumber = outBuf[digSel];
	if (valueEncoder < lastEncoder)
	{
		// Update value
		selNumber --;
		if( selNumber < '0')
			selNumber = '9';

		outBuf[digSel] = selNumber;
		value = strtol(outBuf, NULL, 10);
		if (value < minv)
			value = minv;
		else if (value > maxv)
			value = maxv;
	} else if(valueEncoder > lastEncoder){
		if(digSel==0)
			digSel = padPos-1;
		else digSel--;

		//digSel++;
		//if(digSel>=padPos) digSel = 0;
	}
	
	lastEncoder = valueEncoder;

	// If button pressed (LOW input), finish
	if (!EncButton->rose())
		return false;
	else return true;
 }

/**
 * \brief Change a long value and show in screen title and value.
 * 
 * \param title Title to show
 * \param value Value to modify
 * \param minv Minimum value
 * \param maxv Maximum value
 * \param padPos Number of positions for number
 * \param posx Popuo position X
 * \param posy Popuo position Y
 * \return bool 
 */
bool OLEDMenu::EncoderChangeLong(const char *title, long &value, long minv, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy)
{
	char outBuf[17];

	ClearInputArea( title, padPos, posx, posy); // Erase input area
	DrawInputArea(title, padPos, posx, posy);  // Draw Input area

	//get value string
	printStrInput((char*)outBuf, value, padPos);
	// theOLED->display();

	// Read encoder and change the value
	bool retVal = true;
	if( digSel==255)
		retVal = EncoderChangeVal(value, minv, maxv, 1.0, NULL);
	else
		retVal = EncoderselDigitVal(value, minv, maxv, padPos, outBuf);

	if (retVal)	{
		ClearInputArea( title, padPos, posx, posy); // Erase input area
		digSel = 255;
		return true;
	}

	return false;
}


bool OLEDMenu::EncoderSelectStr(const char *title, long &value, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy)
{
/* 	char outBuf[17];

	ClearInputArea( title, padPos, posx, posy); // Erase input area
	DrawInputArea(title, padPos, posx, posy);  // Draw Input area

	//get value string
	 df((char*)outBuf, value, padPos);
	// theOLED->display();

	// Read encoder and change the value
	bool retVal = true;
	if( digSel==255)
		retVal = EncoderChangeVal(value, minv, maxv, 1.0, NULL);
	else
		retVal = EncoderselDigitVal(value, minv, maxv, padPos, outBuf);

	if (retVal)	{
		ClearInputArea( title, padPos, posx, posy); // Erase input area
		digSel = 255;
		return true;
	}
 */
	return false;
}


/**
 * \brief Change a long value digits indiviadully and show in screen title and value.
 * 
 * \param title Title to show
 * \param value Value to modify
 * \param minv Minimum value
 * \param maxv Maximum value
 * \param padPos Number of positions for number
 * \param posx Popuo position X
 * \param posy Popuo position Y
 * \return bool 
 */
bool OLEDMenu::EncoderselDigitLong(const char *title, long &value, long minv, long maxv, uint16_t padPos, uint16_t posx, uint16_t posy)
{
	// Change digSel to first character in the value
    if( digSel==255) digSel = padPos-1;

	if( EncoderChangeLong(title, value, minv, maxv, padPos, posx, posy)){
		digSel = 255;
		return true;
	}

	return false;
}


/**
 * \brief Call this function to change input string chars indiviadully and show in screen title and value.
 * 
 * \param title Title to show
 * \param str String to modify
 * \param padPos Number of positions for string. Memory allocated for string should cover this number of chars plus one for null terminator.
 * \param posx Popuo position X
 * \param posy Popuo position Y
 * \return bool 
 */
bool OLEDMenu::EncoderselDigitStr(const char *title, char *str, uint16_t padPos, uint16_t posx, uint16_t posy)
{
	// Change digSel to first character in the value
    if( digSel==255) {
		//digSel = padPos-1;
		digSel = 0;
		if (strlen(str) < padPos) { // Pad string with spaces
			for (int i = strlen(str); i < padPos;i++){
				str[i] = ' ';
			}
			str[padPos] = 0;
		}	
	}
	if (EncoderChangeStr(title, str, padPos, posx, posy))
	{
		digSel = 255;
		return true;
	}

	return false;
}


/**
 * \brief Change input string chars indiviadully and show in screen title and value.
 * 
 * \param title Title to show
 * \param str String to modify
 * \param padPos Number of positions for string. Memory allocated for string should cover this number of chars plus one for null terminator.
 * \param posx Popuo position X
 * \param posy Popuo position Y
 * \return bool 
 */
bool OLEDMenu::EncoderChangeStr(const char *title, char *str, uint16_t padPos, uint16_t posx, uint16_t posy)
{

	ClearInputArea( title, padPos, posx, posy); // Erase input area
	DrawInputArea(title, padPos, posx, posy);  // Draw Input area

	//print string
	for (uint16_t i = 0; i < strlen(str);i++)
	{
		if( i==digSel) 	theOLED->setTextColor(BLACK, WHITE);
		theOLED->print(str[i]);
		if( i==digSel) 	theOLED->setTextColor(WHITE, BLACK);
	}
	theOLED->println("");

	// theOLED->display();

	// Read encoder and change the value
	bool retVal = true;
	retVal = EncoderselStrChar( str, padPos);

	if (retVal)	{
		ClearInputArea( title, padPos, posx, posy); // Erase input area
		digSel = 255;
		return true;
	}

	return false;
}


/**
 * \brief Change a char string characters individually with the encoder.
 * 
 * \param str Value to modify
 * \param padPos Number of digits to show
 * \return bool 
 */
const uint8_t CONTROLWITHPRESS = 1;
bool OLEDMenu::EncoderselStrChar( char *str, uint16_t padPos)
{
	valueEncoder += encoder->getValue();
	char selNumber = str[digSel];
	if (valueEncoder < lastEncoder)
	{
		selNumber++;
		if( selNumber>LASTNAMECHAR) selNumber = FIRSTNAMECHAR;
		str[digSel] = selNumber;
	} else if(valueEncoder > lastEncoder){
		if(padPos==1 || CONTROLWITHPRESS){
			selNumber --;
			if( selNumber < FIRSTNAMECHAR)	selNumber = LASTNAMECHAR;
			str[digSel] = selNumber;
		}else{
			if(digSel==padPos-1){
				digSel = 0;
			}
			else digSel++;
		}
	}
	
	lastEncoder = valueEncoder;

	if(CONTROLWITHPRESS){
		// If button pressed (LOW input), edit next char 
		if (EncButton->rose()){
			if(digSel==padPos-1) // End of string edit
				return true;
			else if(digSel==0 && str[digSel]==' '){ // First character space means empty string
				str[0] = 0;
				return true;
			}
			digSel++; // Next char
			return false;
			}
		else return false;
	} else {
		// If button pressed (LOW input), finish
		if (!EncButton->rose())
			return false;
		else return true;
	}
	return true;
 }


/**
 * \brief 	Erase input area 
 */
void OLEDMenu::ClearInputArea(const char *title, uint16_t padPos, uint16_t posx, uint16_t posy)
{
	//Clear area used
	theOLED->fillRect(posx, posy, strlen(title) * 6 + padPos * 12 + 3, 18, BLACK); 
}

/**
 * \brief 	Draw input area 
 */
void OLEDMenu::DrawInputArea(const char *title, uint16_t padPos, uint16_t posx, uint16_t posy)
{
	// Display title
	theOLED->setTextSize(1);
	theOLED->setTextColor(WHITE, BLACK);
	theOLED->setCursor(posx + 2, posy + 5);
	theOLED->println(title);
	// Print value
	theOLED->setTextSize(2);
	theOLED->setTextColor(WHITE, BLACK);
	theOLED->setCursor(posx + strlen(title) * 6 + 2, posy + 1);
	// Rect around input area
	theOLED->drawRect(posx, posy, strlen(title) * 6 + padPos * 12 + 3, 18, WHITE);
}



/**
 * \brief Print on screen value marking selected character
 * 
 * \param outBuf character buffer
 * \param value value to print on screen
 * \param padPos number of characters to show
 */
void OLEDMenu::printStrInput(char *outBuf, long value, uint16_t padPos){
	padvalue(value, outBuf, (padPos > 16) ? 16 : padPos);
	for (uint16_t i = 0; i < strlen(outBuf);i++)
	{
		if( i==digSel) 	theOLED->setTextColor(BLACK, WHITE);
		theOLED->print(outBuf[i]);
		if( i==digSel) 	theOLED->setTextColor(WHITE, BLACK);
	}
	//theOLED->println(outBuf);
	theOLED->println("");
}

/**
 * \brief Show a message for a number of milliseconds or until encoder pressed.
 * 			will show a bitmap if it is setup before calling this funcion. See OLEDMenu::setupPopupBMP
 * \param title Text to show (May cover multiple lines)
 * \param milTime Time to keep showing the message
 * \param posx Postion X
 * \param posy Postion Y
 * \param xwidth Number of chars per line
 * \return bool 
 */
bool OLEDMenu::EncoderPopupMsg(const char *title, unsigned long milTime, uint16_t posx, uint16_t posy, uint8_t xwidth)
{
	const uint16_t xMargin = 4, yMargin = 8;
	uint16_t xW, yW;

	if (Inittime == 0)
		Inittime = millis();

	// Calculate area
	if (xwidth > 0)
		yW = ((xwidth - xMargin) / 6 + 1) * 8 + yMargin; // Divide in several lines
	else
		yW = 8 + yMargin;
	yW = (yW > 63 - posy) ? 63 - posy : yW; // Limit to max screen width

	xW = strlen(title) * 6 + xMargin;
	xwidth = (xwidth == 0) ? 127 - posx : xwidth;
	xW = (xW > xwidth) ? xwidth : xW; // Limit to max screen width

	//Clear area used
	theOLED->fillRect(posx, posy, xW, yW, BLACK); // Erase input area
	// Display title
	theOLED->setTextSize(1);
	theOLED->setTextColor(WHITE, BLACK);
	uint16_t posCursor = 0;
	char outBuf[41];
	uint8_t nChars = (xW - xMargin + 1) / 6;
	uint8_t line = 0;
	do
	{
		theOLED->setCursor(posx + xMargin / 2, posy + yMargin / 2 + line * 8);
		strncpy(outBuf, title + posCursor, nChars);
		outBuf[nChars] = 0;
		theOLED->println(outBuf);
		posCursor = (posCursor > strlen(title) - nChars) ? 0 : posCursor + nChars; // Check end of title
		line = ((line + 1) * 8 + yMargin > yW - 8) ? 0 : line + 1;				   // Check limit of lines
	} while (posCursor > 0 && line > 0);
	//	theOLED->setCursor(posx+xMargin/2,posy+yMargin/2);
	//	theOLED->println(title);
	// Rect around input area
	theOLED->drawRect(posx, posy, xW, yW, WHITE);

	if (PopupUseBMP)
		theOLED->drawBitmap(PopupBMPXpos, PopupBMPYpos, PopupBMPInfo, PopupBMPXWidth, PopupBMPYWidth, PopupBMPColor);

	theOLED->display();

	// If button pressed (LOW input), or finish time reached finish
	if (EncButton->rose() || millis() > Inittime + milTime)
	{
		//Clear area used
		theOLED->fillRect(posx, posy, xW, yW, BLACK); // Erase input area
		Inittime = 0;
		//theOLED->display();
		return true;
	}
	else
		return false;

	return true;
}

/**
 * \brief Write a value char array zero padded to specified positions 
 * 
 * \param value 
 * \param outBuf char array
 * \param padPos Number of positions to pad to
 */
void padvalue(long value, char *outBuf, int padPos)
{
	int index = 1;
	int i = 0;
	int endPos = 0;

	outBuf[padPos] = 0; //NULL terminate the string

	if (value < 0)
	{
		value = -value;
		outBuf[0] = '-';
		endPos = 1;
	}
	else
		outBuf[padPos] = 0;

	for (i = padPos; i > endPos; i--)
	{
		outBuf[i - 1] = '0' + (value / index) % 10;
		index *= 10;
	}
}

unsigned long ProgInitTimer = 0;
unsigned long intLastTimer = 0;
unsigned long intPrevTimer = 0;

unsigned long intTimer(char *MsgTxt)
{
	unsigned long intCurTimer = micros();

	if (ProgInitTimer == 0)
		ProgInitTimer = intCurTimer;
#ifdef PRINTDEBUG
	Serial.print(MsgTxt);
	Serial.print("#");
	Serial.print(ProgInitTimer);
	Serial.print(": ");
	Serial.print(intCurTimer);
	Serial.print("/");
	Serial.print(intPrevTimer);
	Serial.print("/");
	Serial.println(intCurTimer - intPrevTimer);
#endif
	intPrevTimer = intCurTimer;
	return intCurTimer;
}

#ifdef DEBUGMODE
void beginDebugTimers(void)
{
	clicktimer2 = micros();
}

void endDebugTimers(void)
{
	clicktimer3 = micros();
	mintimer = (clicktimer3 - clicktimer2 < mintimer) ? clicktimer3 - clicktimer2 : mintimer;

	maxtimer = (clicktimer3 - clicktimer2 > maxtimer) ? clicktimer3 - clicktimer2 : maxtimer;
	count++;
	if (clicktimer3 - clicktimer2 > 1000)
	{
		count2++;
		avgtimer -= avgtimer / 100;
		avgtimer += (clicktimer3 - clicktimer2) / 100;
	}
	else
	{
		avgtimer2 -= avgtimer2 / 10;
		avgtimer2 += (clicktimer3 - clicktimer2) / 10;
	}

	if (count > 100000)
	{
		count = 0;
		maxtimer = 0;
		count3 = count2;
		count2 = 0;
		mintimer = 50000;
		avgtimer = 0;
	}
}
#endif

/** @} */