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
  analogWrite(MOTOR_PIN, 255); //Set motor off
  //setting it to 255 means "off" apparently
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0); // Set each LED off
  }
  FastLED.show();
}






void loop() {
  static int count = 0;
 
  if (count < 30) { //run 30 times
    // Turn on LEDs (red) for 2 seconds
    analogWrite(MOTOR_PIN, 255);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255, 0, 0); // Set each LED to red
    }
    FastLED.show();
    delay(2000); // LEDs stay on for 2 seconds before motor starts


    // Now turn on the motor and keep the LEDs on for 5 seconds
    analogWrite(MOTOR_PIN, 0); // Turn on motor
    delay(5000); // Motor and LEDs stay on for 5 seconds


    // After 5 seconds, turn off both the motor and LEDs
    analogWrite(MOTOR_PIN, 255); // Turn off motor
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 0); // Turn off all LEDs
    }
    FastLED.show();


    // Wait for 1 minute and 30 second (90 seconds) before repeating
    delay(90000); // Wait for 1 minute before repeating the cycle
   
    count++;
  } else {
    while(1); //halt the program after 30 trials
  }
}







