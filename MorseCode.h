/*

*/

#ifndef MorseCode_h
#define MorseCode_h
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern int autoSQL(unsigned int pin);
extern String morseRx(unsigned int pin, int tim, int sql, double tol);
extern void morseTx(unsigned int pin, String ip, int t);
String morseEncode(String ip);
String morseDecode(String ip);
#ifdef __cplusplus
}
#endif
#endif