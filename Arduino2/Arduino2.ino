#include <Wire.h>
#include <Servo.h>

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Cheese Arm's Motor
const int CheeseArmPin = 1;
Adafruit_DCMotor *CheeseArm = AFMS.getMotor(CheeseArmPin);

// Ketchup Arm's Servo
const int KetchupArmPin = 10;
Servo KetchupArm;

// Second Bun's Selector
const int SecondBunSelectorPin = 9;
int angles[] = {0, 36, 72, 108, 144, 180};
Servo SecondBunSelector;

// Second Bun's Arm
const int SecondBunArmPin = 4;
Adafruit_DCMotor *SecondBunArm = AFMS.getMotor(SecondBunArmPin);

const unsigned int MaxCommandLen = 8;
const char delimiter[] = " ";

void parseInput(char **commands, String input, const char* delimiter,
                unsigned int maxLen) 
{
  unsigned int inputLen = input.length() + 1;

  if (input.length() > 0) {
    char buf[256];
    int i = 0;

    input.toCharArray(buf, inputLen, 0);

    char *command = strtok(buf, delimiter);
    commands[i] = command;
    while (command != NULL) {
      command = strtok(NULL, delimiter);
      if (command != NULL) {
        commands[++i] = command;
      }
    }
    if (i < maxLen) {
      for (i++ ; i < maxLen; i++) {
         commands[i] = "\0";
      }
    }
  }
}

void runCommand(char **cmds) {
  char *partName = cmds[0];
  
  if (strcmp(partName, "sleep") == 0) {
    int t = atoi(cmds[1]);
    delay(t);
  }
  
  if (strcmp(partName, "cheese") == 0) {
    int value = atoi(cmds[1]);
    int t = atoi(cmds[2]) / 10;

    Serial.println(value);
    Serial.println(t);
    if (value > 0) {
      CheeseArm->run(BACKWARD);
    } else if (value < 0) {
      value *= -1;
      CheeseArm->run(FORWARD);
    }

    for (int i = 0; i < t; i++) {
      CheeseArm->setSpeed(value);
      delay(10);
    }

    CheeseArm->run(RELEASE);
  }

  if (strcmp(partName, "ketchup") == 0) {
    Serial.println(">>>.");
    int value = atoi(cmds[1]);
    
    KetchupArm.write(value);
  }

  if (strcmp(partName, "bun2") == 0) {
    char *doWhat = cmds[1];
    
    if (strcmp(doWhat, "select") == 0) {
      Serial.println(">>> Started running the second bun selector's servo.");
      
      int value = atoi(cmds[2]);
      SecondBunSelector.write(value);
    } else if (strcmp(doWhat, "arm") == 0) {
      Serial.println(">>> Started running the second bun arm's motor.");
      
      int value = atoi(cmds[2]);
      int t = atoi(cmds[3]) / 10;
      if (value > 0) {
        // Serial.println("--- FORWARD ---");
        SecondBunArm->run(FORWARD);
      } else if (value < 0) {
        // Serial.println("--- BACKWARD ---");
        value *= -1;
        SecondBunArm->run(BACKWARD);
      }
      
      for (int i = 0; i < t; i++) {
        SecondBunArm->setSpeed(value);
        delay(10);
      }
      
      SecondBunArm->run(RELEASE);
    }
  }
}

void setup() {
  const int baudrate = 9600;
  Serial.begin(baudrate);

  AFMS.begin();
  
  CheeseArm->run(RELEASE);

  KetchupArm.attach(KetchupArmPin);
  KetchupArm.write(0);

  SecondBunArm->run(RELEASE);
  
  SecondBunSelector.attach(SecondBunSelectorPin);
  SecondBunSelector.write(0);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    delay(100);
    Serial.println(input);
    
    char *commands[MaxCommandLen];
    parseInput(commands, input, delimiter, MaxCommandLen);
    runCommand(commands);
  }
}
