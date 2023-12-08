// Arduino - Real Time Clock Connection
// RTC: DS1307-Module-V03
// @author: Hayk Stepanyan

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <string.h>
#include <Keypad.h>
#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 144
#define RS_PIN 12
#define EN_PIN 11
#define D4_PIN 5
#define D5_PIN 4
#define D6_PIN 3
#define D7_PIN 2
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// LED Light Constants
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);
CRGB leds[NUM_LEDS];
const int MAX_BRIGHTNESS = 255;

// Keypad Constants
byte rowPins[KEYPAD_ROWS] = {38, 40, 42, 44};
byte colPins[KEYPAD_COLS] = {46, 48, 50, 52};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

String ALARM_TIME = "";

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  set_time(1, 1, 0, 0, 0);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  tmElements_t current_time;
  char key = keypad.getKey();
  if (key) {
    if (strcmp(key, 'A') == 0) {
      set_new_time();
    } else if (strcmp(key, 'B') == 0) {
      set_alarm_time();
    }
  }

  if (RTC.read(current_time)) {
    Serial.println(time_to_string(current_time).c_str());
    if (strcmp(time_to_string(current_time).c_str(), ALARM_TIME.c_str()) == 0) {
      Serial.println("ALARM!!!");
      lcd.clear();
      lcd.print("ALARM!!!");
      turn_on_alarm();
      ALARM_TIME = "";
    } else {
      lcd.setCursor(0, 0);
      lcd.print(time_to_string(current_time));
      lcd.setCursor(0, 1);
      if (ALARM_TIME.length() > 0) {
        lcd.print("Alarm: ");
        for (int i = 0; i < 5; i ++) {
          lcd.print(ALARM_TIME[i]);
        }
      }
    }
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is connected but the time should be initialized.");
      Serial.println();
    } else {
      Serial.println("DS1307 reading error! Pleas check the circuit connections.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
}

// Sets initial time on RTC module.
void set_time(int month, int day, int hour, int minute, int second) {
  tmElements_t initial_time;
  initial_time.Month = month;
  initial_time.Day = day;
  initial_time.Hour = hour;
  initial_time.Minute = minute;
  initial_time.Second = second;
  RTC.write(initial_time);
}

// Sets new time from user input.
void set_new_time() {
  Serial.println("We are setting up a new time");
  String new_time_hour = "";
  String new_time_minute = "";
  String new_time_second = "";
  String new_time_day = "";
  String new_time_month = "";

  lcd.clear();
  lcd.print("Enter new time:");
  lcd.setCursor(0, 1);
  lcd.print("HH:MM:SS   DD/MM");

  int i = 0;

  for (int i = 0; i < 10; i++) {
    char input_key = keypad.getKey();
    while (!input_key) {
      input_key = keypad.getKey();
    }
    
    if (i < 2) {
      lcd.setCursor(i, 1);
      lcd.print(input_key);
      new_time_hour += input_key;
    } else if (i < 4) {
      lcd.setCursor(i + 1, 1);
      lcd.print(input_key);
      new_time_minute += input_key;
    } else if (i < 6) {
      lcd.setCursor(i + 2, 1);
      lcd.print(input_key);
      new_time_second += input_key;
    } else if (i < 8) {
      lcd.setCursor(i + 5, 1);
      lcd.print(input_key);
      new_time_day += input_key;
    } else if (i < 10) {
      lcd.setCursor(i + 6, 1);
      lcd.print(input_key);
      new_time_month += input_key;
    }
  }

  Serial.print("Hour: "); Serial.println(new_time_hour.toInt());
  Serial.print("Minute: "); Serial.println(new_time_minute.toInt());
  Serial.print("Second: "); Serial.println(new_time_second.toInt());
  Serial.print("Day: "); Serial.println(new_time_day.toInt());
  Serial.print("Month: "); Serial.println(new_time_month.toInt());
  set_time(new_time_month.toInt(), new_time_day.toInt(), new_time_hour.toInt(), new_time_minute.toInt(), new_time_second.toInt());
  lcd.clear();
  lcd.print("Time set!");
  delay(1900);
  lcd.clear();
}

void set_alarm_time() {
  Serial.println("We are setting up an alarm time");
  String alarm_time = "HH:MM:SS   DD/MM";

  lcd.clear();
  lcd.print("Enter alarm time:");
  lcd.setCursor(0, 1);
  lcd.print(alarm_time);

  int i = 0;

  for (int i = 0; i < 10; i++) {
    char input_key = keypad.getKey();
    while (!input_key) {
      input_key = keypad.getKey();
    }
    
    if (i < 2) {
      alarm_time[i] = input_key;
    } else if (i < 4) {
      alarm_time[i + 1] = input_key;
    } else if (i < 6) {
      alarm_time[i + 2] = input_key;
    } else if (i < 8) {
      alarm_time[i + 5] = input_key;
    } else if (i < 10) {
      alarm_time[i + 6] = input_key;
    }
    lcd.setCursor(0, 1);
    lcd.print(alarm_time);
  }

  lcd.clear();
  lcd.print("Alarm set!");
  ALARM_TIME = alarm_time;
  Serial.println(alarm_time);
  delay(1900);
}

void turn_on_alarm() {
  // Blue light gradual turn on
  for (int brightness = 0; brightness < MAX_BRIGHTNESS; brightness = brightness + 25) {
    Serial.println(brightness);
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(0, 0, brightness);
      FastLED.show();

    }
  }

  // Yellow light gradual turn on
  for (int brightness = 0; brightness < MAX_BRIGHTNESS; brightness = brightness + 25) {
    Serial.println(brightness);
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(255, 255, brightness);
      FastLED.show();
    }
  }

  // Turn off lights
  for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(0, 0, 0);
      FastLED.show();
  }
}

// Returns a string representing the time in "Hour:Minute:Second Day/Month" format.
String time_to_string(tmElements_t time) {
  String time_string;
  time_string = convert_to_two_digits(time.Hour);
  time_string += ":";
  time_string += convert_to_two_digits(time.Minute);
  time_string += ":";
  time_string += convert_to_two_digits(time.Second);
  time_string += "   ";
  time_string += convert_to_two_digits(time.Day);
  time_string += "/";
  time_string += convert_to_two_digits(time.Month);
  return time_string;
}

// Converts one digit numbers as two digits.
String convert_to_two_digits(int number) {
  String number_string = "";
  if (number >= 0 && number < 10) {
    number_string += "0";
  }
  number_string += number;
  return number_string;
}
