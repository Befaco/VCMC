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
 UDP
 ************************************************************************************/

//UDP communication
UDP udp;

int inPort = 7200;
int outPort = 1234;
String ipAddress = "192.168.1.52";

void setupUDP()
{
  udp = new UDP(this, inPort);
  udp.log( true );     // <-- printout the connection activity
  udp.listen(true);
}

void stopUDP()
{
  udp.close();
}

void UDPSendBuffer(byte[] data)
{
  udp.send(data, ipAddress, outPort);
}

//called when UDP receives some data
void receive(byte[] data)
{
  drawIncomingUDP();
  //send it over to serial
  serialSend(data);
  
  String strMsg= new String(data);
  strMsg = "UDP: "+"("+strMsg+")";

  // Add message to Message Monitor
  monitorList.add(strMsg);          
  // remove items from beginning of the monitor list when size is bigger than
  // set monitorListLength
  while(monitorList.size()>monitorListLength)
  { monitorList.remove(0);
  }

}
