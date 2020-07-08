// Copyright 2016 Sergio Retamero.
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

#include "Definitions.h"

/** @addtogroup OSC 
 *  @{
 */

/**
 *  \file OSCConfigPort.h
 *  \brief OSC management functions for Config classes
 */

/** \file OSCConfigPort.ino
  \brief Functions for OSC processing 

 - Output OSC messages :<BR>
	/VCMC/Config/GlobalCfg iiiiii <BR>
	/VCMC/Config/[1-9]/Function i<BR>
	/VCMC/Config/[1-9]/CV/InputCfg iifi<BR>
	/VCMC/Config/[1-9]/CV/AnagCfg iiiiii<BR>
	/VCMC/Config/[1-9]/CV/Range iiii<BR>
	/VCMC/Config/[1-9]/Fad/InputCfg iifi<BR>
	/VCMC/Config/[1-9]/Fad/AnagCfg iiiiii<BR>
	/VCMC/Config/[1-9]/Fad/Range iiii<BR>
	/VCMC/Config/[1-9]/Gate/InputCfg iifi<BR>
	/VCMC/Config/[1-9]/Gate/DigCfg iiii<BR>

 - Input OSC messages :<BR>
	Command messages:  /VCMC/Command s [Command as text]<BR>
		String accepted:<BR>
 			FullConfig	Send all config parameters as OSC messages<BR>
 
 	Config Messages:<BR>
 		/VCMC/Config/GlobalCfg iiiiii<BR>
 		/VCMC/Config/[1-9]/Function i<BR>
 		/VCMC/Config/[1-9]/CV/InputCfg iifi<BR>
 		/VCMC/Config/[1-9]/CV/AnagCfg iiiiii<BR>
 		/VCMC/Config/[1-9]/CV/Range iiii<BR>
 		/VCMC/Config/[1-9]/Fad/InputCfg iifi<BR>
 		/VCMC/Config/[1-9]/Fad/AnagCfg iiiiii<BR>
 		/VCMC/Config/[1-9]/Fad/Range iiii<BR>
 		/VCMC/Config/[1-9]/Gate/InputCfg iifi<BR>
 		/VCMC/Config/[1-9]/Gate/DigCfg iiii<BR>
 
*/


#ifdef USEOSC
void GlobalCfg::SaveCfgOSC(char *address)
{
char MsgAddr[120];

	strcpy(MsgAddr, address);
	strcat(MsgAddr, "/GlobalCfg");
	OSCMessage msg(MsgAddr);
	msg.add( InitMinDAC).add( InitRangeDAC).add( FaderMinDAC).add( FaderRangeDAC).add( AuxAMinDAC).add( AuxARangeDAC).add( AuxBMinDAC).add( AuxBRangeDAC);
	theApp.getSLIPSerial()->beginPacket();
	msg.send(*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
	theApp.getSLIPSerial()->endPacket(); // mark the end of the OSC Packet
	msg.empty(); // free space occupied by message}
}


void GlobalCfg::ReadCfgOSC(OSCMessage *pMsg)
{
	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isInt(2) && pMsg->isInt(3) &&
		pMsg->isInt(4) && pMsg->isInt(5) && pMsg->isInt(6) && pMsg->isInt(7) )) return;
      InitMinDAC= pMsg->getInt(0);
      InitRangeDAC= pMsg->getInt(1);
      FaderMinDAC= pMsg->getInt(2);
      FaderRangeDAC= pMsg->getInt(3);
      AuxAMinDAC= pMsg->getInt(4);
      AuxARangeDAC= pMsg->getInt(5);
      AuxBMinDAC= pMsg->getInt(6);
      AuxBRangeDAC= pMsg->getInt(7);
}


