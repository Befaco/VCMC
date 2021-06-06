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
#include"Definitions.h"

/** @addtogroup GeneralUtil
 *  @{
 */

/**
 *  \file setValues.ino
 *  \brief Definiton and support for pre defined set of configuration values
 */

#define MAXPRESETPAGES 2
const char *strMatr[]={"Cancel","Mutable Shruthi","Yamaha RX5"};


/* Channel Function:
    INDEP,   ///< Each Input acts independently
    SUM,     ///< Sum CV and Fader Inputs
    MULTIPL, ///< Multiply CV anf Fader inputs. If Fader is set to Percent, it will act as percentage of CV.
	// When MULTIPL is selected, set the respective channel slider function to PERCENT
*/
InputCtrlCfg configChan[][9] ={
	//Shruthi
	{
		{SUM},
    {SUM},
    {SUM},
    {SUM},
    {INDEP},
    {SUM},
    {SUM},
    {SUM},
    {INDEP}
	},
   //Yamaha RX5
	{
		{SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {INDEP}
	}, 
	//Shruthi 2
	{
		{SUM},
    {SUM},
    {SUM},
    {SUM},
    {INDEP},
    {SUM},
    {SUM},
    {SUM},
    {INDEP}
	},
   //Yamaha RX5 2
	{
		{SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {SUM},
    {INDEP}
	}
};

// Required
//  MIDI Channel,  CC Number, MIDI function, 
// Optional values:
// byte RangeBip= 0, 
// uint8_t NRPNMSB=0x7f , uint8_t NRPNLSB=0x7f,
// int16_t ClipL=0, int16_t ClipH=120, 
// float clkDiv = 1.0, int8_t clkSh = 0, 
// byte ccVal=0, byte nSend=60
/* 
MIDI Channel: 1-16
CC Number: CC function number 0-127
MIDI function: 
    PITCHTRIG,         ///< Note Mode triggered by gate
    PITCH,             ///< Not in use
    CONTROLCHANGE,     ///< MIDI Control Change message
    VELOCITY,          ///< Velocity Mode. Will be used when sending a Note On on the same channel.
    PROGRAMCHANGE,     ///< MIDI Program Change message
    PITCHBEND,         ///< MIDI Pitch Bend message
    PERCENT,           ///< Set value to be between 0 and 100. This apply to the slider value only when the function of the Bank is Multiply
    ANAGCLOCK,         ///< Send MIDI clock
    ANAGSTARTSTOP,     ///< Start/Stop MIDI messages based on High /Low value of input
    ANAGPAUSECONT,     ///< Pause/Stop MIDI messages based on High /Low value of input
    ANAGTRIGSTARTSTOP, ///< Start/Stop MIDI messages triggered on High value of input
    ANAGTRIGPAUSECONT, ///< Pause/Stop MIDI messages triggered on High value of input
    ANAGFREEVALUE,
    ANAGNRPN7bits,     ///< Send 7 bits NRPN values
    ANAGNRPN14bits,    ///< Send 14 bits NRPN values
    NOANFFUNC,
	PITCHLEVEL			///< When Gate ON, Send Note Off/Sebd Note On
Bipolar: 
  0 = 0/10V
  1 = -5/5 V
  2 = 0/5 V
NRPNMSB: Most significant byte address for NRPN 
NRPNLSB: Least significant byte address for NRPN, 0, 120
ClipLow:  Clip MIDI to this minimum value
ClipHigh: Clip MIDI to this maximum value
clock Divider: fine adjustment for MIDI clock
clock Shift: Multiplier/divider for MIDI clock
CC Number: CC function number 0-127
Note to Send: MIDI Note to Send
*/
AnInputPortCfg configCV[][9] ={
  //Shruthi
  {
  {1, 20, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 24, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 14, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 15, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 23, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 101, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 111, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 103, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, ANAGCLOCK, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60}
  },
 //Yamaha RX5
  {
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60}
  },  
  //Shruthi 2
  {
  {1, 20, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 24, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 14, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 15, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 23, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 101, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 111, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 103, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, ANAGCLOCK, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60}
  },
 //Yamaha RX5 2
  {
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60}
  }
  
};

