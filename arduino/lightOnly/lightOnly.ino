#include <FastLED.h>

#define LED_PIN     7    // Pin for the LED strip
#define NUM_LEDS    30   // The number of LEDs in your stri

CRGB leds[NUM_LEDS];


void setup() {
 // Set up LED strip on pin 7 (separate from motor pin)
 FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
 FastLED.setBrightness(50); // Set the brightness of the LEDs
 Serial.begin(9600);

 //setting it to 255 means "off" apparently
 for (int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CRGB(0, 0, 0); // Set each LED off
 }
 FastLED.show();


 delay(10000);
}




void loop() {


 static int count = 0;
 if (count < 30) { //run 30 times
   // Turn on LEDs (red) for 2 seconds
   for (int i = 0; i < NUM_LEDS; i++) {


     leds[i] = CRGB(255, 0, 0); // Set each LED to red
   }
   FastLED.show();
   delay(5000); // LEDs stay on for 5 seconds before motor starts




   // After 5 seconds, turn off LED and turn on motor
   for (int i = 0; i < NUM_LEDS; i++) {
     leds[i] = CRGB(0, 0, 0); // Turn off all LEDs
   }
   FastLED.show();


   // Now turn on the motor for 5 seconds
   delay(5000); // Motor on for 5 seconds



   // Wait for 1 minute and 15 second (75 seconds) before repeating
   delay(75000); // Wait for 1 minute before repeating the cycle
 
   count = count + 1;
 }
}