/**
 *  \brief Send the configuration to OSC Serial
 *  
 *  \param [in] address Address text for OSC messasge header
 *  
 *  \details The structure to the message sent is:
 *  Message 1: address/Function/(Chanfunction)
 *  Message 2: Calls CV function to save config with header address/CV
 *  Message 3: Calls Fader function to save config with header address/Fad
 *  Message 4: Calls Gate function to save config with header address/Gate
*/
void InputControl::SaveCfgOSC (char *address) {
    char MsgAddr[120];

    strcpy (MsgAddr, address);
    strcat (MsgAddr, "/Function");
    OSCMessage msg (MsgAddr);
    msg.add (Config.Chanfunction);
    theApp.getSLIPSerial()->beginPacket ();
    msg.send (*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
    theApp.getSLIPSerial()->endPacket (); // mark the end of the OSC Packet
    msg.empty (); // free space occupied by message

    strcpy (MsgAddr, address);
    strcat (MsgAddr, "/CV");
    CVPort.PortCfg.SaveCfgOSC (MsgAddr);

    strcpy (MsgAddr, address);
    strcat (MsgAddr, "/Fad");
    Slider.PortCfg.SaveCfgOSC (MsgAddr);

    strcpy (MsgAddr, address);
    strcat (MsgAddr, "/Gate");
    GateBut.PortCfg.SaveCfgOSC (MsgAddr);
}

void InputControl::ReadCfgOSC(OSCMessage *pMsg)
{
	if( !(pMsg->isInt(0) )) return;
	
      Config.Chanfunction= (CtrlFunctions)pMsg->getInt(0);

}


void InputPortCfg::SaveCfgOSC(char *address)
{
char MsgAddr[120];

	strcpy(MsgAddr, address);
	strcat(MsgAddr, "/InputCfg");
	OSCMessage msg(MsgAddr);
	msg.add( MIDIChannel).add( ControllerNumber).add( ClockDivider).add(ClockShift).add( ControllerValue).add(NoteToSend);

	theApp.getSLIPSerial()->beginPacket();
	msg.send(*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
	theApp.getSLIPSerial()->endPacket(); // mark the end of the OSC Packet
	msg.empty(); // free space occupied by message
}

/*
void InputPortCfg::ReadBaseCfgOSC(OSCMessage *pMsg)
{

	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isFloat(2) && pMsg->isInt(3) )) return;
   
	MIDIChannel= pMsg->getInt(0);
    ControllerNumber= pMsg->getInt(1);
    ClockDivider= pMsg->getFloat(2);
    ClockShift= pMsg->getInt(3);
				#ifdef DEBUGMODE
				    char outBuf[80];
					//pMsg->getAddress (outBuf);
					//SetMessageText (outBuf+12);
					sprintf(outBuf, "%ld/%ld/%4.2f/%ld", pMsg->getInt(0), pMsg->getInt(1), (float)pMsg->getFloat(2), pMsg->getInt(3));
					SetMessageText (outBuf);
				#endif   
}
*/

void AnInputPortCfg::ReadBaseCfgOSC(OSCMessage *pMsg)
{

	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isFloat(2) && pMsg->isInt(3) && pMsg->isInt(4) && pMsg->isInt(5) )) return;
   
	MIDIChannel= (byte)pMsg->getInt(0);
    ControllerNumber= pMsg->getInt(1);
    ClockDivider= pMsg->getFloat(2);
    ClockShift= pMsg->getInt(3);
      ControllerValue= pMsg->getInt(4);
      NoteToSend= pMsg->getInt(5);
	#ifdef DEBUGMODE
		char outBuf[80];
		//pMsg->getAddress (outBuf);
		//SetMessageText (outBuf+12);
		sprintf(outBuf, "%ld/%ld/%4.2f/%ld/%ld/%ld/", pMsg->getInt(0), pMsg->getInt(1), (float)pMsg->getFloat(2), 
			pMsg->getInt(3), pMsg->getInt(4), pMsg->getInt(5));
		SetMessageText (outBuf);
	#endif   
}



