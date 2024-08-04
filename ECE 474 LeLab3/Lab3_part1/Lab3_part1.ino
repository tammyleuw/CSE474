//Filename: Lab3_part1.ino
//Author: Yan Sheng Luo, Tammy Le
//Date: 7/16
//Description: This file takes input from serial then print on a lcd

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//========= Global Variables=============
LiquidCrystal_I2C lcd(0x27, 16, 2);

//========= Function Prototypes==========
void setup() {
    Serial.begin(115200);
    Wire.begin();
    lcd.init();
    lcd.backlight();
    delay(2);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        Serial.println(input);  // Print the user input to the Serial Monitor
        sendToLCD(input);
    }
}

void sendToLCD(String input) {
    sendCommand(0x01);  // Clear display 
    delay(2);

    sendCommand(0x80);  // Set cursor to first line

    for (int i = 0; i < input.length() && i < 32; i++) { // Max 32 chars for 2 lines of 16 chars each
        if (i == 16) {
          sendCommand(0xC0); // Move to the second line if necessary
        } 
        sendData(input[i]);
    }
}

void sendCommand(byte command) {
    sendByte(command, 0x00); 
}

void sendData(byte data) {
    sendByte(data, 0x01);
}

void sendByte(byte data, byte mode) {
    byte highNibble = (data & 0xF0) | mode | 0x08; // Ensure backlight is on
    byte lowNibble = ((data << 4) & 0xF0) | mode | 0x08; 
    sendInput(highNibble);
    sendInput(lowNibble);
}

void sendInput(byte nibble) {
    Wire.beginTransmission(0x27);
    Wire.write(nibble | 0x04);  // Enable bit HIGH
    Wire.endTransmission();
    delay(2);
    Wire.beginTransmission(0x27);
    Wire.write(nibble & ~0x04);  // Enable bit LOW
    Wire.endTransmission();
    delay(50);
}