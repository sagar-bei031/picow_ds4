/**
 * @file main.c
 * @brief Main implementation file.
 * @author Robotics Team 2024, Robotics Club Pulchowk Campus
 * @date 2023
 */

#include "main.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "bt_hid.h"
#include "crc8.h"
#include "shared.h"

#include <memory.h>
#include <stdio.h>

// Global variables
bt_hid_state state;
JoyData jdata = {0, 0, 0, 0, 0, 0, 0};
uint8_t sending_bytes[10];
uint8_t crc_table[CRC8_TABLE_SIZE];
uint32_t last_sent_tick;
bool is_ps4_connected = false;

/**
 * @brief Main function of the application.
 */
void main()
{
	// Initialize UART for communication
	uart_init(uart0, 115200);

	// Set up UART pins
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);

	stdio_init_all();
	shared_init();

	// Launch Bluetooth HID main function on core 1
	multicore_launch_core1(bt_main);

	// Wait for initialization (a handshake with the fifo could be done here)
	printf("Picow Init Done\n");
	sleep_ms(1000);

	// Initialize CRC8 lookup table
	init_CRC_Table(7, crc_table, sizeof(crc_table));

	last_sent_tick = get_tick();

	while (1)
	{
		uint32_t current_tick = get_tick();
		if (current_tick - last_sent_tick < 50)
			continue;

		Message msg;
		if (queue_try_remove(&shared_queue, &msg))
		{
			is_ps4_connected = msg.is_ps4_connected;
		}

		if (is_ps4_connected)
		{
			// Get the latest Bluetooth HID state
			bt_hid_get_latest(&state);

			// Map Bluetooth HID state to JoystickData structure
			jdata.buttons = 0;
			jdata.buttons |= (state.buttons & B_CROSS) ? 1 << Cross : 0;
			jdata.buttons |= (state.buttons & B_CIRCLE) ? 1 << Circle : 0;
			jdata.buttons |= (state.buttons & B_SQUARE) ? 1 << Square : 0;
			jdata.buttons |= (state.buttons & B_TRIANGLE) ? 1 << Triangle : 0;
			jdata.buttons |= (state.buttons & B_SHARE) ? 1 << Share : 0;
			jdata.buttons |= (state.buttons & B_PS4) ? 1 << Power : 0;
			jdata.buttons |= (state.buttons & B_OPTIONS) ? 1 << Option : 0;
			jdata.buttons |= (state.buttons & B_LEFT_STICK) ? 1 << L3 : 0;
			jdata.buttons |= (state.buttons & B_RIGHT_STICK) ? 1 << R3 : 0;
			jdata.buttons |= (state.buttons & B_L1) ? 1 << L1 : 0;
			jdata.buttons |= (state.buttons & B_R1) ? 1 << R1: 0;
			jdata.buttons |= H_UP(state.hat) ? 1 << Up : 0;
			jdata.buttons |= H_DOWN(state.hat) ? 1 << Down : 0;
			jdata.buttons |= H_LEFT(state.hat) ? 1 << Left : 0;
			jdata.buttons |= H_RIGHT(state.hat) ? 1 << Right : 0;
			jdata.buttons |= (state.buttons & B_TOUCH) ? 1 << Touch : 0;

			jdata.lt = state.l2;
			jdata.rt = state.r2;

			jdata.lx = state.lx < 128 ? map(state.lx, 0, 127, -128, -1) : map(state.lx, 128, 255, 0, 127);
			jdata.ly = state.ly < 128 ? map(state.ly, 0, 127, 127, 0) : map(state.ly, 128, 255, -1, -128);

			jdata.rx = state.rx < 128 ? map(state.rx, 0, 127, -128, -1) : map(state.rx, 128, 255, 0, 127);
			jdata.ry = state.ry < 128 ? map(state.ry, 0, 127, 127, 0) : map(state.ry, 128, 255, -1, -128);

			// Prepare data for transmission
			sending_bytes[0] = START_BYTE;
			memcpy(sending_bytes + 1, (uint8_t *)&jdata, 8);
			sending_bytes[9] = get_CRC_Hash(sending_bytes + 1, 8, crc_table);

			// Send data via UART
			uart_write_blocking(uart0, sending_bytes, 10);

			// Print joystick data for debugging
			print_joystick_data();
			last_sent_tick = get_tick();
		}
		else
		{
			printf("PS4 not connected\n");
			sleep_ms(50);
		}
	}
}

/**
 * @brief Function to print joystick data.
 */
void print_joystick_data()
{
	printf("%d %d %d %d %u %u %04x\n",
		   jdata.lx,
		   jdata.ly,
		   jdata.rx,
		   jdata.ry,
		   jdata.lt,
		   jdata.rt,
		   jdata.buttons);
}

/**
 * @brief Function to get the current system tick.
 * @return The current system tick.
 */
uint32_t get_tick()
{
	return (uint32_t)(time_us_64() / 1000);
}

/**
 * @brief Function to map a value from one range to another.
 *
 * This function maps a value from an input range to an output range.
 *
 * @param value The input value to be mapped.
 * @param inMin The minimum value of the input range.
 * @param inMax The maximum value of the input range.
 * @param outMin The minimum value of the output range.
 * @param outMax The maximum value of the output range.
 * @return The mapped value.
 */
int map(int value, int inMin, int inMax, int outMin, int outMax)
{
	return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}
