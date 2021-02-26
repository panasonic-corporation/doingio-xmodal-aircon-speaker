#pragma once
#include "Config.h"
#include <AudioFileSourceSD.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <driver/i2s.h>

#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define CONFIG_I2S_DATA_IN_PIN  23
#define SPEAKER_I2S_NUMBER      I2S_NUM_0

class SoundPlayer{
private:

public:
    static void playMP3(char *filename, float gain);
    static void playMP3(String filename, float gain);
    static void playLogo();
    static bool loop();
    static void stop();
    static void close();
    static void setGain(float gain);
};