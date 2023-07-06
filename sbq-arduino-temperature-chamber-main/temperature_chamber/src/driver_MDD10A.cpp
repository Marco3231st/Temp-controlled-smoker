#include <Arduino.h>
#include "../include/driver_MDD10A.h"

constexpr uint32_t HEATING_MODE = LOW;
constexpr uint32_t COOLING_MODE = HIGH;

DriverMDD10A::DriverMDD10A(uint32_t _dir_pin_peltier_1, uint32_t _pwm_pin_peltier_1, uint32_t _dir_pin_peltier_2, uint32_t _pwm_pin_peltier_2){
    dir_pin_peltier_1 = _dir_pin_peltier_1;
    pwm_pin_peltier_1 = _pwm_pin_peltier_1;
    dir_pin_peltier_2 = _dir_pin_peltier_2;
    pwm_pin_peltier_2 = _pwm_pin_peltier_2;

    pinMode(dir_pin_peltier_1, OUTPUT);
    pinMode(pwm_pin_peltier_1, OUTPUT);
    pinMode(dir_pin_peltier_2, OUTPUT);
    pinMode(pwm_pin_peltier_2, OUTPUT);
}

void DriverMDD10A::write_pwm_cmd(u_int32_t pwm){
    analogWrite(pwm_pin_peltier_1, pwm);
    analogWrite(pwm_pin_peltier_2, pwm);
}

void DriverMDD10A::set_peltier_current(float current_cmd_ampere){
    float max_current_ampere = 5.4;
    float min_current_ampere = 0.0;
    float max_pwm_current_signal = 255.0;

    if(current_cmd_ampere > max_current_ampere) current_cmd_ampere = max_current_ampere;
    if(current_cmd_ampere < min_current_ampere) current_cmd_ampere = min_current_ampere;

    uint32_t pwm_cmd = round(current_cmd_ampere*max_pwm_current_signal/max_current_ampere);
    write_pwm_cmd(pwm_cmd);
}

void DriverMDD10A::turn_off_peltier(){
    write_pwm_cmd(0);
}

void DriverMDD10A::set_mode(u_int32_t mode){
    digitalWrite(dir_pin_peltier_1, mode);
    digitalWrite(dir_pin_peltier_2, mode);
}

void DriverMDD10A::set_heating_mode(){
    set_mode(HEATING_MODE);
}

void DriverMDD10A::set_cooling_mode(){
    set_mode(COOLING_MODE);
}
