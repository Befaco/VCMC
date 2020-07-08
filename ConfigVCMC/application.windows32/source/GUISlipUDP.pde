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
 GUI
 ************************************************************************************/

ControlP5 cp5;

DropdownList serialddl;
DropdownList baudddl;
Textlabel arduinoLabel;
Textlabel UDPLabel;
Textlabel incomingPacket;
Button startButton;
Button stopButton;
Textfield ipAddressField;
Textfield incomingPortField;
Textfield outgoingPortField;
Textarea mTextarea;
//Toggle SerialIn, UPDIn;
ButtonCanvas SerialCanvas, UDPCanvas;
Chart liveChart;
ScrollableList Commanddl;
Group gComm, gMonitor, gRealTime;
Group gControlCfg, gControlView, gControlChange;
Accordion Mainacc;
Accordion ControlCfgacc;

String buf;
ArrayList monitorList;
int monitorListLength = 32;
String[] CommandTextStrings = {
    "Request Full config", "1200", "2400", "4800", "9600", "14400",
    "19200", "28800", "38400", "57600", "115200"};
List CommandStrings = Arrays.asList(
    "FullConfig", "1200", "2400", "4800", "9600", "14400",
    "19200", "28800", "38400", "57600", "115200");

void setupGUI()
{
  buf = "";
  monitorList = new ArrayList();

  //the ControlP5 object
  cp5 = new ControlP5(this);

  gControlCfg = cp5.addGroup("Control Config")
                    .setBackgroundColor(color(0, 64))
                    .setBackgroundHeight(110);
  gControlView = cp5.addGroup("Local Config")
                     .setBackgroundColor(color(0, 64))
                     .setBackgroundHeight(600);
  gControlChange = cp5.addGroup("Control Change")
                       .setBackgroundColor(color(0, 64))
                       .setBackgroundHeight(550);

  SerialCanvas = new ButtonCanvas(50, 20, 80, 80);
  UDPCanvas = new ButtonCanvas(50, 120, 80, 80);

  gComm = cp5.addGroup("Communications Setup")
              .setBackgroundColor(color(0, 64))
              .setBackgroundHeight(250)
      //.addCanvas(SerialCanvas)
      ;
  gComm.addCanvas(SerialCanvas);
  gComm.addCanvas(UDPCanvas);
  gMonitor = cp5.addGroup("Monitor Comm")
                 .setBackgroundColor(color(0, 64))
                 .setBackgroundHeight(380);
  gRealTime = cp5.addGroup("Real Time")
                  .setBackgroundColor(color(0, 64))
                  .setBackgroundHeight(250);

  /*  SerialIn = cp5.addToggle("SerialIn")
     .setPosition(40,100)
     .setSize(50,20)
     //.lock()
     .moveTo(gComm)
     ;
  UPDIn = cp5.addToggle("UDPIn")
     .setPosition(40,200)
     .setSize(50,20)
     .lock()
    .moveTo(gComm)
    ;
  */
  setupMainGUIControls();
  setupGUIControlConfig();

  Mainacc = cp5.addAccordion("Mainacc")
                .setPosition(20, 10)
                .setWidth(370)
                .addItem(gComm)
                .addItem(gRealTime)
                .addItem(gMonitor);
  Mainacc.open(0, 0);

  // use Accordion.MULTI to allow multiple group
  // to be open at a time.
  Mainacc.setCollapseMode(Accordion.MULTI);

  // when in SINGLE mode, only 1 accordion
  // group can be open at a time.
  // Mainacc.setCollapseMode(Accordion.SINGLE);

  ControlCfgacc = cp5.addAccordion("Controlacc")
                      .setPosition(410, 10)
                      .setWidth(370)
                      .addItem(gControlCfg)
                      .addItem(gControlView)
                      .addItem(gControlChange);
  ControlCfgacc.open(0, 1);
  ControlCfgacc.setCollapseMode(Accordion.MULTI);
}

void controlEvent(ControlEvent theEvent)
{
  String eventName = theEvent.getName();
  String otherText = eventName;

  //println(eventName);

  //if (theEvent.isGroup()) {
  if (eventName == "SerialPort")
  {
    //set the serial port
    serialListNumber = int(theEvent.getValue());
  }
  else if (eventName == "BaudRate")
  {
    int index = int(theEvent.getValue());
    baud = Integer.parseInt(serialRateStrings[index]);
  }
  //if (theEvent.isAssignableFrom(Textfield.class))
  //{
   else if (eventName == "IP address")
    {
      ipAddressField.setFocus(false);
      ipAddress = theEvent.getStringValue();
    }
    else if (eventName == "Incoming Port Number")
    {
      incomingPortField.setFocus(false);
      inPort = Integer.parseInt(theEvent.getStringValue());
    }
    else if (eventName == "Outgoing Port Number")
  //if (theEvent.isFrom(outgoingPortField))
    {
      outgoingPortField.setFocus(false);
      outPort = Integer.parseInt(theEvent.getStringValue());
    }
  //}
  if (theEvent.isFrom(cp5.getController("OSC Commands")))
  {
    //println(int(theEvent.getValue()), CommandStrings.get(int(theEvent.getValue())), cp5.get(ScrollableList.class, "OSC Commands").getItem(int(theEvent.getValue())));
    SendOSCCommand((String)CommandStrings.get(int(theEvent.getValue())));
  }
}

//start everything
public
void START(int theValue)
{
  // Update values from controls
  ipAddress = ipAddressField.getText();
  inPort = Integer.parseInt(incomingPortField.getText());
  outPort = Integer.parseInt(outgoingPortField.getText());
  
  setupSerial();
  if (serial == null)
    return;
  setupUDP();
  hideControls();
  applicationRunning = true;
}

//hide all the controls and show the stop button
void hideControls()
{
  serialddl.hide();
  baudddl.hide();
  startButton.hide();
  outgoingPortField.hide();
  incomingPortField.hide();
  ipAddressField.hide();
  //incomingPacket.show();
  //show the stop button
  stopButton.show();

  Mainacc.open(1, 2);
  Mainacc.close(0);
}

void showControls()
{
  serialddl.show();
  baudddl.show();
  startButton.show();
  outgoingPortField.show();
  incomingPortField.show();
  ipAddressField.show();
  incomingPacket.hide();
  //hide the stop button
  stopButton.hide();
}

public
void STOP()
{
  stopSerial();
  stopUDP();
  showControls();
  applicationRunning = false;
}

static int n = 0, a = 0;
class ButtonCanvas extends Canvas
{

  int Status = 0;
  int theX = 0, theY = 0;
  int Width = 80, Height = 80;

  public
  ButtonCanvas(int x, int y, int W, int H)
  {
    theX = x;
    theY = y;
    Width = W;
    Height = H;
  }
  public
  void setup(PGraphics pg)
  {
    Status = 0;
  }
  public
  void draw(PGraphics pg)
  {
    if (!applicationRunning)
      return;
    if (Status == 0)
      pg.fill(0);
    else
      pg.fill(255);
    pg.rect(theX, theY, Width, Height);
    /*
    n += 0.01;
    pg.ellipseMode(CENTER);
    pg.fill(lerpColor(color(0,100,200),color(0,200,100),map(sin(n),-1,1,0,1)));
    pg.rect(0,0,200,200);
    pg.fill(255,150);
    a+=0.01;
    ellipse(100,100,abs(sin(a)*150),abs(sin(a)*150));
    ellipse(40,40,abs(sin(a+0.5)*50),abs(sin(a+0.5)*50));
    ellipse(60,140,abs(cos(a)*80),abs(cos(a)*80));
    */
  }
}
