#include "Config.h"
#include "SoundPlayer.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

// MP3再生
void SoundPlayer::playMP3(char *filename, float gain) {
    file = new AudioFileSourceSD(filename);
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 0);
    out->SetPinout(12, 0, 2);
    out->SetOutputModeMono(true);
    out->SetGain(gain);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
}

void SoundPlayer::playMP3(String filename, float gain) {
    char str[64];
    filename.toCharArray(str, 64);
    playMP3(str, gain);
}

bool SoundPlayer::loop() {
    if (mp3->isRunning()) {
        return mp3->loop();
    }
    return false;
}

void SoundPlayer::stop() {
    mp3->stop();
    close();
}

void SoundPlayer::close() {
    file->close();
    delete mp3;
    delete file;
    delete out;
    delete id3;
}

void SoundPlayer::setGain(float gain) {
    out->SetGain(gain);
}
