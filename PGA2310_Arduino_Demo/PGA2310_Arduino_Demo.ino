/*
 *  ti-pga2310
 *  v 1.0.0
 *  github.com/kashev/ti-pga2310
 *      
 *      An Arduino / Energia Library for control of a Texas Instruments
 *      PGA2310 Stereo Volume Control.
 *  
 *  Kashev Dalmia - kashev.dalmia@gmail.com
 *
 *  PGA2310_Arduino_Demo.ino
 */

/*
The MIT License (MIT)

Copyright (c) 2013 Kashev Dalmia

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Arduino.h"
#include "PGA2310.h"

/*
 * PINS
 */
#define PIN_PGA_ZCEN  2
#define PIN_PGA_CS    3
#define PIN_PGA_SCLK  4
#define PIN_PGA_SDATA 5
#define PIN_PGA_MUTE  6

/*
 * VOLUME CONTROLLER
 */
PGA2310 pga = PGA2310(PIN_PGA_CS,
                      PIN_PGA_SDATA,
                      PIN_PGA_SCLK,
                      PIN_PGA_ZCEN,
                      PIN_PGA_MUTE);

/*
 * SETUP & LOOP
 */

void
setup (void)
{
    pga.begin();
}

void
loop (void)
{
    /* Sweeps through Volumes Under MAX_GAIN */
    if (pga.getVolume() <= MAX_GAIN)
    {
        pga.incVolume();
    }
    else
    {
        pga.setVolume(0);
    }
    delay(250);
}
