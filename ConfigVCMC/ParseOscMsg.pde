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

void ParseOScMsg(OscMessage oscMsg)
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
        CVvalues[idx - 1] = 1.0 - oscMsg.get(0).floatValue();
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

void SendOSCCommand(String comString)
{
  println(comString);
  OscMessage comMsg = new OscMessage("/VCMC/Command");
  comMsg.add(comString);

  SendOSCMsg(comMsg);
}
