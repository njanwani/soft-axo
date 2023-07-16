#include "logger.h"
#include <SD.h>

Logger::Logger(size_t elements, size_t MAX_KEY_LENGTH, size_t CHIP_SELECT, size_t BAUD) {
    // Serial.begin(9600);
    Logger::elements = elements;
    Logger::MAX_KEY_LENGTH = MAX_KEY_LENGTH;
    Logger::CHIP_SELECT = CHIP_SELECT;
    Logger::BAUD = BAUD;
}


void Logger::begin(const char *filename) {
    Logger::values = (float *)calloc(Logger::elements, sizeof(float));
    Logger::key = (char *)calloc(Logger::elements, MAX_KEY_LENGTH * sizeof(char));
    Serial.begin(Logger::BAUD);
    while (!SD.begin(Logger::CHIP_SELECT)) {
        Serial.println("Card failed, or not present. Trying again...");
        delay(1000);
    }
    Logger::filename = filename;
    Serial.printf("Card initialized. Deleting contents and writing to %s\n", Logger::filename);
    if (SD.exists(Logger::filename)) {
        SD.remove(Logger::filename);
    }
}


void Logger::add_key(const char *keyname) {
    if (Logger::idx < Logger::elements) {
        strcpy(Logger::key + Logger::idx * Logger::MAX_KEY_LENGTH, keyname);
        // Serial.printf("key = %s\n", (char *)(Logger::key + Logger::idx * Logger::MAX_KEY_LENGTH));
        Logger::idx += 1;
    }
    
}


void Logger::log(const char *keyname, float value) {
    for (int i = 0; i < Logger::idx; i++) {
        if (strcmp(keyname, Logger::key + i * Logger::MAX_KEY_LENGTH) == 0) {
            Logger::values[i] = value;
            break;
        }
    }
}


void Logger::print_headers() {
    Logger::datafile = SD.open(Logger::filename, FILE_WRITE);
    if (Logger::datafile) {
        for (uint16_t i = 0; i < Logger::idx; i++) {
            Logger::datafile.printf("%s,", Logger::key + i * Logger::MAX_KEY_LENGTH);
        }
        Logger::datafile.println();
        Logger::datafile.close();
    } else {
        // if the file isn't open, pop up an error:
        Serial.println("LOGGER ERROR: cannot write to file");
    }
}


void Logger::write_to_SD() {
    Logger::datafile = SD.open(Logger::filename, FILE_WRITE);
    if (Logger::datafile) {
        for (uint16_t i = 0; i < Logger::idx; i++) {
            Logger::datafile.printf("%.2f,", Logger::values[i]);
        }
        Logger::datafile.println();
        Logger::datafile.close();
    } else {
        // if the file isn't open, pop up an error:
        Serial.println("LOGGER ERROR: cannot write to file");
    }
}


void Logger::shutdown() {
    Logger::datafile.close();
}