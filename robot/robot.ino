// The main program of the robot

// Include libs
#include "motor.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

// Define masks
#define LEFT_DIR_MASK 1
#define RIGHT_DIR_MASK 2
#define MILLIS_PER_DEGREE 3.3
#define FORWARD_BACKWARD_DELAY_MILLIS 1000

// Define nRF24L01 and motors
RF24 receiver(9, 10);
Motor leftMotor(3, 2, 4);
Motor rightMotor(6, 7, 5);

// The nRF24L01 address, must be the same as the address of the remote
const unsigned char ADDRESS[6] = "2Node";

// Converts degrees to radians
// from https://forum.arduino.cc/t/degree-to-radian/418739
float deg2rad(float deg)
{
  return (deg * 71.0) / 4068.0;
}

// The setup function
void setup()
{
  // Initialize serial for debugging purposes
  Serial.begin(9600);
  // Used by receiver.printDetails()
  printf_begin();
  // Inialize receiver
  if (!receiver.begin())
  {
    Serial.println("Receiver radio not responding");
    while (1)
      ;
    {
    }
  }
  else
  {
    Serial.println("Connected");
  }
  receiver.openReadingPipe(0, ADDRESS);
  receiver.setPALevel(RF24_PA_MIN);
  receiver.startListening();
  Serial.println(receiver.isChipConnected() ? "Receiver connected" : "Receiver NOT connected");
  receiver.printDetails(); // For debugging purposes
  leftMotor.begin();
  rightMotor.begin();
}

void loop()
{
  // Try to receive and decode messages
  if (receiver.available())
  {
    unsigned char data[3];
    receiver.read(data, sizeof(data));
    switch (data[0])
    {
    case 0:
    case 1:
    case 2:
    case 3:
      set_motor_power(data[0], data[1], data[2]);
      break;
    case 4:
      forward();
      break;
    case 5:
      backward();
      break;
    case 6:
      turn(-90);
      break;
    case 7:
      turn(90);
      break;
    }
  }
}

// Set the motor power
// motorDirMasks are the direction masks as defined in the protocol in README.md
// left/right Speed is the unsigned speed in the range [0, 256)
void set_motor_power(unsigned char motorDirMasks, unsigned char leftSpeed, unsigned char rightSpeed)
{
  if (motorDirMasks & LEFT_DIR_MASK)
    leftMotor.backward(leftSpeed);
  else
    leftMotor.forward(leftSpeed);
  if (motorDirMasks & RIGHT_DIR_MASK)
    rightMotor.backward(rightSpeed);
  else
    rightMotor.forward(rightSpeed);
}

// Go forward for one second
void forward()
{
  leftMotor.forward(0xFF);
  rightMotor.forward(0xFF);
  delay(FORWARD_BACKWARD_DELAY_MILLIS);
  leftMotor.stop();
  rightMotor.stop();
}

// Got backwards for one second
void backward()
{
  leftMotor.backward(0xFF);
  rightMotor.backward(0xFF);
  delay(FORWARD_BACKWARD_DELAY_MILLIS);
  leftMotor.stop();
  rightMotor.stop();
}

// Turn `angle` degrees
void turn(float angle)
{
  // Set the direction mask based on the direction
  unsigned char dirMask;
  if (angle < 0)
    dirMask = LEFT_DIR_MASK;
  else
    dirMask = RIGHT_DIR_MASK;
  unsigned long delayMillis = abs(angle) * MILLIS_PER_DEGREE;
  set_motor_power(dirMask, 0xFF, 0xFF);
  delay(delayMillis);
  leftMotor.stop();
  rightMotor.stop();
}
