//declare variables here
int words = 0;
const String morse[36] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
}

void loop() {
//  while (!Serial.available()) {}
//  String s = "";
//  while (Serial.available() > 0)
//  {
//    char c = Serial.read();
//    s += c;
//    delay(10);
//  }
  Serial.println("Begin");
  Serial.println(morseRead(2));
}

// functions here

String morseRead(uint8_t digi_pin) {
  int tm = 500;             // this is the duration of one unit of time ( time of one dit )
  int t1 = 0, t2 = 0, flg = 0, a = 0, b = 0, an = 0, bn = 0;
  int t[13], dit[12], dash[12], flag[12];
  String op = "";
  while (digitalRead(digi_pin) == LOW) {}
  t[0] = millis();
  for (int i = 0; i < 12; i += 2) {             //read first 6 inputs (may be either dits or dashes)
    while (digitalRead(digi_pin) == HIGH) {}
    t[i + 1] = millis() - t[i];
    while (digitalRead(digi_pin) == LOW) {}
    t[i + 2] = millis() - t[i + 1];
  }
  t1 = millis();
  for (int m = 1; m < 13; m += 2) {             //differentiate between a dit and a dash and calculate average time for a dit and a dash
    a = 0, b = 0, an = 0, bn = 0;
    int A[6] = {0, 0, 0, 0, 0, 0};
    int B[6] = {0, 0, 0, 0, 0, 0};
    A[0] = t[m];
    for (int i = 1, j = 1, k = 0; i < 13; i += 2) {               //check if it is a dit or a dash
      if (t[i] > ((A[0] * 2) / 3) && t[i] < (A[0] * 2)) {
        A[j] = t[i];
      }
      else {
        B[k] = t[i];
        k++;
      }
    }
    for (int i = 0; A[i] != 0; i++) {                         //find average of array
      a += A[i], an++;
    }
    for (int i = 0; B[i] != 0; i++) {
      b += B[i], bn++;
    }
    a = a / an, b = b / bn;
    if (a < b) {                                            //save average time in the dit or dash array
      dit[m - 1] = a, dash[m - 1] = b;
    }
    else {
      dit[m - 1] = b, dash[m - 1] = a;
    }
    if (dash[m - 1] > (2 * dit[m - 1]) && dash[m - 1] < (dit[m - 1] * 4)) {             //check if the data is valid and set flag accordingly
      flag[m - 1] = 1;                                                             // 1 means data is correct
    }
    else {
      flag[m - 1] = 0;
    }
  }
  a = 0, b = 0;
  for (int i = 0; i < 12; i++) {                              //average time of a dit and a dash
    if (flag[i] == 1) {
      a += dit[i], b += dash[i];
    }
  }
  if (b > (2 * a) && b < (a * 4)) {
    words = 1200 / a;
  }
  tm = a;
  for (int i = 0; i < 13; i++) {
    if (i % 2 == 0) {
      if (t[i] > (tm * 2) && t[i] < (4 * tm)) {
        op += " ";
      }
      else if (t[i] > (tm * 5) && t[i] < (8 * tm)) {
        op += "   ";
      }
    }
    else {
      if (t[i] > (tm / 2) && t[i] < (tm * 2)) {
        op += ".";
      }
      else if (t[i] > (tm * 2) && t[i] < (4 * tm)) {
        op += "-";
      }
    }
  }
  //find tm here...
  // while(digitalRead(digi_pin)==LOW){}
  while (millis()<(tm*10)) {
    while (digitalRead(digi_pin) == HIGH) {}
    t2 = millis() - t1;
    t1 = millis();
    if (t2 > (tm / 2) && t2 < (tm * 2)) {
      op += ".";
    }
    else if (t2 > (tm * 2) && t2 < (4 * tm)) {
      op += "-";
    }
    while (digitalRead(digi_pin) == LOW&&millis()<(tm*10)) {}
    // if (digitalRead(digi_pin) == HIGH) {
    t2 = millis() - t1;
    t1 = millis();
    if (t2 > (tm * 2) && t2 < (4 * tm)) {
      op += " ";
    }
    else if (t2 > (tm * 5) && t2 < (8 * tm)) {
      op += "   ";
    }
    else if (t2 > (tm * 10)) {
      break;
    }
  }
  return op;
}



String morseDecode(String str) {
  int flg = 0;
  str.trim();
  str += " ";
  String key = "", op = "";
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (c == 32) {
      flg++;
      if (flg == 3) {
        op += " ";
      }
      for (int j = 0; j < 36 ; j++) {
        if (key == morse[j]) {
          char ch;
          if (j < 26) {
            ch = 'A' + j;
          }
          else {
            ch = '0' + j - 26;
          }
          op += ch;
          key = "";
          flg = 0;
          break;
        }
      }
    }
    else {
      key += c;
    }
  }
  return op;
}

void morseTransmit(uint8_t digi_pin, String str, int wpm) {     //(Digital Pin No , Message to be transmitted , Speed of message in Words per minute(WPM))
  wpm = 1200 / wpm;     //here the new wpm is one unit of time i.e., duration of one dit.     Source : Wikipedia
  int flg = 0;  // flag that counts consecutive no of spaces.
  str = morseEncode(str);
  for (int i = 0; i < str.length(); i++) {
    char c = str.charAt(i);
    if (c != ' ') {
      if (flg == 1) {
        delay(wpm * 2);
      }
      else if (flg == 3) {
        delay(wpm * 6);
      }
      flg = 0;
    }
    if (c == '.') {
      digitalWrite(digi_pin, HIGH);
      delay(wpm);
      digitalWrite(digi_pin, LOW);
      delay(wpm);
    }
    else if (c == '-') {
      digitalWrite(digi_pin, HIGH);
      delay(wpm * 3);
      digitalWrite(digi_pin, LOW);
      delay(wpm);
    }
    else if (c == ' ') {
      flg++;
    }
  }
}

String morseEncode(String str) {
  String op = "";
  str.trim();
  for (int i = 0; i < str.length(); i++) {
    int ch = str.charAt(i);
    if (ch > 47 && ch < 58) {
      op = op + morse[ch - 22];
    }
    else if (ch > 64 && ch < 91) {
      op = op + morse[ch - 65];
    }
    else if (ch > 96 && ch < 123) {
      op = op + morse[ch - 97];
    }
    else if (ch == 32) {
      op = op + "  ";
    }
    op = op + " ";
  }
  op.trim();
  return op;
}
