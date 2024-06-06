#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <TFT_eSPI.h>

// Pin definitions
#define CLK_PIN 35
#define DT_PIN 32
#define SW_PIN 34

// Rotary encoder steps
extern const int next_button;
extern const int previous_button;

// Global variables for song metadata
extern char song_name[64];  // Assuming maximum song name length is 63
extern char artist[64];     // Assuming maximum artist name length is 63

// TFT and Sprite objects
extern TFT_eSPI tft;
extern TFT_eSprite spr;

void setup();
void loop();

#endif // MAIN_H
