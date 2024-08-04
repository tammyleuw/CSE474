//Filename: Lab3_part3_send.ino
//Author: Yan Sheng Luo, Tammy Le
//Date: 7/18
//Description: This file is a sender code, which sends a message to a receiver device via ESP_NOW.
//              It sets up a button with an interrupt, and send message when the button is pressed.


#include <esp_now.h>
#include <WiFi.h>

//========= Macros ==========
#define buttonPin 12


//========= Global Variables=============
uint8_t BroadcastAddress[] = {0xDC, 0xDA, 0x0C, 0x21, 0x5B, 0xF8}; 
volatile bool buttonPressed = false;



//========= Function Prototypes==========
// ISR function to handle button press
void IRAM_ATTR sendButtonPress() {
  buttonPressed = true;
}

// Callback function called when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println("Setup started"); // indicator for setup start

  WiFi.mode(WIFI_STA);

  // Set button pin as input and attach an interrupt
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), sendButtonPress, FALLING);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK)
    return;

  // Register the send callback function
  esp_now_register_send_cb(onDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memset(&peerInfo, 0, sizeof(esp_now_peer_info_t)); // initialize the peerInfo
  memcpy(peerInfo.peer_addr, BroadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;

  // Add peer check
  if (esp_now_add_peer(&peerInfo) == ESP_OK) {
    Serial.println("Peer added!");
  } else {
    Serial.println("Failed to add peer!");
    return;
  }

  Serial.println("Setup completed"); // indicator for setup has successfully competed
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false; // Reset the flag

    Serial.println("Button Pressed"); // Check if button press is detected

    // Prepare data to send
    uint8_t message = 1;
    esp_err_t result = esp_now_send(BroadcastAddress, &message, sizeof(message));

    // Check the result of sending
    if (result == ESP_OK) {
      Serial.println("Message sent successfully");
    } else {
      Serial.println("Error sending the message");
    }
  }
}