/* min MIDI, Range MIDI */
RangeConv CVRanges[][9] ={
  //Shruthi
  {
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127}
  },
  //Yamaha RX5
  {
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  },
   //Shruthi 2
  {
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127}
  },
  //Yamaha RX5 2
  {
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  }
};


//  MIDI Channel,  CC Number, MIDI function, Bipolar, NRPNMSB, NRPNLSB
AnInputPortCfg configFader[][9] ={
  //Shruthi
  {
  {1, 20, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 24, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 14, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 15, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 101, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 111, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 103, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, ANAGSTARTSTOP, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60}
  },
  //Yamaha RX5
  {
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60}
  },
  //Shruthi 2
  {
  {1, 20, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 24, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 14, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 15, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 101, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 111, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 103, CONTROLCHANGE, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60},
  {1, 30, ANAGSTARTSTOP, 0, 0x7f , 0x7f, 0, 127, 1.0, 1, 0, 60}
  },
  //Yamaha RX5 2
  {
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60},
  {10, 12, PITCHTRIG, 0, 0x7f , 0x7f, 0, 120, 1.0, 1, 0, 60}
  }
};

/* min MIDI, Range MIDI */
RangeConv FaderRanges[][9] ={
	//Shruthi
  {
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127}
  },
  //Yamaha RX5
  {
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  }, 
	//Shruthi 2
  {
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127},
  {0, 127}
  },
  //Yamaha RX5 2
  {
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  {40, 23},
  }
};


//  MIDI Channel,  CC Number,  clock Divider, clock Shift, MIDI function, CC Value,  Note to Send
/*
MIDI Channel: 1-16
CC Number: CC function number 0-127
clock Divider: fine adjustment for MIDI clock
clock Shift: Multiplier/divider for MIDI clock
MIDI function: 
    TRIGGER,           ///< Gate as trigger
    LATCH,             ///< Latched trigger. flips output on High value
    CCBUT,             ///< Send CC value on  High, set to 0 value on Low
    CCLATCH,           ///< Send CC value or 0 value on High value
    GATECLOCK,         ///< MIDI Clock
    GATESTARTSTOP,     ///< Start/Stop MIDI messages based on High /Low value of input
    GATEPAUSECONT,     ///< Pause/Stop MIDI messages based on High /Low value of input
    GATETRIGSTARTSTOP, ///< Start/Stop MIDI messages triggered on High value of input
    GATETRIGPAUSECONT, ///< Pause/Stop MIDI messages triggered on High value of input
    GATESTART,         ///< Send MIDI Start
    GATESTOP,          ///< Send MIDI Stop
    GATECONTINUE,      ///< Send MIDI Continue
    NODIGFUNC
CC Value: CC value to send 0-127
Note to Send: MIDI Note to Send
*/
DigPortCfg configGate[][8] ={
  //Shruthi
  {
  {1, 112, 1.0, 0, CCLATCH, 62, 50},
  {1, 116, 1.0, 0, CCLATCH, 62, 51},
  {1, 31, 1.0, 0, CCBUT, 45, 52},
  {1, 102, 1.0, 0, CCBUT, 53, 53},
  {1, 28, 1.0, 0, CCLATCH, 13, 54},
  {1, 105, 1.0, 0, CCBUT, 70, 55},
  {1, 109, 1.0, 0, CCBUT, 94, 56},
  {1, 75, 1.0, 0, CCLATCH, 2, 57}
  },
   //Yamaha RX5
  {
  {10, 22, 1.0, 0, TRIGGER, 60, 50},
  {10, 23, 1.0, 0, TRIGGER, 60, 50},
  {10, 24, 1.0, 0, TRIGGER, 60, 50},
  {10, 25, 1.0, 0, TRIGGER, 60, 50},
  {10, 26, 1.0, 0, TRIGGER, 60, 50},
  {10, 27, 1.0, 0, TRIGGER, 60, 50},
  {10, 28, 1.0, 0, TRIGGER, 60, 50},
  {10, 29, 1.0, 0, TRIGGER, 60, 50}
  },
  //Shruthi 2
  {
  {1, 112, 1.0, 0, CCLATCH, 62, 50},
  {1, 116, 1.0, 0, CCLATCH, 62, 51},
  {1, 31, 1.0, 0, CCBUT, 45, 52},
  {1, 102, 1.0, 0, CCBUT, 53, 53},
  {1, 28, 1.0, 0, CCLATCH, 13, 54},
  {1, 105, 1.0, 0, CCBUT, 70, 55},
  {1, 109, 1.0, 0, CCBUT, 94, 56},
  {1, 75, 1.0, 0, CCLATCH, 2, 57}
  },
   //Yamaha RX5 2
  {
  {10, 22, 1.0, 0, TRIGGER, 60, 50},
  {10, 23, 1.0, 0, TRIGGER, 60, 50},
  {10, 24, 1.0, 0, TRIGGER, 60, 50},
  {10, 25, 1.0, 0, TRIGGER, 60, 50},
  {10, 26, 1.0, 0, TRIGGER, 60, 50},
  {10, 27, 1.0, 0, TRIGGER, 60, 50},
  {10, 28, 1.0, 0, TRIGGER, 60, 50},
  {10, 29, 1.0, 0, TRIGGER, 60, 50}
  }
};

