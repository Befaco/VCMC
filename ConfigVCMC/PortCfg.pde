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
  void ReadOSC(OscMessage oscMsg)
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

  void SendOSC(String header)
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

  void FillJSON(JSONObject jsonObj)
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

  void ReadJSON(JSONObject jsonObj)
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
    ClockDivider = 1.0;
    ClockShift = 0;
  }

  void ReadBaseOSC(OscMessage oscMsg)
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

  void SendBaseOSC(String header)
  {
    oscMsg = new OscMessage(header + "/InputCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIChannel).add(ControllerNumber).add(ClockDivider).add(ClockShift);
    SendOSCMsg(oscMsg);
    println("Sent Base Input Config OSC: " + oscMsg);
  }

  void MIDIChannel(int theValue)
  {
    MIDIChannel = theValue;
  }

  void ClockDivider(float theValue)
  {
    ClockDivider = theValue;
  }

  void ControllerNumber(int theValue)
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
  void ReadOSC(OscMessage oscMsg)
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

  void SendOSC(String header)
  {
    SendBaseOSC(header);
    oscMsg = new OscMessage(header + "/AnagCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIfunction).add(ClipLow).add(ClipHigh).add(RangeBipolar).add(NRPNparMSB).add(NRPNparLSB);
    SendOSCMsg(oscMsg);
    println("Sent Analog Input Config OSC: " + oscMsg);
    Ranges.SendOSC(header);
  }

  void FillJSON(JSONObject jsonObj)
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

  void ReadJSON(JSONObject jsonObj)
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

  void ReadOSC(OscMessage oscMsg)
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

  void SendOSC(String header)
  {
    SendBaseOSC(header);
    oscMsg = new OscMessage(header + "/DigCfg");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(MIDIfunction).add(ControllerValue).add(DelayGate).add(NoteToSend);
    SendOSCMsg(oscMsg);
    println("Sent Digital Input Config OSC: " + oscMsg);
  }

  void FillJSON(JSONObject jsonObj)
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

  void ReadJSON(JSONObject jsonObj)
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

  void ReadOSC(OscMessage oscMsg)
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

  void SendOSC(String header)
  {
    oscMsg = new OscMessage(header + "/Range");
    //initPage= oscMsg.get(0).intValue();
    oscMsg.add(minMIDI).add(rangeMIDI).add(minDAC).add(rangeDAC);
    SendOSCMsg(oscMsg);
    println("Sent Range Config OSC: " + oscMsg);
  }

  void FillJSON(JSONObject jsonObj)
  {
    jsonObj.setInt("Min MIDI", minMIDI);
    jsonObj.setInt("Range MIDI", rangeMIDI);
    jsonObj.setInt("Min DAC", minDAC);
    jsonObj.setInt("Range DAC", rangeDAC);
  }

  void ReadJSON(JSONObject jsonObj)
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

  void ReadOSC(OscMessage oscMsg)
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

  void SendOSC(String header)
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

  void FillJSON(JSONObject jsonObj)
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

  void ReadJSON(JSONObject jsonObj)
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
