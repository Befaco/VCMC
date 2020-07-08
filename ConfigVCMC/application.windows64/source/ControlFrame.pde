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

  void setupInputAnag()
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

  void setupInputRange()
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

  void setupInputControl()
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

  void draw()
  {
    background(100);
  }

  void controlEvent(ControlEvent theControlEvent)
  {
    if (theControlEvent.isFrom("rangeMIDI"))
    {
      prueConf.Ranges.minMIDI = int(theControlEvent.getController().getArrayValue(0));
      prueConf.Ranges.rangeMIDI = int(theControlEvent.getController().getArrayValue(1)) - prueConf.Ranges.minMIDI;
    }
    if (theControlEvent.isFrom("rangeDAC"))
    {
      prueConf.Ranges.minDAC = int(theControlEvent.getController().getArrayValue(0));
      prueConf.Ranges.rangeDAC = int(theControlEvent.getController().getArrayValue(1)) - prueConf.Ranges.minDAC;
    }
    if (theControlEvent.isFrom("LimitsMIDI"))
    {
      prueConf.ClipLow = int(theControlEvent.getController().getArrayValue(0));
      prueConf.ClipHigh = int(theControlEvent.getController().getArrayValue(1));
    }
  }
}
