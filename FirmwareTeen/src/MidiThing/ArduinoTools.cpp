#include "ArduinoTools.h"

uint32_t getClock(uint32_t newval)
{
    volatile static uint32_t prevCycles = 0;
    volatile static uint32_t newCycles = 0;
    volatile static uint32_t initClock = 0;

    if(newval!=0xffff)
        initClock = newval;
    newCycles += getMicros() / 100 - prevCycles; // Resolution 1/10 milli secs
    prevCycles = newCycles;

    return (newCycles-initClock);
}