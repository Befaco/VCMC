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

import java.nio.ByteBuffer;
import java.util.*;
import processing.serial.*;
//download at http://ubaa.net/shared/processing/udp/
import hypermedia.net.*;
//download at http://www.sojamo.de/libraries/controlp5
import controlP5.*;
//download at http://www.sojamo.de/libraries/oscp5/
import oscP5.*;
import netP5.*;

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
void settings()
{
  // configure the screen size and frame rate
  size(800, 800, P3D);
}

void setup()
{

  theGlobalConfig = new GlobalCfg();
  CVvalues = new float[9];
  Fadervalues = new float[9];
  GateValues = new float[9];
  InputControlsCFG = new InputCtrlCfg[9];
  for (int i = 0; i < 9; i++)
  {
    //AnControlsCFG[i] = new AnInputPortCfg();
    CVvalues[i] = Fadervalues[i] = 0.0;
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

void draw()
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

void drawIncomingPackets()
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

void drawIncomingSerial()
{
  lastSerialPacket = millis();
}

void drawIncomingUDP()
{
  lastUDPPacket = millis();
}
