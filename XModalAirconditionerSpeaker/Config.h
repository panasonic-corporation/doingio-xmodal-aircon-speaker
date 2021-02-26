#pragma once

#define IR_PIN  33

#define FONT_PATH       "/fonts/genshin-regular-20"  // フォント名(.vlwは省略)
#define LOGO_IMAGE_PATH "/images/logo.jpg"           // ロゴ画像
#define HEAT_SOUND_DIR  "/sounds/heat"
#define COOL_SOUND_DIR  "/sounds/cool"

#define HEAT_COLOR ORANGE
#define COOL_COLOR 0x03FF

#define STATUS_COOL   0    // 冷房
#define STATUS_IDLE   1    // アイドリング
#define STATUS_HEAT   2    // 暖房
#define STATUS_AUTO   3    // 自動
#define STATUS_FAN    4    // 送風
#define STATUS_DRY    5    // 除湿