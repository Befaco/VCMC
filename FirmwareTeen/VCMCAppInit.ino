// Copyright 2019 Sergio Retamero.
//
// Author: Sergio Retamero (sergio.retamero@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
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

/** @ingroup Maingroup
 * @{
 */

/** 
 * \file VCMCAppInit.ino
 * \brief Define the functions for App initialisation
 * 
 */



/**
 * \brief Main setup Function
 * 
 */
void VCMCApp::setup(void)
{
    // Initialise display
    initdisplay();

    // Initialise encoder
    initEncoder();

    // Initialise Demux pins
    pinMode(PINDEMUXA, OUTPUT);
    pinMode(PINDEMUXB, OUTPUT);
    pinMode(PINDEMUXC, OUTPUT);

    // Set Voltage Offset low
    pinMode(PINOFFSET, OUTPUT);
    digitalWrite(PINOFFSET, LOW); //HIGH);

    // Init MIDI
    MidiMerge.begin();

    #ifdef USEOSC
        OSCMerge.begin(); 
    #else
        Serial.begin(115200);
    #endif
/* 
    ////// Test empty mem
    for (size_t i = 0; i < 500; i++)
    {
        EEPROM.write(i, 0);
    }
     */
    // Lod initial configutration from EEPROM
    FlashAccess->LoadInitialConfig();

    // Setup Controls
    initControls();

    // Init ADC
    initADC();

    // Lod initial configutration from EEPROM
    FlashAccess->LoadInitialPage();

    #ifdef USEI2C
        I2CMerge.begin();
    #endif


    // Init Menu
    gotoMenuSettings();
    theOLED->display();

    // Splash screen
#ifndef DEBUGMODE
    CreditsScreen();
    AutoCal = false;
#endif
// Ports reading interrupt
#ifdef USEREADINTERR
    beginPortsTimer();
#endif

    // Init Menu
    gotoMenuSettings();
    theOLED->display();
}

/**
 *  \brief Setup Controls. Default values for each port are set here
 *
 */
void VCMCApp::initControls(void)
{
    for (int i = 0; i < NUMCHAN; i++)
    {
        CVControls[i].SetControlNumber(i);
        CVControls[i].CVPort.PortCfg.MIDIChannel = 1;
        //CVControls[i].CVPort.PortCfg.SetMIDIFunc(PITCHTRIG);
        CVControls[i].CVPort.PortCfg.SetMIDIFunc(CONTROLCHANGE);
        CVControls[i].CVPort.PortCfg.ControllerNumber = i + 14; //29;

        CVControls[i].Slider.PortCfg.MIDIChannel = 1;
        CVControls[i].Slider.PortCfg.SetMIDIFunc(CONTROLCHANGE);
        //CVControls[i].Slider.PortCfg.SetMIDIFunc(VELOCITY);
        CVControls[i].Slider.PortCfg.ControllerNumber = i + 23; //20;

        CVControls[i].GateBut.PortCfg.MIDIChannel = 1;
        CVControls[i].GateBut.PortCfg.NoteToSend = 12 + i * 12;
    }
    CVControls[0].GateBut.PortCfg.NoteToSend = 60;
    CVControls[1].GateBut.PortCfg.NoteToSend = 62;
    CVControls[2].GateBut.PortCfg.NoteToSend = 64;
    CVControls[3].GateBut.PortCfg.NoteToSend = 65;
    CVControls[4].GateBut.PortCfg.NoteToSend = 67;
    CVControls[5].GateBut.PortCfg.NoteToSend = 69;
    CVControls[6].GateBut.PortCfg.NoteToSend = 71;
    CVControls[7].GateBut.PortCfg.NoteToSend = 72;

    CVControls[8].CVPort.PortCfg.ControllerNumber = 32;
    CVControls[8].Slider.PortCfg.ControllerNumber = 33;

    #ifdef USEI2C
    I2CMerge.InitDefControls();
    #endif
}

/**
 *  \brief Init the SSD1306 display
 *
 */
