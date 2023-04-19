#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// connect the voltage sensor to A0
const int voltageSensor = A0;

// the pin the chargingLed is attached to
const int chargingLed = A1;

// set the led state to low
int ledState = LOW;

// voltage value
int sensorValue = 0;

// duration between lcd refreshes
const long interval = 1000;

// buffer variables to hold values to print on the lcd
char floatBuffer[8];
char buffer[40];

/*** Battery charging animations ***/
byte emptyBattery[8] = {
  0b00100,
	0b11111,
	0b10001,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111
};

byte quarterFullBattery[8] = {
  0b00100,
	0b11111,
	0b10001,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111
};

byte hallFullBattery[8] = {
  0b00100,
	0b11111,
	0b10001,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

byte threeQtrsFullBattery[8] = {
  0b00100,
	0b11111,
	0b10001,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

byte fullBattery[8] = {
  0b00100,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};

/*** Battery charging animations ***/

// charger icon
byte charger[8] = {
  0b01010,
  0b01010,
	0b11111,
	0b10001,
	0b10001,
	0b01110,
	0b00100,
	0b00100,
};

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  // clear the screen to ensure ensure that nothing is displayed
  lcd.clear();

  // show the company info on start
  lcd.setCursor(0, 0);
  // print without wasting RAM
  lcd.print(F("Visionaries Systems"));
  lcd.setCursor(0, 3);
  lcd.print(F("All Rights Reserved."));
  delay(1000);
  lcd.clear();

  // create a custom character
  lcd.createChar(0, charger);
  // create a custom character
  lcd.createChar(1, emptyBattery);
  // create a custom character
  lcd.createChar(2, quarterFullBattery);
  // create a custom character
  lcd.createChar(3, hallFullBattery);
  // create a custom character
  lcd.createChar(4, threeQtrsFullBattery);
  // create a custom character
  lcd.createChar(5, fullBattery);

  // initialize the voltage sensor as an input
  pinMode(voltageSensor, INPUT);

  // make the charging led as an output
  pinMode(chargingLed, OUTPUT);

  // Start with the charging led off
  digitalWrite(chargingLed, ledState);
}

void loop() {
  // read the value of the voltage sensor
  sensorValue = analogRead(voltageSensor); // it's between 0 and 1023

  // convert the voltage value to a value between
  // 0 and 5V
  float voltage = sensorValue * (5.0 / 1023.0);

  // any voltage value above 0 means the charger is connected
  if(voltage > 0){
    // show the battery animation
    chargingAnimation(0, 0);
    // blink the led to show that it's charging
    ledBlinking(chargingLed);
  } else {
    //  show that the charger needs to be connected 
    connectCharger(0, 0, chargingLed);
  }

  // show the voltage value 
  lcd.setCursor(0, 3);
  // convert float voltage to string and store in float buffer
  dtostrf(voltage, 4, 2, floatBuffer); // value is stored to 2dp 
  // now we can use sprintf
  sprintf(buffer, "VOLTAGE: %sV", floatBuffer);
  // now print the buffered result on the lcd
  lcd.print(buffer);
}
// function to clear the lcd row without clearing the whole screen
void clearLCDLine(int line){
  lcd.setCursor(0, line);
  for(int n = 0; n < 20; n++){ // 20 is the number of columns
    lcd.print(" ");
  }
}

// blink the chargingLED to show that the device is charging
void ledBlinking(int led){
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

// show that the charger is connected
void connectCharger(int col, int row, int chargingLed){
  // turn off the charging led
  digitalWrite(chargingLed, LOW);
  lcd.setCursor(col, row);
  lcd.write(byte(0));
  lcd.setCursor(col + 2, row);
  lcd.print(F("NOT CONNECTED"));
  delay(5);
}

// show that the device is charging
void chargingAnimation(int col, int row){
  for(int i = 1; i < 6; i++){
    lcd.setCursor(col, row);
    lcd.write(i);
    lcd.setCursor(col + 2, row);
    lcd.print(F("CHARGING...    "));
    delay(100);   
  }
}
