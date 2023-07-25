#define A_4 440
#define B_4 493
#define C_5 523
#define D_5 587
#define E_5 659
#define F_5 698
#define G_5 784
#define A_5 880
#define B_5 987
#define C_6 1047
#define D_6 1175
#define E_6 1319
#define F_6 1397
#define G_6 1568
#define A_6 1760
#define B_6 1976
#define C_7 2093

void kill(int pin, int len, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, C_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, D_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, E_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, F_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, G_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, A_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, B_6, len);
    delay(len);
//    noTone(pin);
    tone(pin, C_7, len);
    delay(len);
//    noTone(pin);
  }
}

void end_axo(int pin, int len, int repeat) {
    tone(pin, G_6, len);
    delay(len);
    tone(pin, E_6, len);
    delay(len);
    tone(pin, C_6, len);
    delay(len);
}

void axo_error(int pin, int len, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, C_7 + 50, len);
    delay(len);
    noTone(pin);
    tone(pin, C_7, len);
    delay(len);
    noTone(pin);
  }
}

void armed(int pin, int len, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, C_6, len);
    delay(len);
    noTone(pin);
    tone(pin, E_6, len);
    delay(len);
    noTone(pin);
    tone(pin, G_6, len);
    delay(len);
    noTone(pin);
  }
}

void recieved_command(int pin, int len, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, C_6, len);
    delay(len);
    noTone(pin);
    tone(pin, E_6, len);
    delay(len);
    noTone(pin);
  }
}


void sent_command(int pin, int len, int repeat) {
  for (int i = 0; i < repeat; i++) {
    tone(pin, E_6, len);
    delay(len);
    noTone(pin);
    tone(pin, C_6, len);
    delay(len);
    noTone(pin);
  }
}

// void setup() {
//   // put your setup code here, to run once:
//   pinMode(PIN, OUTPUT);
//   pinMode(LED_BUILTIN, OUTPUT);
//   Serial.begin(9600);
//   Serial.println("Error");
//   error(PIN, 15, 40);
//   delay(1000);
//   Serial.println("Armed");
//   armed(PIN, 150, 1);
//   delay(1000);
//   Serial.println("Recieved Message");
//   recieved_command(PIN, 50, 1);
//   delay(1000);
//   Serial.println("Sent Message");
//   sent_command(PIN, 50, 1);
//   delay(1000);
//   Serial.println("Kill");
//   kill(PIN, 50, 2);
//   digitalWrite(LED_BUILTIN, HIGH);
// }


// void loop() {
//   // put your main code here, to run repeatedly:
//   //  noTone(3);
//   //  noTone(5);
// }