#ifdef ST3375SCR
#define RGB(r,g,b) (b<<11|g<<6|r)
//uint16_t ScrBuf[160 * 80];
#endif
void VCMCApp::initdisplay(void)
{
    #ifdef ST3375SCR
    theOLED->initR(INITR_MINI160x80_ST7735S);
    theOLED->setRotation(3);
    theOLED->setTextWrap(false);
    theOLED->setTextColor(RGB(31,31,31), RGB(0,0,0));
    theOLED->setCursor(0, 0);
    //theOLED->setFrameBuffer(ScrBuf);
    if(!theOLED->useFrameBuffer(true))
        Serial.println("Error allocating buffer memory");
#else
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    theOLED->begin(SSD1306_SWITCHCAPVCC);
    SPI.begin();
    // Moce SCK from pin 13 to pin 14
    SPI.setSCK(SCK_PIN_1);
    // Set SPI speed
    SPI.setClockDivider(SPI_CLOCK_DIV2); // 2.25  MHz

    digitalWrite(OLED_RESET, HIGH);
    delay(10);
    digitalWrite(OLED_RESET, LOW);
    delay(100);
    digitalWrite(OLED_RESET, HIGH);
    delay(100);

    digitalWrite(OLED_DC, LOW);
    digitalWrite(OLED_CS, LOW);

    SPI.transfer(0xAE); // Display off
    SPI.transfer(0xD5); // Set display clock divider
    SPI.transfer(0x80);
    SPI.transfer(0xA8); // Set multiplex
    SPI.transfer(0x3F);
    SPI.transfer(0xD3); // Set display offset
    SPI.transfer(SCREEN_OFFSET);
    SPI.transfer(0x40); // Set start line to zero
    SPI.transfer(0x8D); // Set charge pump
    SPI.transfer(0x14);
    SPI.transfer(0x20); // Set memory mode
    SPI.transfer(0x00);
    SPI.transfer(0xA0 | 0x1); // Set segment remapping
    SPI.transfer(0xC8);       // Set command Scan decode
    SPI.transfer(0xDA);       // Set Comm pins
    SPI.transfer(0x12);
    SPI.transfer(0x81); // Set contrast
    SPI.transfer(0xCF);
    SPI.transfer(0xd9); // Set precharge
    SPI.transfer(0xF1);
    SPI.transfer(0xDB); // Set Vcom detect
    SPI.transfer(0x30);//40);
    SPI.transfer(0xA4); // Allow display resume
    SPI.transfer(0xA6); // Set normal display
    SPI.transfer(0xAF); // Display On

    //theOLED->setRotation(2); // Rotate the screen 180 degrees
    // Clear the buffer.
    theOLED->clearDisplay();
    theOLED->setTextWrap(false);
    theOLED->setTextColor(WHITE, BLACK);

    #endif
}

/**
 *  \brief Initialise encoder
 *
 */
void VCMCApp::initEncoder(void)
{
    myTimer.begin(timerIsr, 1000);
    // Setup the button with an internal pull-up :
    pinMode(PINENCBUT, INPUT_PULLUP);
    // After setting up the button, setup the Bounce instance :
    EncButton->attach(PINENCBUT);
    EncButton->interval(5); // interval in ms
    EncButton->update ();
    delay(7);
    EncButton->update ();
}

#define ANREADAVG 1 /**< Number of reads the ADC module will perform every AnalogRead call */
/**
 *  \brief Init Teensy ADC chip
 *
 */
void VCMCApp::initADC(void)
{

    //	Set ADC inputs
    adc->adc0->setAveraging(ANREADAVG); // set number of averages
    //adc->setResolution(8); // set bits of resolution
    adc->adc0->setResolution(ANBITS); // set bits of resolution
                                //adc->adc0->calibrate();//analog_init();

    // it can be any of the ADC_CONVERSION_SPEED enum: VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED_16BITS, HIGH_SPEED or VERY_HIGH_SPEED
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);                  //LOW_SPEED);        //HIGH_SPEED_16BITS); // change the conversion speed
                                                                                     // it can be any of the ADC_MED_SPEED enum: VERY_LOW_SPEED, LOW_SPEED, MED_SPEED, HIGH_SPEED or VERY_HIGH_SPEED
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);                      //LOW_SPEED);              //VERY_HIGH_SPEED); // change the sampling speed
                                                                                     //ANRANGEMAX = adc->getMaxValue(ADC_0);
    adc->adc1->setAveraging(ANREADAVG);                                             // set number of averages
    adc->adc1->setResolution(ANBITS);                                               // set bits of resolution
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);                  //LOW_SPEED, ADC_1); // HIGH_SPEED_16BITS change the conversion speed
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED); // change the sampling speed 
}


/**@}*/