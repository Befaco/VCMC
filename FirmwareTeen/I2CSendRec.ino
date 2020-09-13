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


#ifdef USEI2C


////////////////////////////////////////////////////
// Leader and Follower functions

void I2Cmerger::ReadI2Cdata(int count){
  int i = 0;
  while (pWire->available() > 0 && count > i)
  {                                                   // loop through all but the last
    InMsg.dataRaw[i] = pWire->read(); // receive byte as a character
    i++;
    }

    InMsg.Length = i; //count; // set received flag to count, this triggers print in main loop
}


///////////////////////////////////////////////////
// Master I2C/ Leader

void I2Cmerger::SendI2CdataLeader(uint8_t addr, uint8_t *data, uint8_t l)
{
    pWire->beginTransmission(addr);
    pWire->write(data, l);
    pWire->endTransmission();
    D(printI2CData(addr, data, l));
}

void I2Cmerger::printI2CData(uint8_t addr, uint8_t *data, uint8_t l)
{
    Serial.printf("Addr:%2X: ", addr);
    for (size_t i = 0; i < l; i++)
    {
        Serial.printf("%2x - ", data[i]);
    }
    Serial.println("EOM");
}

void I2Cmerger::ReadI2CLeader(uint8_t addr, uint8_t *data, uint8_t l)
{
  ReadI2Cdata(l);
  if(l!=InMsg.Length) return; // Not enough data to read
  memcpy(data, InMsg.dataRaw, l);
}

uint16_t I2Cmerger::LeaderReceiveSimple( uint8_t addr, uint8_t Bank, uint8_t Port)
{
    // Ask for CV, Fader or Gate
    OutMsg.I2CCommand = Port;
    OutMsg.Port = Bank;
    OutMsg.Length = 2;
    SendI2CdataLeader(addr, OutMsg.dataRaw, 2);
    // Receive value
    pWire->requestFrom(addr, (uint8_t)2);
    ReadI2Cdata(2);

    D(Serial.printf("Data from 0x%x:%d/%d\n", addr, InMsg.Length, *((uint16_t *)InMsg.dataRaw)));
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
void receiveEvent(int count)
{
  theApp.I2CMerge.ReadI2Cdata(count);
  theApp.I2CMerge.ProcessInputRequest();
}

/**
 * \brief handle Tx Event (outgoing I2C data)
 * 
 */
void requestEvent(void)
{
  //DP("request");
  if (theApp.I2CMerge.InMsg.Length == 0)
  {
    pWire->write(0);
    return;
  }

  if( theApp.I2CMerge.OutMsg.Length == NOMSGLEN){
    DP("Incorrect Msg");
    return;
    }
  
  //uint16_t dataReq = CVControls[0].CVPort.PortValue;
  //pWire->write(dataReq >> 8);
  //pWire->write(dataReq & 255);
  //D(Serial.printf("Sent Requested value: %d/%d\n", dataReq, 2));

  pWire->write(theApp.I2CMerge.OutMsg.dataRaw, theApp.I2CMerge.OutMsg.Length);
  D(Serial.printf("Sent Requested value: %d/%d\n", theApp.I2CMerge.OutMsg.iValue, theApp.I2CMerge.OutMsg.Length));
  theApp.I2CMerge.InMsg.Length = NOMSGLEN;
  theApp.I2CMerge.OutMsg.Length = NOMSGLEN;
}

// TODO implement


/*
 * The function that responds to read requests over i2c.
 * This uses the port from the write request to determine which slider to send.
 */
/*
void i2cReadRequest()
{

  D(Serial.print("i2c Read\n"));

  // get and cast the value
  uint16_t shiftReady = 0;
  switch (activeMode)
  {
  case 1:
    shiftReady = (uint16_t)currentValue[activeInput];
    break;
  case 2:
    shiftReady = (uint16_t)currentValue[activeInput];
    break;
  default:
    shiftReady = (uint16_t)currentValue[activeInput];
    break;
  }

  D(Serial.printf("delivering: %d; value: %d [%d]\n", activeInput, currentValue[activeInput], shiftReady));

// send the puppy as a pair of bytes
#ifdef V125
  pWire->write(shiftReady >> 8);
  pWire->write(shiftReady & 255);
#else
  Wire.write(shiftReady >> 8);
  Wire.write(shiftReady & 255);
#endif
}
 */
#endif