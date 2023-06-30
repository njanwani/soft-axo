#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {

private:
    float *values;
    uint16_t elements;  // yes it is bounded but it is large enough for us
    char *key;
    uint16_t idx; // current idx added
    size_t MAX_KEY_LENGTH;
public:
    Logger(int elements, size_t MAX_KEY_LENGTH);
    void begin();
    void add_key(const char *keyname);
    void log(const char *keyname, float value);
    void print_headers();
    void write_to_SD();
};

#endif
