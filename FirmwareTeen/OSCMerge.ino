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
#include "Definitions.h"

/** @addtogroup OSC 
 *  @{
 */

/**
 *  \file OSCMerge.ino
 *  \brief OSC I/O management functions
 */


#ifdef USEOSC

bool OSCmerger::poll(void)
{
    // Send OSC
    if( OSCOutput) sendOSC ();
    // Receive OSC
    if( OSCInput) readOSC (); 

    #ifdef PRINTDEBUG
    if( received )
    {
        Serial.printf("Received %d:",received);
        Serial.println((char*)databuf);
        received = 0;
    }
    #endif

    return true;
}


void OSCmerger::sendOSC () 
{
    static long microsOSC = 0;
    if (micros () - microsOSC > OSCINTERVAL && Serial.availableForWrite () > 20) {
        microsOSC = micros ();
        char msgTxt[100];
        uint16_t NewValue;
        static uint16_t OldValue[2][9];
        static uint16_t OldGate[9];
        int i;
        // strcpy( msgTxt, "/VCMC/CV");
		
        for (i = 0; i < 9; i++) {
            // Send port values
            NewValue = CVControls[i].CVPort.PortValue;
            if (NewValue - OldValue[0][i] > TRIMOSC || OldValue[0][i] - NewValue > TRIMOSC) {
                sprintf (msgTxt, "/VCMC/CV/%d", i + 1);
                SendOSCMsgfloat (msgTxt, NewValue / (float)ANRANGEMAX);
                OldValue[0][i] = NewValue;
            }
            NewValue = CVControls[i].Slider.PortValue;
            if (NewValue - OldValue[1][i] > TRIMOSC || OldValue[1][i] - NewValue > TRIMOSC) {
                // if(  NewValue != OldValue[1][i]){
                sprintf (msgTxt, "/VCMC/Fad/%d", i + 1);
                SendOSCMsgfloat (msgTxt, NewValue / (float)ANRANGEMAX);
                OldValue[1][i] = NewValue;
            }
            NewValue = CVControls[i].GateBut.GateStatus;
            if (NewValue != OldGate[i]) {
                sprintf (msgTxt, "/VCMC/Gate/%d", i + 1);
                SendOSCMsgint (msgTxt, NewValue);
                OldGate[i] = NewValue;
            }
        }
		
    }
}


void OSCmerger::readOSC () {

    if (getSLIPSerial()->endofPacket ()) {
        if (!msgIn.hasError ()) {
        /*#ifdef DEBUGMODE
            char outMsg[270];
                    charIn[msgInSize] = 0;
                    sprintf (outMsg, "%d,%s", msgInSize, charIn);
                    SetMessageText (outMsg); //"Slip received");
        #endif*/
            ProcessOscMsg (&msgIn);
        } else { // just empty it if there was an error
            #ifdef DEBUGMODE
            myMenu.SetMessage ("Error Slip");
            #endif
        }
        msgIn.empty (); // free space occupied by message
        msgInSize = 0;
    } else {
        if (getSLIPSerial()->available ()) {
            char rChar = getSLIPSerial()->read ();
            if (msgInSize == 0) msgIn.empty ();
            msgIn.fill (rChar);
            charIn[msgInSize] = rChar;
            msgInSize++;
            /*#ifdef DEBUGMODE
            SetMessageText ("Serial Rec", 100);
            #endif*/
            if (msgInSize > 255) { // Overflow, reset
                msgInSize = 0;
                msgIn.empty ();
                #ifdef DEBUGMODE
                myMenu.SetMessage ("Overflow");
                #endif
            }
        }
    }
}


