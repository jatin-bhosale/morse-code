/*

*/

#ifndef MorseLib_h
#define MorseLib_h

#include "Arduino.h"


struct morseStruct {
  int wpm;
  String output;
};

extern int autoSQL(unsigned int pin);

extern String morseRx(unsigned int pin, int tim, int sql, double tol);

extern void morseTx(unsigned int pin, String ip, int t);

extern morseStruct getWPM(unsigned int pin, int sql);

String morseEncode(String ip);

String morseDecode(String ip);

String autoCorr(String ip);

boolean verifyMorse(String ip);

#endif

