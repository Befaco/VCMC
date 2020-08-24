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
 *  \file MultiPointConv.h
 *  \brief Helper class for MIDI to DAC conversions 
 */


#define MAXCLIP 1.0 //0.9925 // Clip to maximum when close to it

// Utility class to convert MIDI to CV Range
// Linear scale input to output (defined as minimun/range)
class RangeConv {
private:
    // byte DACnum = 0;
    int16_t minMIDI, rangeMIDI;
    int16_t minDAC, rangeDAC;

public:
    RangeConv (int16_t minM, int16_t rangeM):
		minMIDI(minM), rangeMIDI(rangeM), minDAC(DEFMINDAC), rangeDAC(DEFRANGEDAC)
		{
        }
	
    RangeConv ():
        // Default to 12 bits in the CV for 120 MIDI values; (10 octaves)
		minMIDI(0), rangeMIDI(120), minDAC(DEFMINDAC), rangeDAC(DEFRANGEDAC)
		{
		}

    // Make conversion
    int16_t Convert (int inp) {
        return linealConvert (inp);
    }
    int invConvert (int16_t inp) {
            return invlinealConvert (inp);
    }
    float PercentScale (int inp) // Return a value from 0 to 1.0 to indicate inp position on the MIDI scale
    {
        return (inp - minMIDI) / (float)rangeMIDI;
    }
    float invPercentScale (int16_t inp) // Return a value from 0 to 1.0 to indicate inp position on the DAC scale
    {
		//float result =((float)inp - minDAC) / rangeDAC;
		float result =((float)inp - minDAC) / rangeDAC + 0.5/rangeMIDI;
		if(result>MAXCLIP) return 1.0;
		else if( result<0.0) return 0.0;
		return result;
        //return ((float)inp - minDAC) / rangeDAC;
    }
    ///< Return Max value for the DAC
    int16_t getMaxDAC(void) {return minDAC+rangeDAC;}
    int16_t getMaxMIDI(void) {return minMIDI+rangeMIDI;}     ///< Return Max value for the MIDI
    // Conversion functions
    private:
    int16_t linealConvert (int inp) { // Convert MIDI to DAC
        return (minDAC + PercentScale (inp) * rangeDAC);
    }
	
    int invlinealConvert (int16_t inp) { // Convert DAC to MIDI
        return (minMIDI + invPercentScale (inp) * rangeMIDI);
    }
	


    public:
    // Set/get DATA
    void SetDAC (int16_t minD, int16_t RangeD){    
        minDAC = minD;
        rangeDAC = RangeD;
    }

    void SetMIDI (int16_t minD, int16_t RangeD) {
        minMIDI = minD;
        rangeMIDI = RangeD;
    }
    void getDAC (int16_t &minD, int16_t &RangeD) {
        minD = minDAC;
        RangeD = rangeDAC;
    }
    void getMIDI (int16_t &minD, int16_t &RangeD) {
        minD = minMIDI;
        RangeD = rangeMIDI;
    }

#ifdef USECONFIGOSC
    void SaveCfgOSC (char *address);
	void ReadCfgOSC(OSCMessage *pMsg);
#endif
};

/** @} */