void OSCmerger::ProcessOscMsg (OSCMessage *pMsg) {
    char msgTxt[100];
    int i;

    char outBuf[80];
    pMsg->getAddress (outBuf);

    // Commands
    if (pMsg->fullMatch ("/VCMC/Command")) {
        if (pMsg->isString (0)) {
            pMsg->getString (0, msgTxt, 100); // Get command string
            if (strcmp (msgTxt, "FullConfig") == 0) { // Send Full Config command
				sprintf (msgTxt, "/VCMC/Config/Global");
				theApp.theGlobalCfg.SaveCfgOSC (msgTxt);
                for (i = 0; i < 9; i++) {
                    sprintf (msgTxt, "/VCMC/Config/%d", i + 1);
                    CVControls[i].SaveCfgOSC (msgTxt);
                }
            }
        }
    }
	// Config messages
	int posit;
    if ((posit= pMsg->match ("/VCMC/Config"))) {
		if(pMsg->match ("/Global/GlobalCfg",posit)){
			D(SetMessageText( "Global Config msg received/"));
		} else{
			int Chan = (int)outBuf[13]-48;
			posit+=2;
			posit = 14;
			if(Chan<1 || Chan >9) return;
			/*if(pMsg->match ("/CV/InputCfg",posit)){
				#ifdef DEBUGMODE
					SetMessageText (outBuf);
				#endif
				}*/
			if(pMsg->match ("/Function",posit)){
				CVControls[Chan-1].ReadCfgOSC(pMsg);
			} else if(pMsg->match ("/CV/InputCfg",posit)){
				CVControls[Chan-1].CVPort.PortCfg.ReadBaseCfgOSC(pMsg);
			} else if(pMsg->match ("/CV/AnagCfg",posit)){
				CVControls[Chan-1].CVPort.PortCfg.ReadCfgOSC(pMsg);
			} else if(pMsg->match ("/CV/Range",posit)){
				CVControls[Chan-1].CVPort.PortCfg.Ranges.ReadCfgOSC(pMsg);
			} else if(pMsg->match ("/Fad/InputCfg",posit)){
				CVControls[Chan-1].Slider.PortCfg.ReadBaseCfgOSC(pMsg);
			} else if(pMsg->match ("/Fad/AnagCfg",posit)){
				CVControls[Chan-1].Slider.PortCfg.ReadCfgOSC(pMsg);
			} else if(pMsg->match ("/Fad/Range",posit)){
				CVControls[Chan-1].Slider.PortCfg.Ranges.ReadCfgOSC(pMsg);
			} else if(pMsg->match ("/Gate/InputCfg",posit)){
				CVControls[Chan-1].GateBut.PortCfg.ReadBaseCfgOSC(pMsg);
			} else if(pMsg->match ("/Gate/DigCfg",posit)){
				CVControls[Chan-1].GateBut.PortCfg.ReadCfgOSC(pMsg);
			} else {
				#ifdef DEBUGMODE
					SetMessageText (outBuf);
				#endif
		/*		#ifdef DEBUGMODE
					sprintf (outBuf, "Error Config /%d",Chan);
					SetMessageText (outBuf);
				#endif*/
			}
		}
	}
}


/**
 * \brief handle Rx Event (incoming I2C data)
 * 
 * \param count bytes received
 */
void receiveEvent(int count)
{
    int i =0;
    while(Wire1.available() > 0) {  // loop through all but the last
        theApp.OSCMerge.databuf[i] = Wire1.read();        // receive byte as a character
        i++;
    }
    //Wire1.read(theApp.OSCMerge.databuf, count);  // copy Rx data to databuf
    theApp.OSCMerge.received = i; //count; // set received flag to count, this triggers print in main loop
}



/**
 * \brief handle Tx Event (outgoing I2C data)
 * 
 */
void requestEvent(void)
{
    if( theApp.OSCMerge.msgOut.size()==0)
    {
        Wire1.write(0);
        return;
    }
    theApp.OSCMerge.msgOut.send(Wire1);
    D(Serial.println((char*)theApp.OSCMerge.databuf));
    theApp.OSCMerge.msgOut.empty();
}

void OSCmerger::begin(void)
{
    //begin SLIPSerial just like Serial
    SLIPSerial.begin(115200); // set this as high as you can reliably run on your platform

    // Begin I2C serial channel
    // Setup for Slave mode, address 0x66, pins 18/19, external pullups, 400kHz
    Wire1.begin(ClientPort);
    Wire1.setSDA(I2C_SDA);
    Wire1.setSCL(I2C_SCL);
    // Wire1.begin(I2C_SLAVE, 0x66, I2C_PINS_29_30, I2C_PULLUP_EXT, 400000);
    // register events
    Wire1.onReceive(receiveEvent);
    Wire1.onRequest(requestEvent);
    D(Serial.println("Init OSC Merger"));
}

void OSCmerger::SendOSCMsgint (char *address, int value) {
    //char msgTxt[100];
    //the message wants an OSC address as first argument
    //OSCMessage msg (address);
    msgOut.empty();
    msgOut.setAddress(address);
    msgOut.add (value);

    getSLIPSerial()->beginPacket ();
    msgOut.send (*getSLIPSerial());   // send the bytes to the SLIP stream
    getSLIPSerial()->endPacket (); // mark the end of the OSC Packet
    msgOut.empty ();            // REMOVE to send through I2C free space occupied by message
}


void OSCmerger::SendOSCMsgfloat (char *address, float value) {
    // char msgTxt[100];
    // the message wants an OSC address as first argument

    //OSCMessage msg (address);
    msgOut.empty();
    msgOut.setAddress(address);
    msgOut.add (value);

    getSLIPSerial()->beginPacket ();
    msgOut.send (*getSLIPSerial());   // send the bytes to the SLIP stream
    getSLIPSerial()->endPacket (); // mark the end of the OSC Packet
    msgOut.empty ();            // REMOVE to send through I2C free space occupied by message
}

#endif

/** @} */
