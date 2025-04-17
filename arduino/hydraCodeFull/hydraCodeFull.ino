#include <FastLED.h>


#define MOTOR_PIN   6    // Pin for the motor
#define LED_PIN     7    // Pin for the LED strip
#define NUM_LEDS    30   // The number of LEDs in your strip


CRGB leds[NUM_LEDS];


void setup() {
  // Set up LED strip on pin 7 (separate from motor pin)
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255); // Set the brightness of the LEDs


  // Set motor pin as output
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  analogWrite(MOTOR_PIN, 255); // Set motor off (255 = OFF for this setup)


  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0); // Turn off all LEDs
  }
  FastLED.show();

  //dealy 10 sec at start
  delay(10000);
}


void loop() {
  static int count = 0;

  if (count < 30) { // Run 30 times


    // Turn on LEDs (red) for 5 seconds
    analogWrite(MOTOR_PIN, 255); // Ensure motor is off
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 0, 0); // Red LEDs
    }
    FastLED.show();
    delay(5000); // LEDs on for 5 seconds


    // Turn on motor (LEDs still on) for 5 seconds
    analogWrite(MOTOR_PIN, 0); // Turn on motor
    delay(5000); // Motor + LEDs both on


    // Turn everything off
    analogWrite(MOTOR_PIN, 255); // Turn off motor
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 0); // LEDs off
    }
    FastLED.show();


    // Wait for 1 minute and 15 seconds (75 seconds)
    delay(75000);


    count++;
  } else {
    while (1); // Stop forever after 30 trials
  }
}







