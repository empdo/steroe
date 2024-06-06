#include "rotary_encoder.h"
#include "main.h"
#include "bluetooth.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(CLK_PIN, DT_PIN, SW_PIN, -1, ROTARY_ENCODER_STEPS);

int volume = 0;

void rotary_onButtonClick() {
    static unsigned long lastTimePressed = 0;
    if (millis() - lastTimePressed < 200) {
        return;
    }
    lastTimePressed = millis();
    
    if (is_playing) {
        a2dp_sink.pause();
    } else {
        a2dp_sink.play();
    }
}

void rotaryLoop() {
    if (rotaryEncoder.encoderChanged()) {
        volume = rotaryEncoder.readEncoder();
        a2dp_sink.set_volume(127 - volume);
        Serial.printf("Volume: %d\n", volume);
    }
    if (rotaryEncoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
    }
}

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void setupRotaryEncoder() {
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    bool circleValues = false;
    rotaryEncoder.setBoundaries(0, 127, circleValues);
}
