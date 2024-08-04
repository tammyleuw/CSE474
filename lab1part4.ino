// Define LED and switch pins
const int LED_PINS[5] = {2, 3, 4, 5, 6};    // Define the LED pins
const int SWITCH_PINS[5] = {7, 8, 9, 10, 11}; // Define the switch pins

// Variables to store the state of the LEDs
bool ledStates[5] = {true, true, true, true, true};

// Variables to store the previous state of the switches
bool switchPreviousStates[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};

// Variables to store the debounce time
unsigned long lastDebounceTimes[5] = {0, 0, 0, 0, 0};
const unsigned long debounceDelay = 50;

void setup() {
  // Initialize the LED pins as outputs
  for (int i = 0; i < 5; i++) {
    pinMode(LED_PINS[i], OUTPUT);
  }

  // Initialize the switch pins as inputs with internal pull-up resistors
  for (int i = 0; i < 5; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  // Handle each LED and Switch
  for (int i = 0; i < 5; i++) {
    handleSwitch(i);
    
    // Blink the LEDs if they are in the flashing state
    if (ledStates[i]) {
      blinkLED(LED_PINS[i], 500);
    } else {
      digitalWrite(LED_PINS[i], LOW);
    }
  }
}

// Function to handle switch state and debounce
void handleSwitch(int index) {
  int switchState = digitalRead(SWITCH_PINS[index]);

  if (switchState != switchPreviousStates[index]) {
    lastDebounceTimes[index] = millis();
  }

  if ((millis() - lastDebounceTimes[index]) > debounceDelay) {
    if (switchState == LOW && switchPreviousStates[index] == HIGH) {
      ledStates[index] = !ledStates[index];
    }
  }

  switchPreviousStates[index] = switchState;
}

// Function to blink an LED
void blinkLED(int pin, int delayTime) {
  digitalWrite(pin, HIGH);
  delay(delayTime);
  digitalWrite(pin, LOW);
  delay(delayTime);
}
