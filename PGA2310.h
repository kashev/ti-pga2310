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
 *  PGA2310.h
 */


#ifndef TI_PGA2310_H
#define TI_PGA2310_H

#include "Arduino.h"
/* #include "Energia.h" */

/*
 *  GAIN - can set gain to unity, boost, etc. OOB behavior is to not use
 *         amplification portion of chip, but can be changed by commenting
 *         UNITY_GAIN definition of MAX_GAIN and using 255 definition.
 *         
 *         Gain (dB) = 31.5 − 0.5 * (255 − N)
 */
   #define UNITY_GAIN         192 //  0.0 dB
   #define MAX_GAIN    UNITY_GAIN //  0.0 dB
/* #define MAX_GAIN           255 // 31.5 dB */

class PGA2310 {
public:
    /* Constructors */
    PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN, uint8_t pinMUTE);
    PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN);

    /* Initializers */
    void begin (uint8_t zcen_enable = 1);

    /* Getters */
    uint8_t getLeftVolume (void);
    uint8_t getRightVolume (void);
    uint8_t getVolume (void);
    boolean isMuted (void);

    /* Setters */
    void setVolume (uint8_t left, uint8_t right);
    void setVolume (uint8_t vol);
    void restoreVolume (void);

    void mute (void);
    void toggleMute (void);

    void incVolume (void);
    void decVolume (void);

    void incVolume (uint8_t step);
    void decVolume (uint8_t step);

private:
    /* Direct Writing to Pins */
    void SPIWrite (uint8_t b);

    /*
     * DATA MEMBERS
     */
    /* Pins */
    uint8_t _pinCS, _pinSDATA, _pinSCLK, _pinZCEN, _pinMUTE;
    /* Muting Options */
    uint8_t _hard_mute, _muted;

    /* Volume */
    uint8_t _v_left,  _v_right;
    uint8_t _pv_left, _pv_right;
};


#endif /* TI_PGA2310_H */
