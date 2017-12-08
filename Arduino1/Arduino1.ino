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
  char *doWhat = cmds[1];
  char *value = cmds[2];

  if (strcmp(partName, "bun1") == 0) {
    if (strcmp(doWhat, "select") == 0) {
      Serial.println("Begin FirstBunSelector Control.");
      FirstBunSelector.write(atoi(value));
    }
  }
}

void setup() {
  const int baudrate = 9600;
  Serial.begin(baudrate);

  AFMS.begin();
  BeltConveyorStepper->setSpeed(50);

  FirstBunArm->setSpeed(10);
  FirstBunArm->run(FORWARD);
  FirstBunArm->run(RELEASE);

  FirstBunSelector.attach(FirstBunSelectorPin);
  FirstBunSelector.write(90);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil("\n");
    char *commands[MaxCommandLen];
    parseInput(commands, input, delimiter, MaxCommandLen);
    runCommand(commands);
  }
}
