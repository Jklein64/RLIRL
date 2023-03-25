# see https://www.delftstack.com/howto/python/python-detect-keypress/

import keyboard
from time import sleep

KEYS = { "w", "a", "s", "d", "q", "e", "space" }
DEVICE = "/dev/tty.usbserial-110"

serial = open(DEVICE, "w")

# store last timestamp
is_pressed = {
    key: False for key in KEYS
}

def pressed():
    for key, pressed in is_pressed.items():
        if pressed:
            if key == "space":
                yield " "
            else:
                yield key

while True:
    for key in KEYS:
        is_pressed[key] = keyboard.is_pressed(key)

    for key in pressed():
        serial.write(key)
        serial.flush()

    # small delay so we don't totally spam
    sleep(0.1)