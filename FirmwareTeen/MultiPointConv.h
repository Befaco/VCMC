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


/////////////////////////////////
// External references
//extern uint16_t DACPoints[21];
//int32_t getInitMinDAC ();
//int32_t getInitRangeDAC ();
//boolean UseMultiPoint = false;

#define HALFNOTERANGE 17 // DAC Steps for half a note 4096/120/2
#define MAXCLIP 1.0 //0.9925 // Clip to maximum when close to it

// Utility class to convert MIDI to CV Range
// Linear scale input to output (defined as minimun/range)
class RangeConv {
    private:
    // byte DACnum = 0;
    int16_t minMIDI, rangeMIDI;
    int32_t minDAC, rangeDAC;

    public:
    RangeConv (int16_t minM, int16_t rangeM)://, int32_t minD, int32_t rangeD):
		minMIDI(minM), rangeMIDI(rangeM), minDAC(DEFMINDAC), rangeDAC(DEFRANGEDAC)
		{ResetCalPoints ();}
	
    RangeConv ():
        // Default to 12 bits in the CV for 120 MIDI values; (10 octaves)
		minMIDI(0), rangeMIDI(120), minDAC(DEFMINDAC), rangeDAC(DEFRANGEDAC)
		{
			ResetCalPoints (); 
			//UseMultiPoint = 0;
		}

    // Make conversion
    int32_t Convert (int inp) {
        /*if(UseMultiPoint) return intervalConvert(inp);
        else*/
        return linealConvert (inp);
    }
    int invConvert (int32_t inp) {
        /*if (UseMultiPoint)
            return invintervalConvert (inp);
        else*/
            return invlinealConvert (inp);
    }
    float PercentScale (int inp) // Return a value from 0 to 1.0 to indicate inp position on the MIDI scale
    {
        return (inp - minMIDI) / (float)rangeMIDI;
    }
    float invPercentScale (int32_t inp) // Return a value from 0 to 1.0 to indicate inp position on the DAC scale
    {
		//float result =((float)inp - minDAC) / rangeDAC;
		float result =((float)inp - minDAC) / rangeDAC + 0.5/rangeMIDI;
		if(result>MAXCLIP) return 1.0;
		else if( result<0.0) return 0.0;
		return result;
        //return ((float)inp - minDAC) / rangeDAC;
    }
    ///< Return Max value for the DAC
    int32_t getMaxDAC(void) {return minDAC+rangeDAC;}
    int16_t getMaxMIDI(void) {return minMIDI+rangeMIDI;}     ///< Return Max value for the MIDI
    // Conversion functions
    private:
    int32_t linealConvert (int inp) { // Convert MIDI to DAC
        return (minDAC + PercentScale (inp) * rangeDAC);
    }
	
    int invlinealConvert (int32_t inp) { // Convert DAC to MIDI
        return (minMIDI + invPercentScale (inp) * rangeMIDI);
    }
	
    //int32_t intervalConvert (int inp);
    //int invintervalConvert (int32_t inp);

    public:
    void ResetCalPoints (void) {
        //for (int i = 0; i < 21; i++) DACPoints[i] = minDAC + ((float)i) / 20 * rangeDAC;
    }
    // Set/get DATA
    void SetDAC (int32_t minD, int32_t RangeD);
    void SetMIDI (int16_t minD, int16_t RangeD) {
        minMIDI = minD;
        rangeMIDI = RangeD;
    }
    void getDAC (int32_t &minD, int32_t &RangeD) {
        minD = minDAC;
        RangeD = rangeDAC;
    }
    void getMIDI (int16_t &minD, int16_t &RangeD) {
        minD = minMIDI;
        RangeD = rangeMIDI;
    }
//    void setMultiPointMode (boolean Mode) { UseMultiPoint = Mode; }
#ifdef USEOSC
    void SaveCfgOSC (char *address);
	void ReadCfgOSC(OSCMessage *pMsg);
#endif
};

/*
// Utility class to convert MIDI to CV Range
// Multi-Linear scale input to output (defined as 20 fix points and output as interpolation between each pair of fix points)
class MultiPointConv:public RangeConv{
 public:
  MultiPointConv(){
    // Default to 12 bits in the CV for 120 MIDI values;
    minMIDI=0; rangeMIDI=120; minDAC = 0;rangeDAC = ANRANGEMAX;
    ResetCalPoints();
  }
  // Make conversion
  int32_t intervalConvert( int inp);
  int invintervalConvert( int32_t inp);
  void ResetCalPoints(void) { for( int i=0; i<21; i++) DACPoints[i]= minDAC+ ((float)i)/20*rangeDAC;}
  // Set DATA
  void SetDAC( int32_t minD, int32_t RangeD) { minDAC= minD; rangeDAC= RangeD; ResetCalPoints();}
};
*/

/** @} */
