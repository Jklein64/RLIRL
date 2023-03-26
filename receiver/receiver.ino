#include <RF24.h>
#include <SPI.h>
#include <Servo.h>
#include <nRF24L01.h>

/*
motors:
 - drive #1: Danny
 - drive #2: Derek
 - mass: Madha

servos:
 - gimbal #1: Gary
 - gimbal #2: Greg
 - steering: Sammy
*/

#define DANNY_MIN 85
#define DANNY_MAX 200
#define DANNY_PIN 6

#define SAMMY_CENTER 120
#define SAMMY_PIN 3

#define GARY_MIN 0    // update this!
#define GARY_MAX 180  // update this!
#define GARY_PIN 7

Servo danny, sammy, gary;

int
    steer = SAMMY_CENTER,
    gimbal_1 = (GARY_MIN + GARY_MAX) / 2,
    throttle = 0;  // 0 to 100, and then interpolate for each motor

// if we have received 7 or more keys
// without any w, then we must've let go,
// so start draining the throttle.
// similar with steering
int
    keys_since_last_w = 0,
    keys_since_last_steer = 0;

RF24 radio(9, 8);
const byte address[10] = "ADDRESS01";

void setup() {
    Serial.begin(9600);

    gary.attach(GARY_PIN);
    sammy.attach(SAMMY_PIN);
    danny.attach(DANNY_PIN, 1000, 2000);

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop() {
    // listen for key from transmitter
    if (radio.available()) {
        char key = 0;
        radio.read(&key, sizeof(key));
        Serial.println(key);

        keys_since_last_w++;
        keys_since_last_steer++;

        switch (key) {
            case 'q':
                gimbal_1--;
                break;

            case 'e':
                gimbal_1++;
                break;

            case 'w':
                throttle++;
                keys_since_last_w = 0;
                break;

            case 's':
                throttle--;
                break;

            case 'a':
                steer++;
                keys_since_last_steer = 0;
                break;

            case 'd':
                steer--;
                keys_since_last_steer = 0;
                break;
        }
    }

    gimbal_1 = clamp(gimbal_1, GARY_MIN, GARY_MAX);

    if (keys_since_last_w >= 7 && throttle >= 0)
        throttle--;

    if (keys_since_last_steer >= 6) {
        // move steer towards the middle
        if (steer < SAMMY_CENTER)
            steer++;
        else
            steer--;
    }

    sammy.write(steer);
    danny.write(throttle);
    gary.write(gimbal_1);
}

int clamp(int value, int low, int high) {
    return max(low, min(value, high));
}