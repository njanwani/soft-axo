#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"


// #define VERBOSE_MODE                   false  // If set to 'true' enables debug output
// #define BLUEFRUIT_HWSERIAL_NAME      Serial8
// #define BLUEFRUIT_UART_MODE_PIN        33    // Set to -1 if unused
// #define FACTORYRESET_ENABLE         1

class Bluetooth {

private:
    Adafruit_BluefruitLE_UART ble;
    size_t mode_pin;
    bool verbose;
    bool factory_reset;
public:
    Bluetooth(HardwareSerial *port, bool verbose, int mode_pin, bool factory_reset);
    bool begin(size_t baud);
    void write(const char *c, int len);
    char read();
    bool available();
    void print_helper(const char *str);
};

#endif
