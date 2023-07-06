#ifndef DRIVER_MDD10A_H
#define DRIVER_MDD10A_H

#include <Arduino.h>

class DriverMDD10A {
public:
   DriverMDD10A(uint32_t, uint32_t, uint32_t, uint32_t);
   void set_peltier_current(float);
   void set_heating_mode();
   void set_cooling_mode();
   void turn_off_peltier();
private:
    uint32_t dir_pin_peltier_1;
    uint32_t pwm_pin_peltier_1;
    uint32_t dir_pin_peltier_2;
    uint32_t pwm_pin_peltier_2;

    void set_mode(u_int32_t);
    void write_pwm_cmd(u_int32_t);
};

#endif /* DRIVER_MDD10A_H */
