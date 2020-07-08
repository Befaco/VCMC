
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

void setupMainGUIControls()
{
  liveChart = cp5.addChart("hello")
                  .setPosition(10, 35)
                  .setSize(350, 200)
                  .setRange(0, 1)
                  .setView(Chart.BAR) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
                  .moveTo(gRealTime);

  liveChart.addDataSet("CV");
  liveChart.setColors("CV", color(255, 0, 255), color(255, 0, 0));
  liveChart.setStrokeWeight(1.5);
  liveChart.addDataSet("Fader");
  liveChart.setColors("Fader", color(255), color(0, 255, 0));
  liveChart.addDataSet("Gate");
  liveChart.setColors("Gate", color(255, 255, 0), color(0, 0, 255));

  Commanddl = cp5.addScrollableList("OSC Commands")
                  .setPosition(10, 10)
                  .setSize(120, 100)
                  .bringToFront()
                  .moveTo(gRealTime)
                  .close();
  Commanddl.setItemHeight(20);
  Commanddl.setBarHeight(15);
  for (int i = 0; i < CommandTextStrings.length; i++)
  {
    String baudString = CommandTextStrings[i];
    Commanddl.addItem(baudString, CommandStrings.get(i));
  }
  Commanddl.hide();

  // text area for displaying incoming OSC data (monitor)
  mTextarea = cp5.addTextarea("textarea", "", 10, 10, 350, 350);
  mTextarea.setLineHeight(12);
  //  mTextarea.hideScrollbar();
  mTextarea.setColorBackground(0xff222222);
  mTextarea.showScrollbar();
  mTextarea.moveTo(gMonitor);

  //////////////////////////////////////////////////////////
  // Comm setp controls

  //start button
  startButton = cp5.addButton("START")
                    .setPosition(50, 220)
                    .setSize(120, 19)
                    .moveTo(gComm);

  //stop button
  stopButton = cp5.addButton("STOP")
                   .setPosition(50, 220)
                   .setSize(120, 19)
                   .moveTo(gComm);
  stopButton.hide();

  //udp IP/port
  UDPLabel = cp5.addTextlabel("UDPLabel")
                 .setText("UDP")
                 .setPosition(10, 10) //60)
                 .setColorValue(0xffffff00)
                 .setFont(createFont("SansSerif", 11))
                 .moveTo(gComm);

  ipAddressField = cp5.addTextfield("IP address")
                       .setPosition(10, 10 + 15) //60+15)
                       .setAutoClear(false)
                       .setText(ipAddress)
                       .moveTo(gComm);
  incomingPortField = cp5.addTextfield("Incoming Port Number")
                          .setPosition(10, 10 + 55) //60+55)
                          .setSize(120, 19)
                          .setAutoClear(false)
                          .setText(str(inPort))
                          .moveTo(gComm);

  outgoingPortField = cp5.addTextfield("Outgoing Port Number")
                          .setPosition(140, 10 + 55) //70+45)//110+45)
                          .setSize(120, 19)
                          .setAutoClear(false)
                          .setText(str(outPort))
                          .moveTo(gComm);

  //text labels

  incomingPacket = cp5.addTextlabel("incomingPacketLabel")
                       .setText("Incoming Packet")
                       .setPosition(60, 10)
                       .setColorValue(0xffffff00)
                       .setFont(createFont("SansSerif", 10))
                       .moveTo(gComm);
  incomingPacket.hide();

  //Serial Port selector
  arduinoLabel = cp5.addTextlabel("arduinoLabel")
                     .setText("Serial")
                     .setPosition(10, 10 + 90)
                     .setColorValue(0xffffff00)
                     .setFont(createFont("SansSerif", 11))
                     .moveTo(gComm);

  serialddl = cp5.addDropdownList("SerialPort")
                  .setPosition(150, 30 + 90)
                  .setSize(120, 100)
                  .moveTo(gComm);
  serialddl.setItemHeight(20);
  serialddl.setBarHeight(15);
  serialddl.getCaptionLabel().set("SELECT SERIAL PORT");
  //serialddl.getCaptionLabel().getStyle().marginTop = 3;
  //serialddl.getCaptionLabel().getStyle().marginLeft = 3;
  //serialddl.valueLabel().style().marginTop = 3;
  //set the serial options
  String SerialList[] = Serial.list();
  for (int i = 0; i < SerialList.length; i++)
  {
    String portName = SerialList[i];
    serialddl.addItem(portName, i);
  }
  serialddl.close();

  //setup the baud list
  baudddl = cp5.addDropdownList("BaudRate")
                .setPosition(10, 30 + 90)
                .setSize(120, 100)
                .bringToFront()
                .moveTo(gComm);
  baudddl.setItemHeight(20);
  baudddl.setBarHeight(15);
  //baudddl.captionLabel().set("SELECT THE BAUD RATE");
  //baudddl.captionLabel().style().marginTop = 3;
  //baudddl.captionLabel().style().marginLeft = 3;
  //baudddl.valueLabel().style().marginTop = 3;
  //the baud options
  for (int i = 0; i < serialRateStrings.length; i++)
  {
    String baudString = serialRateStrings[i];
    baudddl.addItem(baudString, i);
  }
  //baudddl.setIndex(4);
  baudddl.close();
}
