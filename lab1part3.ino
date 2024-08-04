// Define LED and switch pins
const int LED_PIN = 2;
const int SWITCH_PIN = 3;

void setup() {
  // Initialize the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  // Initialize the switch pin as an input with an internal pull-up resistor
  pinMode(SWITCH_PIN, INPUT_PULLUP);
}

void loop() {
  // Read the state of the switch
  int switchState = digitalRead(SWITCH_PIN);

  // If the switch is pressed, turn on the LED
  if (switchState == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    // Otherwise, turn off the LED
    digitalWrite(LED_PIN, LOW);
  }
}