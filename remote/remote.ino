#include <nRF24L01.h>
#include <RF24.h>

#define JOY_STICK_X_PIN A0 // TODO: actual pins
#define JOY_STICK_Y_PIN A1 // TODO: actual pins

RF24 sender(123, 123); // TODO: actual pins

const unsigned char ADDRESS[6] = "2Node";

void setup()
{
    sender.begin();
    sender.openWritingPipe(ADDRESS);
    sender.setPALevel(RF24_PA_MIN);
    sender.stopListening();
    Serial.begin(9600);
}

void loop()
{
    int x = analogRead(JOY_STICK_X_PIN);
    int y = analogRead(JOY_STICK_Y_PIN);
    x -= 512;
    y -= 512;
    int left = 0;
    int right = 0;
    left += y;
    right += y;
    left += x;
    right -= x;
    left >>= 2;
    right >>= 2;
    bool leftReverse = left < 0;
    bool rightReverse = right < 0;
    left = abs(left);
    right = abs(right);
    unsigned char data[] = {(unsigned char)(leftReverse | rightReverse << 1), (unsigned char)left, (unsigned char)right};
    Serial.print(data[0], BIN);
    Serial.print(" ");
    Serial.print(data[1]);
    Serial.print(" ");
    Serial.println(data[2]);
    // sender.write(data, sizeof(data));
}
