#include <stdio.h>

#include <Servo.h>
#include <Wire.h>

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Belt Conveyor
const int steps = 400;
const int stepperPin = 1;
Adafruit_StepperMotor *BeltConveyorStepper = AFMS.getStepper(steps, stepperPin);

// First Bun's Arm
const int FirstBunArmPin = 3;
Adafruit_DCMotor *FirstBunArm = AFMS.getMotor(FirstBunArmPin);

// First Bun's Selector
const int FirstBunSelectorPin = 10;
int angles[] = {0, 36, 72, 108, 144, 180};
Servo FirstBunSelector;

// Cheese Arm

const unsigned int MaxCommandLen = 8;
const char delimiter[] = " ";

/**
 * Utils
 */

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
  
  if (strcmp(partName, "bun1") == 0) {
    char *doWhat = cmds[1];
    
    if (strcmp(doWhat, "select") == 0) {
      int value = atoi(cmds[2]);
      Serial.println(">>> Started running the first bun selector's servo.");
      FirstBunSelector.write(value);
    } else if (strcmp(doWhat, "arm") == 0) {
      int value = atoi(cmds[2]);
      int t = atoi(cmds[3]) / 10;
      
      Serial.println(">>> Started running the first bun arm's motor.");
      if (value > 0) {
        // Serial.println("--- FORWARD ---");
        FirstBunArm->run(FORWARD);
      } else if (value < 0) {
        // Serial.println("--- BACKWARD ---");
        value *= -1;
        FirstBunArm->run(BACKWARD);
      }
      
      for (int i = 0; i < t; i++) {
        FirstBunArm->setSpeed(value);
        delay(10);
      }
      
      FirstBunArm->run(RELEASE);
    }
  }
}

void setup() {
  const int baudrate = 9600;
  Serial.begin(baudrate);

  AFMS.begin();
  BeltConveyorStepper->setSpeed(50);

  FirstBunArm->run(RELEASE);

  FirstBunSelector.attach(FirstBunSelectorPin);
  FirstBunSelector.write(90);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil("\n");
    Serial.println(input);
    
    char *commands[MaxCommandLen];
    parseInput(commands, input, delimiter, MaxCommandLen);
    runCommand(commands);
  }
}
