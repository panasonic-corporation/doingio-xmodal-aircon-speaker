#include <M5Core2.h>
#include <IRrecv.h>
#include <IRac.h>
#include <vector>
#include "SoundPlayer.h"
#include "EEPROMController.h"
#include "Config.h"
#include "UIController.h"

using namespace std;

vector<String> heat_sound_files;
vector<String> cool_sound_files;

int command_status = STATUS_IDLE;
int last_command_status = STATUS_IDLE;

bool playing = false;
bool last_playing = false;

String playing_filename;
float gain;

bool btn_status = false;
bool last_btn_status = false;

bool stop_flag = false;
bool is_mute = false;

// IR Settings
const uint16_t kRecvPin = IR_PIN;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
const uint16_t kMinUnknownSize = 12;
const uint8_t kTolerancePercentage = kTolerance;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results;

// ランダム再生
void playRandom() {
    if (playing) {
        SoundPlayer::stop();
        playing = false;
    }
    float gain;
    if (is_mute) {
        gain = 0;
    } else {
        EEPROMController::read(&gain);
    }
    switch (command_status) {
        case STATUS_HEAT:
            playing_filename = heat_sound_files[random(0, heat_sound_files.size())];
            SoundPlayer::playMP3(playing_filename, gain);
            playing = true;
            Serial.printf("play : %s\n", playing_filename.c_str());
            break;
        case STATUS_COOL:
            playing_filename = cool_sound_files[random(0, cool_sound_files.size())];
            SoundPlayer::playMP3(playing_filename, gain);
            playing = true;
            Serial.printf("play : %s\n", playing_filename.c_str());
            break;
        default:
            playing_filename = "";
            break;
    }
}

// dir_nameからファイル名一覧を取得
vector<String> getFileList(const char* dir_name) {
    vector<String> list;
    File sounds_file;
    sounds_file = SD.open(dir_name);
    while (true) {
        File entry =  sounds_file.openNextFile();
        if (!entry) {
            break;
        }
        const char* name = entry.name();
        if (String(name).indexOf(String(dir_name) + "/.") == -1) {
            list.push_back(String(entry.name()));
        }
    }
    sounds_file.close();
    return list;
}

// ディスプレイタッチ状態検出
void getTouchStatus() {
    TouchPoint_t pos = M5.Touch.getPressPoint();

    // ボリュームバーのタッチ判定
    if (pos.y > 200) {
        if (stop_flag) {
            stop_flag = false;
            UIController::updateButtonUI(command_status, stop_flag);
        }
        if (playing) {
            gain = (float)(M5.Lcd.height() - pos.x) / (float)M5.Lcd.height();
            Serial.printf("gain : %f\n", gain);
            SoundPlayer::setGain(gain);
            EEPROMController::write(gain);
            UIController::updateGainUI(command_status, gain);
        }
    }

    // ミュートボタンのタッチ判定
    if (pos.y > (M5.Lcd.width() - btn_width) * 0.5
        && pos.y < (M5.Lcd.width() - btn_width) * 0.5 + btn_width
        && pos.x > btn_posy
        && pos.x < btn_posy + btn_height) {
            btn_status = true;
    } else {
        btn_status = false;
    }
    if (btn_status && !last_btn_status && playing) {
        stop_flag = !stop_flag;
        if (stop_flag) {
            is_mute = true;
            SoundPlayer::setGain(0);
        } else {
            is_mute = false;
            SoundPlayer::setGain(gain);
        }
        UIController::updateButtonUI(command_status, stop_flag);
    }
    last_btn_status = btn_status;
}

// 赤外線受信詳細をシリアルに表示
void printIRResults(decode_results results) {
    Serial.println("=========receive=========");
    Serial.printf("address : %x\n", results.address);
    Serial.printf("value   : %x\n", results.value);
    Serial.printf("command : %x\n", results.command);
    Serial.printf("overflow: %d\n", results.overflow);
    Serial.printf("repeat  : %d\n", results.repeat);
    Serial.println("=========receive=========");
}

void setup() {
    M5.begin();
    M5.Axp.SetSpkEnable(true);
    delay(50);

    // Lcd設定
    M5.Lcd.setRotation(0);
    M5.Lcd.setTextWrap(true, true);
    M5.Lcd.clear();

    // EEPROMからGain取得
    EEPROMController::read(&gain);
    if (gain < 0 || gain > 1) {
        gain = 0.5;
        EEPROMController::write(gain);
    }
    Serial.printf("gain : %f\n", gain);

    // mp3リスト取得
    heat_sound_files  = getFileList(HEAT_SOUND_DIR);
    cool_sound_files = getFileList(COOL_SOUND_DIR);
    Serial.println("Sound HEAT");
    for (int i = 0; i < heat_sound_files.size(); i++) {
        Serial.println(heat_sound_files[i]);
    }
    Serial.println();
    Serial.println("Sound COOL");
    for (int i = 0; i < cool_sound_files.size(); i++) {
        Serial.println(cool_sound_files[i]);
    }
    delay(100);

    // ロゴ表示
    M5.Lcd.drawJpgFile(SD, LOGO_IMAGE_PATH);
    M5.Lcd.loadFont(FONT_PATH, SD);
    delay(1000);

    // UI表示
    UIController::updateMainUI(command_status);

    // 赤外線受信スタート
    irrecv.setTolerance(kTolerancePercentage);
    irrecv.enableIRIn();
}

void loop() {
    // スクリーンタッチ位置取得
    getTouchStatus();

    // 赤外線受信
    if (irrecv.decode(&results)) {
        printIRResults(results);
        IRPanasonicAc ac(kGpioUnused);
        ac.setRaw(results.state);
        if (ac.getPower()) {
            switch (ac.getMode()) {
                case kPanasonicAcFan:
                    command_status = STATUS_FAN;
                    Serial.println("送風");
                    break;
                case kPanasonicAcAuto:
                    command_status = STATUS_AUTO;
                    Serial.println("自動");
                    break;
                case kPanasonicAcCool:
                    command_status = STATUS_COOL;
                    Serial.println("冷房");
                    break;
                case kPanasonicAcHeat:
                    command_status = STATUS_HEAT;
                    Serial.println("暖房");
                    break;
                case kPanasonicAcDry:
                    command_status = STATUS_DRY;
                    Serial.println("除湿");
                    break;
                default:
                    break;
            }
        } else {
            Serial.println("停止");
            command_status = STATUS_IDLE;
        }
    }

    if (playing) {
        playing = SoundPlayer::loop();
    }

    if (last_command_status != command_status) {
        // 状態が変化したときはランダムに再生
        if (!stop_flag) {
            playRandom();
        }
        UIController::updateMainUI(command_status);
        UIController::updateGainUI(command_status, gain);
        UIController::updateButtonUI(command_status, stop_flag);
        Serial.println();
    } else if (!playing && last_playing) {
        // 最後まで再生しきったとき
        Serial.println("end of file");
        SoundPlayer::close();
        playRandom();
        Serial.println();
    }

    last_playing = playing;
    last_command_status = command_status;
}