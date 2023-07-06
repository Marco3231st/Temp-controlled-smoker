#include "Arduino.h"
#include "../include/serial_port.h"
#include "../include/thermistor_reader.h"
#include "../include/temperature_chamber_driver.h"
#include "../include/array.h"
#include "../include/LCD_controller.h"


constexpr unsigned long period_ms = 250;

void wait_for_period_to_end(unsigned long);

const float temperature_goal_celsius = 21.0;
uint8_t thermistors_arduino_analog_pins[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const Array<uint8_t> thermistors_pins_list = Array<uint8_t>(thermistors_arduino_analog_pins, 12);

void setup() {
	SerialPort::initialize();
	ThermistorReader::initialize(thermistors_pins_list);
	TemperatureChamberDriver::initialize(temperature_goal_celsius);
	//LcdController::initialize();
}

void loop() {
	unsigned long start_time = millis();
	const Array<float> temperatures_celsius = ThermistorReader::get_temperatures_from_thermistors();
	TemperatureChamberDriver::compute_pid_current_output(temperatures_celsius);
	SerialPort::print_temperatures_to_serial(temperatures_celsius);
	//LcdController::user_input(temperatures_celsius);
	if (SerialPort::serial_available()) {
		int8_t temperature_byte = SerialPort::read_input_from_serial();
		TemperatureChamberDriver::serial_set_temperature_goal_celsius_from_byte(temperature_byte);
	}
	wait_for_period_to_end(start_time);
}

void wait_for_period_to_end(unsigned long start_time) {
	const int minimum_delay_ms = 25; // To give enough time for serial_port message reception.
	unsigned long elapsed_time = millis()-start_time;

	int time_to_wait_ms = period_ms - elapsed_time;
	if (time_to_wait_ms < minimum_delay_ms){
		time_to_wait_ms = minimum_delay_ms;
	}
	delay(time_to_wait_ms);
}
