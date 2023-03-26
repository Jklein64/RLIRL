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

 sensors:
  - ultrasonic: Uma
*/

#define DANNY_MIN 80
#define DANNY_MAX 200
#define DANNY_PIN 6

#define SAMMY_CENTER 120
#define SAMMY_PIN 3

#define GARY_MIN 0
#define GARY_MAX 180
#define GARY_PIN 7

#define MADHA_MIN 0
#define MADHA_MAX 100
#define MADHA_PIN 10

#define UMA_TRIG_PIN A7
#define UMA_ECHO_PIN A6

Servo danny, sammy, gary, madha;

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

unsigned long duration, distance;
// unsigned long
//     last_uma_trig = micros(),
//     last_uma_trig_elapsed = 0;

RF24 radio(9, 8);
const byte address[10] = "ADDRESS01";

void setup() {
    Serial.begin(9600);

    gary.attach(GARY_PIN);
    sammy.attach(SAMMY_PIN);
    danny.attach(DANNY_PIN, 1000, 2000);
    madha.attach(MADHA_PIN, 1000, 2000);
    madha.write(0);

    // setup ultrasonic
    pinMode(UMA_TRIG_PIN, OUTPUT);
    pinMode(UMA_ECHO_PIN, INPUT);
    digitalWrite(UMA_TRIG_PIN, LOW);
    delayMicroseconds(5);

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    Serial.println("Radio begun!");
}

void loop() {
    /*     // ultrasonic things
        digitalWrite(UMA_TRIG_PIN, LOW);
        delayMicroseconds(5);
        // trigger sensor by setting high for 10us
        digitalWrite(UMA_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(UMA_TRIG_PIN, LOW);

        duration = pulseIn(UMA_ECHO_PIN, HIGH, 35);
        distance = duration * 0.034 / 2;
        Serial.print("Distance = ");
        Serial.print(distance);
        Serial.println(" cm"); */

    // last_uma_trig_elapsed = micros() - last_uma_trig;
    // if (last_uma_trig_elapsed < 5)
    //     digitalWrite(UMA_TRIG_PIN, LOW);
    // // trigger sensor by setting high for 10us
    // else if (last_uma_trig_elapsed < 5 + 10) {
    //     digitalWrite(UMA_TRIG_PIN, HIGH);
    //     last_uma_trig = micros();
    // } else
    //     digitalWrite(UMA_TRIG_PIN, LOW);

    // listen for key from transmitter
    if (radio.available()) {
        Serial.println("something was available from radio");
        char key = 0;
        radio.read(&key, sizeof(key));
        Serial.println(key);

        keys_since_last_w++;
        keys_since_last_steer++;

        switch (key) {
            case 'q':
                gimbal_1 -= 10;
                break;

            case 'e':
                gimbal_1 += 10;
                break;

            case 'w':
                throttle += 10;
                keys_since_last_w = 0;
                break;

            case 's':
                throttle -= 10;
                break;

            case 'a':
                steer += 10;
                keys_since_last_steer = 0;
                break;

            case 'd':
                steer -= 10;
                keys_since_last_steer = 0;
                break;

            case 'p':
                for (size_t i = 0; i < MADHA_MAX; i++) {
                    madha.write(i);
                    delay(5);
                }
                break;

            case 'o':
                madha.write(0);
                break;
        }
    }

    gimbal_1 = clamp(gimbal_1, GARY_MIN, GARY_MAX);

    if (keys_since_last_w >= 5 && throttle >= 0)
        throttle--;

    if (keys_since_last_steer >= 3) {
        // move steer towards the middle
        if (steer < SAMMY_CENTER)
            steer++;
        else
            steer--;
    }

    throttle = clamp(throttle, 0, 100);

    sammy.write(steer);
    danny.write(map(throttle, 0, 100, DANNY_MIN, DANNY_MAX));
    gary.write(gimbal_1);
}

int clamp(int value, int low, int high) {
    return max(low, min(value, high));
}