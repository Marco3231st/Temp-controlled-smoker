#include <Arduino.h>
#include <cassert>
#include "../include/temperature_chamber_driver.h"

constexpr uint32_t COLD_FAN_SSR_PIN = 6;
constexpr uint32_t HOT_FAN_SSR_PIN = 7;
constexpr uint32_t FAN_ON = HIGH;
constexpr uint32_t FAN_OFF = LOW;
constexpr float MAXIMUM_CURRENT_AMPERE = 5.4;
constexpr float MINIMUM_CURRENT_AMPERE = 0.5;

float TemperatureChamberDriver::current_cmd_ampere = 0.0;
String TemperatureChamberDriver::freezer_state = "OFF";
bool TemperatureChamberDriver::initialized = false;
float TemperatureChamberDriver::temperature_goal_celsius = 0.0;
Pid TemperatureChamberDriver::pid = Pid(5.0 , 1.0, 0.5);
DriverMDD10A TemperatureChamberDriver::peltier_driver_1 = DriverMDD10A(2, 3, 4, 5);
DriverMDD10A TemperatureChamberDriver::peltier_driver_2 = DriverMDD10A(8, 9, 10, 11);

void TemperatureChamberDriver::initialize(float temperature_goal) {
    assert(!initialized);
    manual_set_temperature_goal_celsius(temperature_goal);
    peltier_driver_1.set_heating_mode();
    peltier_driver_2.set_heating_mode();
    pinMode(COLD_FAN_SSR_PIN, OUTPUT);
    pinMode(HOT_FAN_SSR_PIN, OUTPUT);
    initialized = true;
}

void TemperatureChamberDriver::compute_pid_current_output(Array<float> temperatures_celsius) {
    float chamber_temperature_celsius = average_chamber_temperature(temperatures_celsius);
    float error = temperature_goal_celsius - chamber_temperature_celsius;

    if (temperature_goal_celsius > chamber_temperature_celsius) {
        digitalWrite(COLD_FAN_SSR_PIN, FAN_OFF);
        pid.compute_pid_term(error);
        return heat_command(error);
    } else {
        digitalWrite(HOT_FAN_SSR_PIN, FAN_OFF);
        return cool_command(error);
    }
    
}

void TemperatureChamberDriver::heat_command(float error) {
    float fan_error_threshold = 0.5;
    float peltier_error_threshold = 1.0;
    float current_cmd_ampere = pid.compute_pid_command();

    if (error > peltier_error_threshold) {
        digitalWrite(HOT_FAN_SSR_PIN, FAN_ON);
        if(current_cmd_ampere <= MINIMUM_CURRENT_AMPERE) {
            send_command_to_peltiers(MINIMUM_CURRENT_AMPERE);
        } else if(current_cmd_ampere > MAXIMUM_CURRENT_AMPERE) {
            send_command_to_peltiers(MAXIMUM_CURRENT_AMPERE);
        } else {
            send_command_to_peltiers(current_cmd_ampere);
        }
    } else if (error > fan_error_threshold) {
        digitalWrite(HOT_FAN_SSR_PIN, FAN_ON);
        peltier_driver_1.turn_off_peltier();
        peltier_driver_2.turn_off_peltier();
    }
    else {
        digitalWrite(HOT_FAN_SSR_PIN, FAN_OFF);
        peltier_driver_1.turn_off_peltier();
        peltier_driver_2.turn_off_peltier();
    }
}

void TemperatureChamberDriver::cool_command(float error) {
    float bang_bang_band_c = -0.5;
    u_int32_t freezer_state = FAN_OFF;
    if(error < bang_bang_band_c) {
        digitalWrite(COLD_FAN_SSR_PIN, FAN_ON);
        freezer_state = FAN_ON;
    } else {
        digitalWrite(COLD_FAN_SSR_PIN, FAN_OFF);
        freezer_state = FAN_OFF;
    }
    set_freezer_state(freezer_state);
}

void TemperatureChamberDriver::send_command_to_peltiers(float current_cmd_ampere){
    peltier_driver_1.set_peltier_current(current_cmd_ampere);
    peltier_driver_2.set_peltier_current(current_cmd_ampere);
    set_current_cmd_ampere(current_cmd_ampere);
}

float TemperatureChamberDriver::average_chamber_temperature(Array<float> temperatures_celsius) {
    int number_thermistors_in_chamber = 2;
    float sum = 0.0;
    for (int i = 0; i < number_thermistors_in_chamber; i++) {
        sum += temperatures_celsius[i];
    }
    return sum / (float)number_thermistors_in_chamber;
}


float TemperatureChamberDriver::get_current_cmd_ampere() {return current_cmd_ampere;}

void TemperatureChamberDriver::set_current_cmd_ampere(float _current_cmd_ampere){
    current_cmd_ampere = _current_cmd_ampere;
}

String TemperatureChamberDriver::get_freezer_state() {return freezer_state;}

void TemperatureChamberDriver::set_freezer_state(u_int32_t _freezer_state){
    if(_freezer_state == FAN_OFF){
        freezer_state = "OFF";
    } else if(_freezer_state == FAN_ON){
        freezer_state = "ON ";
    }
}

float TemperatureChamberDriver::convert_temperature_byte_to_float(int8_t temperature_byte) {
    float desired_temperature = 0.0;
    desired_temperature = float(temperature_byte)/2;
    return desired_temperature;
}

void TemperatureChamberDriver::manual_set_temperature_goal_celsius(float _temperature_goal_celsius) {
    temperature_goal_celsius = _temperature_goal_celsius;
}

void TemperatureChamberDriver::serial_set_temperature_goal_celsius_from_byte(int8_t temperature_byte) {
    temperature_goal_celsius = TemperatureChamberDriver::convert_temperature_byte_to_float(temperature_byte);
}

float TemperatureChamberDriver::get_temperature_goal_celsius() {return temperature_goal_celsius;}
