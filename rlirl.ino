#include <Servo.h>

/*
motors:
 - drive #1: Danny
 - drive #2: Derek
 - mass: Madha

servos:
 - gimbal #1: Greg
 - gimbal #2: Gary
 - steering: Sammy
*/

#define DANNY_MIN 0
#define DANNY_MAX 0
#define DANNY_PIN 3

Servo danny;  // create servo object to control the ESC

int interpolant = 0;  // value from the analog pin

char input;

void setup() {
    Serial.begin(9600);

    danny.attach(DANNY_PIN, 1000, 2000);  // (pin, min pulse width, max pulse width in microseconds)
    delay(5000);
}

void loop() {
    if (Serial.available() > 0) {
        input = Serial.read();
        if (input == '=') {
            interpolant = interpolant + 10;
        } else if (input == '-') {
            interpolant = interpolant - 10;
        }
        Serial.println(interpolant);
    }

    danny.write(interpolant);
}