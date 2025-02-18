#pragma once

class Motor
{
public:
    Motor(int enablePin, int in1Pin, int in2Pin);

    void begin();

    void forward(int speed);
    void forward(int speed, int durationMilis);
    void backward(int speed);
    void backward(int speed, int durationMilis);
    void stop();

private:
    int enable;
    int in1;
    int in2;
};
