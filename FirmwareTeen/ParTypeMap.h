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
#ifndef ParTypeMap_h_
#define ParTypeMap_h_

struct SysExDataStruct
{
    uint8_t pos;
    uint8_t Type;
    uint8_t ParID;
};

enum PARNames
{
    PAR_Size=100,
    PAR_DefMinDAC,
    PAR_DefRangeDAC,
    PAR_DefMinFader,
    PAR_DefRangeFader,
    PAR_DefMinAux,
    PAR_DefRangeAux,
    PAR_DefMinAuxB,
    PAR_DefRangeAuxB,
    PAR_AutoOff,
    PAR_ClockDivider,
    PAR_ClockShift,
    PAR_GENOptions,

    PAR_MIDIChannel,
    PAR_PortName,
    PAR_NameSufix,
    PAR_DelayGate,
    PAR_Function,
    PAR_Range_minMIDI,
    PAR_Range_rangeMIDI,
    PAR_Range_minDAC,
    PAR_Range_rangeDAC,
    PAR_ClipLow,
    PAR_ClipHigh,
    PAR_FunctionData,
    PAR_Option1,

    PAR_GateMIDIChannel,
    PAR_GatePortName,
    PAR_GateNameSufix,
    PAR_GateDelayGate,
    PAR_GateFunction,
    PAR_GateFunctionData,

    PAR_ChannelFunction
};

enum typesEnum
{
    ui08TYPE = 0, // uint8_t
    si08TYPE,     // int8_t
    charTYPE,     // char
    ui16TYPE,     // uint16_t
    si16TYPE,     // int16_t
    ui32TYPE,     // uint32_t
    si32TYPE,     // int32_t
    floaTYPE      // float
};
const uint8_t sizeTypes[] = {1, 1, 1, 2, 2, 4, 4, 4};

static const SysExDataStruct DATA_General[] = {
    {28, ui16TYPE, PAR_Size},

    { 4, si16TYPE, PAR_DefMinDAC},
    { 6, si16TYPE, PAR_DefRangeDAC},
    { 8, si16TYPE, PAR_DefMinFader},
    {10, si16TYPE, PAR_DefRangeFader},
    {12, si16TYPE, PAR_DefMinAux},
    {14, si16TYPE, PAR_DefRangeAux},
    {16, si16TYPE, PAR_DefMinAuxB},
    {18, si16TYPE, PAR_DefRangeAuxB},
    {20, si16TYPE, PAR_AutoOff},
    {22, floaTYPE, PAR_ClockDivider},
    {26, si08TYPE, PAR_ClockShift},
    {27, si08TYPE, PAR_GENOptions},
};

static const SysExDataStruct DATA_Analog[] = {
    {20, ui16TYPE, PAR_Size},
    // Base Port Data
    { 0, ui08TYPE, PAR_MIDIChannel},
    { 1, ui08TYPE, PAR_PortName},
    { 2, charTYPE, PAR_NameSufix},
    { 3, ui16TYPE, PAR_DelayGate},
    // Analog Data
    { 5, ui08TYPE, PAR_Option1},
    { 6, si16TYPE, PAR_Range_minMIDI},
    { 8, si16TYPE, PAR_Range_rangeMIDI},
    {10, si16TYPE, PAR_Range_minDAC},
    {12, si16TYPE, PAR_Range_rangeDAC},
    {14, si16TYPE, PAR_ClipLow},
    {16, si16TYPE, PAR_ClipHigh},
    {18, ui16TYPE, PAR_FunctionData}};

static const SysExDataStruct DATA_Fader[] = {
    {14, ui16TYPE, PAR_Size},
    // Base Port Data
    { 0, ui08TYPE, PAR_MIDIChannel},
    { 1, ui08TYPE, PAR_PortName},
    { 2, charTYPE, PAR_NameSufix},
    // Analog Data
    { 3, ui08TYPE, PAR_Function},
    { 4, si16TYPE, PAR_Range_minMIDI},
    { 6, si16TYPE, PAR_Range_rangeMIDI},
    { 8, si16TYPE, PAR_ClipLow},
    {10, si16TYPE, PAR_ClipHigh},
    {12, ui16TYPE, PAR_FunctionData}};

static const SysExDataStruct DATA_Digital[] = {
    { 8, ui16TYPE, PAR_Size},
    // Base Port Data
    { 0, ui08TYPE, PAR_GateMIDIChannel},
    { 1, ui08TYPE, PAR_GatePortName},
    { 2, charTYPE, PAR_GateNameSufix},
    { 3, ui16TYPE, PAR_GateDelayGate},
    // Analog Data
    { 5, ui08TYPE, PAR_GateFunction},
    { 6, ui16TYPE, PAR_GateFunctionData}};

static const SysExDataStruct DATA_Bank[] = {
    { 1, ui16TYPE, PAR_Size},
    // Base Port Data
    { 0, ui08TYPE, PAR_ChannelFunction}};

#endif