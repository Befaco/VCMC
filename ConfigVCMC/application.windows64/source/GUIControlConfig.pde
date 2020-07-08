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

void setupGUIControlConfig()
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

void fileSelected(File selection)
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

void setupControlChangeTab()
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
