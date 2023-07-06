#include <Arduino.h>
#include "../include/LCD_controller.h"

constexpr int BACKLIGHT_ON = 0x1;
constexpr int BACKLIGHT_OFF = 0x0;

constexpr int MENU_LEVEL = 0;
constexpr int ACTION_LEVEL = 1;

constexpr int GENERAL_INFO_STATE = 0;
constexpr int TIME_RUNNING_STATE = 1;
constexpr int TEMPERATURE_GOAL_STATE = 2;
constexpr int KP_STATE = 3;
constexpr int KD_STATE = 4;
constexpr int KI_STATE = 5;
constexpr int FIRST_STATE = GENERAL_INFO_STATE;
constexpr int LAST_STATE = KI_STATE;

constexpr int COOLING_MODE = 0;
constexpr int HEATING_MODE = 1;
constexpr int MONITORED_THERMISTOR_ID = 0;
constexpr float TEMPERATURE_ALERT_CELSIUS = 25.0;

/*------------------------------------
Menu :
- General Info (level 0, state 0)
  |---> Thermistors temperature (level 1, state 0)

- Time Running (level 0, state 1)

- Temperature Goal (level 0, state 2)
  |---> Set Goal (level 1, state 2)

- Kp value (level 0, state 3)
  |---> Set Kp (level 1, state 3)

- Kd value (level 0, state 4)
  |---> Set Kd (level 1, state 4)

- Ki value (level 0, state 5)
  |---> Set Ki (level 1, state 5)
--------------------------------------*/

