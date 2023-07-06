#ifndef THERMISTOR_READER_H
#define THERMISTOR_READER_H

#include <cstdint> // uint8_t
#include "array.h"

class ThermistorReader {
public:
    static void initialize(const Array<uint8_t>&);
    static Array<float> get_temperatures_from_thermistors();
    static Array<uint8_t> thermistors_pins_list;
private:
    static Array<int> get_voltage_12_bits_from_thermistors();
    static float calculate_temperature_celsius_from_voltage_code(int);
};

#endif /* THERMISTOR_READER_H */
