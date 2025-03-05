# Arduino
## Parts
1. joystick
1. 2x nRF24L01
1. 4x button
1. L293D (or other motor driver)
1. 2x DC motoren
1. 2x 5V batteries
1. 2x arduino unos
1. 3d printed frame for robot
1. 3d printed frame for remote
1. some screws and bolts
1. 2x driver wheels
1. bal bearing for front/back wheel

## Protocol
Bit(s)|description
------|-----------
0     |Left motor direction 0=forwards, 1=backwards
1     |Right motor direction 0=forwards, 1=backwards
8-15  |Left motor speed
16-23 |Right motor speed
