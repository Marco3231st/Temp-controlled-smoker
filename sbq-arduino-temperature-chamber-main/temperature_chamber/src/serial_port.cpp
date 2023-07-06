#include <Arduino.h>
#include <cassert>
#include "../include/serial_port.h"

int SerialPort::data_rate_bps = 115200;
bool SerialPort::initialized = false;

void SerialPort::initialize() {
    assert(!initialized);
    Serial.begin(data_rate_bps);
    Serial.flush();
    analogReadResolution(12);
    initialized = true;
}

void SerialPort::print_temperatures_to_serial(const Array<float>& temperatures_celsius) {
    unsigned long time = millis();
    String msg = "T[";
    for (int i = 0; i < temperatures_celsius.size; ++i) {
        msg = msg + "{\"time_stamp_ms\":" + time + ", \"physical_quantity\":{\"data\":[" + (String)temperatures_celsius[i] + "]}}";
        if(i < temperatures_celsius.size - 1) msg = msg + ", ";
    }
    Serial.println(msg + "]");
}

bool SerialPort::serial_available() {
    return Serial.available();
}

int8_t SerialPort::read_input_from_serial() {
    int8_t incoming_byte = Serial.read();
    return incoming_byte;
}
