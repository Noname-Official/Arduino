// A single motor

#pragma once

class Motor
{
public:
    Motor(int enablePin, int in1Pin, int in2Pin);

    // Initialize the motor, must be called once during setup
    void begin();

    // Turn the motor forward, speed is in the range [0, 256)
    void forward(int speed);
    // Turn the motor forward, speed is in the range [0, 256)
    void forward(int speed, int durationMilis);
    // Turn the motor backwards, speed is in the range [0, 256)
    void backward(int speed);
    // Turn the motor backwards, speed is in the range [0, 256)
    void backward(int speed, int durationMilis);
    // Stop the motor
    void stop();

private:
    // The pins for this motor
    int enable;
    int in1;
    int in2;
};
