# see https://www.delftstack.com/howto/python/python-detect-keypress/

import keyboard
from time import perf_counter

def millis():
    return perf_counter() * 1000

# wait 10ms before counting again
DEBOUNCE_DURATION = 10
KEYS = { "w", "a", "s", "d", "q", "e", "space" }

# store last timestamp
held = {
    key: False for key in KEYS
}


while True:
    for key in KEYS:
        held[key] = keyboard.is_pressed(key)
    # event = keyboard.read_event()
    # if event.name in KEYS:
    #     if event.event_type == keyboard.KEY_DOWN:
    #         held[event.name] = True
    #     elif event.event_type == keyboard.KEY_UP:
    #         held[event.name] = False
    print([key for (key, b) in held.items() if b])
awd
    # for key in KEYS:
    #     if keyboard.read_key() == key:
    #         if millis() - debounce[key] > DEBOUNCE_DURATION:
    #             print(f"pressed {key}!")
    #             debounce[key] = millis()