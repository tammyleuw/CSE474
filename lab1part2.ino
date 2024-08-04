// Define LED pins
const int LED1_PIN = 2;
const int LED2_PIN = 3;

void setup() {
  // Initialize the LED pins as outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop() {
  // Blink the first LED
  blinkLED(LED1_PIN, 500); // Blink every 500 ms
  
  // Blink the second LED
  blinkLED(LED2_PIN, 1000); // Blink every 1000 ms
}

// Function to blink an LED
void blinkLED(int pin, int delayTime) {
  digitalWrite(pin, HIGH);  // Turn the LED on
  delay(delayTime);         // Wait for delayTime
  digitalWrite(pin, LOW);   // Turn the LED off
  delay(delayTime);         // Wait for delayTime
}