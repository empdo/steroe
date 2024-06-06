#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "AiEsp32RotaryEncoder.h"
#include "config.h"

extern AiEsp32RotaryEncoder rotaryEncoder;
extern int volume;

void setupRotaryEncoder();
void rotaryLoop();
void IRAM_ATTR readEncoderISR();

#endif // ROTARY_ENCODER_H
