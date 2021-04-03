/*

*/

#ifndef MorseLib_h
#define MorseLib_h

#include "Arduino.h"


extern int autoSQL(unsigned int pin);
extern String morseRx(unsigned int pin, int tim, int sql, double tol);
extern  void morseTx(unsigned int pin, String ip, int t);
String morseEncode(String ip);
String morseDecode(String ip);
}

