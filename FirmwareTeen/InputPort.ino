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

/** @addtogroup InputPortsGroup
* @{
*/

/**
 *  \file InputPort.ino
 *  \brief Definition of base class functions related to both Analog and Digital input ports
 */

uint32_t AcumInputInterv = 0;
uint32_t AcumOutputInterv = 0;
int32_t AcumDif = 0;
int32_t PrevDif = 0;

//extern CalModes calMode;
//extern int PointSelected;
//int32_t NegPercentComp=100  ;

/**
 *  \brief  Every clock received, recalculate interval for MIDI Clock sent
 *  
 *  \param [in] float clkDiv Clock Divider 
 *  \param [in] int8_t bitShift Clock shift 
 *  \return true when Clock processed 
 *  
 *  \details This functions perform two main tasks \n
 *  1. Calculate new interval for clock MIDI \n
 *      The interval is defined by two consecutive calls to this function, so the first call returns false.\n
 *      The calculated interval is filtered to avoid changes on rate due to small variations.\n
 *  2. Send the first clock message.\n
 *      When ClockDivider is 1 this is the only message sent\n
 *  See DigitalPort::ReadPort() and AnalogPort::ReadPort() for more details on processing  when ClockDivider is bigger than 1
 */
bool InputPort::ClockReceived (float clkDiv, int8_t bitShift) {
    uint32_t NewInterval;
	union{
		uint32_t Inter;
		uint16_t Word[2];
	} NewInt;

    #ifdef DEBUGMODE
    char Msg[80];
    sprintf (Msg, "ClockRcv (%d): ", PortNumber);
    Serial.print (Msg);
    #endif
    CurrentClock = micros ();
    // Check if this is the first or second clock received
    if (IntervalMIDIClock == 0) {
        // It is the first clock, store Initial clock
        if (PreviousClock == 0) {
            PreviousClock = PreviousMIDIClock = CurrentClock;
            return false;
        }
    }
    #ifdef USEREADINTERR
    noInterrupts ();
    #endif
    // It is the second clock, store Interval
    ClockCounter = INPUTCLKDIVIDER;//clkDiv; // getClockDivider();

    NewInt.Inter = IntervalClock = CurrentClock - PreviousClock;

    // Use filter for the input
    AcumDif = AcumInputInterv - AcumOutputInterv - (CurrentClock - PreviousMIDIClock); //- PrevDif;
    PrevDif = AcumInputInterv - AcumOutputInterv;
    //PrevDif = AcumDif;

    
    /*#define MAXDIFCOMP 1000
    if( PrevDif>MAXDIFCOMP)
        NewInt.Inter += MAXDIFCOMP;
    else if( PrevDif<-MAXDIFCOMP)
        NewInt.Inter += -MAXDIFCOMP*NegPercentComp/100;
    else if( PrevDif<0)
        NewInt.Inter += PrevDif*NegPercentComp/100;
    else
        NewInt.Inter += PrevDif; */
    /*  if(PrevDif>0 )
        NewInt.Inter = IntervalClock + PrevDif / 10;
    else
    */        NewInt.Inter = (int32_t)IntervalClock + PrevDif / 10;

    // Filter first 16 bits	
    FilteredInterval->update (NewInt.Word[0]);
	NewInt.Word[0] = FilteredInterval->getValue();
	
	NewInterval = NewInt.Inter;
    unsigned long IntClock = IntervalClock;
    if( bitShift > 0){
		NewInterval = NewInterval << bitShift;
        IntClock = IntClock << bitShift;
    } else if( bitShift <0){
		NewInterval = NewInterval >> (bitShift*-1);
        IntClock = IntClock >> (bitShift*-1);
	}

    AcumInputInterv += IntClock;
    AcumOutputInterv += NewInterval;

    NewInterval = NewInterval / clkDiv / INPUTCLKDIVIDER ;
    // Roung BPM to integer value
	/* float MIDIBPM = 60000.0/(NewInterval/1000.0)/24.0;
    MIDIBPM = roundf(MIDIBPM);
    NewInterval= 60000.0/(MIDIBPM/1000.0)/24.0; */

    //uint32_t dif = (NewInterval > IntervalMIDIClock)?NewInterval - IntervalMIDIClock:IntervalMIDIClock - NewInterval;
    //if( dif*100 > NewInterval || (calMode==SetClockMode && PointSelected==3)) // 10% difference or fine tuning clock
        IntervalMIDIClock = NewInterval;

    #ifdef DEBUGMODE
    Serial.print ("      /");
    Serial.print (ClockCounter);
    /* Serial.print ("/");
    Serial.print (clkDiv);
    Serial.print ("/");
    Serial.print (bitShift);
    Serial.print ("    /");
    Serial.print (NewInt.Word[0]); */
    Serial.print ("/");
    Serial.print (NewInt.Inter);
    Serial.print ("/");
    Serial.print (IntervalClock);
    Serial.print ("/");
    /* Serial.print (NewInterval);
    Serial.print ("/"); */
    Serial.print (IntervalMIDIClock);
    Serial.print ("/");
    Serial.print (AcumDif);
    /* Serial.print ("/");
    Serial.print (PrevDif); */
    Serial.print ("/");
    Serial.print (AcumInputInterv);
    Serial.print ("/");
    Serial.print (AcumOutputInterv);
    
    Serial.print ("/");
    Serial.print ((int32_t)(IntervalClock-NewInt.Inter));
    /* Serial.print ("/");
    Serial.print((int32_t)(CurrentClock - PreviousMIDIClock)); */
    Serial.print ("/");
    Serial.println(PrevDif);//(int32_t)(AcumInputInterv)-AcumOutputInterv);
    #endif

    PreviousClock = CurrentClock;

    #ifdef USEREADINTERR
    interrupts ();
    #endif

    return true;
}


/**
 *  \brief Process MIDI Clock
 *  
 *  \details More details
 */
void InputPort::ProcessClock(void){
	long cClock = micros ();
	if (IntervalMIDIClock < (cClock - PreviousMIDIClock)) {
		// Send next MIDI Clock
        MidiMerge.sendRealTime (usbMIDI.Clock);
        //AcumOutputInterv += cClock - PreviousMIDIClock;
        //AcumOutputInterv += IntervalMIDIClock;
        
        PreviousMIDIClock += IntervalMIDIClock;
        }
	// Check if we missed NUMBERMISSEDCLOCKS ticks to stop
	if( IntervalClock * NUMBERMISSEDCLOCKS < (cClock - PreviousClock)){
		IntervalClock= PreviousMIDIClock= PreviousClock= IntervalMIDIClock = 0; ClockCounter=0;
        AcumOutputInterv = AcumInputInterv = PrevDif = AcumDif = 0;
    }
}



/** @} */ 