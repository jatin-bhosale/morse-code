//declare variables here

String morse[36] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  while (!Serial.available()) {}
  String s = "";
  while (Serial.available() > 0)
  {
    char c = Serial.read();
    s += c;
    delay(10);
  }
  // Serial.println(s);
  Serial.println(morseDecode (s));
}

// functions here

String morseReceive(uint8_t digi_pin){
  
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
    if (c != " ") {
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
    else if (c == " ") {
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
