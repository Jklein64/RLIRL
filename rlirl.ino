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

#define DANNY_MIN 85
#define DANNY_MAX 0
#define DANNY_PIN 3

#define SAMMY_MIN 2
#define SAMMY_MAX 179
#define SAMMY_PIN 2

Servo danny;  // create servo object to control the ESC
Servo sammy;

int interpolant = 0;  // value from the analog pin

char input;

void setup() {
    Serial.begin(9600);

    danny.attach(DANNY_PIN, 1000, 2000);  // (pin, min pulse width, max pulse width in microseconds)
    sammy.attach(SAMMY_PIN);
    danny.write(0);
    delay(5000);
}

void loop() {
    if (Serial.available() > 0) {
        input = Serial.read();
        switch (input) {
            case 's':
                interpolant -= 5;
                break;
            case 'w':
                interpolant += 5;
                break;
            case '=':
                interpolant += 10;
                break;
            case '-':
                interpolant -= 10;
                break;
        }

        Serial.println(interpolant);
    }

    sammy.write(interpolant);
}