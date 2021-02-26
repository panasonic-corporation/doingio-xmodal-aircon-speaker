#include "EEPROMController.h"
#include "Config.h"

bool EEPROMController::isinit = false;

void EEPROMController::init() {
    isinit = true;
    EEPROM.begin(EEPROM_SIZE);
}

void EEPROMController::write(int size, int* data) {
    if (!isinit) {
        init();
    }
    int n = 0;
    for (int i = 0; i < size; i++) {
        EEPROM.put(n, data[i]);
        n += sizeof(int);
    }
    EEPROM.commit();
}

void EEPROMController::write(int data) {
    int _data[1] = {data};
    write(1, _data);
}

void EEPROMController::write(int size, float* data) {
    if (!isinit) {
        init();
    }
    int n = 0;
    for (int i = 0; i < size; i++) {
        EEPROM.put(n, data[i]);
        n += sizeof(float);
    }
    EEPROM.commit();
}

void EEPROMController::write(float data) {
    float _data[1] = {data};
    write(1, _data);
}

void EEPROMController::read(int size, int* data) {
    if (!isinit) {
        init();
    }
    int n = 0;
    for (int i = 0; i < size; i++) {
        EEPROM.get(n, data[i]);
        n += sizeof(int);
    }
}

void EEPROMController::read(int* data) {
    int _data[1];
    read(1, _data);
    *data = _data[0];
}

void EEPROMController::read(int size, float* data) {
    if (!isinit) {
        init();
    }
    int n = 0;
    for (int i = 0; i < size; i++) {
        EEPROM.get(n, data[i]);
        n += sizeof(float);
    }
}

void EEPROMController::read(float* data) {
    float _data[1];
    read(1, _data);
    *data = _data[0];
}