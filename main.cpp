#include "main.h"
#include "screen.h"
#include "bluetooth.h"
#include "rotary_encoder.h"
#include "buttons.h"

const int next_button = 12;
const int previous_button = 14;

char song_name[64];
char artist[64];

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

void setup(void) {
    Serial.begin(9600);
    Serial.print(F("Hello! ST77xx TFT Test"));

    pinMode(previous_button, INPUT_PULLUP);
    pinMode(next_button, INPUT_PULLUP);

    setupRotaryEncoder();
    setupI2S();
    //setupScreen();

    tft.init();
    tft.fillScreen(TFT_BLUE);
    tft.setRotation(1);
    tft.setSwapBytes(true); 
}

void loop() {
    //Serial.println(esp_get_minimum_free_heap_size());
    //drawScreen();
    readButtons();
    rotaryLoop();
    loopBluetooth();
    delay(32);
}
