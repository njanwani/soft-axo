#include "bluetooth.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

Bluetooth::Bluetooth(HardwareSerial *port,
                      bool verbose,
                      int mode_pin,
                      bool factory_reset) : ble(*port, 5) {
    Bluetooth::mode_pin = mode_pin;
    Bluetooth::verbose = verbose;
    Bluetooth::factory_reset = factory_reset;
}

void Bluetooth::print_helper(const char *str) {
    if (Bluetooth::verbose) {
        Serial.println(str);
    }
}

bool Bluetooth::begin(size_t baud) {
    pinMode(Bluetooth::mode_pin, OUTPUT);
    digitalWrite(Bluetooth::mode_pin, HIGH);
    delay(100);
  
//    Serial.begin(baud);
    print_helper("Adafruit Bluefruit AXO");
    print_helper("---------------------------------------");

    /* Initialise the module */
    print_helper("Initialising the Bluefruit LE module: ");

    if (!Bluetooth::ble.begin(Bluetooth::verbose)) {
        Serial.println("BLUETOOTH: Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring.");
        return false;
    }
    print_helper("OK!");

    if (Bluetooth::factory_reset) {
        /* Perform a factory reset to make sure everything is in a known state */
        print_helper("Performing a factory reset: ");
        if (!Bluetooth::ble.factoryReset()) {
            Serial.println("BLUETOOTH: Couldn't factory reset.");
        }
    }

    /* Disable command echo from Bluefruit */
    Bluetooth::ble.echo(false);

    print_helper("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    Bluetooth::ble.info();
    Bluetooth::ble.setMode(BLUEFRUIT_MODE_DATA);

    /* Change the device name to make it easier to find */
    print_helper("Setting device name to 'AXO':");
    Bluetooth::ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO" ));
    Bluetooth::ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ));
    Bluetooth::ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ));

    if (!Bluetooth::ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO")) ) {
        Serial.println("BLUETOOTH: Could not set device name.");
    }

    /* Add or remove service requires a reset */
    print_helper("Performing a SW reset.");
    if (!Bluetooth::ble.reset()) {
        Serial.println("BLUETOOTH: Couldn't reset.");
    }

    print_helper("Success! Starting...");
    digitalWrite(Bluetooth::mode_pin, LOW);
    delay(100);
    return true;
}

void Bluetooth::write(const char *c, int len) {
    for (int i = 0; i < len; i++) {
        Bluetooth::ble.write(c[i]);
    }
}

char Bluetooth::read() {
    return Bluetooth::ble.read();
}

bool Bluetooth::available() {
    return Bluetooth::ble.available();
}
