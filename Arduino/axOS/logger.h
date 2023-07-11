#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SD.h>

class Logger {

private:
    float *values;
    uint16_t elements;  // yes it is bounded but it is large enough for us
    char *key;
    uint16_t idx; // current idx added
    size_t MAX_KEY_LENGTH;
    size_t CHIP_SELECT;
    size_t BAUD;
    File datafile;
    const char *filename;
public:
    Logger(size_t elements, size_t MAX_KEY_LENGTH, size_t CHIP_SELECT, size_t BAUD);
    void begin(const char *filename);
    void add_key(const char *keyname);
    void log(const char *keyname, float value);
    void print_headers();
    void write_to_SD();
    void shutdown();
};

#endif
