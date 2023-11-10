// Arduino - Real Time Clock Connection
// RTC: DS1307-Module-V03
// @author: Hayk Stepanyan

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int initial_year = 2023 - 1970;
int initial_month = 11;
int initial_day = 9;
int intial_hour = 12;
int initial_minute = 30;
int initial_second = 0;

void setup() {
  Serial.begin(115200);
  set_time(initial_year, initial_month, initial_day, intial_hour, initial_minute, initial_second);
}

void loop() {
  tmElements_t current_time;

  if (RTC.read(current_time)) {
    print_time(current_time);
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
void set_time(int year, int month, int day, int hour, int minute, int second) {
  tmElements_t initial_time;
  initial_time.Year = year;
  initial_time.Month = month;
  initial_time.Day = day;
  initial_time.Hour = hour;
  initial_time.Minute = minute;
  initial_time.Second = second;
  RTC.write(initial_time);
}

// Prints time in "Time = Hour:Minute:Second, Data (D/M/Y) = Day/Month/Year" format.
void print_time(tmElements_t time) {
  Serial.print("Time = ");
  print_two_digits(time.Hour);
  Serial.print(":");
  print_two_digits(time.Minute);
  Serial.print(":");
  print_two_digits(time.Second);
  Serial.print(", Data (D/M/Y) = ");
  print_two_digits(time.Day);
  Serial.print("/");
  print_two_digits(time.Month);
  Serial.print("/");
  Serial.print(tmYearToCalendar(time.Year));
  Serial.println();
}

// Prints one digit numbers as two digits.
void print_two_digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}
