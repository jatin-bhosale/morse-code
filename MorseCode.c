/*

*/

#include "Arduino.h"
#include "MorseCode.h"

int autoSQL(unsigned int pin)
{
  int t1 = millis(), op = abs(analogRead(pin));
  while (millis() < t1 + 1000) {
    if (abs(analogRead(pin)) > op) {
      op = abs(analogRead(pin));
    }
  }
  return op;
}

String morseRx(unsigned int pin, int tim, int sql, double tol)
{
  tim = 1200 / tim;
  String op = "";
  int t1 = millis(), t2 = millis(), t = 0;
  while (analogRead(pin) < sql && analogRead(pin) > (sql * (-1))) {}
  t1 = millis();
point1:
  while (!(analogRead(pin) < sql && analogRead(pin) > (sql * (-1)))) {}
  t2 = millis();
  delay(3);
  if (analogRead(pin) < sql && analogRead(pin) > (sql * (-1))) {
    t = t2 - t1;
    if ((t > (1 - tol)*tim) && (t < (1 + tol)*tim)) {
      op += '.';
    }
    else if ((t > (3 - tol)*tim) && (t < (3 + tol)*tim)) {
      op += '-';
    }
    else {
      op += '?';      //error detecting high
    }
  }
  else {
    goto point1;
  }
  while (1) {
    if (!(analogRead(pin) < sql && analogRead(pin) > (sql * (-1)))) {
      t1 = millis();
      t = t1 - t2;
      if ((t > (1 - tol)*tim) && (t < (1 + tol)*tim)) {}
      else if ((t > (3 - tol)*tim) && (t < (3 + tol)*tim)) {
        op += ' ';
      }
      else if ((t > (7 - tol)*tim) && (t < (7 + tol)*tim)) {
        op += "   ";
      }
      else {
        op += '$';      //error detecting low
      }
      goto point1;
    }
    else if ((millis() - t2) > 2000) {
      break;
    }
  }
  return morseDecode(op);
  //return
}


void morseTx(unsigned int pin, String ip, int t)          // t is rate in wpm
{
  ip = morseEncode(ip);
  Serial.println(ip);
  // transmit
  //duration of 1 unit of time = 1200/wpm

  t = 1200 / t;
  for (int i = 0; i < ip.length(); i++) {
    if (ip.charAt(i) == '.') {
      analogWrite(pin, 1023);
      delay(t);
      analogWrite(pin, 0);
      delay(t);
    }
    else if (ip.charAt(i) == '-') {
      analogWrite(pin, 1023);
      delay(3 * t);
      analogWrite(pin, 0);
      delay(t);
    }
    else if (ip.charAt(i) == ' ') {
      if (ip.charAt(i + 1) == ' ') {
        delay(6 * t);
        i += 2;
      }
      else {
        delay(2 * t);
      }
    }
  }

}


String morseEncode(String ip)
{
  String op = "";
  for (int i = 0; i < ip.length(); i++) {
    char c = ip.charAt(i);
    if (c < 58 && c > 47) {
      op += morse[c - 48];
      op += " ";
    }
    else if (c < 91 && c > 64) {
      op += morse[c - 55];
      op += " ";
    }
    else if (c < 123 && c > 96) {
      op += morse[c - 87];
      op += " ";
    }
    else if (c == ' ') {
      op += "  ";
    }
  }
  return op;
}


String morseDecode(String ip)
{
  String op = "", tmp = "";
  ip += " ";
  for (int i = 0; i < (ip.length()); i++) {
    if (ip.charAt(i) != ' ') {
      tmp += ip.charAt(i);
    }
    else {
      for (int j = 0; j < 36; j++) {
        if (tmp == morse[j]) {
          if (j <= 9) {
            op += j;
          }
          else {
            char c = (char) ('A' + (j - 10));
            op += c;
          }
          break;
        }
      }
      if (ip.charAt(i + 1) == ' ' && ip.charAt(i + 2) == ' ') {
        op += " ";
      }
      tmp = "";
    }
  }
  return (op);
}

