#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// LCD display initialization
LiquidCrystal_I2C lcd(0x27, 16, 2); 
volatile bool messageReceived = false;
int counter = 0;
hw_timer_t *timer = NULL;

// ISR function to handle data received via ESP-NOW
void IRAM_ATTR dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
    Serial.print("Received message from: ");
    for (int i = 0; i < 6; i++) {
        Serial.printf("%02X:", mac[i]);
    }
    Serial.println();

    // Print received message
    Serial.print("Received message: ");
    Serial.write(incomingData, len);
    Serial.println();

    // Check if the incoming message matches "Button Pressed"
    if (strncmp((char*)incomingData, "Button Pressed", len) == 0) {
        messageReceived = true;
    }
}

// Timer ISR to increment counter
void IRAM_ATTR timerInterrupt() {
    counter++;
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    // Initialize LCD display
    lcd.init();
    lcd.backlight();

    // Create a timer, attach an interrupt, set an alarm which will update the counter every second.
    timer = timerBegin(0, 80, true); // Timer 0, prescaler 80 (1 tick = 1us)
    timerAttachInterrupt(timer, &timerInterrupt, true);
    timerAlarmWrite(timer, 1000000, true); // 1 second interval
    timerAlarmEnable(timer);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(dataReceived);

    Serial.println("Setup completed"); // Debug print
}

void loop() {
    // Print out an incrementing counter to the LCD.
    lcd.setCursor(0, 0);
    lcd.print("Counter: ");
    lcd.setCursor(10, 0);
    lcd.print(counter);
    lcd.print("   "); // Clear trailing digits

    // If a signal has been received over ESP-NOW, print out "New Message!" on the LCD
    if (messageReceived) {
        lcd.setCursor(0, 1);
        lcd.print("New Message!    ");
        messageReceived = false; // Reset the flag
    } else {
        lcd.setCursor(0, 1);
        lcd.print("                "); // Clear the second line
    }

    delay(1000); // Update the display every second
}