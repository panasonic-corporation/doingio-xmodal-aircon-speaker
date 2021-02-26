#include <EEPROM.h>

#define EEPROM_SIZE 4 // float = 4byte

class EEPROMController{
private:
    static bool isinit;
    static void init();

public:
    static void write(int size, int* data);
    static void write(int data);
    static void write(int size, float* data);
    static void write(float data);
    static void read(int size, int* data);
    static void read(int* data);
    static void read(int size, float* data);
    static void read(float* data);
};