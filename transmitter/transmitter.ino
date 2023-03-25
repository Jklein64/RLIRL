boolean blinking = true;
boolean is_high = false;
unsigned long last_blink = millis();

char input = 0;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    delay(5000);
    Serial.println("Wheee!");
}

void loop() {
    if (Serial.available() > 0)
        input = Serial.read();
    else
        input = 0;

    blinking = input != 0;

    if (blinking) {
        if (millis() - last_blink > 500) {
            is_high = !is_high;
            last_blink = millis();
        }

        digitalWrite(LED_BUILTIN, is_high ? HIGH : LOW);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
    }
}