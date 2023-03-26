#include <NewPing.h>
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

RF24 radio(9, 8);
const byte address[10] = "ADDRESS01";

NewPing sonar(UMA_TRIG_PIN, UMA_ECHO_PIN, 6);

void setup() {
    Serial.begin(9600);

    gary.attach(GARY_PIN);
    sammy.attach(SAMMY_PIN);
    danny.attach(DANNY_PIN, 1000, 2000);
    madha.attach(MADHA_PIN, 1000, 2000);
    danny.write(0);
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
    delay(50);  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    Serial.print("Ping: ");
    Serial.print(sonar.ping_cm());  // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");

    // listen for key from transmitter
    if (radio.available()) {
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
        throttle -= 10;

    if (keys_since_last_steer >= 3) {
        // fix spazzing from increments
        if (abs(steer - SAMMY_CENTER) <= 20)
            steer = SAMMY_CENTER;
        // move steer towards the middle
        if (steer < SAMMY_CENTER)
            steer += 10;
        else
            steer -= 10;
    }

    throttle = clamp(throttle, 0, 100);

    sammy.write(steer);
    danny.write(map(throttle, 0, 100, DANNY_MIN, DANNY_MAX));
    gary.write(gimbal_1);
}

int clamp(int value, int low, int high) {
    return max(low, min(value, high));
}