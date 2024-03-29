
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

#ifndef __have__DefString_h__
#define __have__DefString_h__

/** @addtogroup GeneralUtil 
 *  @{
 */

/**
 *  \file Definitions.h
 *  \brief Definitions and configuration flags 
 */


/// Predefined port names
static const char PortNames[][SIZEPORTNAMES+1] = {
	//"", "Volume", "Pan", "Balanc", "Other"
	"",
	"REVE_D", "REVE_T", "REVE_M", "DELAYT", "DELAYD", "DELAYM", 
	"NOISE ", "CUTOFF", "FEEDBK", "RESON ", "RING  ", "LFORAT",
	"LFOINI", "LFOSHA", "LFOMOD", "LFOTAR", "VCOOCT", "VCOSHA",
	"VCOFRE", "VCOTUN", "PORTAM", "BANK  ", "PROGCH", "MODUL ", 
	"PITCHB", "PAN   ", "VOL   ", "RELEAS", "ATTACK", "DECAY ",
	"SUSTAI", "RATE  ", "DEPTH ", "TIME  ", "INIT  ", "SHAPE ",
	"MOD   ", "TARGET", "BREATH", "BALANC", "EXPRES", "LEGATO",
	"ALLOFF", "OMNI  ", "PANIC ", "POLY  ", "MONO  "
	};


/// Analog Functions names
static const char * const PortFuncStr[] = {
	"V/Oct", "Note", "CC#", "Vel", "Prog", "After", "Bend", "Perc",
	"Clock", "ST/SP", "ST/PA", "ST/SP", "ST/PA",
	"Free", "NRPN7", "NRPN14", "No Func",
	"Gate", "Start", "Stop", "Cont", "CC Tr", "CC Lt", "Trig","Latch","V/O Pol","CC14b",
	"Scale","Chord","ChInv","ScRoot","Delay","Rand"};
/// Digital Functions names
static const char * const PortGateFuncStr[] = {"Note", "Latc", "CC", "CC"/*"CC Lt#"*/, "Clock", 
	"ST/SP", "CNT/SP", "ST/SP Lt", "CNT/SP Lt", "Start", "Stop", "Continue",
	"Panic","No Func", "Poly", "Chord" };
/// MIDI Clock multiplier texts
static const char * const ClokShiftTxt[] = { "x32", "x16", "x08", "x04", "x02", "x01", "/02", "/04", "/08", "/16", "/32"};
/// MIDI Clock multiplier texts
static const char * const RangeInTxt[] = { "+10V", "Bip.", "+5V"};

/// Credit text for splah screen
#ifdef CVTHING
static const char CreditText[] =
	"                    "
	"                    "
	"                    "
	"      CV Thing      "
	"       v1.4.1       "
	"   www.befaco.org";
#else
static const char CreditText[] =
	"                    "
	"                    "
	"                    "
	"       VCMC         "
	"      v1.4.1        "
	"   www.befaco.org";
#endif

/// Credits graph 128x24 pixels
static const unsigned char mini_splash [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xE3, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xC7, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x8F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x1F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0x3F, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFF, 0xFE, 0x7F, 0xDF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x00, 0x1F, 0xC3, 0xFC, 0x00, 0xFF, 0x9F, 0xC7, 0xC0, 0x7E, 0x3F, 0x07, 0xF0,
0x0F, 0xC3, 0xFC, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0xFE, 0x1F, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0E, 0x00, 0x1C, 0x00, 0xFF, 0x00, 0x7F, 0x80, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xC3, 0xFC, 0x00, 0x7F, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xCF, 0xFC, 0x00, 0x3F, 0x01, 0xFE, 0x00, 0xFF, 0xFF, 0xC7, 0xC0, 0x00, 0x3F, 0x07, 0xF0,
0x0F, 0xDF, 0xFC, 0x00, 0x1F, 0xC3, 0xFC, 0x00, 0xFF, 0xFF, 0xC7, 0xEF, 0xFC, 0x3F, 0xF7, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFC, 0x00, 0xFF, 0xFF, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0xFF, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x1F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xF0,
0x0F, 0xFF, 0xFC, 0x7F, 0xFF, 0xE3, 0xFC, 0x00, 0xFC, 0x0F, 0xC7, 0xFF, 0xFE, 0x3F, 0xFF, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/// Notes text names
static const char NotesNamesScale[][3] = {
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

static const char NotesNames [][5]= {
	"C-2", "C#-2", "D-2", "D#-2", "E-2", "F-2", "F#-2", "G-2", "G#-2", "A-2", "A#-2", "B-2",
	"C-1", "C#-1", "D-1", "D#-1", "E-1", "F-1", "F#-1", "G-1", "G#-1", "A-1", "A#-1", "B-1", 
	"C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0", 
	"C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1", 
	"C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2", 
	"C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", 
	"C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", 
	"C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", 
	"C6", "C#6", "D6", "E6", "D#6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6", 
	"C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
	"C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8",
	"C9", "C#9", "D9", "D#9", "E9", "F9", "F#9", "G9", "G#9", "A9", "A#9", "B9",
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B",
	"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
};

#endif

/** @} */
