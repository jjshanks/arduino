/**
 * Traffic light with cross walk simulator.
 *
 * Example wiring can be found at http://fritzing.org/projects/digital-inputoutput-traffic-light/
 * 
 * By Joshua Shanks
 */
// Define which light is connected to which pin
const int RED_TRAFFIC = 10;
const int YELLOW_TRAFFIC = 11;
const int GREEN_TRAFFIC = 12;
const int RED_WALK = 8;
const int GREEN_WALK = 9;

// Define which pin the button is wired to
const int BUTTON = 2;

int traffic_timer = 2500;
int walk_timer = 2500;
// default so that green is the first cycle
int currentTrafficLight = RED_TRAFFIC;

// crosswalk interrupt var
volatile boolean needsWalk = false;

void setup() {
  // set all lights to output
  for(int pin = 8; pin < 13; pin++) {
    pinMode(pin, OUTPUT);
  }
  // set button to input
  pinMode(BUTTON, INPUT);
  // fire when button is pressed
  // interrupt 0 is actually pin 2
  attachInterrupt(0, buttonPress, CHANGE);
  digitalWrite(RED_WALK, HIGH);
}

// called when button is pressed
void buttonPress() {
  needsWalk = true;
}

/**
 * Cycles traffic light from green -> yellow -> red -> green ...
 *
 * Returns the pin of the current light
 */
int cycleTrafficLight(int current) {
  int next = current - 1;
  if(next < RED_TRAFFIC) {
    next = GREEN_TRAFFIC;
  }
  digitalWrite(current, LOW);
  digitalWrite(next, HIGH);
  return next;
}

/**
 * Crosswalk pattern
 */
void crosswalk() {
  digitalWrite(GREEN_WALK, HIGH);
  digitalWrite(RED_WALK, LOW);
  delay(walk_timer);
  digitalWrite(GREEN_WALK, LOW);
  // flash red walk
  for(int i = 0; i < 4; i ++) {
    digitalWrite(RED_WALK, HIGH);
    delay(walk_timer / 4);
    digitalWrite(RED_WALK, LOW);
    delay(walk_timer / 6);
  }
  digitalWrite(RED_WALK, HIGH);
  needsWalk = false;
}

void loop() {
  // cycle traffic light
  currentTrafficLight = cycleTrafficLight(currentTrafficLight);
  // if traffic light is red and the button was pressed do crosswalk signal
  if(currentTrafficLight == RED_TRAFFIC && needsWalk) {
    crosswalk();
  } else {
    delay(traffic_timer);
  }
}
