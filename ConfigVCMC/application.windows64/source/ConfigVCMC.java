import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.nio.ByteBuffer; 
import java.util.*; 
import processing.serial.*; 
import hypermedia.net.*; 
import controlP5.*; 
import oscP5.*; 
import netP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class ConfigVCMC extends PApplet {

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




//download at http://ubaa.net/shared/processing/udp/

//download at http://www.sojamo.de/libraries/controlp5

//download at http://www.sojamo.de/libraries/oscp5/



boolean applicationRunning = false;

ControlFrame cf;
GlobalCfg theGlobalConfig;
InputCtrlCfg[] InputControlsCFG; //= new InputCtrlCfg[9];
float[] CVvalues;                // = {0.0, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,};
float[] Fadervalues;             // = {0.0, 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,};
float[] GateValues;

String sCurrentpath;
String sCurrentFile = "data/newconfig.json";
File fCurrentFile;
boolean ConfigChanged = false;

/************************************************************************************
 SETUP/DRAW
 ************************************************************************************/
public void settings()
{
  // configure the screen size and frame rate
  size(800, 800, P3D);
}

public void setup()
{

  theGlobalConfig = new GlobalCfg();
  CVvalues = new float[9];
  Fadervalues = new float[9];
  GateValues = new float[9];
  InputControlsCFG = new InputCtrlCfg[9];
  for (int i = 0; i < 9; i++)
  {
    //AnControlsCFG[i] = new AnInputPortCfg();
    CVvalues[i] = Fadervalues[i] = 0.0f;
    GateValues[i] = 0;
    InputControlsCFG[i] = new InputCtrlCfg();
  }

  surface.setLocation(20, 10);
  noStroke();
  frameRate(30);
  setupGUI();

  // Currentpath = sketchPath();

  fCurrentFile = new File(sCurrentFile);
  LoadJSONConfig();
  //cf = new ControlFrame(this, 400, 800, "Controls");
}

int lastSerialPacket = 0;
int lastUDPPacket = 0;
boolean lastState = false;

public void draw()
{
  liveChart.updateData("CV", CVvalues);
  liveChart.updateData("Fader", Fadervalues);
  liveChart.updateData("Gate", GateValues);
  background(128);
  //int now = millis();
  //if(now%50==1){
  //  SerialCanvas.Status = (SerialCanvas.Status ==0)? 1:0;
  //}
  tlCurrentFile.setText("CURRENT FILE:" + fCurrentFile.getName()); //sCurrentFile);//

  if (ConfigChanged)
  {
    JSONObject json = new JSONObject();
    FillJSONConfig(json);
    DataText.setText(json.toString());
    ConfigChanged = false;
  }
  if (applicationRunning)
  {
    drawIncomingPackets();
  }
}

/************************************************************************************
 VISUALIZING INCOMING PACKETS
 ************************************************************************************/

public void drawIncomingPackets()
{
  /*  
  int x2= 50, y2= 30, w2 = 80, h2= 80; 
  int x1= 50, y1= y2+h2+10, w1 = 80, h1= 80; 
  //the serial packet
  fill(0);
  rect( x1, y1, w1, h1);//75, 50, 100, 100);
  //the udp packet
  rect( x2, y2, w2, h2);//325, 50, 100, 100);
  */
  int now = millis();
  int lightDuration = 75;
  SerialCanvas.Status = 0;

  if (now - lastSerialPacket < lightDuration)
  {
    SerialCanvas.Status = 1;
    /*
    fill(255);
    rect(85, 60, 80, 80);
    */
  }
  UDPCanvas.Status = 0;
  if (now - lastUDPPacket < lightDuration)
  {
    UDPCanvas.Status = 1;
    //fill(255);
    //rect(335, 60, 80, 80);
  }

  buf = "";
  for (int i = 0; i < monitorList.size(); i++)
  {
    buf = buf + (String)monitorList.get(i) + "\n";
  }
  //buf = buf + (String) Integer.toString(prueConf.Ranges.minMIDI);
  mTextarea.setText(buf);
}

public void drawIncomingSerial()
{
  lastSerialPacket = millis();
}

public void drawIncomingUDP()
{
  lastUDPPacket = millis();
}
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

public void setupUDP()
{
  udp = new UDP(this, inPort);
  udp.log( true );     // <-- printout the connection activity
  udp.listen(true);
}

public void stopUDP()
{
  udp.close();
}

public void UDPSendBuffer(byte[] data)
{
  udp.send(data, ipAddress, outPort);
}

//called when UDP receives some data
public void receive(byte[] data)
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

class ControlFrame extends PApplet
{

  int w, h;
  PApplet parent;
  ControlP5 cf5;
  Knob MIDIChannel, ClockDivider;
  Knob ControllerNumber;
  //Slider ControllerNumber,lowlimit;
  Range rangeMIDI, rangeDAC, LimitsMIDI;
  Group gInputControl, gInputRange, gInputAnag;
  Accordion accordion;
  DropdownList MIDIFunctionAnagddl;
  RadioButton r1, r2;
  ButtonBar b;

  AnInputPortCfg prueConf;

public
  ControlFrame(PApplet _parent, int _w, int _h, String _name)
  {
    super();
    parent = _parent;
    w = _w;
    h = _h;
    PApplet.runSketch(new String[]{this.getClass().getName()}, this);
  }

public
  void settings()
  {
    size(w, h);
  }

public
  void setup()
  {
    surface.setLocation(10, 10);
    cf5 = new ControlP5(this);
    prueConf = new AnInputPortCfg();
    PFont pfont = createFont("Arial", 12, true); // use true/false for smooth/no-smooth
    ControlFont font = new ControlFont(pfont, 14);

    cf5.addTextlabel("Channel")
        .setText("Channel Selected")
        .setPosition(10, 10)
        .setColorValue(0xffffff00)
        .moveTo(gInputAnag)
        .setFont(createFont("SansSerif", 11));
    /*  b = cf5.addButtonBar("bar")
      .setPosition(10, 30)
      .setSize(370, 20)
      .addItems(split("1 2 3 4 5 6 7 8 A"," "))
      .setItems(1)
      ;
    */
    r1 = cf5.addRadioButton("radioButton")
             .setPosition(10, 30)
             .setSize(35, 20)
             .setColorForeground(color(120))
             .setColorActive(color(58, 155, 198)) //color(255))
             .setColorLabel(0xffffff00)           //color(255))
             .setItemsPerRow(9)
             .setSpacingColumn(5);
    for (int i = 1; i < 9; i++)
    {
      r1.addItem(str(i), i);
    }
    r1.addItem("A", 8);
    for (Toggle t : r1.getItems())
    {
      t.getCaptionLabel()
          .align(CENTER, CENTER);
    }
    r1.activate(0);

    r2 = cf5.addRadioButton("radioControl")
             .setPosition(10, 60)
             .setSize(95, 20)
             .setColorForeground(color(120))
             .setColorActive(color(58, 155, 198)) //color(255))
             .setColorLabel(0xffffff00)           //color(255))
             .setItemsPerRow(9)
             .setSpacingColumn(5);
    String[] lab = {"CV", "Fader", "Gate"};
    for (int i = 0; i < 3; i++)
    {
      r2.addItem(lab[i], i);
    }
    for (Toggle t : r2.getItems())
    {
      t.getCaptionLabel()
          .align(CENTER, CENTER);
    }
    r2.activate(0);

    setupInputControl();
    setupInputRange();
    setupInputAnag();
    accordion = cf5.addAccordion("acc")
                    .setPosition(10, 90)
                    .setWidth(370)
                    .addItem(gInputControl)
                    .addItem(gInputAnag)
                    .addItem(gInputRange);
    accordion.open(0, 1, 2);

    // use Accordion.MULTI to allow multiple group
    // to be open at a time.
    accordion.setCollapseMode(Accordion.MULTI);

    // when in SINGLE mode, only 1 accordion
    // group can be open at a time.
    // accordion.setCollapseMode(Accordion.SINGLE);
  }

  List AnagMIDIF = Arrays.asList(
      "PITCHTRIG", "PITCH", "VELOCITY", "CONTROLCHANGE", "PROGRAMCHANGE", "PITCHBEND",
      "PERCENT", "ANAGCLOCK", "ANAGSTARTSTOP", "ANAGPAUSECONT", "ANAGTRIGSTARTSTOP", "ANAGPAUSECONT", "ANAGFREEVALUE");
  /*String[] AnagMIDIF = {
  "PITCHTRIG", "PITCH", "VELOCITY", "CONTROLCHANGE", "PROGRAMCHANGE", "PITCHBEND", 
  "PERCENT", "ANAGCLOCK", "ANAGSTARTSTOP", "ANAGPAUSECONT", "ANAGTRIGSTARTSTOP", "ANAGPAUSECONT", "ANAGFREEVALUE"
  };*/

  public void setupInputAnag()
  {
    // Analog Controls
    /*
    uint8_t MIDIfunction;
    long Offset; // Offset value
    long Amp;  // Amplification factor (in percent)
    byte RangeBipolar=0;
    */
    gInputAnag = cf5.addGroup("Config Anag")
                     .setBackgroundColor(color(0, 64))
                     .setBackgroundHeight(200);

    // group number 1, contains 2 bangs
    Group g1 = cf5.addGroup("myGroup1")
                   .setBackgroundColor(color(0, 64))
                   .setBackgroundHeight(150);

    cf5.addBang("bang")
        .setPosition(10, 20)
        .setSize(100, 100)
        .moveTo(g1);

    // group number 2, contains a radiobutton
    Group g2 = cf5.addGroup("myGroup2")
                   .setBackgroundColor(color(0, 64))
                   .setBackgroundHeight(150);

    cf5.addRadioButton("radio")
        .setPosition(10, 20)
        .setItemWidth(20)
        .setItemHeight(20)
        .addItem("black", 0)
        .addItem("red", 1)
        .addItem("green", 2)
        .addItem("blue", 3)
        .addItem("grey", 4)
        .setColorLabel(color(255))
        .activate(2)
        .moveTo(g2);

    // group number 3, contains a bang and a slider
    Group g3 = cf5.addGroup("myGroup3")
                   .setBackgroundColor(color(0, 64))
                   .setBackgroundHeight(150);

    cf5.addBang("shuffle")
        .setPosition(10, 20)
        .setSize(40, 50)
        .moveTo(g3);

    cf5.addSlider("hello")
        .setPosition(60, 20)
        .setSize(100, 20)
        .setRange(100, 500)
        .setValue(100)
        .moveTo(g3);

    cf5.addSlider("world")
        .setPosition(60, 50)
        .setSize(100, 20)
        .setRange(100, 500)
        .setValue(200)
        .moveTo(g3);

    // create a new accordion
    // add g1, g2, and g3 to the accordion.
    Accordion acco = cf5.addAccordion("acco")
                         .setPosition(10, 10)
                         .setWidth(200)
                         .addItem(g1)
                         .addItem(g2)
                         .addItem(g3)
                         .moveTo(gInputAnag);
    //acco.open(0,1,2);
    // use Accordion.MULTI to allow multiple group
    // to be open at a time.
    //acc.setCollapseMode(Accordion.MULTI);
    // when in SINGLE mode, only 1 accordion
    // group can be open at a time.
    acco.setCollapseMode(Accordion.SINGLE);
    acco.moveTo(gInputAnag);
  }

  public void setupInputRange()
  {
    // Analog Ranges
    gInputRange = cf5.addGroup("Config Ranges")
                      .setBackgroundColor(color(0, 64))
                      .setBackgroundHeight(200);

    rangeMIDI = cf5.addRange("rangeMIDI")
                    // disable broadcasting since setRange and setRangeValues will trigger an event
                    .setBroadcast(false)
                    .setPosition(10, 10)
                    .setSize(300, 40)
                    .setHandleSize(20)
                    .setRange(0, 127)
                    .setRangeValues(0, 120)
                    // after the initialization we turn broadcast back on again
                    .setBroadcast(true)
                    .setColorForeground(color(155, 40))
                    .setColorBackground(color(155, 40))
                    .moveTo(gInputRange)
                    .setDecimalPrecision(0);
    // rangeMIDI.plugTo(prueConf);
    rangeDAC = cf5.addRange("rangeDAC")
                   // disable broadcasting since setRange and setRangeValues will trigger an event
                   .setBroadcast(false)
                   .setPosition(10, 60)
                   .setSize(300, 40)
                   .setHandleSize(20)
                   .setRange(-4096, 4096)
                   .setRangeValues(0, 4095)
                   // after the initialization we turn broadcast back on again
                   .setBroadcast(true)
                   .setColorForeground(color(155, 40))
                   .setColorBackground(color(155, 40))
                   .moveTo(gInputRange)
                   .setDecimalPrecision(0);
    // rangeMIDI.plugTo(prueConf);
    LimitsMIDI = cf5.addRange("LimitsMIDI")
                     // disable broadcasting since setRange and setRangeValues will trigger an event
                     .setBroadcast(false)
                     .setPosition(10, 110)
                     .setSize(300, 40)
                     .setHandleSize(20)
                     .setRange(0, 127)
                     .setRangeValues(0, 120)
                     // after the initialization we turn broadcast back on again
                     .setBroadcast(true)
                     .setColorForeground(color(155, 40))
                     .setColorBackground(color(155, 40))
                     .moveTo(gInputRange)
                     .setDecimalPrecision(0);
  }

  public void setupInputControl()
  {
    gInputControl = cf5.addGroup("Control General")
                        .setBackgroundColor(color(0, 64))
                        .setBackgroundHeight(180);

    // InputPort
    MIDIChannel = cf5.addKnob("MIDIChannel")
                      .plugTo(parent, "MIDIChannel")
                      .setPosition(10, 10 + 30)
                      .setSize(100, 100)
                      .setRange(1, 16)
                      .setNumberOfTickMarks(15)
                      .setTickMarkLength(4)
                      .snapToTickMarks(true)
                      .moveTo(gInputControl)
                      .setValue(1);
    MIDIChannel.plugTo(prueConf);

    ClockDivider = cf5.addKnob("ClockDivider")
                       .plugTo(parent, "ClockDivider")
                       .setPosition(10 + 110, 10 + 30)
                       .setSize(100, 100)
                       .setRange(1, 128)
                       .setRadius(50)
                       .moveTo(gInputControl)
                       .setValue(6);
    ClockDivider.plugTo(prueConf);

    ControllerNumber = cf5.addKnob("ControllerNumber")
                           .plugTo(parent, "ControllerNumber")
                           .setPosition(10 + 220, 10 + 30)
                           .setSize(100, 100)
                           .setRange(1, 128)
                           .setRadius(50)
                           .moveTo(gInputControl)
                           .setValue(6);
    ControllerNumber.plugTo(prueConf);

    /*
    ControllerNumber= cf5.addSlider("ControllerNumber")
       //.plugTo(parent, "speed")
       .setRange(0, 127)
       //.setValue(0.01)
       .setPosition(10, 150+30)
    //       .setFont(font)
       .moveTo(gInputControl)
       .setSize(200, 30);
    ControllerNumber.plugTo(prueConf);
    */

    cf5.addScrollableList("MIDI Function")
        .setPosition(10, 10)
        .setSize(200, 100)
        .setBarHeight(20)
        .setItemHeight(20)
        .moveTo(gInputControl)
        .addItems(AnagMIDIF)
        //.setType(ControlP5.LIST)
        .setValue(1)
        // .setType(ScrollableList.LIST) // currently supported DROPDOWN and LIST
        ;
    cf5.addTextlabel("Function")
        .setText("Function")
        .setPosition(230, 10)
        .setColorValue(0xffffff00)
        .moveTo(gInputControl)
        .setFont(createFont("SansSerif", 11));
  }

  public void draw()
  {
    background(100);
  }

  public void controlEvent(ControlEvent theControlEvent)
  {
    if (theControlEvent.isFrom("rangeMIDI"))
    {
      prueConf.Ranges.minMIDI = PApplet.parseInt(theControlEvent.getController().getArrayValue(0));
      prueConf.Ranges.rangeMIDI = PApplet.parseInt(theControlEvent.getController().getArrayValue(1)) - prueConf.Ranges.minMIDI;
    }
    if (theControlEvent.isFrom("rangeDAC"))
    {
      prueConf.Ranges.minDAC = PApplet.parseInt(theControlEvent.getController().getArrayValue(0));
      prueConf.Ranges.rangeDAC = PApplet.parseInt(theControlEvent.getController().getArrayValue(1)) - prueConf.Ranges.minDAC;
    }
    if (theControlEvent.isFrom("LimitsMIDI"))
    {
      prueConf.ClipLow = PApplet.parseInt(theControlEvent.getController().getArrayValue(0));
      prueConf.ClipHigh = PApplet.parseInt(theControlEvent.getController().getArrayValue(1));
    }
  }
}
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

// This function returns all the files in a directory as an array of Strings
public String[] listFileNames(String dir)
{
  File file = new File(dir);
  if (file.isDirectory())
  {
    String names[] = file.list();
    return names;
  }
  else
  {
    // If it's not a directory
    return null;
  }
}
//  listFiles(FileFilter filter)
//  list(FilenameFilter filter)

// This function returns all the files in a directory as an array of File objects
// This is useful if you want more info about the file
public File[] listFiles(String dir)
{
  File file = new File(dir);
  if (file.isDirectory())
  {
    File[] files = file.listFiles();
    return files;
  }
  else
  {
    // If it's not a directory
    return null;
  }
}

// Function to get a list of all files in a directory and all subdirectories
public ArrayList<File> listFilesRecursive(String dir)
{
  ArrayList<File> fileList = new ArrayList<File>();
  recurseDir(fileList, dir);
  return fileList;
}

// Recursive function to traverse subdirectories
public void recurseDir(ArrayList<File> a, String dir)
{
  File file = new File(dir);
  if (file.isDirectory())
  {
    // If you want to include directories in the list
    a.add(file);
    File[] subfiles = file.listFiles();
    for (int i = 0; i < subfiles.length; i++)
    {
      // Call this function on all files in this directory
      recurseDir(a, subfiles[i].getAbsolutePath());
    }
  }
  else
  {
    a.add(file);
  }
}
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

Textlabel tlCurrentFile;
Textfield Test1Field;
Textfield Test2Field;
Textarea DataText;
Button sendRequestButton;
Button loadFileButton, saveFileButton, sendOSCButton;
RadioButton r1, r2;

public void setupGUIControlConfig()
{
  int POSY = 10;

  tlCurrentFile = cp5.addTextlabel("File")
                      .setText("File :")
                      .setPosition(10, POSY) //60)
                      .setColorValue(0xffffff00)
                      .setFont(createFont("Arial", 12))
                      .moveTo(gControlCfg);
  POSY = POSY + 20;
  //start button
  saveFileButton = cp5.addButton("SAVE")
                       .setPosition(10, POSY)
                       .setSize(90, 19)
                       .moveTo(gControlCfg);
  loadFileButton = cp5.addButton("LOAD")
                       .setPosition(120, POSY)
                       .setSize(90, 19)
                       .moveTo(gControlCfg);
  //POSY= POSY+20;

  POSY = POSY + 30;
  cp5.addTextlabel("VCMCReq")
      .setText("VCMC Config :")
      .setPosition(10, POSY) //60)
      .setColorValue(0xffffff00)
      .setFont(createFont("Arial", 12))
      .moveTo(gControlCfg);
  POSY = POSY + 20;
  sendOSCButton = cp5.addButton("SEND")
                      .setPosition(120 /*230*/, POSY)
                      .setSize(90, 19)
                      .moveTo(gControlCfg);
  cp5.addButton("ReqConfig")
      .setLabel("REQUEST")
      .setPosition(10, POSY)
      .setSize(90, 19)
      .moveTo(gControlCfg);
  /////////////////////////
  // COntrol Change Tab
  setupControlChangeTab();

  /////////////////////////
  // COntrol View Tab
  POSY = 10;
  /*cp5.addTextlabel("CurConfig")
    .setText("Current Config :")
    .setPosition(10, POSY)//60)
    .setColorValue(0xffffff00)
    .setFont(createFont("Arial", 12))
    .moveTo(gControlCfg)
    ;
  */
  // text area for displaying config JSON data (monitor)
  DataText = cp5.addTextarea("Configtextarea", "", 10, POSY, 350, 590);
  DataText.setLineHeight(12);
  DataText.setFont(createFont("Arial", 14));
  //  mTextarea.hideScrollbar();
  DataText.setColorBackground(0xff222222);
  DataText.showScrollbar();
  DataText.moveTo(gControlView);
}

boolean bSave = false;
public
void SAVE(int theValue)
{
  bSave = true;
  selectOutput("Select a file to write to:", "fileSelected");
}

public
void LOAD(int theValue)
{
  bSave = false;
  selectInput("Select a file to read from:", "fileSelected");
}

public
void SEND(int theValue)
{
  theGlobalConfig.SendOSC("/VCMC/Config");
}

public
void ReqConfig(int theValue)
{
  SendOSCCommand("FullConfig");
}

public void fileSelected(File selection)
{
  //fCurrentFile = new File(selection);
  if (selection == null)
  {
    println("Window was closed or the user hit cancel.");
    return;
  }
  else
  {
    println("User selected " + selection.getAbsolutePath());
  }
  sCurrentFile = selection.getAbsolutePath();
  //  sCurrentFile= selection.getCanonicalPath();
  fCurrentFile = selection;
  if (bSave)
    SaveJSONConfig();
  else
    LoadJSONConfig();
}

public void setupControlChangeTab()
{
  int POSY = 10;
  r1 = cp5.addRadioButton("radioButton")
           .setPosition(10, POSY)
           .setSize(35, 20)
           .setColorForeground(color(120))
           .setColorActive(color(58, 155, 198)) //color(255))
           .setColorLabel(0xffffff00)           //color(255))
           .setItemsPerRow(9)
           .setSpacingColumn(5)
           .moveTo(gControlChange);
  for (int i = 1; i < 9; i++)
  {
    r1.addItem(str(i), i);
  }
  r1.addItem("A", 8);
  for (Toggle t : r1.getItems())
  {
    t.getCaptionLabel()
        .align(CENTER, CENTER);
  }
  r1.activate(0);

  POSY = POSY + 30;
  r2 = cp5.addRadioButton("radioControl")
           .setPosition(10, POSY)
           .setSize(95, 20)
           .setColorForeground(color(120))
           .setColorActive(color(58, 155, 198)) //color(255))
           .setColorLabel(0xffffff00)           //color(255))
           .setItemsPerRow(9)
           .setSpacingColumn(5)
           .moveTo(gControlChange);
  String[] lab = {"CV", "Fader", "Gate"};
  for (int i = 0; i < 3; i++)
  {
    r2.addItem(lab[i], i);
  }
  for (Toggle t : r2.getItems())
  {
    t.getCaptionLabel()
        .align(CENTER, CENTER);
  }
  r2.activate(0);
}
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

public void setupGUI()
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

public void controlEvent(ControlEvent theEvent)
{
  String eventName = theEvent.getName();
  String otherText = eventName;

  //println(eventName);

  //if (theEvent.isGroup()) {
  if (eventName == "SerialPort")
  {
    //set the serial port
    serialListNumber = PApplet.parseInt(theEvent.getValue());
  }
  else if (eventName == "BaudRate")
  {
    int index = PApplet.parseInt(theEvent.getValue());
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
    SendOSCCommand((String)CommandStrings.get(PApplet.parseInt(theEvent.getValue())));
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
public void hideControls()
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

public void showControls()
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

public boolean checkJSONItem(JSONObject jObj, String theItem)
{
  if (jObj.isNull(theItem))
  {
    println("JSON file format not found:" + theItem);
    return false;
  }
  return true;
}

public void FillJSONConfig(JSONObject newConfig)
{

  theGlobalConfig.FillJSON(newConfig);

  JSONArray ConfigArr;
  ConfigArr = new JSONArray();
  for (int i = 0; i < 9; i++)
  {
    JSONObject newInputCtrlConfig = new JSONObject();
    InputControlsCFG[i].FillJSON(newInputCtrlConfig);
    newInputCtrlConfig.setInt("Bank id", i + 1);
    ConfigArr.setJSONObject(i, newInputCtrlConfig);
  }

  // Create a new JSON config object

  newConfig.setJSONArray("CV Config", ConfigArr);
  //  saveJSONArray(ConfigArr, "data/new.json");
  //  saveJSONObject(newConfig, "data/newconfig.json");
}

public void SaveJSONConfig()
{
  JSONObject newConfig = new JSONObject();
  FillJSONConfig(newConfig);
  saveJSONObject(newConfig, sCurrentFile);
}

public void LoadJSONConfig()
{
  // A JSON object
  JSONObject json;
  json = loadJSONObject(sCurrentFile); //("data/newconfig.json");

  theGlobalConfig.ReadJSON(json);

  if (json.isNull("CV Config"))
  {
    println("JSON file format incorrect; CV Config");
    return;
  }

  JSONArray ConfigArr;
  ConfigArr = json.getJSONArray("CV Config");
  for (int i = 0; i < 9; i++)
  {
    JSONObject newInputCtrlConfig = ConfigArr.getJSONObject(i);
    InputControlsCFG[i].ReadJSON(newInputCtrlConfig);
  }

  DataText.setText(json.toString());
}

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

public void setupMainGUIControls()
{
  liveChart = cp5.addChart("hello")
                  .setPosition(10, 35)
                  .setSize(350, 200)
                  .setRange(0, 1)
                  .setView(Chart.BAR) // use Chart.LINE, Chart.PIE, Chart.AREA, Chart.BAR_CENTERED
                  .moveTo(gRealTime);

  liveChart.addDataSet("CV");
  liveChart.setColors("CV", color(255, 0, 255), color(255, 0, 0));
  liveChart.setStrokeWeight(1.5f);
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

public void ParseOScMsg(OscMessage oscMsg)
{
  int Chann;
  if (oscMsg.addrPattern().contains("/VCMC/Config/") == true)
  {
    if (oscMsg.addrPattern().contains("/Global") == true)
    {
      println("### received /Global pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
      theGlobalConfig.ReadOSC(oscMsg);
    }
    else
    {
      Chann = Character.getNumericValue(oscMsg.addrPattern().charAt(13));
      if (Chann < 1 || Chann > 9)
      {
        println("Error in channel:" + Chann);
        return;
      }

      if (oscMsg.addrPattern().contains("/Function") == true)
      {
        println("### received /Function pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].ReadOSC(oscMsg);
        SaveJSONConfig();
      }
      else if (oscMsg.addrPattern().contains("/CV/Range") == true)
      {
        println("### received /CV/Range pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].CVCfg.Ranges.ReadOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/CV/InputCfg") == true)
      {
        println("### received /CV/InputCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].CVCfg.ReadBaseOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/CV/AnagCfg") == true)
      {
        println("### received /CV/AnagCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].CVCfg.ReadOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/Fad/Range") == true)
      {
        println("### received /Fad/Range pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].FadCfg.Ranges.ReadOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/Fad/InputCfg") == true)
      {
        println("### received /Fad/InputCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].FadCfg.ReadBaseOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/Fad/AnagCfg") == true)
      {
        println("### received /Fad/AnagCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].FadCfg.ReadOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/Gate/InputCfg") == true)
      {
        println("### received /Gate/InputCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].GateCfg.ReadBaseOSC(oscMsg);
      }
      else if (oscMsg.addrPattern().contains("/Gate/DigCfg") == true)
      {
        println("### received /Gate/DigCfg pattern " + oscMsg.addrPattern() + " Tag:" + oscMsg.typetag());
        InputControlsCFG[Chann - 1].GateCfg.ReadOSC(oscMsg);
      }
    }
  }
  else if (oscMsg.addrPattern().contains("/VCMC/Fad/") == true)
  {
    if (oscMsg.checkTypetag("f"))
    {
      int idx = Character.getNumericValue(oscMsg.addrPattern().charAt(10));
      if (idx < 1 || idx > 9)
        println("Error in channel:" + idx);
      else
        Fadervalues[idx - 1] = oscMsg.get(0).floatValue();
    }
  }
  else if (oscMsg.addrPattern().contains("/VCMC/CV/") == true)
  {
    if (oscMsg.checkTypetag("f"))
    {
      int idx = Character.getNumericValue(oscMsg.addrPattern().charAt(9));
      if (idx < 1 || idx > 9)
        println("Error in channel:" + idx);
      else
        CVvalues[idx - 1] = 1.0f - oscMsg.get(0).floatValue();
    }
  }
  else if (oscMsg.addrPattern().contains("/VCMC/Gate/") == true)
  {
    if (oscMsg.checkTypetag("i"))
    {
      int idx = Character.getNumericValue(oscMsg.addrPattern().charAt(11));
      if (idx < 1 || idx > 9)
        println("Error in channel" + idx);
      else
        GateValues[idx - 1] = oscMsg.get(0).intValue();
    }
  }
  else
    println("### received unknown osc message. with address pattern " + oscMsg.addrPattern());
}

public void SendOSCCommand(String comString)
{
  println(comString);
  OscMessage comMsg = new OscMessage("/VCMC/Command");
  comMsg.add(comString);

  SendOSCMsg(comMsg);
}
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

class GlobalCfg
{

  int initPage;
  int InitMinDAC;    ///< Minimum default CV DAC value
  int InitRangeDAC;  ///< Minimum default CV DAC range
  int FaderMinDAC;   ///< Minimum default Fader DAC value
  int FaderRangeDAC; ///< Minimum default Fader DAC range
  int AuxMinDAC;     ///< Minimum default Aux DAC value
  int AuxRangeDAC;   ///< Minimum default Aux DAC range
  int AuxBMinDAC;    ///< Minimum default Aux DAC value
  int AuxBRangeDAC;  ///< Minimum default Aux DAC range
  GlobalCfg()
  {
    initPage = 0;
    InitMinDAC = ANRANGEMAX;
    InitRangeDAC = -ANRANGEMAX;
    FaderMinDAC = 0;
    FaderRangeDAC = ANRANGEMAX;
    AuxMinDAC = ANRANGEMAX;
    AuxRangeDAC = -ANRANGEMAX;
    AuxBMinDAC = ANRANGEMAX;
    AuxBRangeDAC = -ANRANGEMAX;
  }
  public void ReadOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("iiiiiiii") == true)
    {
      //initPage= oscMsg.get(0).intValue();
      InitMinDAC = oscMsg.get(0).intValue();
      InitRangeDAC = oscMsg.get(1).intValue();
      FaderMinDAC = oscMsg.get(2).intValue();
      FaderRangeDAC = oscMsg.get(3).intValue();
      AuxMinDAC = oscMsg.get(4).intValue();
      AuxRangeDAC = oscMsg.get(5).intValue();
      AuxBMinDAC = oscMsg.get(6).intValue();
      AuxBRangeDAC = oscMsg.get(7).intValue();
      println("Values: " + oscMsg.get(0).intValue() + "/" + oscMsg.get(1).intValue() + "/" + 
        oscMsg.get(2).intValue() + "/" + oscMsg.get(3).intValue() + "/" + 
        oscMsg.get(4).intValue() + "/" + oscMsg.get(5).intValue() + "/" + 
        oscMsg.get(6).intValue() + "/" + oscMsg.get(7).intValue());
      ConfigChanged = true;
    }
    else
      println("Wrong GlobalCfg TypeTag: " + oscMsg.typetag());
  }

  public void SendOSC(String header)
  {
    for (int i = 0; i < 9; i++)
    {
      InputControlsCFG[i].SendOSC(header + "/" + (i + 1));
      delay(50);
    }
    oscMsg = new OscMessage(header + "/GlobalCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(InitMinDAC).add(InitRangeDAC).add(FaderMinDAC).add(FaderRangeDAC).
      add(AuxMinDAC).add(AuxRangeDAC).add(AuxBMinDAC).add(AuxBRangeDAC);
    SendOSCMsg(oscMsg);
    println("Sent Global Config OSC: " + oscMsg);
  }

  public void FillJSON(JSONObject jsonObj)
  {
    //jsonObj.setInt("Init Bank", initPage);
    jsonObj.setInt("Def Min DAC", InitMinDAC);
    jsonObj.setFloat("Def Range DAC", InitRangeDAC);
    jsonObj.setInt("Def Min Fader", FaderMinDAC);
    jsonObj.setInt("Def Range Fader", FaderRangeDAC);
    jsonObj.setInt("Def Min Aux", AuxMinDAC);
    jsonObj.setInt("Def Range Aux", AuxRangeDAC);
    jsonObj.setInt("Def Min Aux B", AuxBMinDAC);
    jsonObj.setInt("Def Range Aux B", AuxBRangeDAC);
  }

  public void ReadJSON(JSONObject jsonObj)
  {
    if (!( //checkJSONItem(jsonObj, "Init Bank") &&
            checkJSONItem(jsonObj, "Def Min DAC") && checkJSONItem(jsonObj, "Def Range DAC") &&
            checkJSONItem(jsonObj, "Def Min Fader") && checkJSONItem(jsonObj, "Def Range Fader") &&
            checkJSONItem(jsonObj, "Def Min Aux") && checkJSONItem(jsonObj, "Def Range Aux") &&
            checkJSONItem(jsonObj, "Def Min Aux B") && checkJSONItem(jsonObj, "Def Range Aux B")))
    {
      println("Error loading Global Config :" + jsonObj);
      return;
    }

    InitMinDAC = jsonObj.getInt("Def Min DAC");
    InitRangeDAC = jsonObj.getInt("Def Range DAC");
    FaderMinDAC = jsonObj.getInt("Def Min Fader");
    FaderRangeDAC = jsonObj.getInt("Def Range Fader");
    AuxMinDAC = jsonObj.getInt("Def Min Aux");
    AuxRangeDAC = jsonObj.getInt("Def Range Aux");
    AuxBMinDAC = jsonObj.getInt("Def Min Aux B");
    AuxBRangeDAC = jsonObj.getInt("Def Range Aux B");
    //println("Load :"+jsonObj);
  }

}

class InputPortCfg
{
  int MIDIChannel;
  int ControllerNumber; // MIDI CC Controller number
  float ClockDivider;
  int ClockShift;

  InputPortCfg()
  {
    MIDIChannel = 1;
    ControllerNumber = 1;
    ClockDivider = 1.0f;
    ClockShift = 0;
  }

  public void ReadBaseOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("iifi") == true)
    {
      MIDIChannel = oscMsg.get(0).intValue();
      ControllerNumber = oscMsg.get(1).intValue();
      ClockDivider = oscMsg.get(2).floatValue();
      ClockShift = oscMsg.get(3).intValue();
      println("Values: " + oscMsg.get(0).intValue() + "/" + oscMsg.get(1).intValue() + "/" + 
        oscMsg.get(2).floatValue() + "/" + oscMsg.get(3).intValue());
    }
    else
      println("Wrong InputPortCfg TypeTag: " + oscMsg.typetag());
  }

  public void SendBaseOSC(String header)
  {
    oscMsg = new OscMessage(header + "/InputCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIChannel).add(ControllerNumber).add(ClockDivider).add(ClockShift);
    SendOSCMsg(oscMsg);
    println("Sent Base Input Config OSC: " + oscMsg);
  }

  public void MIDIChannel(int theValue)
  {
    MIDIChannel = theValue;
  }

  public void ClockDivider(float theValue)
  {
    ClockDivider = theValue;
  }

  public void ControllerNumber(int theValue)
  {
    ControllerNumber = theValue;
  }
}

class AnInputPortCfg extends InputPortCfg
{
  //InputFunctions MIDIfunction;
  int MIDIfunction;
  RangeConv Ranges; // Ranges for ADC and MIDI with conversion functions
  int ClipLow = 0, ClipHigh = 120;
  int RangeBipolar = 0;
  int NRPNparMSB, NRPNparLSB;

  AnInputPortCfg()
  {
    MIDIfunction = 0;
    Ranges = new RangeConv();
    RangeBipolar = 0;
    NRPNparMSB = 127;
    NRPNparLSB = 127;
  }

  //void LowLimit(int theValue){ClipLow= theValue;}
  public void ReadOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("iiiiii") == true)
    {
      MIDIfunction = oscMsg.get(0).intValue();
      ClipLow = oscMsg.get(1).intValue();
      ClipHigh = oscMsg.get(2).intValue();
      RangeBipolar = oscMsg.get(3).intValue();
      NRPNparMSB = oscMsg.get(4).intValue();
      NRPNparLSB = oscMsg.get(5).intValue();
      ConfigChanged = true;
      println("Values: " + oscMsg.get(0).intValue() + "/" + oscMsg.get(1).intValue() + "/" + 
        oscMsg.get(2).intValue() + "/" + oscMsg.get(3).intValue() + "/" + oscMsg.get(4).intValue() + "/" + 
        oscMsg.get(5).intValue());
    }
    else
      println("Wrong AnInputPortCfg TypeTag: " + oscMsg.typetag());
  }

  public void SendOSC(String header)
  {
    SendBaseOSC(header);
    oscMsg = new OscMessage(header + "/AnagCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIfunction).add(ClipLow).add(ClipHigh).add(RangeBipolar).add(NRPNparMSB).add(NRPNparLSB);
    SendOSCMsg(oscMsg);
    println("Sent Analog Input Config OSC: " + oscMsg);
    Ranges.SendOSC(header);
  }

  public void FillJSON(JSONObject jsonObj)
  {
    jsonObj.setInt("MIDI Channel", MIDIChannel);
    jsonObj.setInt("Controller Number", ControllerNumber);
    jsonObj.setFloat("Clock Divider", ClockDivider);
    jsonObj.setInt("Clock Shift", ClockShift);
    jsonObj.setInt("Function", MIDIfunction);
    jsonObj.setInt("Clip Low", ClipLow);
    jsonObj.setInt("Clip High", ClipHigh);
    jsonObj.setInt("Bipolar Input", RangeBipolar);
    jsonObj.setInt("NRPN MSB", NRPNparMSB);
    jsonObj.setInt("NRPN LSB", NRPNparLSB);
    JSONObject RangeObj = new JSONObject();
    Ranges.FillJSON(RangeObj);
    jsonObj.setJSONObject("Range", RangeObj);
  }

  public void ReadJSON(JSONObject jsonObj)
  {
    if (!(checkJSONItem(jsonObj, "MIDI Channel") && checkJSONItem(jsonObj, "Controller Number") &&
          checkJSONItem(jsonObj, "Clock Divider") && checkJSONItem(jsonObj, "Clock Shift") &&
          checkJSONItem(jsonObj, "Function") && checkJSONItem(jsonObj, "Clip Low") &&
          checkJSONItem(jsonObj, "Clip High") && checkJSONItem(jsonObj, "Bipolar Input") &&
          checkJSONItem(jsonObj, "NRPN MSB") && checkJSONItem(jsonObj, "NRPN LSB") && checkJSONItem(jsonObj, "Range")))
      return;

    MIDIChannel = jsonObj.getInt("MIDI Channel");
    ControllerNumber = jsonObj.getInt("Controller Number");
    ClockDivider = jsonObj.getFloat("Clock Divider");
    ClockShift = jsonObj.getInt("Clock Shift");
    MIDIfunction = jsonObj.getInt("Function");
    ClipLow = jsonObj.getInt("Clip Low");
    ClipHigh = jsonObj.getInt("Clip High");
    RangeBipolar = jsonObj.getInt("Bipolar Input");
    NRPNparMSB = jsonObj.getInt("NRPN MSB");
    NRPNparLSB = jsonObj.getInt("NRPN LSB");
    JSONObject RangeObj = jsonObj.getJSONObject("Range");
    Ranges.ReadJSON(RangeObj);
    //println("Load :"+jsonObj);
  }
}

class DigPortCfg extends InputPortCfg
{
  //GateFunctions MIDIfunction;
  int MIDIfunction;
  int ControllerValue; // MIDI CC Controller Value
  int DelayGate = 2;
  int NoteToSend = 60;
  DigPortCfg()
  {
    MIDIfunction = 0;
    ControllerValue = 0;
    DelayGate = 2;
    NoteToSend = 60;
  } //LATCH;}

  public void ReadOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("iiii") == true)
    {
      MIDIfunction = oscMsg.get(0).intValue();
      ControllerValue = oscMsg.get(1).intValue();
      DelayGate = oscMsg.get(2).intValue();
      NoteToSend = oscMsg.get(3).intValue();
      ConfigChanged = true;
      println("Values: " + oscMsg.get(0).intValue() + "/" + oscMsg.get(1).intValue() + "/" + 
        oscMsg.get(2).intValue() + "/" + oscMsg.get(3).intValue());
    }
    else
      println("Wrong DigCfg TypeTag: " + oscMsg.typetag());
  }

  public void SendOSC(String header)
  {
    SendBaseOSC(header);
    oscMsg = new OscMessage(header + "/DigCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIfunction).add(ControllerValue).add(DelayGate).add(NoteToSend);
    SendOSCMsg(oscMsg);
    println("Sent Digital Input Config OSC: " + oscMsg);
  }

  public void FillJSON(JSONObject jsonObj)
  {
    jsonObj.setInt("MIDI Channel", MIDIChannel);
    jsonObj.setInt("Controller Number", ControllerNumber);
    jsonObj.setFloat("Clock Divider", ClockDivider);
    jsonObj.setInt("Clock Shift", ClockShift);
    jsonObj.setInt("Function", MIDIfunction);
    jsonObj.setInt("Controller value", ControllerValue);
    jsonObj.setInt("Delay Gate", DelayGate);
    jsonObj.setInt("Note to Send", NoteToSend);
  }

  public void ReadJSON(JSONObject jsonObj)
  {
    if (!(checkJSONItem(jsonObj, "MIDI Channel") && checkJSONItem(jsonObj, "Controller Number") &&
          checkJSONItem(jsonObj, "Clock Divider") && checkJSONItem(jsonObj, "Clock Shift") &&
          checkJSONItem(jsonObj, "Function") && checkJSONItem(jsonObj, "Controller value") &&
          checkJSONItem(jsonObj, "Delay Gate") && checkJSONItem(jsonObj, "Note to Send")))
      return;

    MIDIChannel = jsonObj.getInt("MIDI Channel");
    ControllerNumber = jsonObj.getInt("Controller Number");
    ClockDivider = jsonObj.getFloat("Clock Divider");
    ClockShift = jsonObj.getInt("Clock Shift");
    MIDIfunction = jsonObj.getInt("Function");
    ControllerValue = jsonObj.getInt("Controller value");
    DelayGate = jsonObj.getInt("Delay Gate");
    NoteToSend = jsonObj.getInt("Note to Send");
    //println("Load :"+jsonObj);
  }
}

int ANRANGEMAX = 4095;
int INITMINDAC = ANRANGEMAX;
int INITRANGEDAC = -ANRANGEMAX;

class RangeConv
{
  int minMIDI, rangeMIDI;
  int minDAC, rangeDAC;
  boolean UseMultiPoint = false;
  RangeConv()
  {
    // Default to 12 bits in the CV for 120 MIDI values; (10 octaves)
    minMIDI = 0;
    rangeMIDI = 120;
    minDAC = INITMINDAC;
    rangeDAC = INITRANGEDAC;
    UseMultiPoint = false;
  }

  public void ReadOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("iiii") == true)
    {
      minMIDI = oscMsg.get(0).intValue();
      rangeMIDI = oscMsg.get(1).intValue();
      minDAC = oscMsg.get(2).intValue();
      rangeDAC = oscMsg.get(3).intValue();
      ConfigChanged = true;
      println("Values: " + oscMsg.get(0).intValue() + "/" + oscMsg.get(1).intValue() + "/" +
        oscMsg.get(2).intValue() + "/" + oscMsg.get(3).intValue());
    }
    else
      println("Wrong Ranges TypeTag: " + oscMsg.typetag());
  }

  public void SendOSC(String header)
  {
    oscMsg = new OscMessage(header + "/Range");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(minMIDI).add(rangeMIDI).add(minDAC).add(rangeDAC);
    SendOSCMsg(oscMsg);
    println("Sent Range Config OSC: " + oscMsg);
  }

  public void FillJSON(JSONObject jsonObj)
  {
    jsonObj.setInt("Min MIDI", minMIDI);
    jsonObj.setInt("Range MIDI", rangeMIDI);
    jsonObj.setInt("Min DAC", minDAC);
    jsonObj.setInt("Range DAC", rangeDAC);
  }

  public void ReadJSON(JSONObject jsonObj)
  {
    if (jsonObj.isNull("Min MIDI"))
    {
      println("JSON file format incorrect; Range");
      return;
    }
    minMIDI = jsonObj.getInt("Min MIDI");
    rangeMIDI = jsonObj.getInt("Range MIDI");
    minDAC = jsonObj.getInt("Min DAC");
    rangeDAC = jsonObj.getInt("Range DAC");
    //println("Load :"+jsonObj);
  }
}

class InputCtrlCfg
{
  int Chanfunction;
  AnInputPortCfg CVCfg;
  AnInputPortCfg FadCfg;
  DigPortCfg GateCfg;

  InputCtrlCfg()
  {
    Chanfunction = 0;
    CVCfg = new AnInputPortCfg();
    FadCfg = new AnInputPortCfg();
    GateCfg = new DigPortCfg();
  }

  public void ReadOSC(OscMessage oscMsg)
  {
    if (oscMsg.typetag().equals("i") == true)
    {
      Chanfunction = oscMsg.get(0).intValue();
      println("Values: " + oscMsg.get(0).intValue());
      ConfigChanged = true;
    }
    else
      println("Wrong InputCtrlCfg TypeTag: " + oscMsg.typetag());
  }

  public void SendOSC(String header)
  {
    CVCfg.SendOSC(header + "/CV");
    delay(10);
    FadCfg.SendOSC(header + "/Fad");
    delay(10);
    GateCfg.SendOSC(header + "/Gate");
    delay(10);
    oscMsg = new OscMessage(header + "/Function");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(Chanfunction);
    SendOSCMsg(oscMsg);
    println("Sent Input Control Config OSC: " + oscMsg);
  }

  public void FillJSON(JSONObject jsonObj)
  {
    JSONObject GateObj = new JSONObject();
    GateCfg.FillJSON(GateObj);
    jsonObj.setJSONObject("Gate", GateObj);
    JSONObject FadObj = new JSONObject();
    FadCfg.FillJSON(FadObj);
    jsonObj.setJSONObject("Fadder", FadObj);
    JSONObject CVObj = new JSONObject();
    CVCfg.FillJSON(CVObj);
    jsonObj.setJSONObject("CV", CVObj);
    jsonObj.setInt("Channel Function", Chanfunction);
  }

  public void ReadJSON(JSONObject jsonObj)
  {
    if (!(checkJSONItem(jsonObj, "Channel Function") && checkJSONItem(jsonObj, "CV") &&
          checkJSONItem(jsonObj, "Fadder") && checkJSONItem(jsonObj, "Gate")))
      return;

    Chanfunction = jsonObj.getInt("Channel Function");
    JSONObject CVObj = jsonObj.getJSONObject("CV");
    CVCfg.ReadJSON(CVObj);
    JSONObject FadObj = jsonObj.getJSONObject("Fadder");
    FadCfg.ReadJSON(FadObj);
    JSONObject GateObj = jsonObj.getJSONObject("Gate");
    GateCfg.ReadJSON(GateObj);
    //println("Load :"+jsonObj);
  }
}

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

public void setupSerial()
{
  serial = new Serial(this, Serial.list()[serialListNumber], baud);
}

public void stopSerial()
{
  serial.stop();
}

public void serialEvent(Serial serial)
{
  //decode the message
  while (serial.available() > 0)
  {
    slipDecode(PApplet.parseByte(serial.read()));
  }
}

OscMessage oscMsg;

// Send OSC message to both serial and UDP
public void SendOSCMsg(OscMessage comMsg)
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
public void SerialSendToUDP()
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

public void serialSend(byte[] data)
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

public int byteArrayToInt(int[] b, int offset)
{
  int value = 0;
  for (int i = 0; i < 4; i++)
  {
    int shift = (4 - 1 - i) * 8;
    value += (b[i + offset] & 0x000000FF) << shift;
  }
  return value;
}

public float arr2float(byte[] buf, int pos)
{
  ByteBuffer bb = ByteBuffer.allocate(4);
  bb.put(buf, pos, 4);
  return bb.getFloat(0);
}

public OscMessage oscDataReady()
{
  //////Provisional hasta usar solo un tipo de buffer
  if (serialBuffer.size() < 1)
    return null;
  ArrayList serBuffer = new ArrayList();
  serBuffer.clear();
  //copy the buffer over
  for (int i = 0; i < serialBuffer.size(); i++)
  {
    serBuffer.add(PApplet.parseInt(serialBuffer.get(i)));
  }

  String address = "";
  String strMsg = "";

  int pos = 0;
  int b = (Integer)serBuffer.get(pos++);
  ArrayList argumentTypes = new ArrayList();

  while (b != 0)
  {
    address += PApplet.parseChar(b);
    b = (Integer)serBuffer.get(pos++);
  }

  // Skip address zeros and the comma for the parameters
  pos += 4 - ((address.length()) % 4);

  b = (Integer)serBuffer.get(pos++);
  while (b != 0)
  {
    argumentTypes.add(PApplet.parseChar(b));
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
      byteArr[0] = PApplet.parseByte((Integer)serBuffer.get(pos++));
      byteArr[1] = PApplet.parseByte((Integer)serBuffer.get(pos++));
      byteArr[2] = PApplet.parseByte((Integer)serBuffer.get(pos++));
      byteArr[3] = PApplet.parseByte((Integer)serBuffer.get(pos++));
      float f = arr2float(byteArr, 0);
      oscMsg.add(f);
      strMsg=strMsg+"/"+f;
      break;
    case 's':
      String str = "";
      char c = PApplet.parseChar((Integer)serBuffer.get(pos++));
      while (c != 0x00)
      {
        str += c;
        c = PApplet.parseChar((Integer)serBuffer.get(pos++));
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

byte eot = PApplet.parseByte(192);
byte slipesc = PApplet.parseByte(219);
byte slipescend = PApplet.parseByte(220);
byte slipescesc = PApplet.parseByte(221);

byte previousByte;

public void slipDecode(byte incoming)
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

public void slipEncode(byte incoming)
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
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "ConfigVCMC" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
