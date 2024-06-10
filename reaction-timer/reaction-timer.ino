#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Adafruit_NeoPixel.h>

#define PIN A0
#define NUMPIXELS 5
#define BUTTON_PIN 3
#define MODE_BUTTON_PIN 4
#define LED_VALUE 45
#define LED_VALUE_WHITE 15

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
uint8_t state = 0;
uint8_t mode = 0;
bool newState = digitalRead(BUTTON_PIN);
long randNumber;
long timeStart;
long timeEnd;
long timeSpent;

//Output control
void setDisplayInt(long val){
	boolean dpOne = false;
	boolean dpTwo = false;
	uint8_t dpThree = false;
	boolean dpFour = false;
	double newVal = 0;
	if(val < 9999){
		newVal = val;
		dpOne = true;
	}
	else if(val < 99999){
		newVal = val/10;
		dpTwo = 1;
	}
	else if(val < 999999){
		val = val/100;
		dpThree = true;
	}
	else if(val < 9999999){
		val = val/1000;
		dpFour = true;
	}

	String timeStr = String(val);
	if(val == 0){
		alpha4.writeDigitAscii(0, '0');
		alpha4.writeDigitAscii(1, '0');
		alpha4.writeDigitAscii(2, '0');
		alpha4.writeDigitAscii(3, '0');
	}
	else if(val < 10)
	{
		alpha4.writeDigitAscii(0, '0');
		alpha4.writeDigitAscii(1, '0');
		alpha4.writeDigitAscii(2, '0');
		alpha4.writeDigitAscii(3, timeStr.charAt(0));
	}
	else if(val < 100)
	{
		alpha4.writeDigitAscii(0, '0');
		alpha4.writeDigitAscii(1, '0');
		alpha4.writeDigitAscii(2, timeStr.charAt(0));
		alpha4.writeDigitAscii(3, timeStr.charAt(1));
	}
	else if(val < 1000)
	{
		alpha4.writeDigitAscii(0, '0');
		alpha4.writeDigitAscii(1, timeStr.charAt(0));
		alpha4.writeDigitAscii(2, timeStr.charAt(1));
		alpha4.writeDigitAscii(3, timeStr.charAt(2));
	}
	else
	{
		alpha4.writeDigitAscii(0, timeStr.charAt(0), dpOne);
		alpha4.writeDigitAscii(1, timeStr.charAt(1), dpTwo);
		alpha4.writeDigitAscii(2, timeStr.charAt(2), dpThree);
		alpha4.writeDigitAscii(3, timeStr.charAt(3), dpFour);
	}
	alpha4.writeDisplay();
}
void setLEDColor(uint8_t r,uint8_t g, uint8_t b){
	pixels.setPixelColor(0, pixels.Color(r, g, b));
	pixels.setPixelColor(1, pixels.Color(r, g, b));
	pixels.setPixelColor(2, pixels.Color(r, g, b));
	pixels.setPixelColor(3, pixels.Color(r, g, b));
	pixels.setPixelColor(4, pixels.Color(r, g, b));
	pixels.show();
}
void setSingleLEDColor(uint8_t led, uint8_t r,uint8_t g, uint8_t b){
	pixels.setPixelColor(led, pixels.Color(r, g, b));
	pixels.show();
}
void setError(){
	alpha4.writeDigitAscii(0, ' ');
	alpha4.writeDigitAscii(1, 'E');
	alpha4.writeDigitAscii(2, 'R');
	alpha4.writeDigitAscii(3, 'R');
	alpha4.writeDisplay();
}
//Input checking
uint8_t waitWithButtonCheck(long dly){
	for(long i = 0; i < dly; i++){
		if(digitalRead(BUTTON_PIN) == LOW){
			setError();
			if(mode == 1)
		    	setLEDColor(0, 0, 0);
			else if(mode == 2)
				setSingleLEDColor(0, LED_VALUE, 0, 0);
			else
				setLEDColor(0, 0, 0);
			return 1;
		}
		delay(1);
	}
	return 0;
}
void waitForReset(){
	state = 0;
	while(state == 0)
	{
		newState = digitalRead(BUTTON_PIN);
		if(newState == HIGH)
		{
			state = 1;
		}
	}
	delay(100);
}
void waitForButton(){
	state = 0;
	while(state == 0)
	{
		newState = digitalRead(BUTTON_PIN);
		if(newState == LOW)
		{
			state = 1;
		}
	}
}
void waitForInput(){
	while (true) {
		if(digitalRead(BUTTON_PIN) == LOW){
			delay(50);
			while (digitalRead(BUTTON_PIN) == LOW) {}
			delay(150);
			return;
		}
		if(digitalRead(MODE_BUTTON_PIN) == LOW){
			delay(50);
			while (digitalRead(BUTTON_PIN) == LOW) {}
			setLEDColor(LED_VALUE_WHITE, LED_VALUE_WHITE, LED_VALUE_WHITE);
			mode = (mode + 1) % 3;
			if (mode == 0) {
				alpha4.writeDigitAscii(0, ' ');
				alpha4.writeDigitAscii(1, 'S');
				alpha4.writeDigitAscii(2, 'T');
				alpha4.writeDigitAscii(3, 'D');
				alpha4.writeDisplay();
			}
			else if (mode == 1) {
				alpha4.writeDigitAscii(0, 'D');
				alpha4.writeDigitAscii(1, 'T');
				alpha4.writeDigitAscii(2, 'A');
				alpha4.writeDigitAscii(3, 'P');
				alpha4.writeDisplay();
			}
			else if (mode == 2) {
				alpha4.writeDigitAscii(0, 'T');
				alpha4.writeDigitAscii(1, 'r');
				alpha4.writeDigitAscii(2, 'e');
				alpha4.writeDigitAscii(3, 'e');
				alpha4.writeDisplay();
			}
			delay(500);
		}
	}
}
//Modes
void standardReaction(){
	// Set Red
	setDisplayInt(0);
	setLEDColor(LED_VALUE, 0, 0);
	// Wait some random time
	if(waitWithButtonCheck(random(1500, 8000))){
		delay(300);
		return;
	}
	// Set Green
	setLEDColor(0, LED_VALUE, 0);
	// Get reaction time
	timeStart = micros();
	waitForButton();
	timeEnd = micros();
	waitForReset();
	// Display reaction
	timeSpent = (timeEnd - timeStart);
	setDisplayInt(timeSpent);
	setLEDColor(0,0,LED_VALUE);
}
void doubleTap(){
	// Set Red
	setDisplayInt(0);
	setLEDColor(LED_VALUE, 0, 0);
	// Get reaction time
	waitForButton();
	while(digitalRead(BUTTON_PIN) == LOW){}
	timeStart = micros();
	waitForButton();
	timeEnd = micros();
	waitForReset();
	// Display reaction
	timeSpent = (timeEnd - timeStart);
	setDisplayInt(timeSpent);
	setLEDColor(0,0,LED_VALUE);
}
void randomColor(){

}
void reactionTree(){
	setDisplayInt(0);
	setLEDColor(0, 0, 0);
	delay(500);
	setSingleLEDColor(4, LED_VALUE, LED_VALUE, 0);
	if(waitWithButtonCheck(500)){
		delay(300);
		return;
	}
	setSingleLEDColor(3, LED_VALUE, LED_VALUE, 0);
	if(waitWithButtonCheck(500)){
		delay(300);
		return;
	}
	setSingleLEDColor(2, LED_VALUE, LED_VALUE, 0);
	if(waitWithButtonCheck(500)){
		delay(300);
		return;
	}
	setSingleLEDColor(1, 0, LED_VALUE, 0);
	timeStart = micros();
	waitForButton();
	timeEnd = micros();
	waitForReset();
	// Display reaction
	timeSpent = (timeEnd - timeStart);
	setDisplayInt(timeSpent);

}

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));

    Serial.println("Start display!");
    alpha4.begin(0x70);
	alpha4.setBrightness(8);

	alpha4.writeDigitAscii(0, ' ');
	alpha4.writeDigitAscii(1, 'S');
	alpha4.writeDigitAscii(2, 'T');
	alpha4.writeDigitAscii(3, 'D');
	alpha4.writeDisplay();

    Serial.println("Start LED!");
    pixels.begin();
    setLEDColor(LED_VALUE_WHITE, LED_VALUE_WHITE, LED_VALUE_WHITE);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
	waitForInput();
	waitForReset();
	delay(100);
	if(mode == 0){
		standardReaction();
	}
	else if(mode == 1){
		doubleTap();
	}
	else if(mode == 2){
		reactionTree();
	}
	else if(mode == 3){
		randomColor();
	}
}
