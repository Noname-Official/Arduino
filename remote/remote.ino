// The main program of the remote of the robot

// Include libs
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

// Deadzone of the joystick out of 512
#define DEAD_ZONE 10

// Fake grounds
// There weren't enough ground ports on the arduino to connect all the buttons to,
// so we use three pins as extra ground pins by permanantly setting them low.
#define FAKE_GND_1 2
#define FAKE_GND_2 3
#define FAKE_GND_3 8

// Define pins
#define JOY_STICK_X_PIN A0
#define JOY_STICK_Y_PIN A1
#define FORWARD_BUTTON 4
#define BACKWARD_BUTTON 6
#define TURN_LEFT_BUTTON 5
#define TURN_RIGHT_BUTTON 7

// Define nRF24L01
RF24 sender(9, 10);

// The nRF24L01 address, must be the same from the address of the motor
const unsigned char ADDRESS[6] = "2Node";

// Joystickdata, contains x and y values of the joystick in the range [-512, 512)
struct JoyStickData
{
    int x;
    int y;

    bool isDeadZone()
    {
        return -DEAD_ZONE < this->x && this->x < DEAD_ZONE && -DEAD_ZONE < this->y && this->y < DEAD_ZONE;
    }
};

// Get the joystick data
JoyStickData get_joy_stick()
{
    int x = analogRead(JOY_STICK_X_PIN);
    int y = analogRead(JOY_STICK_Y_PIN);
    x -= 512;
    y -= 512;
    return JoyStickData{x, y};
}

// Setup code
void setup()
{
    // Initialize ground pins
    pinMode(FAKE_GND_1, OUTPUT);
    digitalWrite(FAKE_GND_1, LOW);
    pinMode(FAKE_GND_2, OUTPUT);
    digitalWrite(FAKE_GND_2, LOW);
    pinMode(FAKE_GND_3, OUTPUT);
    digitalWrite(FAKE_GND_3, LOW);
    // Initialize button pins
    pinMode(FORWARD_BUTTON, INPUT_PULLUP);
    pinMode(BACKWARD_BUTTON, INPUT_PULLUP);
    pinMode(TURN_LEFT_BUTTON, INPUT_PULLUP);
    pinMode(TURN_RIGHT_BUTTON, INPUT_PULLUP);
    // Begin serial for debugging
    Serial.begin(9600);
    // Used by sender.printDetails
    printf_begin();
    // Initialize the nRF24L01
    if (!sender.begin())
    {
        Serial.println("Sender radio not responding");
        while (1)
            ;
    }
    sender.openWritingPipe(ADDRESS);
    sender.setPALevel(RF24_PA_MIN);
    sender.stopListening();
    Serial.println(sender.isChipConnected() ? "Sender connected" : "Sender NOT connected");
    sender.printDetails(); // Used for debugging purposes
}

void loop()
{
    // Loop until the joystick is in the deadzone
    while (true)
    {
        // Get joystick data
        JoyStickData joy_stick = get_joy_stick();
        if (!joy_stick.isDeadZone())
        {
            // Set motor power if not in deadzone
            set_motor_power(joy_stick.x, joy_stick.y);
            return;
        }
        else
        {
            // Joystick is in the deadzone, check buttons
            break;
        }
    }
    // Joystick is in the deadzone, disable motors
    set_motor_power(0, 0);
    // Handle button presses
    handle_button(FORWARD_BUTTON, []()
                  { simple_command(4); });
    handle_button(BACKWARD_BUTTON, []()
                  { simple_command(5); });
    handle_button(TURN_LEFT_BUTTON, []()
                  { simple_command(6); });
    handle_button(TURN_RIGHT_BUTTON, []()
                  { simple_command(7); });
    delay(10); // Give the robot a bit of time to receive the data
}

// Handle button press
// button is the button pin and fn is the function to be called when the button is pressed
// Returns when the button is no longer pressed
void handle_button(int button, void (*fn)())
{
    // Return if the button is not pressed
    if (digitalRead(button) != LOW)
        return;
    fn();
    // While the button is pressed and the joy stick is in the deadzone
    // The joystick takes presedence over the buttons
    while (digitalRead(button) == LOW && get_joy_stick().isDeadZone())
    {
        delay(10);
    }
}

// Set motor power, x and y are the joystick values in the range [-512,512)
void set_motor_power(int x, int y)
{
    // Calculate left and right motor powers
    int left = 0;
    int right = 0;
    // Increase both motor powers if going forwards
    left += y;
    right += y;
    // Decrease motor power on the left if going right
    left -= x;
    // Increase motor power on the right if going left
    right += x;
    // Map to range [-512,512)
    left >>= 1;
    right >>= 1;
    bool leftReverse = left < 0;
    bool rightReverse = right < 0;
    left = abs(left);
    right = abs(right);
    // Clamp to [0,256)
    left = min(left, 255);
    right = min(right, 255);
    // Send data
    unsigned char data[]{(unsigned char)(leftReverse | rightReverse << 1), (unsigned char)left, (unsigned char)right};
    send_data(data, sizeof(data));
}

// Send a simple command consisting of a single byte to the robot
void simple_command(unsigned char command)
{
    unsigned char data[] = {command};
    send_data(data, 1);
}

// Send data to the robot and to the serial monitor for debugging purposes
void send_data(unsigned char data[], unsigned char len)
{
    for (int i = 0; i < len; i++)
    {
        Serial.print(data[i]);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println(sender.write(data, len));
}
