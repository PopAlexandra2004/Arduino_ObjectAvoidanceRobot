#include <Adafruit_NeoPixel.h>

// Define WS2812B LED Strip
#define LED_PIN 7       // Data pin for LED strip
#define NUM_LEDS 8      // Number of LEDs in the strip
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define pins for motor speed control (EnA and EnB)
int VelocidadMotor1 = 6;
int VelocidadMotor2 = 5;

// Define control pins for motor direction (In1, In2, In3, In4)
int Motor1A = 13;
int Motor1B = 12;
int Motor2C = 11;
int Motor2D = 10;

// Infrared sensors - left and right
int infraPin = 2; // Right IR sensor
int infraPin1 = 3; // Left IR sensor

// Variables to capture sensor values: 0 - light background, 1 - black line
int valorInfra = 0;
int valorInfra1 = 0;

// Ultrasonic sensor pins
#define PIN_TRIG 8
#define PIN_ECHO 9

long duration, distance;

// Initial setup
void setup() {
Serial.begin(9600);
delay(1000);
// Configure ultrasonic sensor
pinMode(PIN_TRIG, OUTPUT);
pinMode(PIN_ECHO, INPUT);
// Set up IR sensor pins
pinMode(infraPin, INPUT);
pinMode(infraPin1, INPUT);

// Set up motor control pins
pinMode(Motor1A,OUTPUT);
pinMode(Motor1B,OUTPUT);
pinMode(Motor2C,OUTPUT);
pinMode(Motor2D,OUTPUT);
pinMode(VelocidadMotor1, OUTPUT);
pinMode(VelocidadMotor2, OUTPUT);

// Set initial motor speeds
analogWrite(VelocidadMotor1, 150);
analogWrite(VelocidadMotor2, 180);

// Initialize motor direction
digitalWrite(Motor1A, LOW);
digitalWrite(Motor1B, LOW);
digitalWrite(Motor2C, LOW);
digitalWrite(Motor2D, LOW);

// Initialize LED Strip
strip.begin();
strip.show(); // Turn off all LEDs initially
}

// Function to set all LEDs to a specific color
void setAllLeds(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

// Function for blinking left indicator
void blinkLeftIndicator() {
    for (int i = 0; i < 3; i++) { // Blink 3 times
        setAllLeds(strip.Color(255, 0, 0));  // All Red
        strip.setPixelColor(0, strip.Color(255, 255, 0)); // Leftmost LED Yellow
        strip.show();
        delay(200);
        setAllLeds(strip.Color(255, 0, 0)); // Reset to all Red
        strip.show();
        delay(200);
    }
}

// Function for blinking right indicator
void blinkRightIndicator() {
    for (int i = 0; i < 3; i++) { // Blink 3 times
        setAllLeds(strip.Color(255, 0, 0));  // All Red
        strip.setPixelColor(NUM_LEDS - 1, strip.Color(255, 255, 0)); // Rightmost LED Yellow
        strip.show();
        delay(200);
        setAllLeds(strip.Color(255, 0, 0)); // Reset to all Red
        strip.show();
        delay(200);
    }
}


// Main loop
void loop() {
// Ultrasonic sensor measurement
digitalWrite(PIN_TRIG, LOW);
delayMicroseconds(5);
digitalWrite(PIN_TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(PIN_TRIG, LOW);
duration = pulseIn(PIN_ECHO, HIGH);
distance = (duration / 2) / 29.1; // Convert time to distance in cm
Serial.print("Distance to object: ");
Serial.print(distance);
Serial.println(" cm.");
// Read IR sensor values
valorInfra = digitalRead(infraPin); // Right sensor
valorInfra1 = digitalRead(infraPin1); // Left sensor

Serial.print("Right Sensor: ");
Serial.println(valorInfra);
Serial.print("Left Sensor: ");
Serial.println(valorInfra1);

if (distance > 10){ // If no obstacle is detected within 10 cm
// Move forward
if(valorInfra == 1 && valorInfra1 == 1){
Serial.println("Moving Forward");
// Set motors to move forward
digitalWrite(Motor1A, HIGH); // Left motor forward
digitalWrite(Motor1B, LOW);
digitalWrite(Motor2C, HIGH); // Right motor forward
digitalWrite(Motor2D, LOW);
analogWrite(VelocidadMotor1, 150); // Adjust speed
analogWrite(VelocidadMotor2, 150);
setAllLeds(strip.Color(0, 255, 0)); // All LEDs Red



}
// Turn right
else if(valorInfra == 1 && valorInfra1 == 0){
Serial.println("Turning Right");
digitalWrite(Motor1A, LOW);
digitalWrite(Motor2C, LOW);
delay(25);
digitalWrite(Motor1A, LOW);
digitalWrite(Motor2C, HIGH);
delay(20);
blinkRightIndicator(); // Blink Rightmost LED Yellow
}
// Turn left
else if(valorInfra == 0 && valorInfra1 == 1){
Serial.println("Turning Left");
digitalWrite(Motor1A, LOW);
digitalWrite(Motor2C, LOW);
delay(25);
digitalWrite(Motor1A, HIGH);
digitalWrite(Motor2C, LOW);
delay(20);
blinkLeftIndicator(); // Leftmost LED Blinks Yellow
}
// Stop at end of line
else if(valorInfra == 0 && valorInfra1 == 0){
Serial.println("Line End Detected");
// Stop motors
digitalWrite(Motor1A, LOW);
digitalWrite(Motor1B, HIGH);  // Left motor moves backward
digitalWrite(Motor2C, LOW);
digitalWrite(Motor2D, LOW);   // Right motor stops
analogWrite(VelocidadMotor1, 120);
analogWrite(VelocidadMotor2, 0);
setAllLeds(strip.Color(255, 0, 0)); // All LEDs Red
delay(500);  // Small reverse delay
}
}
else{
Serial.println("Obstacle Detected - Stopping");
// Stop motors
digitalWrite(Motor1A, LOW);
digitalWrite(Motor1B, LOW);   // Left motor stops
digitalWrite(Motor2C, LOW);
digitalWrite(Motor2D, HIGH);  // Right motor moves backward
analogWrite(VelocidadMotor1, 0);
analogWrite(VelocidadMotor2, 120);
setAllLeds(strip.Color(255, 0, 0)); // All LEDs Red
delay(500);  // Small reverse delay
}
}