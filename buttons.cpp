#include "buttons.h"
#include <Arduino.h>
#include "bluetooth.h"

#include "main.h"
#include "config.h"

void readButtons() {
    int next_button_state = digitalRead(next_button);

    if (next_button_state == LOW) {
        printf("next_button: %d, previous_button: %d\n", digitalRead(next_button), digitalRead(previous_button));
        a2dp_sink.next();
    }

    int previous_button_state = digitalRead(previous_button);

    if (previous_button_state == LOW) {
        printf("next_button: %d, previous_button: %d\n", digitalRead(next_button), digitalRead(previous_button));
        a2dp_sink.previous();
    }
}
