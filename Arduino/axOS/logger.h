#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SD.h>

#define SD_FAT_TYPE 3
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else 
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif 
#define SPI_CLOCK SD_SCK_MHZ(50)
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#define FILE_SIZE 5000000 // 5Mb

class Logger {

private:
    float *values;
    uint16_t elements;  // yes it is bounded but it is large enough for us
    char *key;
    uint16_t idx; // current idx added
    size_t MAX_KEY_LENGTH;
    size_t CHIP_SELECT;
    size_t BAUD;
    FsFile file;
    SdFs sd;
    const char *filename;
public:
    Logger(size_t elements, size_t MAX_KEY_LENGTH, size_t CHIP_SELECT, size_t BAUD);
    bool begin(const char *filename);
    void add_key(const char *keyname);
    void log(const char *keyname, float value);
    void print_headers();
    void write_to_SD();
    void shutdown();
};

#endif
