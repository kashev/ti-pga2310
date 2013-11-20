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

}

PGA2310::PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN)
{
    _pinCS    = pinCS;
    _pinSDATA = pinSDATA;
    _pinSCLK  = pinSCLK;
    _pinZCEN  = pinZCEN;

    _hard_mute = 0;
}