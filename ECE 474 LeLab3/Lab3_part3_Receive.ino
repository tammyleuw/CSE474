//Filename: Lab3_part1.ino
//Author: Yan Sheng Luo, Tammy Le
//Date: 7/18
//Description: This file is receiver code that control the timer and lcd display.
//              As well implement interrupt to pause the time counter and display
//              message on LCD when received an interrupt signal from the sender device. 


#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>

//========= Global Variables=============
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile bool messageReceived = false;
int counter = 0; 
hw_timer_t *timer = NULL;
volatile bool pauseCounter = false; 


//========= Function Prototypes==========

// ISR function to handle data received via ESP-NOW
void IRAM_ATTR dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(uint8_t) && *incomingData == 1) {
        messageReceived = true;
        pauseCounter = true;
    }
}

// Timer ISR to increment counter
void IRAM_ATTR timerInterrupt() {
  if (!pauseCounter) {
    counter++;
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  WiFi.mode(WIFI_STA);

  // Initialize LCD display
  lcd.init();
  lcd.backlight();

  // Create a timer, attach an interrupt, set an alarm which will update the counter every second.
  timer = timerBegin(0, 80, true); // Timer 0, prescaler 80 (1 tick = 1us)
  timerAttachInterrupt(timer, &timerInterrupt, true);
  timerAlarmWrite(timer, 1000000, true); // 1 second interval
  timerAlarmEnable(timer);

  // Initializes ESP-NOW and check if it was successful; if not, exit the setup function
  if (esp_now_init() != ESP_OK) {
    Serial.println("error initializing ESP_NOW");
    return;
  }
  esp_now_register_recv_cb(dataReceived);
  Serial.println("setup completed");  // Indicate the setup completed successfully
}

void loop() {
  // Print out an incrementing counter to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("Counter: ");
  lcd.setCursor(10,0);
  lcd.print(counter);

  // If a signal has been received over ESP-NOW, print out "New Message!" on the LCD
  if (messageReceived) {
    lcd.setCursor(0, 1);
    lcd.print("New Message!    ");
    delay(2000); // Pause the counter for 2 seconds while displaying the message
    messageReceived = false; // Reset the flag
    pauseCounter = false; // Resume the counter after displaying the message
  } else {
    lcd.setCursor(0,1);
    lcd.print("                "); // Clear the second line
  }

  delay(1000); // Update the display every second
}