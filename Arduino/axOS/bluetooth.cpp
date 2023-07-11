#include "bluetooth.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"

Bluetooth::Bluetooth(HardwareSerial *port,
                      bool verbose,
                      int mode_pin,
                      bool factory_reset) : ble(*port, 4) {
    Bluetooth::mode_pin = mode_pin;
    Bluetooth::verbose = verbose;
    Bluetooth::factory_reset = factory_reset;
}

void Bluetooth::error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void Bluetooth::begin(size_t baud) {
    pinMode(Bluetooth::mode_pin, OUTPUT);
    digitalWrite(Bluetooth::mode_pin, HIGH);
    delay(100);
  
    Serial.begin(baud);
    Serial.println(F("Adafruit Bluefruit AXO"));
    Serial.println(F("---------------------------------------"));

    /* Initialise the module */
    Serial.print(F("Initialising the Bluefruit LE module: "));

    if ( !Bluetooth::ble.begin(Bluetooth::verbose) )
    {
    Bluetooth::error(F("ERROR: Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring."));
    }
    Serial.println( F("OK!") );

    if ( Bluetooth::factory_reset )
    {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! Bluetooth::ble.factoryReset() ) {
        Bluetooth::error(F("ERROR: Couldn't factory reset."));
    }
    }

    /* Disable command echo from Bluefruit */
    Bluetooth::ble.echo(false);

    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    Bluetooth::ble.info();
    Bluetooth::ble.setMode(BLUEFRUIT_MODE_DATA);

    /* Change the device name to make it easier to find */
    Serial.println(F("Setting device name to 'AXO something': "));
    Bluetooth::ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO-LEFT" ));
    Bluetooth::ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ));
    Bluetooth::ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ));

    if (! Bluetooth::ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO" )) ) {
    Bluetooth::error(F("ERROR: Could not set device name."));
    }

    /* Add or remove service requires a reset */
    Serial.println(F("Performing a SW reset."));
    if (! Bluetooth::ble.reset() ) {
    Bluetooth::error(F("ERROR: Couldn't reset."));
    }

    Serial.println("Success! Starting...");
    digitalWrite(Bluetooth::mode_pin, LOW);
    delay(100);
}

void Bluetooth::write(char c) {
    Bluetooth::ble.write(c);
}

char Bluetooth::read() {
    return Bluetooth::ble.read();
}

bool Bluetooth::available() {
    return Bluetooth::ble.available();
}

