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
 *  PGA2310.cpp
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

#include "PGA2310.h"

PGA2310::PGA2310 (uint8_t pinCS,
                  uint8_t pinSDATA,
                  uint8_t pinSCLK,
                  uint8_t pinZCEN,
                  uint8_t pinMUTE)
: _pinCS     (pinCS),
  _pinSDATA  (pinSDATA),
  _pinSCLK   (pinSCLK),
  _pinZCEN   (pinZCEN),
  _pinMUTE   (pinMUTE),
  _hard_mute (1),
  _muted     (0),
  _v_left    (0),
  _v_right   (0),
  _pv_left   (0),
  _pv_right  (0)
{
    /* Intentionally Empty */
}

PGA2310::PGA2310 (uint8_t pinCS,
                  uint8_t pinSDATA,
                  uint8_t pinSCLK,
                  uint8_t pinZCEN)
: _pinCS     (pinCS),
  _pinSDATA  (pinSDATA),
  _pinSCLK   (pinSCLK),
  _pinZCEN   (pinZCEN),
  _hard_mute (0),
  _muted     (0),
  _v_left    (0),
  _v_right   (0),
  _pv_left   (0),
  _pv_right  (0)
{
    /* Intentionally Empty */
}

/* By Default, Enable Zero Crossing Detection */
void
PGA2310::begin (uint8_t zcen_enable)
{
    pinMode(_pinCS,    OUTPUT);
    pinMode(_pinSDATA, OUTPUT);
    pinMode(_pinSCLK,  OUTPUT);

    digitalWrite(_pinCS,    HIGH);
    digitalWrite(_pinSDATA, HIGH);
    digitalWrite(_pinSCLK,  HIGH);

    if (_hard_mute)
    {
        pinMode(_pinMUTE, OUTPUT);
    }

    pinMode(_pinZCEN,  OUTPUT);

    if (zcen_enable)
    {
        digitalWrite(_pinZCEN, HIGH);
    }
    else
    {
        digitalWrite(_pinZCEN, LOW);
    }
}

uint8_t
PGA2310::getLeftVolume (void)
{
    if (_muted)
    {
        return 0;
    }
    else
    {
        return _v_left;
    }
}

uint8_t
PGA2310::getRightVolume (void)
{
    if (_muted)
    {
        return 0;
    }
    else
    {
        return _v_right;
    }
}

uint8_t
PGA2310::getVolume (void)
{
    return getLeftVolume();
}

boolean
PGA2310::isMuted (void)
{
    return (boolean)_muted;
}

void
PGA2310::SPIWrite (uint8_t b)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(_pinSCLK, LOW);
        
        if (0x80 & b)
        {
            digitalWrite(_pinSDATA, HIGH);
        }
        else
        {
            digitalWrite(_pinSDATA, LOW);
        }
        
        digitalWrite(_pinSCLK, HIGH);
        
        b <<= 1;
    }
}

void
PGA2310::setVolume (uint8_t left, uint8_t right)
{
    if ((left > MAX_GAIN) || (right > MAX_GAIN))
    {
        return; /* don't allow gains above MAX_GAIN */
    }

    digitalWrite(_pinCS, LOW);
    SPIWrite(right);
    SPIWrite(left);
    digitalWrite(_pinCS, HIGH);

    _pv_left = _v_left; _pv_right = _v_right;
    _v_left = left; _v_right = right;

    if (!(_hard_mute) && (_v_left == 0) && (_v_right == 0))
    {
        _muted = 1;
    }
    else
    {
        _muted = 0;
    }
}

void
PGA2310::setVolume (uint8_t vol)
{
    setVolume(vol, vol);
}

void
PGA2310::restoreVolume (void)
{
    uint8_t tr, tl;

    digitalWrite(_pinCS, LOW);
    SPIWrite(_pv_right);
    SPIWrite(_pv_left);
    digitalWrite(_pinCS, HIGH);

    tr = _v_right; tl = _v_left;

    _v_right = _pv_right; _v_left = _pv_left;
    _pv_right = tr; _pv_left = tl;
}

void
PGA2310::mute (void)
{
    if (_hard_mute)
    {
        digitalWrite(_pinMUTE, LOW);
    }
    else
    {
        setVolume(0, 0);
    }
    _muted = 1;
}

void
PGA2310::toggleMute (void)
{
    if (_muted)
    {   
        /* unmute */
        if (_hard_mute)
        {
            digitalWrite(_pinMUTE, HIGH);
        }
        else
        {
            restoreVolume();
        }    
        _muted = 0;
    }
    else
    {
        /* mute */
        if (_hard_mute)
        {
            digitalWrite(_pinMUTE, LOW);
        }
        else
        {
            setVolume(0, 0);
        }
        _muted = 1;
    }
}


void
PGA2310::incVolume (void)
{
    incVolume(1);
}

void
PGA2310::decVolume (void)
{
    decVolume(1);
}

void
PGA2310::incVolume (uint8_t step)
{
    int16_t nl, nr;

    if (_muted)
    {
        if (_hard_mute)
        {
            digitalWrite(_pinMUTE, HIGH);
        }
        _muted = 0;
    }

    nl = _v_left + step; nr = _v_right + step;

    nl = nl > MAX_GAIN ? MAX_GAIN : nl;
    nr = nr > MAX_GAIN ? MAX_GAIN : nr;

    if ((nl != _v_left) || (nr != _v_right))
    {
        /* minimize writes to device */
        setVolume((uint8_t)nl, (uint8_t)nr);
    }   
}

void
PGA2310::decVolume (uint8_t step)
{
    int16_t nl, nr;

    if (_muted)
    {
        if (_hard_mute)
        {
            digitalWrite(_pinMUTE, HIGH);
        }
        _muted = 0;
    }

    nl = _v_left - step; nr = _v_right - step;

    nl = nl < 0 ? 0 : nl;
    nr = nr < 0 ? 0 : nr;

    if ((nl != _v_left) || (nr != _v_right))
    {
        /* minimize writes to device */
        setVolume((uint8_t)nl, (uint8_t)nr);
    }   
}
