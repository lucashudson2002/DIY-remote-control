#include <SSD1306Wire.h>
#include <RadioLib.h>
#include <ArduinoJson.h>

SSD1306Wire display(0x3c, 17, 18);

SX1262 radio = new Module(8, 14, 12, 13);

volatile bool transmittedFlag = false;

void IRAM_ATTR setFlag() {
	transmittedFlag = true;
}

StaticJsonDocument<256> json;
String msg;

// pinos internos
#define VBAT_READ 1
#define ADC_CTRL 37
#define USER_SW 0
#define LED_WRITE 35

// pinos externos
#define SCL 33
#define SDA 34
#define JOY_BTN1 39
#define JOY_X1 3
#define JOY_Y1 4
#define JOY_BTN2 38
#define JOY_X2 6
#define JOY_Y2 5
#define POT1 2
#define POT2 7
#define BTN1 41
#define BTN2 40
#define BTN3 47
#define BTN4 48
#define TS1 42
#define TS2 26
#define BUZZER 45

void readingSensors();
void sendLora();
void writeDisplay(int x, int y, String text);

void IRAM_ATTR USER_SW_ISR(){
	json["SW"] = (int)!digitalRead(USER_SW);
}
void IRAM_ATTR JOY_BTN1_ISR(){
	json["JB1"] = (int)!digitalRead(JOY_BTN1);
}
void IRAM_ATTR JOY_BTN2_ISR(){
	json["JB2"] = (int)!digitalRead(JOY_BTN2);
}
void IRAM_ATTR BTN1_ISR(){
	json["B1"] = (int)!digitalRead(BTN1);
}
void IRAM_ATTR BTN2_ISR(){
	json["B2"] = (int)!digitalRead(BTN2);
}
void IRAM_ATTR BTN3_ISR(){
	json["B3"] = (int)!digitalRead(BTN3);
}
void IRAM_ATTR BTN4_ISR(){
	json["B4"] = (int)!digitalRead(BTN4);
}

void setup() {
	Serial.begin(115200);

	Wire.begin(17, 18);
	pinMode(RST_OLED, OUTPUT);
	digitalWrite(RST_OLED, LOW);
	delay(20);
	digitalWrite(RST_OLED, HIGH);
	display.init();
	//display.flipScreenVertically();
	display.setFont(ArialMT_Plain_16);
	Wire.end();

	pinMode(VBAT_READ, INPUT);
	pinMode(ADC_CTRL,OUTPUT);
	digitalWrite(ADC_CTRL, LOW);
	pinMode(USER_SW, INPUT);
	pinMode(LED_WRITE, OUTPUT);
	attachInterrupt(USER_SW, USER_SW_ISR, CHANGE);

	pinMode(JOY_BTN1, INPUT_PULLUP);
	pinMode(JOY_BTN2, INPUT_PULLUP);
	pinMode(BTN1, INPUT_PULLUP);
	pinMode(BTN2, INPUT_PULLUP);
	pinMode(BTN3, INPUT_PULLUP);
	pinMode(BTN4, INPUT_PULLUP);
	pinMode(JOY_X1, INPUT);
	pinMode(JOY_Y1, INPUT);
	pinMode(JOY_X2, INPUT);
	pinMode(JOY_Y2, INPUT);
	pinMode(POT1, INPUT);
	pinMode(POT2, INPUT);
	pinMode(TS1, INPUT_PULLUP);
	pinMode(TS2, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);
	attachInterrupt(JOY_BTN1, JOY_BTN1_ISR, CHANGE);
	attachInterrupt(JOY_BTN2, JOY_BTN2_ISR, CHANGE);
	attachInterrupt(BTN1, BTN1_ISR, CHANGE);
	attachInterrupt(BTN2, BTN2_ISR, CHANGE);
	attachInterrupt(BTN3, BTN3_ISR, CHANGE);
	attachInterrupt(BTN4, BTN4_ISR, CHANGE);
	analogReadResolution(8);

	Serial.print(F("[SX1262] Initializing ... "));
	int state = radio.begin();
	if (state != RADIOLIB_ERR_NONE) {
		writeDisplay(6, 25, String("LORA ") + String(state));
		while (true);
	}

	radio.setFrequency(915.0);
	radio.setSpreadingFactor(7);
	radio.setBandwidth(500);
	radio.setCodingRate(5);
	radio.setOutputPower(22.0);
	
	radio.setPacketSentAction(setFlag);

	sendLora();
}

void loop() {
	writeDisplay(6, 25, String("Hello World!"));

	if(transmittedFlag) {
		transmittedFlag = false;

		radio.finishTransmit();

		Serial.println(F("[SX1262] Sending another packet ... "));
		sendLora();
	}
}

void readingSensors(){
	json["X1"] = analogRead(JOY_X1);
	json["Y1"] = analogRead(JOY_Y1);
	json["X2"] = analogRead(JOY_X2);
	json["Y2"] = analogRead(JOY_Y2);
	json["P1"] = analogRead(POT1);
	json["P2"] = analogRead(POT2);
	json["T1"] = digitalRead(TS1);
	json["T2"] = digitalRead(TS2);
}

void sendLora(){
	readingSensors();
	msg = "";
	serializeJson(json, msg);
	radio.startTransmit(msg);
}

void writeDisplay(int x, int y, String text){
	Wire.begin(17, 18);
	display.clear();
	display.drawString(x, y, String(text));
	display.display();
	Wire.end();
}