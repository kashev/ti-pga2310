/*
 *  ti-pga2310
 *      An Arduino / Energia Library for control of a Texas Instruments
 *      PGA2310 Stereo Volume Control.
 *  github.com/kashev/ti-pga2310 - Kashev Dalmia - kashev.dalmia@gmail.com
 *
 *  PGA2310.h
 */

#ifndef TI_PGA2310_H
#define TI_PGA2310_H

/* #include "Energia.h" */
/* #include "Arduino.h" */

class PGA2310 {
public:
    /* Constructors */
    PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN, uint8_t pinMUTE);
    PGA2310 (uint8_t pinCS, uint8_t pinSDATA, uint8_t pinSCLK, uint8_t pinZCEN);

    void begin (void);

    /* Getters */

    uint8_t getLeftVolume (void);
    uint8_t getRightVolume (void);

    /* Setters */

    void setVolume (uint8_t left, uint8_t right);\
    void restoreVolume (void);

    void mute (void); // Note: mute() toggles muted state.

private:
    
    void SPIWrite (uint8_t byte);

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
