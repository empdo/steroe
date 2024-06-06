#include "bluetooth.h"
#include "main.h"
#include "AudioLibs/AudioEspressifFFT.h"

bool is_playing = false;
uint32_t playtime = 0;

uint16_t sample_rate=44100;
uint8_t channels = 2;
I2SStream in;
BluetoothA2DPSink a2dp_sink(in);
AudioEspressifFFT fft;
StreamCopy copier(fft, in);

String labels[] = {"125", "250", "500", "1K", "2K", "4K", "8K", "16K"};
int bands[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void avrc_rn_play_pos_callback(uint32_t play_pos) {
    playtime = play_pos;
}

void mapToBands(int frequency, float magnitude) {
    if (frequency < 250) {
        bands[0] += magnitude;
    } else if (frequency < 500) {
        bands[1] += magnitude;
    } else if (frequency < 1000) {
        bands[2] += magnitude;
    } else if (frequency < 2000) {
        bands[3] += magnitude;
    } else if (frequency < 4000) {
        bands[4] += magnitude;
    } else if (frequency < 8000) {
        bands[5] += magnitude;
    } else if (frequency < 16000) {
        bands[6] += magnitude;
    } else {
        bands[7] += magnitude;
    }
}

void fftResult(AudioFFTBase &fft) {
float diff;
    auto _result = fft.result();
    if (_result.magnitude>100){
        Serial.print(_result.frequency);
        Serial.print(" ");
        Serial.print(_result.magnitude);  
        Serial.print(" => ");
        Serial.print(_result.frequencyAsNote(diff));
        Serial.print( " diff: ");
        Serial.println(diff);
    }
     const int num_bins = 86; // Number of frequency bins your FFT library provides
     AudioFFTResult result[num_bins];
     fft.resultArray(result);

     for (int i = 0; i < num_bins; i++) {
         float magnitude = result[i].magnitude;
         int frequency = result[i].frequencyAsInt();
         mapToBands(frequency, magnitude);
     }

     for (int i = 0; i < 8; i++) {
         Serial.printf("%s: %d\n", labels[i].c_str(), bands[i]);
     }
}

void avrc_rn_playstatus_callback(esp_avrc_playback_stat_t playback) {
    switch (playback) {
        case ESP_AVRC_PLAYBACK_STOPPED:
            Serial.println("Stopped.");
            is_playing = false;
            break;
        case ESP_AVRC_PLAYBACK_PLAYING:
            is_playing = true;
            Serial.println("Playing.");
            break;
        case ESP_AVRC_PLAYBACK_PAUSED:
            is_playing = false;
            Serial.println("Paused.");
            break;
        case ESP_AVRC_PLAYBACK_FWD_SEEK:
            Serial.println("Forward seek.");
            break;
        case ESP_AVRC_PLAYBACK_REV_SEEK:
            Serial.println("Reverse seek.");
            break;
        case ESP_AVRC_PLAYBACK_ERROR:
            Serial.println("Error.");
            break;
        default:
            Serial.printf("Got unknown playback status %d\n", playback);
    }
}

void avrc_metadata_callback(uint8_t id, const uint8_t *text) {
    Serial.printf("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
    if (id == ESP_AVRC_MD_ATTR_TITLE) {
        strncpy((char *)song_name, (char *)text, 63);
    } else if (id == ESP_AVRC_MD_ATTR_ARTIST) {
        strncpy((char *)artist, (char *)text, 63);
    }
}

void setupI2S() {
    auto cfg = in.defaultConfig(RXTX_MODE);
    cfg.sample_rate = sample_rate;
    cfg.channels = 2;
    cfg.bits_per_sample = 16;
    cfg.i2s_format = I2S_STD_FORMAT;
    cfg.is_master = true;
    cfg.port_no = 0;
    cfg.pin_bck = GPIO_NUM_26;
    cfg.pin_ws = GPIO_NUM_25;
    cfg.pin_data = GPIO_NUM_22;
    cfg.pin_mck = 0;

    if (!in.begin(cfg)) {
        Serial.println("Failed to initialize I2S input stream");
        return;
    }

    auto tcfg = fft.defaultConfig();
    tcfg.length = 512;
    tcfg.channels = channels;
    tcfg.sample_rate = sample_rate;
    tcfg.bits_per_sample = 16;
    tcfg.callback = &fftResult;
    fft.begin(tcfg);

    setupBluetooth();
}

void setupBluetooth() {
    a2dp_sink.set_channels(I2S_CHANNEL_STEREO);
    a2dp_sink.set_auto_reconnect(true, 1000);
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    a2dp_sink.set_avrc_rn_playstatus_callback(avrc_rn_playstatus_callback);
    a2dp_sink.set_avrc_rn_play_pos_callback(avrc_rn_play_pos_callback);
    a2dp_sink.start("InternalDac");
    a2dp_sink.set_volume(50);
}

void loopBluetooth() {
   copier.copy();
}