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

/** @addtogroup SysEx
 *  @{
 */

/**
 *  \file SysExSendRec.ino
 *  \brief SysEx Send/Receive support
 */  


/**
 * \brief Send VCMC Config via SysEX
 * 
 * \param page Valid pages 0 and 1. Set to -1 to send current page
 */
bool SaveLoadClass::SaveCfgSysEx(int page)
{
  byte Buffer[PageSize];
    if (page < 0 || page > MAXSAVEPAGES - 1)
    {        
      //page = CurrentPage;
      // Send individual messages
				theApp.theGlobalCfg.SaveCfgSysEx ();
        for (int i = 0; i < 9; i++) {
          CVControls[i].SaveCfgSysEx ();
        }
    } else
    {    
      EEPROM.get(PageBase[page], Buffer);
      SaveCfgSysEx(Buffer, PageSize);
    }
    return true;
}

/**
 * \brief Save To Single SysEx message
 * 
 * \param pSource  Data pointer
 * \param uLengthSource  number of bytes to send (Max 49 bytes)
 * \param bSlot 0ssspppp sss=Port pppp=parameter
 * \param bType 0tttcccc ttt= Msg type cccc= channel
 * \return true 
 * \return false 
 */
bool SaveLoadClass::SaveCfgSingleSysEx(byte *pSource, uint16_t uLengthSource, byte bSlot,byte bType)
{
  VCMCSysExPacket SysExPacket;

  if( uLengthSource>49)
    return false; // Max length exceded
  
  SysExPacket.Type = bType;
  SysExPacket.Slot = bSlot;

  // Encode data to MIDI 7 bit format
  uint8_t dumpMemory[56]={0};
  
  unsigned int encLen = midi::encodeSysEx((const byte *)pSource, dumpMemory, uLengthSource);
  if( encLen>56)
    return false;
  SysExPacket.Length = encLen;
  memcpy(SysExPacket.pData, dumpMemory, encLen);
  usbMIDI.sendSysEx(sizeof(SysExPacket), (const uint8_t *)&SysExPacket, true);

  #ifdef PRINTDEBUG
  Serial.print("Send Single SysEx Packet #: ");
  Serial.print(bSlot);
  Serial.print(" / ");
  Serial.print(bType);
  Serial.print(" data bytes:");
  Serial.print((int)sizeof(SysExPacket));
  Serial.print(" / ");
  Serial.print(uLengthSource);
  Serial.print(" / ");
  Serial.println(encLen);
  #endif

  return true;
}

/**
 * \brief Function to send a byte array through several SysEX 
 * 
 * \param pSource Data pointer
 * \param uLengthSource number of bytes to send
 * \return true Success
 * \return false Error on processing
 */
bool SaveLoadClass::SaveCfgSysEx(byte *pSource, uint16_t uLengthSource)
{
    uint8_t dumpMemory[uLengthSource * 8 / 7 + 8];
    uint16_t posDump = 0;
    union {
        unsigned encLen = 0;
        uint8_t bLen[2];
    };
    union {
        unsigned long ulMicros;
        uint8_t nIDMsg[4];
    };
    GenericSysExPacket SysExPacket;
    uint8_t pCount = 0, csum = 0;
    uint8_t MIDIencodedData[9];
    unsigned encDataLen;
    unsigned i;

    #ifdef PRINTDEBUG
    Serial.print("Total Unencoded: ");
    Serial.println(uLengthSource);
    Serial.println("Unencoded Data: ");
    for (i = 0; i < uLengthSource; i++)
    {
        Serial.print(pSource[i] >> 4, HEX);
        Serial.print(pSource[i] & 0x0F, HEX);
        Serial.print(" ");
    }
    Serial.println("<-End Data");
    #endif
    // Calculate checksum
    csum = pSource[0];
    for (i = 1; i < uLengthSource; i++)
        csum ^= pSource[i];
    csum &= 0x7F; // Strip to MIDI standard
    #ifdef PRINTDEBUG
    Serial.print("CheckSum: ");
    Serial.println(csum);
    #endif

    // encode to 7 bit MIDI leavinh two bytes for message length
    encLen = midi::encodeSysEx((const byte *)pSource, dumpMemory + 2, uLengthSource);
    dumpMemory[0] = csum;
    encLen += 2; // Add checksum byte
    #ifdef PRINTDEBUG
    Serial.print("Encoded Length: ");
    Serial.println(encLen);
    #endif
    // Calculate number of messages
    pCount = encLen / SysExpacketDataLength;
    pCount += (pCount * SysExpacketDataLength < encLen) ? 1 : 0;
    dumpMemory[1] = pCount;
    #ifdef PRINTDEBUG
    Serial.print("# Messages: ");
    Serial.println(pCount);
    #endif
    pCount = 0;

    // Store data length to beginning of message (Not including these two bytes nor trailing
    // checksum byte) COnvert to 7 bit length
    // encDataLen = midi::encodeSysEx((const byte*)&encLen, MIDIencodedData, 2);
    // memcpy( dumpMemory+encLen, MIDIencodedData, encDataLen);

    // Create a message ID using millis function (useful as timing also?)
    ulMicros = millis();
    // Mark first 16 bits as CFGDATATAG to mark as a config message
    ulMicros = (ulMicros & 0x0000FFFF) + (((unsigned long)CFGDATATAG) << 16);
    encDataLen = midi::encodeSysEx((const byte *)nIDMsg, MIDIencodedData, 4);
    memcpy(SysExPacket.ID, MIDIencodedData, encDataLen);

    // Send SysEx messages with length + encoded data + checksum.
    // Each SysEx contains a maximum of SysExpacketDataLength bytes
    unsigned long pausMil = millis();
    while (posDump < encLen - SysExpacketDataLength)
    {
        if (pausMil + 30 < millis())
        { // Wait between messages sent out
            #ifdef PRINTDEBUG
            Serial.print("Send SysEx Packet #: ");
            Serial.print(pCount);
            Serial.print(" / ");
            Serial.print(posDump);
            Serial.println(" data bytes");
            #endif
            SysExPacket.pCounter = pCount;
            pCount++;
            memcpy(SysExPacket.pData, dumpMemory + posDump, SysExpacketDataLength);
            posDump += SysExpacketDataLength;
            usbMIDI.sendSysEx(sizeof(SysExPacket), (const uint8_t *)&SysExPacket, true);
            pausMil = millis();
        }
    }
    if (posDump >= encLen - SysExpacketDataLength)
    {
        #ifdef PRINTDEBUG
        Serial.print("Last Send SysEx Packet #: ");
        Serial.print(pCount);
        Serial.print(" / ");
        Serial.print(posDump);
        Serial.println(" data bytes");
        #endif
        SysExPacket.pCounter = pCount;
        memcpy(SysExPacket.pData, dumpMemory + posDump, encLen - posDump);
        SysExPacket.pData[encLen - posDump] = 0xF7; // Mark the EOX
        usbMIDI.sendSysEx(sizeof(SysExPacket), (const uint8_t *)&SysExPacket, true);
    }
    return true;
}

