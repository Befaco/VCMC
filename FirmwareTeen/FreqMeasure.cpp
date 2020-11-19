
#include "PrjIncludes.h"
#include <FreqMeasureMulti.h>

// From CalMenu.ino
void ClearCalArea();
boolean selectionCalMode();
int updateCalSelection(int maxval);
extern boolean InitCal;
extern int PointSelected;
extern CalModes calMode;

// Definitions for current file
FreqMeasureMulti freq1;
float sum1=0;
int count1=0;


const float NotesFreq[]={
 16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,
 32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49,00, 51.91, 55.00, 58.27, 61.74,
 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00,103.83,110.00,116.54,123.47,
130.81,138.59,146.83,155.56,164.81,174.61,185.00,196.00,207.65,220.00,233.08,246.94,
261.63,277.18,293.66,311.13,329.63,349.23,369.99,392.00,415.30,440.00,466.16,493.88,
523.25,554.37,587.33,622.25,659.25,698.46,739.99,783.99,830.61,880.00,932.33,987.77,
1046.5,1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,1760,1864.66,1975.53,
2093,2217.46,2349.32,2489.02,2637.02,2793.83,2959.96,3135.96,3322.44,3520,3729.31,3951.07,
4186.01,4434.92,4698.63,4978.03,5274.04,5587.65,5919.91,6271.93,6644.88,7040,7458.62,7902.13
};

int FindNote(float freq){
    for (size_t i = 0; i < 12*9; i++)
    {
        if( NotesFreq[i]>freq)
            return i;
    }
    return -1;
}

boolean InitFreqMeas () {
    PointSelected = 1;

    theApp.stopPortsTimer();

    CVControls[0].CVPort.SetDemux ();

    sum1=0;
    count1=0;
    freq1.begin(22);

    InitCal = true;

    return true;
}

void displayFreqMeas () {
    int posx = 0, posy = 0;
    //char outBuf[17];

    // Clear area used
    theOLED->fillRect (posx, posy, 128 - posx, 64 - posy, BLACK); // Erase input area
    // Display title
    theOLED->setTextSize (1);
    theOLED->setCursor (posx, posy);
    theOLED->setTextColor (WHITE, BLACK);
    theOLED->print ("Frequency:");
    if (PointSelected == 0)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->print (" Cancel ");
    if (PointSelected == 1)
        theOLED->setTextColor (BLACK, WHITE);
    else
        theOLED->setTextColor (WHITE, BLACK);
    theOLED->println ("END");

    // Print values
    theOLED->setCursor (posx, posy + 12);
    /* theOLED->setTextColor (BLACK, WHITE);
    theOLED->println ("  1    2    3    4  ");
     */
    theOLED->setTextColor (WHITE, BLACK);

    float freq = freq1.countToFrequency(sum1 / count1);
    int posN = FindNote(freq);
    if( posN<1)
        theOLED->println("No Freq detected");
    else{
        float f1 = NotesFreq[posN - 1];
        float f2 = NotesFreq[posN];
        float cents = 3986.313714 * log10(freq / f1);

        theOLED->printf("Prev. %s / %4.1fHz\n\n", NotesNames[posN + 22], f1);
        theOLED->setCursor (posx, posy + 26);
        theOLED->printf(" Real: %4.2fHz\n", freq);
        theOLED->setCursor (posx, posy + 36);
        theOLED->setTextSize(2);
        if( cents > 50.)
            theOLED->printf(" %s -%02.0f c\n", NotesNames[posN + 23], 100-cents);
        else
            theOLED->printf(" %s +%02.0f c\n", NotesNames[posN + 22], cents);
        theOLED->setTextSize(1);
        theOLED->setCursor (posx, posy + 55);
        theOLED->printf(" Next %s / %4.1fHz\n", NotesNames[posN + 23], f2);
    }


    theOLED->display ();
}


boolean FreqMeas(){

    calMode = FreqMeasure;

    // Init Calibration
    if (!InitCal)
        if (!InitFreqMeas ()) {
            myMenu.setupPopup ("Freq. failed", 5000, 0, 17);
            return true; // If Init fails, abort
        }

    // Measure
    if (freq1.available()) {
        sum1 = sum1 + freq1.read();
        count1 = count1 + 1;
    }

    updateCalSelection (2); // Update Selection: cancel, OK

    static uint32_t prevMil =0;
    uint32_t current = millis();
    if( current-prevMil>500){
        prevMil = current;
        displayFreqMeas (); // Display values
        sum1 = 0;
        count1 = 0;
    }

    if (!selectionCalMode ()) {
        return false;
    }
    if (PointSelected < 2) { // Cancel or END Selected
        // TODO Check here if OK or Cancel
        ClearCalArea (); // Clear area used
        freq1.end();
        theApp.beginPortsTimer();

        InitCal = false;
        calMode = NoCalMode;
        return true;
    }
    return false;
}