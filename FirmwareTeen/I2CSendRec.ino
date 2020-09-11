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



void I2Cmerger::SendI2Cint(uint8_t model, uint8_t cmd, uint8_t devicePort, int16_t value)
{
    OutMsg.Command = cmd;
    OutMsg.Port = devicePort;
    OutMsg.iValue = value;

    SendI2Cdata(model, OutDatabuf, 4);

    /* D(Serial.printf("Sent Int %d:%d,%d,%d,%d\n", value, 
        outDatabuf[0], outDatabuf[1], outDatabuf[2], outDatabuf[3])); */
}


void I2Cmerger::SendI2Cdata(uint8_t addr, uint8_t *data, uint8_t l)
{
    pWire->beginTransmission(addr);
    pWire->write(data, l);
    pWire->endTransmission();
    D(printI2CData(addr, data, l));
}

void printI2CData(uint8_t addr, uint8_t *data, uint8_t l)
{
    Serial.printf("Addr:%2X: ", addr);
    for (size_t i = 0; i < l; i++)
    {
        Serial.printf("%2x - ", data[i]);
    }
    Serial.println("EOM");
}


/**
 * \brief handle Rx Event (incoming I2C data)
 * 
 * \param count bytes received
 */
void receiveEvent(int count)
{
    int i =0;
    while(pWire->available() > 0) {  // loop through all but the last
        theApp.I2CMerge.InDatabuf[i] = pWire->read();        // receive byte as a character
        i++;
    }
    
    theApp.I2CMerge.received = i; //count; // set received flag to count, this triggers print in main loop
}



/**
 * \brief handle Tx Event (outgoing I2C data)
 * 
 */
void requestEvent(void)
{
  if (theApp.I2CMerge.received == 0)
  {
    pWire->write(0);
    return;
  }
  DP("request");
  uint16_t dataReq = theApp.I2CMerge.ProcessInputRequest();
  pWire->write(dataReq >> 8);
  pWire->write(dataReq & 255);
  D(Serial.printf("Requested value: %d\n", dataReq));
}




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