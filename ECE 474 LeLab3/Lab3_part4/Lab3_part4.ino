//Filename: Lab3_part4.ino
//Author: Yan Sheng Luo, Tammy Le
//Date: 7/18
//Description: This file implement a simple scheduler with 3 tasks in FCFS manner, and can be interrupted
//             by a button press that will cause the scheduler to move to the next task right away.



#include <Wire.h>
#include <LiquidCrystal_I2C.h>


//========= Macros ==========
#define LEDPIN D2
#define BUZZERPIN D5
#define BUTTONPIN D12

//========= Global Variables=============
LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile bool buttonPressed = false;
volatile bool interrupted = false;
int currentTask = 0;

// Define TCB structure
struct TCB { 
  void (*taskFunction)();
  bool isDone;
  String taskName; 
};

// Button press interrupt
void IRAM_ATTR ButtonPress(){
  buttonPressed = true;
  interrupted = true;
}

//========== Function Prototypes===========

void setup() {
  Serial.begin(1152000);
  Wire.begin();
  lcd.init();
  lcd.backlight();

  pinMode(LEDPIN, OUTPUT);
  ledcSetup(0,2000,8);
  ledcAttachPin(BUZZERPIN,0);

  pinMode(BUTTONPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTONPIN), ButtonPress, FALLING);
  delay(5);

}

void loop() {
  nextTask();

}



void LED_Blinker() { //LED task function
  static int LEDcount = 0;
  for (int i = 0; i < 10; i++){
      digitalWrite(LEDPIN, HIGH);
      delay(25); // each toggle on and off takes 50ms
      digitalWrite(LEDPIN, LOW);
      delay(25);
      LEDcount++;
      if (buttonPressed){
        buttonPressed = false; //reset the buttonPress flag
          break;  // stop the task right away
      }
        }
  LEDcount = 0;
}

void LCDCounter() { //LCD counter task function
  lcd.clear();
  for (int count = 1; count <= 10; count++){
    lcd.setCursor(0,0);  // set LCD cursor at the leftmost column and the fist line
    lcd.print("Count: ");
    lcd.setCursor(14,0);
    lcd.print(count);
    delay (1000);
    if (buttonPressed) {
      buttonPressed = false;
      break; 
    }
  }
}

void MusicPlayer() {
  int melody[] = {220, 247, 262, 294, 330, 349, 392, 415, 440, 466};
  int duration = 500;

  for (int i = 0; i < 10; i++){
    tone(BUZZERPIN, melody[i], duration);
    delay(duration);
    if (buttonPressed) {
      buttonPressed = false;
      noTone(BUZZERPIN); //stop sound
      break;
    }
  }
}

//Task list that in TCB structure, the task on top will run first
TCB TaskList[] = { 
  {LED_Blinker,false, "LED Blinker: 1st"},
  {LCDCounter,false, "LCD Counter: 2nd"},
  {MusicPlayer,false, "Music Player: 3rd"}
};



void nextTask() {
  while (true) {
    for (int i = 0; i < 3; i++) {
      int taskIndex = (currentTask + i) % 3; // Ensure currentTask won't excess 3
      Serial.println("Running: " + TaskList[taskIndex].taskName);
      TaskList[taskIndex].isDone = false;
      TaskList[taskIndex].taskFunction(); // Run task function
      TaskList[taskIndex].isDone = true; // Reset isDone flag

      if (interrupted) {
        interrupted = false;
        currentTask = (taskIndex + 1) % 3; // Move to the next task
        break; // Exit the for loop to continue with the next task
      }
    }
  }
}