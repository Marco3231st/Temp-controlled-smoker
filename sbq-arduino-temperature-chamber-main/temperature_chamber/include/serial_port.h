#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <cstdint> // uint8_t
#include "array.h"


class SerialPort {
public:
    static void initialize();
    static void print_temperatures_to_serial(const Array<float>&);
    static int8_t read_input_from_serial();
    static bool serial_available();
private:
    static int data_rate_bps;
    static bool initialized;
};

#endif /* SERIAL_PORT_H */
