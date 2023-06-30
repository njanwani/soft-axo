#include "logger.h"

Logger::Logger(int elements, size_t MAX_KEY_LENGTH) {
    // Serial.begin(9600);
    Logger::elements = elements;
    Logger::MAX_KEY_LENGTH = MAX_KEY_LENGTH;
}


void Logger::begin() {
    Logger::values = (float *)calloc(Logger::elements, sizeof(float));
    Logger::key = (char *)calloc(Logger::elements, MAX_KEY_LENGTH * sizeof(char));
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
            Serial.println();
            break;
        }
    }
}


void Logger::print_headers() {
    for (uint16_t i = 0; i < Logger::idx; i++) {
        Serial.printf("%s,", Logger::key + i * Logger::MAX_KEY_LENGTH);
    }
}


void Logger::write_to_SD() {
    for (uint16_t i = 0; i < Logger::idx; i++) {
        Serial.printf("%f,", Logger::values[i]);
    }
    Serial.println();
}


// void Logger::shutdown() {

// }