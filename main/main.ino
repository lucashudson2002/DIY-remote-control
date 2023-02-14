#define POT1 36
#define X1 39
#define Y1 34
#define POT2 35
#define Y2 32
#define X2 33
#define BTN_JOY1 25
#define BTN1 26
#define BTN2 27
#define TS1 14
#define TS2 16
#define BTN3 17
#define BTN4 18
#define BTN_JOY2 19
#define BUZZER 23

void setup{
  pinMode(POT1, INPUT);
  pinMode(X1, INPUT);
  pinMode(Y1, INPUT);
  pinMode(POT2, INPUT);
  pinMode(Y2, INPUT);
  pinMode(X2, INPUT);
  pinMode(BTN_JOY1, INPUT_PULLUP);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(TS1, INPUT);
  pinMode(TS2, INPUT);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP);
  pinMode(BTN_JOY2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);
}

void loop{
	Serial.print(analogRead(POT1));
  Serial.print(analogRead(X1));
  Serial.print(analogRead(Y1));
  Serial.print(analogRead(POT2));
  Serial.print(analogRead(Y2));
  Serial.print(analogRead(X2));
  Serial.print(digitalRead(BTN_JOY1));
  Serial.print(digitalRead(BTN1));
  Serial.print(digitalRead(BTN2));
  Serial.print(digitalRead(TS1));
  Serial.print(digitalRead(TS2));
  Serial.print(digitalRead(BTN3));
  Serial.print(digitalRead(BTN4));
  Serial.print(digitalRead(BTN_JOY2));
  // buzzer
}
