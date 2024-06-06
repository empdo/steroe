#include "screen.h"
#include "main.h"
#include "config.h"

int scroll_position = 0;
int song_name_offset = 0;
int scroll_speed = 0;
int song_name_length = 0;
int song_name_width = 0;

void setupScreen() {
    spr.createSprite(WIDTH, 120);
    spr.setColorDepth(2);
    spr.setTextColor(TFT_WHITE, TFT_BLACK);
    spr.setTextDatum(MC_DATUM);
    // Assuming song_name and artist are set somewhere before
}

void drawScreen() {
    spr.fillSprite(TFT_BLACK);  // Clear the sprite instead of deleting it

    song_name_length = strlen(song_name);
    song_name_width = tft.textWidth(song_name, 4);

    if (song_name_length > 0 && song_name_width > WIDTH) {
        if (song_name_offset >= tft.textWidth(song_name, 4)) {
            char first_char = song_name[0];
            memmove(song_name, song_name + 1, song_name_length - 1);
            song_name[song_name_length - 1] = first_char;
            song_name[song_name_length] = '\0';
            song_name_offset = 0;
        } else {
            song_name_offset += 5;
        }
    }

    Serial.printf("Song: %s, Artist: %s\n", song_name, artist);
    spr.drawString(song_name, WIDTH / 2 - song_name_offset, 40, 4);
    spr.drawString(artist, WIDTH / 2, 80, 4);
    spr.pushSprite(tft.width() / 2 - WIDTH / 2, 0);
}
