#define MOTOR_PIN   6    // Pin for the motor

void setup() {
 // Set motor pin as output
 pinMode(MOTOR_PIN, OUTPUT);
 //pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
 analogWrite(MOTOR_PIN, 255); //Set motor off
 //setting it to 255 means "off" apparently


 delay(10000);
}

void loop() {


 static int count = 0;
 if (count < 30) { //run 30 times
   // Turn on LEDs (red) for 2 seconds
   analogWrite(MOTOR_PIN, 255);
   
   delay(5000); // LEDs stay on for 5 seconds before motor starts


   // Now turn on the motor for 5 seconds
   analogWrite(MOTOR_PIN, 0); // Turn on motor
   delay(5000); // Motor on for 5 seconds


   //turn off motor
   analogWrite(MOTOR_PIN, 255);


   // Wait for 1 minute and 15 second (75 seconds) before repeating
   delay(75000); // Wait for 1 minute before repeating the cycle
 
   count = count + 1;
 }
}



