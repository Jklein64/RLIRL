# see https://www.delftstack.com/howto/python/python-detect-keypress/

import keyboard
from time import sleep

KEYS = { "w", "a", "s", "d", "q", "e", "p", "o", "space" }
DEVICE = "/dev/tty.usbserial-210"

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
    wrote = False

    for key in KEYS:
        is_pressed[key] = keyboard.is_pressed(key)

    for key in pressed():
        wrote = True
        serial.write(key)
        serial.flush()

    if not wrote:
        # write a stupid char
        serial.write('*')
        serial.flush()

    # small delay so we don't totally spam
    sleep(0.1)