Adafruit_RGBLCDShield LcdController::lcd = Adafruit_RGBLCDShield();
float temperatures[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
Array<float> LcdController::temperatures_celsius = Array<float>(temperatures, 12);
float LcdController::average_chamber_temperature_c = 0.0;
int LcdController::chamber_mode = HEATING_MODE;
int LcdController::level = 0;
int LcdController::state = 0;
float LcdController::value = 0.0;
float LcdController::increment = 1.0;
int LcdController::thermistor_id = 0;

void LcdController::initialize() {
    lcd.setBacklight(BACKLIGHT_ON);
    lcd.begin(16, 2);
    lcd.print("Temp Chamber");
    delay(1000);
    lcd.clear();
    lcd.print("TEMP  AMP  GOAL");
    lcd.setCursor(11,1);
    lcd.print(((String)TemperatureChamberDriver::get_temperature_goal_celsius()).substring(0,5));
}

void LcdController::set_chamber_mode(float average_chamber_temperature_c) {
    float goal_c = TemperatureChamberDriver::get_temperature_goal_celsius();
    if(goal_c >= average_chamber_temperature_c){
        chamber_mode = HEATING_MODE;
    } else{
        chamber_mode = COOLING_MODE;
    }
}

void LcdController::user_input(const Array<float>& _temperatures_celsius) {
    temperatures_celsius = _temperatures_celsius;
    average_chamber_temperature_c = TemperatureChamberDriver::average_chamber_temperature(temperatures_celsius);
    set_chamber_mode(average_chamber_temperature_c);
    uint8_t buttons = lcd.readButtons();

    if (buttons) {
        if (buttons & BUTTON_UP) {
            button_up_pressed();
        }
        if (buttons & BUTTON_DOWN) {
            button_down_pressed();
        }
        if (buttons & BUTTON_LEFT) {
            button_left_pressed();
        }
        if (buttons & BUTTON_RIGHT) {
            button_right_pressed();
        }
        if (buttons & BUTTON_SELECT) {
            button_select_pressed();
        }
    }
    refresh();
}

void LcdController::button_up_pressed() {
    if(level == MENU_LEVEL) {
          state--;
          if(state < FIRST_STATE) state = KI_STATE;
          navigate();
      } else if(level == ACTION_LEVEL && state != GENERAL_INFO_STATE) {
          value += increment;
          adjust_value();
      }
}

void LcdController::button_down_pressed() {
    if(level == MENU_LEVEL) {
          state++;
          if(state > LAST_STATE) state = GENERAL_INFO_STATE;
          navigate();
      } else if(level == ACTION_LEVEL && state != GENERAL_INFO_STATE) {
          value -= increment;
          adjust_value();
      }
}

void LcdController::button_left_pressed() {
    if(level == ACTION_LEVEL) {
          if(state == GENERAL_INFO_STATE) {
              thermistor_id--;
              if(thermistor_id < 0) thermistor_id = 11;
              thermistor_info();
          } else if(state!= TIME_RUNNING_STATE) {
              increment *= 0.1;
              adjust_value();
          }
    }
}

void LcdController::button_right_pressed() {
    if(level == ACTION_LEVEL) {
          if(state == GENERAL_INFO_STATE) {
              thermistor_id++;
              if(thermistor_id > 11) thermistor_id = 0;
              thermistor_info();
          } else if(state!= TIME_RUNNING_STATE) {
              increment *= 10;
              adjust_value();
          }
      }
}

void LcdController::button_select_pressed() {
    if(level == MENU_LEVEL && state != TIME_RUNNING_STATE) {
          level++;
          if(state == GENERAL_INFO_STATE) {
              thermistor_info();
          } else {
              set_default_value();
              adjust_value();
          }
      } else if(level == ACTION_LEVEL) {
          level--;
          set_value();
          navigate();
      }
}

void LcdController::navigate() {
    lcd.clear();
    switch (state) {
    case GENERAL_INFO_STATE:
        if(chamber_mode == COOLING_MODE) {set_info_for_freezer();}
        if(chamber_mode == HEATING_MODE) {set_info_for_peltier();}
        break;
    case TIME_RUNNING_STATE:
        lcd.print("Time Running");
        lcd.setCursor(0,1);
        lcd.print(get_time_since_begining().substring(0,8));
        break;
    case TEMPERATURE_GOAL_STATE:
        lcd.print("SET TEMP GOAL");
        lcd.setCursor(0,1);
        lcd.print("Currently " + (String)TemperatureChamberDriver::get_temperature_goal_celsius());
        break;
    case KP_STATE:
        lcd.print("SET KP");
        lcd.setCursor(0,1);
        lcd.print("Currently " + (String)TemperatureChamberDriver::pid.get_kp());
        break;
    case KD_STATE:
        lcd.print("SET KD");
        lcd.setCursor(0,1);
        lcd.print("Currently " + (String)TemperatureChamberDriver::pid.get_kd());
        break;
    case KI_STATE:
        lcd.print("SET KI");
        lcd.setCursor(0,1);
        lcd.print("Currently " + (String)TemperatureChamberDriver::pid.get_ki());
        break;
    }
}

void LcdController::set_info_for_freezer() {
    lcd.setCursor(0,0);
    lcd.print("TEMP  FRZ  GOAL");
    lcd.setCursor(0,1);
    lcd.print(((String)average_chamber_temperature_c).substring(0,5) +
              " " + TemperatureChamberDriver::get_freezer_state() + " " +
              " " + ((String)TemperatureChamberDriver::get_temperature_goal_celsius()).substring(0,5));
}

void LcdController::set_info_for_peltier() {
    lcd.setCursor(0,0);
    lcd.print("TEMP  AMP  GOAL");
    lcd.setCursor(0,1);
    lcd.print(((String)average_chamber_temperature_c).substring(0,5) +
              " " + ((String)TemperatureChamberDriver::get_current_cmd_ampere()).substring(0,4) +
              " " + ((String)TemperatureChamberDriver::get_temperature_goal_celsius()).substring(0,5));
}

void LcdController::adjust_value() {
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print((String)value + " +/-" + (String)increment);
}

void LcdController::set_default_value() {
    switch (state) {
    case TEMPERATURE_GOAL_STATE:
        value = TemperatureChamberDriver::get_temperature_goal_celsius();
        break;
    case KP_STATE:
        value = TemperatureChamberDriver::pid.get_kp();
        break;
    case KD_STATE:
        value = TemperatureChamberDriver::pid.get_kd();
        break;
    case KI_STATE:
        value = TemperatureChamberDriver::pid.get_ki();
        break;
    }
}

void LcdController::set_value() {
    switch (state) {
    case TEMPERATURE_GOAL_STATE:
        TemperatureChamberDriver::manual_set_temperature_goal_celsius(value);
        break;
    case KP_STATE:
        TemperatureChamberDriver::pid.set_kp(value);
        break;
    case KD_STATE:
        TemperatureChamberDriver::pid.set_kd(value);
        break;
    case KI_STATE:
        TemperatureChamberDriver::pid.set_ki(value);
        break;
    }
}

void LcdController::refresh() {
    if (state == GENERAL_INFO_STATE) {
        if(level == MENU_LEVEL) {
            if(chamber_mode == COOLING_MODE) {set_info_for_freezer();}
            if(chamber_mode == HEATING_MODE) {set_info_for_peltier();}
        } else if(level == ACTION_LEVEL) {
            lcd.setCursor(0,1);
            lcd.print(temperatures_celsius[thermistor_id]);
        }
    } else if(state == TIME_RUNNING_STATE) {
        lcd.setCursor(0,1);
        lcd.print(get_time_since_begining().substring(0,8));
    }
}

void LcdController::thermistor_info() {
    lcd.clear();
    lcd.print("Thermistor : " + (String)thermistor_id);
    lcd.setCursor(0,1);
    lcd.print(temperatures_celsius[thermistor_id]);
}

void LcdController::alert() {
    if(temperatures_celsius[MONITORED_THERMISTOR_ID] > TEMPERATURE_ALERT_CELSIUS) {
        lcd.clear();
        lcd.print("TEMP TOO HIGH");
    }
}

String LcdController::get_time_since_begining() {
    unsigned long time = millis();
    int hour = (int)floor(time/(1000*60*60));
    int minute = (int)floor((time/(1000*60)) % 60);
    int second = (int)floor((time/(1000)) % 60);
    int millisecond = (int)(time % 1000);

    String hours = (hour < 10) ? "0" + (String)hour : (String)hour;
    String minutes = (minute < 10) ? "0" + (String)minute : (String)minute;
    String seconds = (second < 10) ? "0" + (String)second : (String)second;

    return hours + ":" + minutes + ":" + seconds + "." + millisecond;
}
