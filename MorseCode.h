/*

*/

#ifndef MorseCode_h
#define MorseCode_h

#include "Arduino.h"


class MorseCode
{
    public:
        MorseCode(unsigned int pin);
        int autoSQL();
        String morseRx(int tim, int sql, double tol);
        void morseTx(String ip, int t);
    private:
        unsigned int _pin
        String morseEncode(String ip);
        String morseDecode(String ip);
}

