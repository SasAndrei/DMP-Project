#include <Servo.h>
#include <NewPing.h>

const int LeftMotorForward = 51;
const int LeftMotorBackward = 50;
const int RightMotorForward = 53;
const int RightMotorBackward = 52;

#define trig_pin A1
#define echo_pin A2

#define max_distance 200

boolean goesForward = false;
int distance = 100;

boolean controlMode = true;

NewPing sonar(trig_pin, echo_pin, max_distance);
Servo servo_motor;

void setup() {

  Serial.begin(9600); // Serial 0 interface for PC
  Serial1.begin(9600); // Serial 1 interface for Bluetooth module

  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);

  servo_motor.attach(10);

  servo_motor.write(90);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();

}

void loop() {
  char mode = 'c';
  if (Serial1.available()) { // Read from Bluetooth and send to PC
    mode = Serial1.read();
    if (mode == 'a')
      controlMode =  false;
    else if (mode == 'c')
      controlMode = true;
  }
  if (controlMode)
    controled();
  else
    autonomus();

}

void controled() {
  char comand = '0';
  if (Serial1.available()) { // Read from Bluetooth and send to PC
    comand = Serial1.read();
  }
  if (comand == '0')
    moveStop();
  if (comand == '1')
    moveForward();
  if (comand == '2')
    moveBackwards();
  if (comand == '3')
    turnRight();
  if (comand == '4')
    turnLeft();
  delay(100);
}

void autonomus() {
  //Serial.write("autonom\n");
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 20) {
    moveStop();
    delay(300);
    moveBackwards();
    delay(400);
    moveStop();
    distanceRight = LookRight();
    delay(300);
    distanceLeft = LookLeft();
    delay(300);

    if (distance >= distanceLeft) {
      turnRight();
      moveStop();
    }
    else {
      turnLeft();
      moveStop();
    }

  }
  else {
    moveForward();
  }
  distance = readPing();
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();

  if (cm == 0)
    cm = 250;

  return cm;
}

int LookLeft() {
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(90);
  return distance;
}

int LookRight() {
  servo_motor.write(10);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(90);
  return distance;
}

void moveStop() {
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void moveForward() {
  //Serial.write("Front\n");
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void moveBackwards() {
  //Serial.write("Back\n");
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, HIGH);
}

void turnLeft() {
  //Serial.write("Right\n");
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, HIGH);
  digitalWrite(RightMotorBackward, LOW);
}

void turnRight() {
  //Serial.write("Left\n");
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}
