#include <Arduino.h>
#include "../include/pid.h"

Pid::Pid(float _kp, float _kd, float _ki) {
    kp = _kp;
    kd = _kd;
    ki = _ki;
    proportional = 0.0;
    derivative = 0.0;
    integral = 0.0;
}

void Pid::compute_pid_term(float error) {
    float dt_second = time_interval();
    static float previous_errors[5] = {0.0, 1.0, 2.0, 3.0, 4.0};

    // Proportional term
    proportional = kp * error;
    // Derivative term
    derivative = 0.0;
    if(dt_second > 0.0) {
        derivative = kd * ((error - mean_errors(previous_errors)) / dt_second);
    }
    permutate_errors(previous_errors, error);
    // Integral term
    integral += ki * (error * dt_second);
}

float Pid::time_interval() {
    static unsigned long last_time_millis;
    unsigned long time_difference = millis() - last_time_millis;
    last_time_millis = millis();
    return (float)time_difference / 1000.0;
}

float Pid::mean_errors(const float *previous_errors) {
    int number_previous_errors = 5;
    float sum = 0.0;
    for (int i = 0; i < number_previous_errors; i++) {
        sum += previous_errors[i];
    }
    return sum / (float)number_previous_errors;
}

void Pid::permutate_errors(float *previous_errors, float error) {
    for (int i = 0; i < 4; i++) {
        previous_errors[i] = previous_errors[i+1];
    }
    previous_errors[4] = error;
}

float Pid::compute_pid_command() {return proportional + derivative + integral;}

void Pid::set_kp(float _kp) {kp = _kp;}

void Pid::set_kd(float _kd) {kd = _kd;}

void Pid::set_ki(float _ki) {ki = _ki;}

float Pid::get_kp() {return kp;}

float Pid::get_kd() {return kd;}

float Pid::get_ki() {return ki;}

float Pid::get_proportional() {return proportional;}

float Pid::get_derivative() {return derivative;}

float Pid::get_integral() {return integral;}
