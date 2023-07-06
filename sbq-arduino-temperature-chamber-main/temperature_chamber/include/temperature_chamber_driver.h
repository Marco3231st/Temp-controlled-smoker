#ifndef TEMPERATURE_CHAMBER_DRIVER_H
#define TEMPERATURE_CHAMBER_DRIVER_H

#include <cstdint> // uint8_t
#include "array.h"
#include "pid.h"
#include "driver_MDD10A.h"

class TemperatureChamberDriver {
public:
    static void initialize(const float);
    static void compute_pid_current_output(Array<float>);
    static float average_chamber_temperature(Array<float>);
    static float get_current_cmd_ampere();
    static void set_current_cmd_ampere(float);
    static String get_freezer_state();
    static void set_freezer_state(u_int32_t);
    static float get_temperature_goal_celsius();
    static void manual_set_temperature_goal_celsius(float);
    static void serial_set_temperature_goal_celsius_from_byte(int8_t);
    static Pid pid;
private:
    static float convert_temperature_byte_to_float(int8_t);
    static void heat_command(float);
    static void cool_command(float);
    static void send_command_to_peltiers(float);
    
    
    static float current_cmd_ampere;
    static String freezer_state;
    static bool initialized;
    static float temperature_goal_celsius;
    static DriverMDD10A peltier_driver_1;
    static DriverMDD10A peltier_driver_2;
};

#endif /* TEMPERATURE_CHAMBER_DRIVER_H */
