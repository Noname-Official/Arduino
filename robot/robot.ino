#include "motor.h"
#include <nRF24L01.h>
#include <RF24.h>

#define LEFT_DIR_MASK 1
#define RIGHT_DIR_MASK 2

RF24 receiver(123, 123);   // TODO: actual pins
Motor leftMotor(1, 2, 3);  // TODO: actual pins
Motor rightMotor(4, 5, 6); // TODO: actual pins

const unsigned char ADDRESS[6] = "1Node";

void setup()
{
  receiver.begin();
  receiver.openReadingPipe(1, ADDRESS);
  receiver.setPALevel(RF24_PA_MIN);
  receiver.startListening();
  leftMotor.begin();
  rightMotor.begin();
}

void loop()
{
  if (receiver.available())
  {
    char data[3];
    receiver.read(&data, sizeof(data));
    unsigned char leftSpeed = data[1];
    unsigned char rightSpeed = data[2];
    if (data[0] & LEFT_DIR_MASK)
      leftMotor.backward(leftSpeed);
    else
      leftMotor.forward(leftSpeed);
    if (data[0] & RIGHT_DIR_MASK)
      rightMotor.backward(rightSpeed);
    else
      rightMotor.forward(rightSpeed);
  }
}
