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
/*
#include "PrjIncludes.h"

union VCMCParData
{
    float fData;
    uint32_t lData;
    uint16_t sData[2];
    uint8_t bytes[4]="";
};

enum typesEnum
{
    ui8TYPE = 0, // uint8_t
    i8TYPE,      // int8_t
    ui16TYPE,   // uint16_t
    i16TYPE,    // int16_t
    ui32TYPE,   // uint32_t
    i32TYPE,    // int32_t
    flTYPE  // float
};

class TypePar{
    private:
        uint16_t parId=0;   // Parameter id
        uint8_t typeDesc=ui8TYPE;   // Parameter Type (see enumerator) 
        uint16_t incrInObj=0; // Incremental Position of parameter in the destination object
        char *sDescr=nullptr;   // Parameter text description
        // void *pointr;   
        //VCMCParData dataCopy = VCMCParData(); // ?? TODO check if neccesary
    public:
        TypePar(uint16_t pId, uint8_t tD, uint16_t iObj, char* sD){ 
            parId= pId;
            typeDesc = tD;
            incrInObj = iObj;
            sDescr = sD;
         };
};


#define MAX_STRING_SIZE 16
const char VCMCParNames[][16]={
    //GlobalCfg
    "InitPage",
    "DefMinDAC",
    "DefRangeDAC",
    "DefMinFader",
    "DefRangeFader",
    "DefMinAux",
    "DefRangeAux",
    "DefMinAuxB",
    "DefRangeAuxB",
    "AutoOff",
    // InputCtrlCfg
    "ChannelFunction"
};

enum VCMCParIds
{

};

TypePar GlobalCfgPar[] = {
{(uint16_t)0,ui8TYPE,(uint16_t) offsetof(GlobalCfg,initPage),(char*)VCMCParNames[0]}
};


0TypePar InputCtrlCfgPar[] = {
{16,ui8TYPE,0,VCMCParNames[10]}
};
*/


#endif