long int valPreset = 0;
bool LoadPreset()
{	
	//bool ret= EncoderchangeValue( "Load Preset :", valPreset, 1, MAXPRESETPAGES, 3, 0, 45);
	bool ret= EncChangeVal(valPreset, 0, MAXPRESETPAGES, 1.0, DisplaySelectPresetStr);
	if( !ret ){
		return false;
		}
	if(valPreset){
		for(int i=0; i<9;i++){
			CVControls[i].Config = configChan[valPreset-1][i];
			
			CVControls[i].CVPort.PortCfg = configCV[valPreset-1][i];
			CVControls[i].CVPort.PortCfg.Ranges = CVRanges[valPreset-1][i];
			
			CVControls[i].Slider.PortCfg = configFader[valPreset-1][i];
			CVControls[i].Slider.PortCfg.Ranges = FaderRanges[valPreset-1][i];
			CVControls[i].Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.FaderMinDAC, theApp.theGlobalCfg.FaderRangeDAC);
			
			if(i<8) CVControls[i].GateBut.PortCfg = configGate[valPreset-1][i];
		}
		CVControls[8].CVPort.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxAMinDAC, theApp.theGlobalCfg.AuxARangeDAC);
		CVControls[8].Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxBMinDAC, theApp.theGlobalCfg.AuxBRangeDAC);
	}
	ClearSelectArea();
	gotoMenuSettings();
	return true;
}

void DisplaySelectPresetStr(long val)
{
static long disptim=0;
int padPos= 3, posx = 0, posy = 37;

	//Clear area used
	ClearSelectArea();

	// Display title
	theOLED->setTextSize(1);
	theOLED->setTextColor(WHITE,BLACK);
	theOLED->setCursor(posx+4,posy+5);
	theOLED->println("Preset: "); 
	// Print value
	theOLED->setCursor(posx+4,posy+5+8);
	theOLED->println(strMatr[val]);
    // Rect around input area
	theOLED->drawRect(posx,posy,14*6+padPos*12+3,18+8,WHITE); 
	if (micros() - disptim >= REFRESH_RATE) {
		theOLED->display ();
        disptim = micros();
        }
}

void ClearSelectArea(void)
{
int padPos= 3, posx = 0, posy = 45;

	//Clear area used
	theOLED->fillRect(posx,posy,14*6+padPos*12+3,18+8,BLACK); // Erase input area
}


/** @} */
