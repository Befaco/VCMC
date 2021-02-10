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

/** @addtogroup InputPortsGroup
  @{
*/
/**
    \file InputControl.ino
    \brief Functions definition for Input Control class
*/

/**
    \brief Helper function to initialize the Controller number

    \param [in] CtrlNum Port number for the module
    \return

    \details
*/
bool InputControl::SetControlNumber (byte CtrlNum) {
  ControlNumber = CtrlNum;
  SetPort (CtrlNum, CtrlNum, PINBUT[CtrlNum], PINLED[CtrlNum]);
  return true;
}

/**
    \brief Initialization function to set port values

    \param [in] PortCV   port number for CV
    \param [in] PortSlid port number for Slider
    \param [in] PortGate port number for Gate
    \param [in] Ledp     port number for led
    \return TRUE if succesful

    \details Calls child CV,Slider and gate SetPort functions.\n
 	Defines custom filter parameters for Faders
*/
bool InputControl::SetPort (byte PortCV, byte PortSlid, byte PortGate, byte Ledp) {

  if (ControlNumber == 8) { // Aux inputs
    CVPort.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxAMinDAC, theApp.theGlobalCfg.AuxARangeDAC);
    Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.AuxBMinDAC, theApp.theGlobalCfg.AuxBRangeDAC);
    return CVPort.SetPort (PortCV, PINAUXA) & Slider.SetPort (PortSlid, PINAUXB) &
           GateBut.SetPort (0, 0);
  } else { // CV and sliders
    // CV port uses reversed values
    //CVPort.PortCfg.Ranges.setMultiPointMode (true); // Use MultiPoint for CV
    // CVPort.PortCfg.Ranges.SetDAC( 4148, -4145);  // Hack for current values in DAC
    CVPort.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.InitMinDAC, theApp.theGlobalCfg.InitRangeDAC);
    Slider.typeSlider = true;
    Slider.PortCfg.Ranges.SetDAC (theApp.theGlobalCfg.FaderMinDAC, theApp.theGlobalCfg.FaderRangeDAC);
#ifdef FILTEREDANAGINPUT
    // Set parameters for FilteredInput
    Slider.setFilter(theApp.theGlobalCfg.filterFader, theApp.theGlobalCfg.ActThrFader);
    //Slider.FilteredInput->setSnapMultiplier (0.0001);
    //Slider.FilteredInput->setActivityThreshold (17);
    //Slider.FilteredInput->enableEdgeSnap();
#endif
  }

  return CVPort.SetPort (PortCV, PINCVINPUT) & Slider.SetPort (PortSlid, PINSLIDERINPUT) &
         GateBut.SetPort (PortGate, Ledp);
}


/**
    \brief Read selected ports and store  data change

    \param [in] readDig True to read digital port (Gate)
    \param [in] readCV True to read CV port
    \param [in] readFader True to read Fader port
    \return TRUE if data has changed

    \details
    Read each of the port with the function input\n
    See individual port read functions for more details
*/
bool InputControl::ReadPorts (bool readDig, bool readCV, bool readFader) {
  long CVData;
  bool changed;

#ifndef CVTHING
  long SlidData, GateSt;
  if (readDig) Gatechanged |= GateBut.ReadPort (GateSt);
  if (readFader) Slidchanged |= Slider.ReadPort (SlidData);
#endif
  if (readCV) CVchanged |= CVPort.ReadPort (CVData);

  changed = CVchanged | Slidchanged | Gatechanged;

  return changed;
}

/**
    \brief Read  ports and react on data change

    \return TRUE if data has changed

    \details Execute functions for each input control module
*/
bool InputControl::DoChanges (void) {
  bool changed = CVchanged | Slidchanged | Gatechanged;
  if (!servicingPorts)
    if (changed) {
      OnDataChange ();
      CVchanged = Slidchanged = Gatechanged = 0;
    }

  return changed;
}

/**
    \brief Read  ports and react on data change

    \return TRUE if data has changed

    \details Main loop function for each input control module, see individual port read functions for more details
 	@note Deprecated, use InputControl::ReadPorts(bool readDig, bool readCV, bool readFader) instead
*/
bool InputControl::ReadPorts (bool onlyDig) {
  long CVData, SlidData, GateSt;
  bool changed;

  if (servicingPorts) {
    if (!onlyDig) {
      CVchanged |= CVPort.ReadPort (CVData);
    }
    Gatechanged |= GateBut.ReadPort (GateSt);
  } else
    Slidchanged |= Slider.ReadPort (SlidData); // Service Fader out of interrupts

  changed = CVchanged | Slidchanged | Gatechanged;

  // if( !onlyDig) // Do not process during digital port reading
  if (!servicingPorts)
    if (changed) {
      OnDataChange ();
      CVchanged = Slidchanged = Gatechanged = 0;
    }

  return changed;
}

