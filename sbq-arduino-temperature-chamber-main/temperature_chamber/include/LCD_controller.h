#ifndef LCD_CONTROLLER_H
#define LCD_CONTROLLER_H

#include <Adafruit_RGBLCDShield.h>
#include <Wire.h>
#include "array.h"
#include "temperature_chamber_driver.h"

class LcdController {
public:
    static void initialize();
    static void user_input(const Array<float>&);
private:
    static void button_up_pressed();
    static void button_down_pressed();
    static void button_left_pressed();
    static void button_right_pressed();
    static void button_select_pressed();
    static void navigate();
    static void set_info_for_freezer();
    static void set_info_for_peltier();
    static void set_value();
    static void set_default_value();
    static void set_chamber_mode(const float);
    static void adjust_value();
    static void thermistor_info();
    static void alert();
    static void refresh();
    static String get_time_since_begining();
    
    static Adafruit_RGBLCDShield lcd;
    static int level;
    static int state;
    static float value;
    static float increment;
    static int thermistor_id;
    static float average_chamber_temperature_c;
    static Array<float> temperatures_celsius;
    static int chamber_mode;
};

#endif /* LCD_CONTROLLER_H */
