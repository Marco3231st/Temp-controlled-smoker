#include <Arduino.h>
#include <cassert>
#include<cmath>
#include "../include/thermistor_reader.h"


void ThermistorReader::initialize(const Array<uint8_t>& _thermistors_pins_list) {
    thermistors_pins_list = _thermistors_pins_list;
}

Array<float> ThermistorReader::get_temperatures_from_thermistors() {
    Array<int> measured_voltage_12_bits = get_voltage_12_bits_from_thermistors();
    return map<int, float>(calculate_temperature_celsius_from_voltage_code, measured_voltage_12_bits);
}

Array<int> ThermistorReader::get_voltage_12_bits_from_thermistors() {
    return map<uint8_t, int>(analogRead, ThermistorReader::thermistors_pins_list);
}

float ThermistorReader::calculate_temperature_celsius_from_voltage_code(int measured_voltage_12_bits) {
    const float full_range_12_bits = 4096.0;
    const float resistance_at_25C_ohm = 10000;
    const float voltage_input_volt = 3.3;
    const float kelvin_to_celsius_difference = -273.15;
    const float a_coef = 0.003354016434680530000;
    const float b_coef = 0.000256523550896126000;
    const float c_coef = 0.000002605970120720520;
    const float d_coef = 0.000000063292612648746;

    float measured_voltage_volt = (float(measured_voltage_12_bits) / full_range_12_bits) * voltage_input_volt;
    float thermistor_resistance_ohm = (measured_voltage_volt * resistance_at_25C_ohm) / (voltage_input_volt - measured_voltage_volt);
    float log_ratio = log(thermistor_resistance_ohm/resistance_at_25C_ohm);
    float measured_temperature_celsius = (1 / (a_coef + b_coef*log_ratio + c_coef*log_ratio*log_ratio + d_coef*log_ratio*log_ratio*log_ratio)) + kelvin_to_celsius_difference;
    return measured_temperature_celsius;
}


static uint8_t thermistors_arduino_analog_pins[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
Array<uint8_t> ThermistorReader::thermistors_pins_list = Array<uint8_t>(thermistors_arduino_analog_pins, 12);
