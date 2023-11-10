// LCD Display - Arduino Connection
// LCD1602 Module
// @author: Hayk Stepanyan

#include <LiquidCrystal.h>

#define RS_PIN 12
#define EN_PIN 11
#define D4_PIN 5
#define D5_PIN 4
#define D6_PIN 3
#define D7_PIN 2

LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);

void setup() {
  lcd.begin(16, 2);
  lcd.print("First line text");
  lcd.setCursor(0, 1);
  lcd.print("Second line text");
}

void loop() {

}
