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
#include "I2CCore.h"

#define PRINTDEBUG

#ifdef USEI2C
#ifdef PRINTDEBUG
#define D(x) x		/// Shorthand for PRINTDEBUG
#define DP(x) Serial.println(x)		/// Shorthand for Deug serial print
#else
#define D(x)
#define DP(x)
#endif

////////////////////////////////////////////////////
// Leader and Follower functions for Teletype implementation
void tele_ii_tx(uint8_t addr, uint8_t *data, uint8_t l)
{
    pWire->beginTransmission(addr);
    pWire->write(data, l);
    pWire->endTransmission();
    D(I2Ccore::printI2CData(addr, data, l));
}

int lenRead = 0;
void tele_ii_rx(uint8_t addr, uint8_t *data, uint8_t l)
{
  int i = 0;
  while (pWire->available() > 0 && l > i)
  {                                                   // loop through all but the last
    data[i] = pWire->read(); // receive byte as a character
    i++;
    }

    lenRead = i; //count; // set received flag to count, this triggers print in main loop

}


///////////////////////////////////////////////////
// Master I2C/ Leader
void I2Ccore::SendI2CdataLeader(uint8_t addr, uint8_t *data, uint8_t l)
{
  tele_ii_tx( addr, data, l);
  D(printI2CData(addr, data, l));
}

void I2Ccore::printI2CData(uint8_t addr, uint8_t *data, uint8_t l)
{
    Serial.printf("Addr:%2X: ", addr);
    for (size_t i = 0; i < l; i++)
    {
        Serial.printf("%2x - ", data[i]);
    }
    Serial.printf("EOM\n");
}

void I2Ccore::ReadI2Cdata(int count){
  tele_ii_rx( 0, InMsg.dataRaw, count);
  InMsg.Length = lenRead;
}

void I2Ccore::ReadI2CLeader(uint8_t addr, uint8_t *data, uint8_t l)
{
  ReadI2Cdata(l);
  if(l!=InMsg.Length) return; // Not enough data to read
  memcpy(data, InMsg.dataRaw, l);
}

uint16_t I2Ccore::LeaderReceiveSimple( uint8_t addr, uint8_t Bank, uint8_t Port)
{
    // Ask for CV, Fader or Gate
    OutMsg.I2CCommand = Port;
    OutMsg.Destination = Bank;
    OutMsg.Length = 2;
    SendI2CdataLeader(addr, OutMsg.dataRaw, 2);
    // Receive value
    pWire->requestFrom(addr, (uint8_t)2);
    ReadI2Cdata(2);

    D(Serial.printf("Data from 0x%x:%d/%d\n", addr, InMsg.Length, InMsg.data16Raw[0]));
    InMsg.Length = NOMSGLEN;
    return InMsg.iValue;
}


///////////////////////////////////////////////////
// Slave I2C/ Follower

/**
 * \brief handle Rx Event (incoming I2C data)
 * 
 * \param count bytes received
 */
void I2Ccore::receiveEvent(int count)
{
  I2CCore.ReadI2Cdata(count);
  I2CCore.ProcessInputRequest();
}

/**
 * \brief handle Tx Event (outgoing I2C data)
 * 
 */
void I2Ccore::requestEvent(void)
{
  //DP("request");
  if (I2CCore.InMsg.Length == 0)
  {
    pWire->write(0);
    return;
  }

  if( I2CCore.OutMsg.Length == NOMSGLEN){
    DP("Incorrect Msg");
    return;
    }
  
  //uint16_t dataReq = CVControls[0].CVPort.PortValue;
  //pWire->write(dataReq >> 8);
  //pWire->write(dataReq & 255);
  //D(Serial.printf("Sent Requested value: %d/%d\n", dataReq, 2));

  pWire->write(I2CCore.OutMsg.dataRaw, I2CCore.OutMsg.Length);
  D(Serial.printf("Sent Requested value: %d/%d\n", I2CCore.OutMsg.data16Raw[0], I2CCore.OutMsg.Length));
  I2CCore.InMsg.Length = NOMSGLEN;
  I2CCore.OutMsg.Length = NOMSGLEN;
}

#endif