void DigPortCfg::ReadBaseCfgOSC(OSCMessage *pMsg)
{

	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isFloat(2) && pMsg->isInt(3) && pMsg->isInt(4) && pMsg->isInt(5) )) return;
   
	MIDIChannel= (byte)pMsg->getInt(0);
    ControllerNumber= pMsg->getInt(1);
    ClockDivider= pMsg->getFloat(2);
    ClockShift= pMsg->getInt(3);
      ControllerValue= pMsg->getInt(4);
      NoteToSend= pMsg->getInt(5);
				#ifdef DEBUGMODE
				    char outBuf[80];
					//pMsg->getAddress (outBuf);
					//SetMessageText (outBuf+12);
					sprintf(outBuf, "%ld/%ld/%4.2f/%ld/%ld/%ld/", pMsg->getInt(0), pMsg->getInt(1), (float)pMsg->getFloat(2), 
						pMsg->getInt(3), pMsg->getInt(4), pMsg->getInt(5));
					SetMessageText (outBuf);
				#endif   
}




void AnInputPortCfg::SaveCfgOSC(char *address)
{
char MsgAddr[120];

	InputPortCfg::SaveCfgOSC(address);
	Ranges.SaveCfgOSC(address);

	strcpy(MsgAddr, address);
	strcat(MsgAddr, "/AnagCfg");
	OSCMessage msg(MsgAddr);
	msg.add( MIDIfunction).add( ClipLow).add( ClipHigh)/*.add( Offset).add( Amp)*/.add( RangeBipolar)
	.add( NRPNparMSB).add( NRPNparLSB);
	theApp.getSLIPSerial()->beginPacket();
	msg.send(*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
	theApp.getSLIPSerial()->endPacket(); // mark the end of the OSC Packet
	msg.empty(); // free space occupied by message
}



void AnInputPortCfg::ReadCfgOSC(OSCMessage *pMsg)
{
	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isInt(2) && 
	pMsg->isInt(3) && pMsg->isInt(4) && pMsg->isInt(5) )) return;
	
      MIDIfunction= pMsg->getInt(0);
      ClipLow= pMsg->getInt(1);
      ClipHigh= pMsg->getInt(2);
      RangeBipolar= pMsg->getInt(3);
      NRPNparMSB= pMsg->getInt(4);
      NRPNparLSB= pMsg->getInt(5);
}



void DigPortCfg::SaveCfgOSC(char *address)
{
char MsgAddr[120];

	InputPortCfg::SaveCfgOSC(address);

	strcpy(MsgAddr, address);
	strcat(MsgAddr, "/DigCfg");
	OSCMessage msg(MsgAddr);
	msg.add( MIDIfunction).add( DelayGate);

	theApp.getSLIPSerial()->beginPacket();
	msg.send(*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
	theApp.getSLIPSerial()->endPacket(); // mark the end of the OSC Packet
	msg.empty(); // free space occupied by message

}


void DigPortCfg::ReadCfgOSC(OSCMessage *pMsg)
{
	if( !(pMsg->isInt(0) && pMsg->isInt(1) )) return;
	
      MIDIfunction= pMsg->getInt(0);
      DelayGate= pMsg->getInt(1);
}

void RangeConv::SaveCfgOSC (char *address) {
    char MsgAddr[120];

    strcpy (MsgAddr, address);
    strcat (MsgAddr, "/Range");
    OSCMessage msg (MsgAddr);
    msg.add (minMIDI).add (rangeMIDI).add (minDAC).add (rangeDAC);//.add (UseMultiPoint);

    theApp.getSLIPSerial()->beginPacket ();
    msg.send (*theApp.getSLIPSerial()); // send the bytes to the SLIP stream
    theApp.getSLIPSerial()->endPacket (); // mark the end of the OSC Packet
    msg.empty (); // free space occupied by message
}


void RangeConv::ReadCfgOSC(OSCMessage *pMsg)
{
	if( !(pMsg->isInt(0) && pMsg->isInt(1)&& pMsg->isInt(2) && pMsg->isInt(3))) return;


      minMIDI= pMsg->getInt(0);
      rangeMIDI= pMsg->getInt(1);
      minDAC= pMsg->getInt(2);
      rangeDAC= pMsg->getInt(3);
}


	
#endif

/** @} */
