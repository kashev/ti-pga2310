/*
 *  ti-pga2310
 *      An Arduino / Energia Library for control of a Texas Instruments
 *      PGA2310 Stereo Volume Control.
 *  github.com/kashev/ti-pga2310 - Kashev Dalmia - kashev.dalmia@gmail.com
 *
 *  PGA2310.cpp
 */

#include "PGA2310.h"

PGA2310::PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN, uint8_t pinMUTE)
{
    _pinCS    = pinCS;
    _pinSDATA = pinSDATA;
    _pinSCLK  = pinSCLK;
    _pinZCEN  = pinZCEN;
    _pinMUTE  = pinMUTE;

    _hard_mute = 1;
    _muted = 0;

    _v_left = _v_right = _pv_left = _pv_right = 0;

}

PGA2310::PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN)
{
    _pinCS    = pinCS;
    _pinSDATA = pinSDATA;
    _pinSCLK  = pinSCLK;
    _pinZCEN  = pinZCEN;

    _hard_mute = 0;
    _muted = 0;

    _v_left = _v_right = _pv_left = _pv_right = 0;
}

void
PGA2310::begin (void)
{
    pinMode(_pinCS,    OUTPUT);
    pinMode(_pinSDATA, OUTPUT);
    pinMode(_pinSCLK,  OUTPUT);
    pinMode(_pinZCEN,  OUTPUT);

    if (_hard_mute)
        pinMode(_pinMUTE, OUTPUT);
}

uint8_t
PGA2310::getLeftVolume (void)
{
    return _v_left;
}

uint8_t
PGA2310::getRightVolume (void)
{
    return _v_right;
}

void
PGA2310::SPIWrite (uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++, byte <<= 1)
    {
        digitalWrite(_pinSCLK, LOW);
        if (0x80 & byte)
            digitalWrite(_pinSDATA, HIGH);
        else
            digitalWrite(_pinSDATA, LOW);
        digitalWrite(_pinSCLK, HIGH);
    }
}

void
PGA2310::setVolume (uint8_t left, uint8_t right)
{
    digitalWrite(_pinCS, LOW);
    SPIWrite(right);
    SPIWrite(left);
    digitalWrite(_pinCS, HIGH);

    _pv_left = _v_left; _pv_right = _v_right;
    _v_left = left; _v_right = right;
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
    _pv_right = tr; _pv_left = pl;
}

void
PGA2310::mute (void)
{
    if (_muted)
    {
        if(_hard_mute)
            digitalWrite(_pinMUTE, LOW);
        else:
            setVolume(0, 0);
        _muted = 0;
    }
    else
    {
        if(_hard_mute)
            digitalWrite(_pinMUTE, HIGH);
        else:
            restoreVolume();
        _muted = 1;
    }
}
