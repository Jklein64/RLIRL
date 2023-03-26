#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

RF24 radio(9, 8);
const byte address[10] = "ADDRESS01";

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("Wheee!");

    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

void loop() {
    if (Serial.available() > 0) {
        const char input = Serial.read();
        radio.write(&input, sizeof(input));
        if (input != '*') {
            Serial.print("Sent ");
            Serial.println(input);
        }
    }
}