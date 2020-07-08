
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

/************************************************************************************
 SERIAL
 ************************************************************************************/

//the Serial communication to the Arduino
Serial serial;

String[] serialRateStrings = {
    "300", "1200", "2400", "4800", "9600", "14400",
    "19200", "28800", "38400", "57600", "115200"};
int baud = 115200;
int serialListNumber = 0;

ArrayList<Byte> serialBuffer = new ArrayList<Byte>();

void setupSerial()
{
  serial = new Serial(this, Serial.list()[serialListNumber], baud);
}

void stopSerial()
{
  serial.stop();
}

void serialEvent(Serial serial)
{
  //decode the message
  while (serial.available() > 0)
  {
    slipDecode(byte(serial.read()));
  }
}

OscMessage oscMsg;

// Send OSC message to both serial and UDP
void SendOSCMsg(OscMessage comMsg)
{
  if (!applicationRunning)
    return;
  println(comMsg);
  byte[] buffer = new byte[comMsg.getBytes().length];
  //copy the buffer over
  for (int i = 0; i < comMsg.getBytes().length; i++)
  {
    buffer[i] = comMsg.getBytes()[i];
  }
  serialSend(buffer);
  UDPSendBuffer(buffer);
}

// Send received serial Msg to UDP
void SerialSendToUDP()
{
  if (serialBuffer.size() < 1)
    return;

  oscMsg = oscDataReady();
  byte[] buffer = new byte[serialBuffer.size()];
  //copy the buffer over
  for (int i = 0; i < serialBuffer.size(); i++)
  {
    buffer[i] = serialBuffer.get(i);
  }
  //send it off
  UDPSendBuffer(buffer);
/*
  String bufSer = new String(buffer);
  //buf = bufSer + "\n";
  monitorList.add((String)bufSer);
  // remove items from beginning of the monitor list when size is bigger than
  // set monitorListLength
  while (monitorList.size() > monitorListLength)
  {
    monitorList.remove(0);
  }
*/
  //clear the buffer
  serialBuffer.clear();
  //light up the indicator
  drawIncomingSerial();
}

void serialSend(byte[] data)
{
  //encode the message and send it
  for (int i = 0; i < data.length; i++)
  {
    slipEncode(data[i]);
  }
  //write the eot
  serial.write(eot);
}

////////////////////////////////////////////////////
// Convert to OSCP5 Message

NetAddress remoteOSC;
OscP5 oscP5;

int byteArrayToInt(int[] b, int offset)
{
  int value = 0;
  for (int i = 0; i < 4; i++)
  {
    int shift = (4 - 1 - i) * 8;
    value += (b[i + offset] & 0x000000FF) << shift;
  }
  return value;
}

float arr2float(byte[] buf, int pos)
{
  ByteBuffer bb = ByteBuffer.allocate(4);
  bb.put(buf, pos, 4);
  return bb.getFloat(0);
}

OscMessage oscDataReady()
{
  //////Provisional hasta usar solo un tipo de buffer
  if (serialBuffer.size() < 1)
    return null;
  ArrayList serBuffer = new ArrayList();
  serBuffer.clear();
  //copy the buffer over
  for (int i = 0; i < serialBuffer.size(); i++)
  {
    serBuffer.add(int(serialBuffer.get(i)));
  }

  String address = "";
  String strMsg = "";

  int pos = 0;
  int b = (Integer)serBuffer.get(pos++);
  ArrayList argumentTypes = new ArrayList();

  while (b != 0)
  {
    address += char(b);
    b = (Integer)serBuffer.get(pos++);
  }

  // Skip address zeros and the comma for the parameters
  pos += 4 - ((address.length()) % 4);

  b = (Integer)serBuffer.get(pos++);
  while (b != 0)
  {
    argumentTypes.add(char(b));
    b = (Integer)serBuffer.get(pos++);
  }

  //println(pos);
  // Skip parameter zeros
  pos--;
  pos += 4 - ((argumentTypes.size() + 1) % 4);

  OscMessage oscMsg = new OscMessage(address);

  for (int i = 0; i < argumentTypes.size(); i++)
  {
    char type = (Character)argumentTypes.get(i);
    switch (type)
    {
    case 'i':
      int data = -1;
      int[] intArr = new int[4];
      intArr[0] = (Integer)serBuffer.get(pos++);
      intArr[1] = (Integer)serBuffer.get(pos++);
      intArr[2] = (Integer)serBuffer.get(pos++);
      intArr[3] = (Integer)serBuffer.get(pos++);
      data = byteArrayToInt(intArr, 0);
      oscMsg.add(data);
      strMsg=strMsg+"/"+data;
      break;
    case 'f':
      byte[] byteArr = new byte[4];
      byteArr[0] = byte((Integer)serBuffer.get(pos++));
      byteArr[1] = byte((Integer)serBuffer.get(pos++));
      byteArr[2] = byte((Integer)serBuffer.get(pos++));
      byteArr[3] = byte((Integer)serBuffer.get(pos++));
      float f = arr2float(byteArr, 0);
      oscMsg.add(f);
      strMsg=strMsg+"/"+f;
      break;
    case 's':
      String str = "";
      char c = char((Integer)serBuffer.get(pos++));
      while (c != 0x00)
      {
        str += c;
        c = char((Integer)serBuffer.get(pos++));
      }

      int zeros = 4 - (str.length() % 4);
      //Skip zeros
      for (int j = 0; j < zeros - 1; j++)
      {
        pos++;
      }
      oscMsg.add(str);
      strMsg=strMsg+"/"+str;
      break;
    }
  }
  ParseOScMsg(oscMsg);

  strMsg = address+"("+oscMsg.	typetag()+")"+strMsg;

  // Add message to Message Monitor
  monitorList.add(strMsg);          
  // remove items from beginning of the monitor list when size is bigger than
  // set monitorListLength
  while(monitorList.size()>monitorListLength)
  { monitorList.remove(0);
  }

  return oscMsg;
}

/************************************************************************************
 SLIP ENCODING
 ************************************************************************************/

byte eot = byte(192);
byte slipesc = byte(219);
byte slipescend = byte(220);
byte slipescesc = byte(221);

byte previousByte;

void slipDecode(byte incoming)
{
  byte previous = previousByte;
  previousByte = incoming;
  //if the previous was the escape char
  if (previous == slipesc)
  {
    //if this one is the esc eot
    if (incoming == slipescend)
    {
      serialBuffer.add(eot);
    }
    else if (incoming == slipescesc)
    {
      serialBuffer.add(slipesc);
    }
  }
  else if (incoming == eot)
  {
    //if it's the eot
    //send off the packet
    SerialSendToUDP();
  }
  else if (incoming != slipesc)
  {
    serialBuffer.add(incoming);
  }
}

void slipEncode(byte incoming)
{
  if (incoming == eot)
  {
    serial.write(slipesc);
    serial.write(slipescend);
  }
  else if (incoming == slipesc)
  {
    serial.write(slipesc);
    serial.write(slipescesc);
  }
  else
  {
    serial.write(incoming);
  }
}
