//Filename: Lab3_part2.ino
//Author: Yan Sheng Luo, Tammy Le
//Date: 7/16
//Description: This file implements a simple task scheduler with four tasks.
//              Each task is defined by a TCB with fucntion pointer, priority and name.
//              Tasks will be excuted according to their priority, and tasks' priority will be 
//              reset after excution of all 4 tasks. Then repeat.


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//========= Macros ==========
#define LEDPIN D2
#define BUZZERPIN D5

//========= Global Variables=============
LiquidCrystal_I2C lcd(0x27, 16, 2);

struct TCB {
  void (*taskFunction)(); // Pointer to the task function
  bool isRunning; // State of the task
  bool isDone; // Whether the task is completed or not
  int pid; // Unique process ID
  int priority; // The priority level of the task
  String taskName;
};

//========= Function Prototypes==========
void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  
  pinMode(LEDPIN, OUTPUT);
  ledcSetup(0,1000,12);
  ledcAttachPin(BUZZERPIN, 0);
  delay(5);

}

void loop() {
  executeTasks();
  delay(3000);
}


void LED_Blinker() { //LED task function
  static int LEDcount = 0;
  for (int i = 0; i < 8; i++){
      digitalWrite(LEDPIN, HIGH);
      delay(63); // each toggle on and off takes 125ms
      digitalWrite(LEDPIN, LOW);
      delay(62);
      LEDcount++;
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
    delay (500);
  }
}

void MusicPlayer() { //Buzzer task function
  int melody[] = {220, 247, 262, 294, 330, 349, 392, 415, 440, 466};
  int duration = 500;
  
 for (int i = 0; i < 10; i++) {
    ledcWrite(0, 2048); // 50% duty cycle of 12 bit
    // Calculate the PWM period
    int period = 1000000 / melody[i];
    int dutycycle = period / 2;

    for (int j = 0; j < (duration * 1000) / period; j++) {
      ledcWrite(0, 2048); // Turn the buzzer on
      delayMicroseconds(dutycycle);
      ledcWrite(0, 0); // Turn the buzzer off
      delayMicroseconds(dutycycle);
    }
    delay(50); // Short delay before the next note
  }
}

void SerialPrint() { //Serial print task function
  for (char letter = 'A'; letter <= 'Z'; letter++){
    Serial.print(letter);
    delay(50);
  }
}

//initialize the TCB array
TCB TaskList[] = {  
  {LED_Blinker, false, false,1,1,"LED Blinker"},
  {LCDCounter, false, false,2,2, "Counter"},
  {MusicPlayer, false, false,3,3,"Music Player"},
  {SerialPrint, false, false,4,4,"Serial Printer"}
};

void executeTasks() {
  while (true) {
    bool allDone = true;

    // Execute tasks based on priority
    for (int priority = 1; priority <= 4; priority++) {
      for (int i = 0; i < 4; i++) {
        if (TaskList[i].priority == priority && !TaskList[i].isDone) {
          TaskList[i].isRunning = true;
          TaskList[i].taskFunction();
          TaskList[i].isRunning = false;
          TaskList[i].isDone = true;
          Serial.println(TaskList[i].taskName + ": " + TaskList[i].priority);
        }
      }
    }

    // Check if all tasks are done
    for (int i = 0; i < 4; i++) {
      if (!TaskList[i].isDone) {
        allDone = false;
        break;
      }
    }

    if (allDone) {
      // Reset tasks and increment priority
      for (int i = 0; i < 4; i++) {
        TaskList[i].isDone = false;
        TaskList[i].priority = TaskList[i].priority % 4 + 1;
      }
      break;
    }
  }
}





