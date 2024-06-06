#ifndef SCREEN_H
#define SCREEN_H

#include <TFT_eSPI.h>

extern TFT_eSprite spr;
extern TFT_eSPI tft;

void drawScreen();
void setupScreen();

#endif // SCREEN_H
