/*

*/

#ifndef MorseCode_h
#define MorseCode_h

#include "Arduino.h"


class MorseCode
{
    public:
        int autoSQL(unsigned int pin);
        String morseRx(unsigned int pin, int tim, int sql, double tol);
        void morseTx(unsigned int pin, String ip, int t);
    private:
        String morseEncode(String ip);
        String morseDecode(String ip);
}

#endif