#include "motor.h"

#include "Arduino.h"

Motor::Motor(int enablePin, int in1Pin, int in2Pin) : enable(enablePin), in1(in1Pin), in2(in2Pin)
{
}

void Motor::begin()
{
    pinMode(this->enable, OUTPUT);
    digitalWrite(this->enable, LOW);
    pinMode(this->in1, OUTPUT);
    digitalWrite(this->in1, LOW);
    pinMode(this->in2, OUTPUT);
    digitalWrite(this->in2, LOW);
}

void Motor::forward(int speed)
{
    digitalWrite(this->in1, HIGH);
    digitalWrite(this->in2, LOW);
    analogWrite(this->enable, speed);
}

void Motor::forward(int speed, int durationMilis)
{
    this->forward(speed);
    delay(durationMilis);
    this->stop();
}

void Motor::backward(int speed)
{
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, HIGH);
    analogWrite(this->enable, speed);
}

void Motor::backward(int speed, int durationMilis)
{
    this->backward(speed);
    delay(durationMilis);
    this->stop();
}

void Motor::stop()
{
    digitalWrite(this->in1, LOW);
    digitalWrite(this->in2, LOW);
    digitalWrite(this->enable, LOW);
}