/**
    \brief Send Note On.

    \details Send a Note On for the specified port, note, and MIDI channel.
*/
void InputControl::SendNoteOn(byte controlNumber, InputPort& port, byte chan, int datatosend,bool chord) {
  SendLastNoteOff(controlNumber, port, chan);
  port.LastSentMIDIData = datatosend;
  if( chord)
    Chord.setRootNote(datatosend, MidiMerge.VelData[port.PortCfg.MIDIChannel - 1], chan, true);
  else
    MidiMerge.sendNoteOn (datatosend, MidiMerge.VelData[port.PortCfg.MIDIChannel - 1], chan);
}

/**
    \brief Send Note Off for last note.

    \details Send a Note Off for any Note that is stll on on the specified port.
*/
void InputControl::SendLastNoteOff(byte controlNumber, InputPort& port, byte chan,bool chord) {
  if ( port.LastSentMIDIData >= 0 ) {
    if( chord)
      Chord.noteoffChord();
    else
      MidiMerge.sendNoteOff (port.LastSentMIDIData, 0, chan);

    port.LastSentMIDIData = -999;
  }
}

// This table is used to determine the Ionian modal chord notes as offsets from the key's root note.
byte scaleChords[12][4] = {
  { 0, 4, 7, 11 },
  { 0, 0, 0, 0 }, // not in key (the next array element wil be used)
  { 0, 3, 7, 10 },
  { 0, 0, 0, 0 },
  { 0, 3, 7, 10 },
  { 0, 4, 7, 11 },
  { 0, 0, 0, 0 },
  { 0, 4, 7, 10 },
  { 0, 0, 0, 0 },
  { 0, 3, 7, 10 },
  { 0, 0, 0, 0 },
  { 0, 3, 6, 10 }
};

// This table contains the offsets of the notes in the modal chord from the scaleChords table for
// the supported inversions, voicings, and combinations thereof.
int invdrop[12][4] = {
  { 0, 0, 0, 0 },     // root (close voicing)
  { 12, 0, 0, 0 },    // 1st inv
  { 12, 12, 0, 0 },   // 2nd inv
  { 12, 12, 12, 0}, //{ 0, 0, 0, -12 },   // 3rd inv

  { 0, 0, -12, 0 },   // root drop 2
  { 12, 0, 0, -12 },  // 1st inv drop 2
  { 0, 12, 0, 0 },    // 2nd inv drop 2
  { 12, 0, 12, 0 },   // 3rd inv drop 2

  { 0, -12, 0, 0 },   // root drop 3
  { 12, 0, -12, 0 },  // 1st inv drop 3
  { 12, 12, 0, -12 }, // 2nd inv drop 3
  { 0, 12, 12, 0  }   // 3rd inv drop 3
};

/* 
// 3 notes chords inversions and drop
{0,0,0}, //close voicing
{12,0,0},
{12,12,0},

{0,-12,0}, // drop 2
{12,0,-12},
{0,12,0},

{}
 */

