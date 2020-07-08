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
/** @addtogroup GeneralUtil 
 *  @{
 */

/**
 *  \file MultiPointConv.ino
 *  \brief Helper class for MIDI to DAC conversions 
 */


/*
// Make conversion interpolating between points. (TODO update using invintervalConvert as reference)
int32_t RangeConv::intervalConvert (int inp) {
    int val = inp - minMIDI; // Adjust learnt zero value

    // Look for interval
    int interv = val * 20 / rangeMIDI;
    if (interv > 19) interv = 19;
    int A = DACPoints[interv]; // low output interpolation point
    int B = DACPoints[interv + 1]; // high output interpolation point

    // get value
    int32_t outp = A + (val - interv * rangeMIDI / 20) * (B - A) * 20 / rangeMIDI;
    // if( outp < 0) outp = 0; else if(outp >ANRANGEMAX) outp = ANRANGEMAX;
    #ifdef PRINTDEBUG
      Serial.print("Int: ");
      Serial.print(interv);
      Serial.print(" Low Int: ");
      Serial.print(A);
      Serial.print(" High Int: ");
      Serial.println(B);

      Serial.print("Note Input: ");
      Serial.print(val);
      Serial.print("/");
      Serial.print(inp);
      Serial.print(" DAC Output: ");
      Serial.println(outp);
      #endif	
    return outp;
}


int RangeConv::invintervalConvert (int32_t inp) {
    int i;

    // Convert input into standard DAC range
    inp = INITMINDAC + ((float)inp - minDAC) * INITRANGEDAC / rangeDAC;
    // Look for interval
    boolean found = false;
    for (i = 2; i < 20 && !found; i++) {
        found = ((inp - DACPoints[i] - HALFNOTERANGE) * rangeDAC < 0) ? true : false;
    }
    i = i - 2;
    int32_t A = DACPoints[i]; // low output interpolation point
    int32_t B = DACPoints[i + 1]; // high output interpolation point
    float interv = (float)(inp - A) / (B - A);

    // get value
    int outp = minMIDI + round ((interv + i) * rangeMIDI / 20);
    #ifdef PRINTDEBUG
    static unsigned long countmil = 0;
    if( millis() > countmil+1000) {
        countmil = millis();
        Serial.print("Int: ");
        Serial.print(interv+i);
        Serial.print(" Low Int: ");
        Serial.print(A);
        Serial.print(" High Int: ");
        Serial.println(B);

        Serial.print("Note Input: ");
        Serial.print(BankSelected	);
        Serial.print("/");
        Serial.print(inp);
        Serial.print(" DAC Output: ");
        Serial.println(outp);
    }
    #endif
    return outp;
}
*/

void RangeConv::SetDAC (int32_t minD, int32_t RangeD) {
    minDAC = minD;
    rangeDAC = RangeD;
    /*	int32_t oldMin = DACPoints[0];
        int32_t oldRange = DACPoints[20]-DACPoints[0];

        // Scale current calibration to new rangeDAC
        if(UseMultiPoint){
            for( int i=0; i<21;i++) DACPoints[i]= minD+(DACPoints[i]-oldMin)*RangeD/oldRange;
            }
    */
}


/** @} */
