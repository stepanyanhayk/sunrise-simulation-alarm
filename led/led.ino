// Arduino - LED Strip Communication
// @author: Hayk Stepanyan 

#include <FastLED.h>

#define LED_PIN 7
#define NUM_LEDS 144

CRGB leds[NUM_LEDS];
int MAX_BRIGHTNESS = 255;

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {

  // Blue light gradual turn on
  for (int brightness = 0; brightness < MAX_BRIGHTNESS; brightness = brightness + 5) {
    Serial.println(brightness);
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(0, 0, brightness);
      FastLED.show();
    }
  }

  // Green light wake up show
  for (int brightness = 0; brightness < MAX_BRIGHTNESS; brightness = brightness + 5) {
    Serial.println(brightness);
    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(0, 155, brightness);
      FastLED.show();
    }

    delay(2);

    for (int led = 0; led < NUM_LEDS; led++) {
      leds[led] = CRGB(0, 0, 0);
      FastLED.show();
    }
  }
}