/**
    \brief Provides functionality when inputs or gate change

    \details Core function for actions based on inputs changes
*/
void InputControl::OnDataChange (void) {
  int SendData = DataCalc ();
  bool GateStat = GetGateState ();

  // Blink LED
  // GateBut.setBlink(100, 100, 1);

  // Gate change management
  if (Gatechanged) {
    // In TRIGGER or LATCH send NoteOn/NoteOff with every change on inputs
    if (GateBut.PortCfg.MIDIfunction == TRIGGER || GateBut.PortCfg.MIDIfunction == LATCH) {
      ProcessGateNotes();
    }
    // Chord mode triggered in gates 1 and 5
    else if (GateBut.PortCfg.MIDIfunction == CHORD && ControlNumber%4 == 0 ) { //ControlNumber == 7 ) { //
      ProcessGateChord();
    }
    else if ( ControlNumber == 7 && GateBut.PortCfg.MIDIfunction == GATE8TRIG ) { // Trigger all CVs with PITCH8TRIG
      for (int i = 0; i < 8; i++) {
        if (CVControls[i].CVPort.PortCfg.MIDIfunction == PITCH8TRIG ) {
          uint8_t oldFn = CVControls[i].GateBut.PortCfg.MIDIfunction;
          //CVControls[i].GateBut.PortCfg.MIDIfunction = TRIGGER; //TF Unnecessary
          CVControls[i].GateBut.GateStatus = GateBut.GateStatus;
          CVControls[i].Gatechanged = true;
          CVControls[i].OnDataChange();
          CVControls[i].Gatechanged = false;
          CVControls[i].GateBut.PortCfg.MIDIfunction = oldFn;
        }
      }
    }
    else
      GateBut.SendMIDI ();
  }
  // Apply data calculation based on selected function
  // In case of operations performed, send calculated data using CVport

  if (CVchanged || Slidchanged) {
    switch (Config.Chanfunction) {
      case NOTEMODE:
        break;
      case INDEP:
        // Do not send Notes CV changes on NOTEMODE
        if (CVchanged) {
          #ifdef CVTHING
          if (CVPort.PortCfg.MIDIfunction == PITCHLEVEL) { // send Trigger to paired CV
            if ( ControlNumber < 1)
              break;
            if ( ControlNumber == 7 && CVControls[6].CVPort.PortCfg.MIDIfunction != PITCHTRIG) { // Port 8 as trigger
              for (int i = 0; i < 7; i++) {
                if (CVControls[i].CVPort.PortCfg.MIDIfunction == PITCH8TRIG) {
                  CVControls[i].GateBut.GateStatus = CVPort.IsHigh();
                  CVControls[i].Gatechanged = true;
                  CVControls[i].OnDataChange();
                  CVControls[i].Gatechanged = false;
                }
              }
            } else
            {
              CVControls[ControlNumber - 1].GateBut.GateStatus = CVPort.IsHigh();
              CVControls[ControlNumber - 1].Gatechanged = true;
              CVControls[ControlNumber - 1].OnDataChange();
              CVControls[ControlNumber - 1].Gatechanged = false;
            }

            return;
          }
          //PITCH8TRIG
          #endif
          CVPort.SendMIDI (-9999, GateStat);
        }
        if (Slidchanged) {
          Slider.SendMIDI (-9999, GateStat);
        }
        break;
      case SUM:
      case MULTIPL:
        CVPort.SendMIDI (SendData, GateStat);
    }
  }
}


// Two blocks with 4 Input controls blocks: 1-4 5-8 for two independent chords
// CVPort in PITCHTRIG mode for chord note to play
// Slider in NOANFUNC mode to add its value to CV
// Gate 1 in CHORD mode for chord 1 and Gate 5 for chord 2
void InputControl::ProcessGateChord(void)
{      
  bool GateStat = GetGateState ();
  int16_t CVData, SlData;

  // Get the chord root, key, voicing index, and overall offset from the CV inputs and sliders.
  //  Adds CV if it is in PITCHTRIG mode and Fader if it is in NOANFFUNC mode
  // Root note
  CVData = (CVControls[ControlNumber + 3].CVPort.PortCfg.MIDIfunction == PITCHTRIG) ? CVControls[ControlNumber + 3].CVPort.MIDIData : 0;
  SlData = (CVControls[ControlNumber + 3].Slider.PortCfg.MIDIfunction == NOANFFUNC) ? CVControls[ControlNumber + 3].Slider.MIDIData : 0;
  byte root =  CVData + SlData;
  // Sliders 1/5 and 2/6 are scaled so that the travel of the slider covers the complete range of values.
  CVData = (CVControls[ControlNumber + 2].CVPort.PortCfg.MIDIfunction == PITCHTRIG) ? CVControls[ControlNumber + 2].CVPort.MIDIData : 0;
  SlData = (CVControls[ControlNumber + 2].Slider.PortCfg.MIDIfunction == NOANFFUNC) ? CVControls[ControlNumber + 2].Slider.MIDIData : 0;
  byte key = ((CVData + (SlData - 9) / 10)) % 12;
  CVData = (CVControls[ControlNumber + 1].CVPort.PortCfg.MIDIfunction == PITCHTRIG) ? CVControls[ControlNumber + 1].CVPort.MIDIData : 0;
  SlData = (CVControls[ControlNumber + 1].Slider.PortCfg.MIDIfunction == NOANFFUNC) ? CVControls[ControlNumber + 1].Slider.MIDIData : 0;
  byte idx = ((CVData + (SlData - 9) / 10)) % 12;
  // Note offset
  CVData = (CVControls[ControlNumber + 0].CVPort.PortCfg.MIDIfunction == PITCHTRIG) ? CVControls[ControlNumber + 0].CVPort.MIDIData : 0;
  SlData = (CVControls[ControlNumber + 0].Slider.PortCfg.MIDIfunction == NOANFFUNC) ? CVControls[ControlNumber + 0].Slider.MIDIData : 0;
  byte sliderOffset = CVData + SlData;

  byte const* offsets = scaleChords[(root + 12 - key) % 12];
  if (offsets[1] == 0) {
    root--;
    offsets = scaleChords[(root + 12 - key) % 12];
  }

  byte chord[4];
  // Create the chosen chord voicing.
  for (int i = 0; i < 4; i++) {
    int note;
    note = root + offsets[i] + invdrop[idx][i] + sliderOffset;
    // Adjust out-of-range chord tones to be in range.
    while (note < 0) note += 12;
    while (note > 127) note -= 12;
    chord[i] = note; 
  }

  // Send Note On and Note Off for the chord notes. 
  byte FIRSTBLK = (ControlNumber==0)?0:4;
  byte LASTBLK = (ControlNumber==0)?3:7;
  for (int i = FIRSTBLK; i < LASTBLK+1; i++) {
    if(CVControls[i].CVPort.PortCfg.MIDIfunction!=PITCHTRIG) // Play only notes on channels with PITCHTRIG
      continue;
    if (GateStat == true) {
      SendNoteOn(i, CVControls[i].CVPort, CVControls[i].CVPort.PortCfg.MIDIChannel, chord[i - FIRSTBLK], false);
      if(CVControls[i].GateBut.PortCfg.MIDIfunction == NODIGFUNC ||
        CVControls[i].GateBut.PortCfg.MIDIfunction == CHORD )
                CVControls[i].GateBut.setBlink(100, 100, 1); // Blink only on NODIGFUNC or CHORD modes
    }
    else
      SendLastNoteOff(i, CVControls[i].CVPort, CVControls[i].CVPort.PortCfg.MIDIChannel, false);
  }
}


