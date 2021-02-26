#include <M5Core2.h>
#include "UIController.h"
#include "Config.h"

void UIController::updateMainUI(int command_status) {
    M5.Lcd.clear();
    switch (command_status) {
        case STATUS_FAN:
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setTextColor(BLACK, WHITE);
            M5.Lcd.drawString("送風", status_txt_posx, status_txt_posy);
            break;
        case STATUS_AUTO:
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setTextColor(BLACK, WHITE);
            M5.Lcd.drawString("自動", status_txt_posx, status_txt_posy);
            break;
        case STATUS_COOL:
            M5.Lcd.fillScreen(COOL_COLOR);
            M5.Lcd.setTextColor(WHITE, COOL_COLOR);
            M5.Lcd.drawString("冷房", status_txt_posx, status_txt_posy);
            break;
        case STATUS_HEAT:
            M5.Lcd.fillScreen(HEAT_COLOR);
            M5.Lcd.setTextColor(WHITE, HEAT_COLOR);
            M5.Lcd.drawString("暖房", status_txt_posx, status_txt_posy);
            break;
        case STATUS_DRY:
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setTextColor(BLACK, WHITE);
            M5.Lcd.drawString("除湿", status_txt_posx, status_txt_posy);
            break;
        case STATUS_IDLE:
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.setTextColor(WHITE, BLACK);
            M5.Lcd.drawString("停止", status_txt_posx, status_txt_posy);
            break;
        default:
            M5.Lcd.fillScreen(WHITE);
            M5.Lcd.setTextColor(BLACK, WHITE);
            break;
    }
    // draw button
    M5.Lcd.drawRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, WHITE);
}

void UIController::updateGainUI(int command_status, float gain) {
    switch (command_status) {
        case STATUS_HEAT:
            M5.Lcd.fillRect(0, 0, 10, M5.lcd.height(), HEAT_COLOR);
            M5.Lcd.fillRect(0, M5.Lcd.height() * (1 - gain), 10, M5.Lcd.height(), WHITE);
            break;
        case STATUS_COOL:
            M5.Lcd.fillRect(0, 0, 10, M5.lcd.height(), COOL_COLOR);
            M5.Lcd.fillRect(0, M5.Lcd.height() * (1 - gain), 10, M5.Lcd.height(), WHITE);
            break;
        default:
            break;
    }
}

void UIController::updateButtonUI(int command_status, bool stop_flag) {
    switch (command_status) {
        case STATUS_HEAT:
            if (!stop_flag) {
                M5.Lcd.setTextColor(HEAT_COLOR, WHITE);
                M5.Lcd.fillRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, WHITE);
                M5.Lcd.drawString("PLAYING", 78, btn_posy + 30, WHITE);
            } else {
                M5.Lcd.setTextColor(WHITE, HEAT_COLOR);
                M5.Lcd.fillRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, HEAT_COLOR);
                M5.Lcd.drawRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, WHITE);
                M5.Lcd.drawString("MUTE", 92, btn_posy + 30, HEAT_COLOR);
            }
            break;
        case STATUS_COOL:
            if (!stop_flag) {
                M5.Lcd.setTextColor(COOL_COLOR, WHITE);
                M5.Lcd.fillRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, WHITE);
                M5.Lcd.drawString("PLAYING", 78, btn_posy + 30, WHITE);
            } else {
                M5.Lcd.setTextColor(WHITE, COOL_COLOR);
                M5.Lcd.fillRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, COOL_COLOR);
                M5.Lcd.drawRect((M5.Lcd.width() - btn_width) * 0.5, btn_posy, btn_width, btn_height, WHITE);
                M5.Lcd.drawString("MUTE", 92, btn_posy + 30, COOL_COLOR);
            }
            break;
        default:
            break;
    }
}

