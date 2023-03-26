#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>

RF24 radio(9, 8);
char input;

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
    const char text[] = "Hello world!";
    radio.write(&text, sizeof(text));
    Serial.println("wrote!");
    delay(1000);
    // if (Serial.available() > 0)
    //     input = Serial.read();
}