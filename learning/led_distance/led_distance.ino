/**
 * Change adafruit neopixels from red to green depending on the distance from an HC-SR04 proximity sensor
 * 
 * Requires the neopixel library from https://github.com/adafruit/Adafruit_NeoPixel
 * 
 * HC-SR04 code adopted from http://www.instructables.com/id/Simple-Arduino-and-HC-SR04-Example/step3/Upload-the-sketch/
 *
 * By Joshua Shanks
 */
 
#include <Adafruit_NeoPixel.h>

// Define pins
#define LED_PIN 10
#define TRIG 4
#define ECHO 5

// Define distance params
#define MARGIN 10 // max distance change between reads
#define CLOSE 10 // distance at which LED's turn red
#define FAR 60 // distance at which the LED's turn green

// init an 8 pixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, LED_PIN, NEO_GRB + NEO_KHZ800);

long lastDistance = NULL;

void setup() {
  Serial.begin (9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  long distance = getDistance();
  if(lastDistance != NULL) {
    distance = constrain(distance, lastDistance - MARGIN, lastDistance + MARGIN);
  }
  int mid = (CLOSE + FAR) / 2;

  // set to green if to far
  if(distance > FAR) {
    colorWipe(strip.Color(0, 255, 0), 0);
  // set to red if to close
  } else if(distance < CLOSE) {
    colorWipe(strip.Color(255, 0, 0), 0);
  } else {
    // tweak red and green values depending on which side of the mid point the distance is
    if(distance > mid) {
      // determine red value, decreasing with greater distance
      int r = map(distance, mid, FAR, 255, 0);
      colorWipe(strip.Color(r, 255, 0), 0);    
    } else {
      // determine green value, increasing with greater distance
      int g = map(distance, CLOSE, mid, 0, 255);
      colorWipe(strip.Color(255, g, 0), 0);
    }
  }
  
  if (distance >= 200 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
  lastDistance = distance;
  
  delay(250);
}

// get distance in centimeters
long getDistance() {
  long duration, distance;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

// Fill the dots one after the other with a color
// copied from adafruit library
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
