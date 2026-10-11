#include <Servo.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DS1307RTC.h>
#include <Stepper.h>
const int stepsPerRevolution = 2000;
const int rpm = 15;
Stepper stepper1(stepsPerRevolution, 10, 12, 11, 13);
Servo IranServo;
#define ControlWire 9
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DOUT  3
#define CLK   2
HX711 scale;
float calibration_factor = -123200;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
tmElements_t tm;
int targetHour = 17;
int targetMinute = 19;
int targetSecond = 0;
int targetWeight = 50;

bool servoMoved = false;
bool stepperActive = false;
bool zeroFlag = false;
unsigned long stepperStartTime = 0;

void setup() {
  Serial.begin(9600);
  stepper1.setSpeed(rpm);
  lcd.init();
  lcd.backlight();
  lcd.print("WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("IoTreat :)");
  IranServo.attach(ControlWire);
  IranServo.write(90);
  delay(1000);
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare();

  if (getDate(__DATE__) && getTime(__TIME__)) {
    if (RTC.write(tm)) {
      lcd.clear();
      lcd.print("RTC initialized");
    }
  } else {
    lcd.clear();
    lcd.print("RTC init failed!");
  }
  delay(1000);
  lcd.clear();
}

void loop() {
  updateLCD();
  checkWeightAndServo();
  manageStepperMotor();
  handleSerialCommands();
}

void updateLCD() {
  if (RTC.read(tm)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    print2digits(tm.Hour);
    lcd.print(':');
    print2digits(tm.Minute);
    lcd.print(':');
    print2digits(tm.Second);

    scale.set_scale(calibration_factor);
    float weight_in_grams = scale.get_units() * 0.453592 * 1000;
    lcd.setCursor(0, 1);
    lcd.print("Weight: ");
    lcd.print(weight_in_grams, 0);
    lcd.print(" gr");
  }
}

void checkWeightAndServo() {
    float weight_in_grams = scale.get_units() * 0.453592 * 1000;

    if (tm.Hour == targetHour && tm.Minute == targetMinute && abs(tm.Second - targetSecond) <= 1 && !servoMoved) {
      IranServo.write(0);
      Serial.println("Servo moved to 0");
      servoMoved = true;
    }

    if (weight_in_grams >= (targetWeight - 25) && servoMoved) {
        IranServo.write(90);
        servoMoved = false;
        zeroFlag = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("It's COMING...!");
        lcd.setCursor(0, 1);
        lcd.print("It takes 3 min");
        Serial.println("COMMAND:STEPPER_MOVE");
        for (int j = 0; j < 10; j++) {
            rotateStepper(-1, 20000);
        }
        lcd.clear();
        lcd.print("Enjoy your treat");
    }

    if (weight_in_grams <= 0 && zeroFlag) {
        Serial.println("WEIGHT:0");
        zeroFlag = false;
        lcd.clear();
        lcd.print("Oooh, it's over");
        delay(5000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bye Bye");
        lcd.setCursor(0, 1);
        lcd.print("It takes 3 min");
        for (int j = 0; j < 10; j++) {
            rotateStepper(1, 20000);
        }
    }
}

void manageStepperMotor() {}

void rotateStepper(int direction, int durationMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < durationMs) {
    stepper1.step(direction * stepsPerRevolution);
    delay(100);
  }
}

void print2digits(int number) {
  if (number < 10) lcd.print('0');
  lcd.print(number);
}

bool getTime(const char *str) {
  int Hour, Min, Sec;
  return sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) == 3 && (tm.Hour = Hour, tm.Minute = Min, tm.Second = Sec, true);
}

bool getDate(const char *str) {
  char Month[12];
  int Day, Year;
  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (int i = 0; i < 12; i++) {
    if (strcmp(Month, monthName[i]) == 0) {
      tm.Day = Day;
      tm.Month = i + 1;
      tm.Year = CalendarYrToTm(Year);
      return true;
    }
  }
  return false;
}

void handleSerialCommands() {
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    if (receivedData.startsWith("SET_TARGET:")) {
      int hourTarget = receivedData.substring(11, 13).toInt();
      int minuteTarget = receivedData.substring(14, 16).toInt();
      float targetWeight = receivedData.substring(17).toFloat();
      lcd.clear();
      lcd.print(hourTarget);
      lcd.print(minuteTarget);
      lcd.print(targetWeight);
      delay(2000);
      targetHour = hourTarget;
      targetMinute = minuteTarget;
      targetWeight = targetWeight;
    } else if (receivedData.startsWith("EJECT_CONTAINER")) {
      for (int j = 0; j < 10; j++) {
            rotateStepper(-1, 20000);
      }
    } else if (receivedData.startsWith("INSERT_CONTAINER")) {
      for (int j = 0; j < 10; j++) {
            rotateStepper(1, 20000);
      }
    } else if (receivedData.startsWith("TARE_SCALE")) {
      scale.tare();
      lcd.clear();
      lcd.print("Scale Tared!");
      delay(1000);
    }
  }
}