void InputControl::ProcessGateNotes(void)
{
  int SendData = DataCalc ();
  bool GateStat = GetGateState ();

    if (GateStat == true) { // Send Note On
      GateBut.setBlink(100, 100, 1);
      
      // Independ channels
      bool played = false;
      if (Config.Chanfunction != INDEP) {
        // If SUM or MULTIP send result through CV channel if it has the trigger option
        if (CVPort.PortCfg.MIDIfunction == PITCHTRIG || CVPort.PortCfg.MIDIfunction == PITCH8TRIG) { //TF
          byte datatosend = CVPort.TrimValue (SendData);
          SendNoteOn(ControlNumber, CVPort, CVPort.PortCfg.MIDIChannel, datatosend);
          played = true;
          // GateBut.setBlink(100, 100, 1);
        }
      } else {

        if (CVPort.PortCfg.MIDIfunction == PITCHTRIG || CVPort.PortCfg.MIDIfunction == PITCH8TRIG) { // CV triggered notes
          byte datatosend = CVPort.TrimValue (CVPort.MIDIData);
          SendNoteOn(ControlNumber, CVPort, CVPort.PortCfg.MIDIChannel, datatosend);
          played = true;
        }
        if (Slider.PortCfg.MIDIfunction == PITCHTRIG) { // Fader triggered notes
          byte datatosend = Slider.TrimValue (Slider.MIDIData);
          SendNoteOn(ControlNumber, Slider, Slider.PortCfg.MIDIChannel, datatosend);
          played = true;
        }
      }
      if (!played) { // Gate play notes
        byte datatosend = GateBut.PortCfg.NoteToSend;
        SendNoteOn(ControlNumber, GateBut, GateBut.PortCfg.MIDIChannel, datatosend);
      }

    }
    else { // Send Note Off
      bool played = false;
      // If SUM or MULTIP send result through CV channel if it has the trigger option
      if (Config.Chanfunction != INDEP) {
                  
        if (CVPort.PortCfg.MIDIfunction == PITCHTRIG || CVPort.PortCfg.MIDIfunction == PITCH8TRIG) { //TF
          SendLastNoteOff(ControlNumber, CVPort, CVPort.PortCfg.MIDIChannel);
          played = true;
        }
      }
      // Independ channels
      else {
        if (CVPort.PortCfg.MIDIfunction == PITCHTRIG || CVPort.PortCfg.MIDIfunction == PITCH8TRIG) {
          SendLastNoteOff(ControlNumber, CVPort, CVPort.PortCfg.MIDIChannel);
          played = true;
        }
        if (Slider.PortCfg.MIDIfunction == PITCHTRIG) {
          SendLastNoteOff(ControlNumber, Slider, Slider.PortCfg.MIDIChannel);
          played = true;
        }
      }
      if (!played) {
        SendLastNoteOff(ControlNumber, GateBut, GateBut.PortCfg.MIDIChannel);
      }
    }
}

