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

Servo danny;
Servo sammy;

int
    steer = (SAMMY_MIN + SAMMY_MAX) / 2,  // SAMMY_MIN to SAMMY_MAX
    throttle = 0;                         // 0 to 100, and then interpolate for each motor

RF24 radio(9, 8);
const byte address[10] = "ADDRESS01";

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

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
    }
}

// void setup() {
//     Serial.begin(9600);

//     danny.attach(DANNY_PIN, 1000, 2000);  // (pin, min pulse width, max pulse width in microseconds)
//     sammy.attach(SAMMY_PIN);
//     danny.write(0);
//     delay(5000);
// }

// void loop() {
//     if (Serial.available() > 0) {
//         input = Serial.read();
//         switch (input) {
//             case 'a':
//                 steer -= 5;
//                 break;
//             case 'd':
//                 steer += 5;
//                 break;
//             case '=':
//                 throttle += 10;
//                 break;
//             case '-':
//                 throttle -= 10;
//                 break;
//         }

//         Serial.println(steer);
//         Serial.println(throttle);
//     }

//     sammy.write(steer);
//     danny.write(throttle);
// }