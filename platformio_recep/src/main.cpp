#include <SSD1306Wire.h>
#include <RadioLib.h>

SSD1306Wire display(0x3c, 17, 18);

SX1262 radio = new Module(8, 14, 12, 13);

volatile bool receivedFlag = false;

void IRAM_ATTR setFlag() {
	receivedFlag = true;
}

void writeDisplay(int x, int y, String text);

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

	radio.setPacketReceivedAction(setFlag);

	Serial.print(F("[SX1262] Starting to listen ... "));
	state = radio.startReceive();
	if (state != RADIOLIB_ERR_NONE) {
		writeDisplay(6, 25, String("LORA ") + String(state));
		while (true);
	}
}

void loop() {
	if(receivedFlag) {
		receivedFlag = false;

		String str;
		int state = radio.readData(str);

		if (state == RADIOLIB_ERR_NONE) {
			Serial.print(F("[SX1262] Data:\t\t"));
			Serial.println(str);
		}
	}
}

void writeDisplay(int x, int y, String text){
	Wire.begin(17, 18);
	display.clear();
	display.drawString(x, y, String(text));
	display.display();
	Wire.end();
}