/**
    \brief Provides the calculation in case Sum or Multiplication is selected

    \return calculated value or -9999 if no calculation required

    \details Limits data output based on selected function (e.g. 0 to 127 for most MIDI functions)
*/
int InputControl::DataCalc (void) {
  int outdata = -9999;

  switch (Config.Chanfunction) {
    case NOTEMODE: //*** Review
    case INDEP:
      return -9999;
    case SUM:
      outdata = (CVPort.MIDIData + Slider.MIDIData);
      break;
    case MULTIPL:
      // Overflow due to result bigger than int is not managed
      // When the slider is set to percent, scale value down by 100
      if (Slider.PortCfg.MIDIfunction == PERCENT)
        outdata = ((long)CVPort.MIDIData * Slider.MIDIData) / 100;
      else
        outdata = ((long)CVPort.MIDIData * Slider.MIDIData);
  }
  byte bBip = CVPort.PortCfg.getInputRange();
  CVPort.PortCfg.setInputRange(NOOFFSET);
  outdata = CVPort.TrimValue(outdata);
  CVPort.PortCfg.setInputRange(bBip);


  return outdata; // CVPort.TrimValue(outdata);
}

/**
    \brief Return State of the gate/button

    \return State of Gate

    \details This function will return HIGH for activation of the gate and LOW for idle state
*/
bool InputControl::GetGateState () {
  return GateBut.GateStatus;
}


/**
    \brief change status of the Blink if neccessary

    \return State of Gate

    \details This function will return HIGH for activation of the gate and LOW for idle state
*/
void InputControl::Blink () {
  GateBut.blinkPort.playBlink ();
}


/**
    \brief Save Control configuration to EEPROM

    \param [in] addr address to save to
    \return number of bytes written

    \details Will save Gate, CV and Fader configs
*/
int InputControl::SaveCfg (int addr) {
  int MemPointer = addr;

  // Bank function
  EEPROM.put (MemPointer, (uint8_t)Config.Chanfunction);
  MemPointer += sizeof (uint8_t);
  // Chords and Scales
  EEPROM.put (MemPointer, (uint8_t)Chord.getScaleId());
  MemPointer += sizeof (uint8_t);
  EEPROM.put (MemPointer, (uint8_t)Chord.getChordType());
  MemPointer += sizeof (uint8_t);
  EEPROM.put (MemPointer, (uint8_t)Chord.getInvDrop());
  MemPointer += sizeof (uint8_t);

  // Reserved BANKGENERALeeSize for general data
  MemPointer += BANKGENERALeeSize - (MemPointer - addr);

  MemPointer += GateBut.SaveCfg (MemPointer);
  MemPointer += CVPort.SaveCfg (MemPointer);
  MemPointer += Slider.SaveCfg (MemPointer);
#ifdef PRINTDEBUG
  Serial.print( "Saved InputControl ");
  Serial.print( ControlNumber);
  Serial.print( ": ");
  Serial.print( addr);
  Serial.print( "/");
  Serial.print( MemPointer - addr);
  Serial.print( "/");
  Serial.println( MemPointer);
#endif
#ifdef USECONFIGOSC
  char msgTxt[120];
  sprintf (msgTxt, "/VCMC/Config/%d", ControlNumber + 1);
  SaveCfgOSC (msgTxt);
#endif
  if (ControlNumber == 8)
    return MemPointer - addr;
  else
    return BANKeeSize;
}

/**
    \brief Load Control configuration from EEPROM

    \param [in] addr address to load from
    \return number of bytes read

    \details Will load Gate, CV and Fader configs
*/
int InputControl::LoadCfg (int addr) {
  int MemPointer = addr;

  Config.Chanfunction = (CtrlFunctions)EEPROM.read (MemPointer);
  MemPointer += sizeof (uint8_t);
  // Chords and Scales
  uint8_t read = EEPROM.read (MemPointer);
  Chord.setScaleId(read);
  MemPointer += sizeof (uint8_t);
  read = EEPROM.read (MemPointer);
  Chord.setChord(read);
  MemPointer += sizeof (uint8_t);
  read = EEPROM.read (MemPointer);
  Chord.setInvDrop(read);
  MemPointer += sizeof (uint8_t);

  // Reserved BANKGENERALeeSize for general data
  MemPointer += BANKGENERALeeSize - (MemPointer - addr);

  MemPointer += GateBut.LoadCfg (MemPointer);
  MemPointer += CVPort.LoadCfg (MemPointer);
  MemPointer += Slider.LoadCfg (MemPointer);

  D(Serial.printf("Input Control %1d, size: %d\n", ControlNumber, MemPointer - addr));
  return BANKeeSize;//MemPointer - addr;
}


/** @} */
