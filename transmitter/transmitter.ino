boolean blinking = false;
boolean is_high = false;
unsigned long last_blink = millis();

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
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