/**
 * \brief this function will be called when a SysEx message is received
 * 
 * \param SysExLength 
 * \param RecArray 
 * \return true 
 * \return false 
 */
bool SaveLoadClass::ReceivedSysEx(unsigned SysExLength, const byte *RecArray)
{
    GenericSysExPacket *SysExPacket = (GenericSysExPacket *)RecArray;
    unsigned long RecPacketID;

  #ifdef PRINTDEBUG
    Serial.print("SysEx Device: ");
    Serial.println(SysExPacket->Dev, HEX);
  #endif
    if (SysExPacket->Dev != 0x7D){
        #ifdef PRINTDEBUG
          Serial.print("Incorrect SysEx Device: ");
          Serial.println(SysExPacket->Dev, HEX);
        #endif
        return false; // Not for us!
      }
  #ifdef PRINTDEBUG
    Serial.print("SysEx Received length: ");
    Serial.println(SysExLength);
  #endif
    if (SysExLength > 64 || SysExLength < 10)
    {
        DP("SysEx size not valid ");
        SetMessageText("SysEx size not valid");
        return false; // Not expected size SysEx
    }

    if(SysExPacket->ID[0]==0x32)
    {
        // Single message SysEx received
        return ReceivedSingleSysEx(SysExLength, RecArray);
    }
    
    // Decode message ID
    midi::decodeSysEx((const byte *)SysExPacket->ID, (byte *)&RecPacketID, 5);
  #ifdef PRINTDEBUG
    Serial.print("SysEx Received ID: ");
    Serial.println(RecPacketID, HEX);
  #endif
    //if ((RecPacketID >> 28) != 6) {
    if ((RecPacketID >> 16) != CFGDATATAG)
    {
        DP("SysEx Received ID not valid ");
        //SetMessageText ("SysEx ID not valid");
        myMenu.setupPopup("SysEx ID not valid", 1000, 10, 20, 0);
        return false; // Not a config message, should begin with 0x60000000
    }
    RecPacketID = RecPacketID & 0x0FFFFFFF; // Remove check bits

    // Check if message is already received
    if (PacketsRec[SysExPacket->pCounter] && ulPacketID == RecPacketID)
    {
      #ifdef PRINTDEBUG
      Serial.print("SysEx Packet #: ");
      Serial.print(SysExPacket->pCounter);
      Serial.println(" already received");
      #endif
      return false;
    }
    char outBuf[80];
    strncpy(outBuf, "SysEx Rec:", 11);
    padvalue(SysExPacket->pCounter, outBuf + 10, 2);
    outBuf[2 + 10] = '/';
    padvalue(nPacketsRec, outBuf + 3 + 10, 2);
    SetMessageText(outBuf);
  #ifdef PRINTDEBUG
    Serial.print("SysEx Packet #: ");
    Serial.print(SysExPacket->pCounter);
    Serial.print(" / ");
    Serial.println(nPacketsRec);
  #endif

    // If this is the first message or a new message ID, initialise variables
    if (!nPacketsRec || ulPacketID != RecPacketID)
    {
        ResetReception();
        ulPacketID = RecPacketID;
        nPacketsRec = 1;
        MidiMerge.tagProcessSysex = 0;
    }
    else
        nPacketsRec++;
    PacketsRec[SysExPacket->pCounter] = 1;

    if (SysExPacket->pCounter == 0)
    {                                          // First message received
        msgCsum = SysExPacket->pData[0];       // Store Checksum
        nPacketsTotal = SysExPacket->pData[1]; // Store # packages
        #ifdef PRINTDEBUG
        Serial.print("SysEx CheckSum: ");
        Serial.println(msgCsum);
        Serial.print("SysEx Msgs to receive: ");
        Serial.println(nPacketsTotal);
        #endif
    }
  // Store data
  #ifdef PRINTDEBUG
    Serial.println("SysEx Data: ");
    for (unsigned i = 0; i < SysExLength - 9; i++)
    {
        Serial.print(SysExPacket->pData[i] >> 4, HEX);
        Serial.print(SysExPacket->pData[i] & 0x0F, HEX);
    }
    Serial.println("<-End Data");
  #endif
    memcpy(rcvSysEx + SysExPacket->pCounter * SysExpacketDataLength, SysExPacket->pData, SysExLength - 9);
    nBytesRec += SysExLength - 9;
  #ifdef PRINTDEBUG
    Serial.print("SysEx Total Received: ");
    Serial.println(nBytesRec);
  #endif

    if (IsSysExRecComplete())
    {
        SetMessageText("SysEx Config Received!");
    }
  // Process finished message
  #ifdef PRINTDEBUG
    if (!IsSysExRecComplete())
        return true;
    Serial.println("Encoded Data: ");
    for (unsigned i = 0; i < nBytesRec; i++)
    {
        Serial.print(rcvSysEx[i] >> 4, HEX);
        Serial.print(rcvSysEx[i] & 0x0F, HEX);
    }
    Serial.println("<-End Data");
    byte csum = calcCSum(); // Decode data and get Checksum
    Serial.print("SysEx Total Decoded: ");
    Serial.println(nDecoded);
    Serial.println("Decoded Data: ");
    for (unsigned i = 0; i < nDecoded; i++)
    {
        Serial.print(rcvData[i] >> 4, HEX);
        Serial.print(rcvData[i] & 0x0F, HEX);
        Serial.print(" ");
    }
    Serial.println("<-End Data");
    Serial.print("SysEx CheckSum: ");
    Serial.println(msgCsum);
    Serial.print("SysEx Data Checksum: ");
    Serial.println(csum);
  #endif

    return true;
}

