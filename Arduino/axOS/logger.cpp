#include "logger.h"
#include "SdFat.h"
#include "sdios.h"
#include "FreeStack.h"

// Set PRE_ALLOCATE true to pre-allocate file clusters.
const bool PRE_ALLOCATE = true;

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
    if (!Logger::sd.begin(SD_CONFIG)) {
        Logger::sd.initErrorHalt(&Serial);
        return;
    }

    Logger::filename = filename;
    Serial.printf("Card initialized. Writing to %s\n", Logger::filename);
    Logger::sd.remove(filename);
    if (!Logger::file.open(Logger::filename, O_RDWR | O_CREAT | O_TRUNC)) {
        error("open failed");
        return;
    }
    // Logger::file.truncate(0);
    if (!Logger::file.preAllocate(FILE_SIZE)) {
        error("preAllocate failed");
        return;
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
    // Logger::datafile = SD.open(Logger::filename, FILE_WRITE);
    char buf[1000];
    size_t c = 0;
    for (uint16_t i = 0; i < Logger::idx; i++) {
        c += snprintf(buf + c, 100, "%s,", Logger::key + i * Logger::MAX_KEY_LENGTH);
    }
    c += snprintf(buf + c, 2, "\n");
    if (file.write(buf, c) != c) {
        error("headers: write failed");
    }
}


void Logger::write_to_SD() {
    // Logger::datafile = SD.open(Logger::filename, FILE_WRITE);
    // if (Logger::datafile) {
    //     for (uint16_t i = 0; i < Logger::idx; i++) {
    //         Logger::datafile.printf("%.2f,", Logger::values[i]);
    //     }
    //     Logger::datafile.println();
    //     Logger::datafile.close();
    // } else {
    //     // if the file isn't open, pop up an error:
    //     Serial.println("LOGGER ERROR: cannot write to file");
    // }
    char buf[1000];
    size_t c = 0;
    for (uint16_t i = 0; i < Logger::idx; i++) {
        c += snprintf(buf + c, 100, "%.2f,", Logger::values[i]);
    }
    c += snprintf(buf + c, 2, "\n");
    if (file.write(buf, c) != c) {
        error("values: write failed");
    }
}


void Logger::shutdown() {
    Logger::file.truncate();
    Logger::file.close();
}