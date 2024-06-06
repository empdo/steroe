#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "AudioTools.h"
#include "BluetoothA2DPSink.h"
#include "config.h"

extern BluetoothA2DPSink a2dp_sink;
extern bool is_playing;
extern uint32_t playtime;

void setupBluetooth();
void setupI2S();
void loopBluetooth();

#endif // BLUETOOTH_H