bool SaveLoadClass::ReceivedSingleSysEx(unsigned SysExLength, const byte *RecArray)
{
  VCMCSysExPacket *SysExPacket = (VCMCSysExPacket *)RecArray;
  byte bType = SysExPacket->Type>>4;
  byte bChannel = SysExPacket->Type & 0xf;
  byte bPort = (SysExPacket->Slot>>4);
  byte bParam = SysExPacket->Slot &0xf;

#ifdef PRINTDEBUG
    Serial.print("Single SysEx Message Receive: ");
    Serial.println(SysExPacket->ID, HEX);
    Serial.print(" Type: ");
    Serial.print(bType);
    Serial.print(" / Channel: ");
    Serial.print(bChannel);
    Serial.print(" / Port: ");
    Serial.print(bPort);
    Serial.print(" / Par: ");
    Serial.println(bParam);
  #endif
  if(bType==DUMPCHANNEL)
  {
    byte DecodedData[64] = {0};
    if(SysExPacket->Length>55)
      return false; //Too much data
    unsigned int decLen = midi::decodeSysEx((const byte *)SysExPacket->pData, (byte *)&DecodedData, SysExPacket->Length);

    if( bChannel== 0) // Show popup only for Global Config
      myMenu.setupPopup("SysEx Received", 1000, 1, 50, 0);
    //Channhel Dump
    if(bChannel==0)
      return theApp.theGlobalCfg.ReadCfgSysEx(DecodedData, decLen, bPort);
    else if(bChannel<10)
      return theApp.Controls[bChannel-1].ReadCfgSysEx(SysExPacket, DecodedData, decLen);
  } else if(bType==SYSEXCOMMAND){ // SysEx Command received
    if(bChannel==0){ // Global command
      theApp.ProcessSysExCommand(SysExPacket);
    }
  }
  return false;
}


// See http://www.personal.kent.edu/~sbirch/Music_Production/MP-II/MIDI/midi_file_dump_messages.htm
/*
const uint8_t FileDumpSysExpacketDataLength=56;
typedef struct {
    byte sysExheader = 0xF0;
    byte fDump = 0x7E;
    byte Dev = 0x7D; // educational purpose
    byte fDat1 = 07;
    byte fDat2 = 02;
    byte pCounter = 0; // Packet number
    byte nBytes = FileDumpSysExpacketDataLength; // Actual data bytes in the message
    byte pData[FileDumpSysExpacketDataLength];
    byte EOX = 0xF7;
    } FileDumpSysExPacket;
*/

/** @} */
