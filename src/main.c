// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2023 Brian Starkey <stark3y@gmail.com>

#include "main.h"

#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "bt_hid.h"
#include "crc.h"

#include <memory.h>
#include <stdio.h>

bt_hid_state state;
JoystickData jdata = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t sending_bytes[10];
uint8_t crc_table[256];
uint32_t last_sent_tick;

void main()
{
	uart_init(uart0, 115200);

	// Set up UART pins
	gpio_set_function(0, GPIO_FUNC_UART);
	gpio_set_function(1, GPIO_FUNC_UART);

	stdio_init_all();
	sleep_ms(1000);

	printf("Picow Init Done\n");

	multicore_launch_core1(bt_main);
	// Wait for init (should do a handshake with the fifo here?)
	sleep_ms(1000);

	init_CRC_Table(7, crc_table, sizeof(crc_table));

	last_sent_tick = get_tick();

	while (1)
	{
		uint32_t current_tick = get_tick();
		if (current_tick - last_sent_tick < 50)
			continue;

		bt_hid_get_latest(&state);

		jdata.button1 = 0;
		jdata.button1 |= (state.buttons & B_SQUARE) << 7;
		jdata.button1 |= (state.buttons & B_TRIANGLE) << 6;
		jdata.button1 |= (state.buttons & B_CROSS) << 5;
		jdata.button1 |= (state.buttons & B_CIRCLE) << 4;
		jdata.button1 |= H_UP(state.hat) << 3;
		jdata.button1 |= H_DOWN(state.hat) << 2;
		jdata.button1 |= (state.buttons & B_L1) << 1;
		jdata.button1 |= (state.buttons & B_R1);

		jdata.button2 = 0;
		jdata.button2 |= (state.buttons & B_SHARE) << 7;
		jdata.button2 |= (state.buttons & B_OPTIONS) << 6;
		jdata.button2 |= (state.buttons & B_PS4) << 5;
		jdata.button2 |= H_LEFT(state.hat) << 4;
		jdata.button2 |= H_RIGHT(state.hat) << 3;
		jdata.button2 |= (state.buttons & B_LEFT_STICK) << 2;
		jdata.button2 |= (state.buttons & B_RIGHT_STICK) << 1;
		jdata.button2 |= (state.buttons & B_TOUCH);

		jdata.l2 = state.l2;
		jdata.r2 = state.r2;

		jdata.lx = state.lx < 128 ? map(state.lx, 0, 127, -128, -1) : map(state.lx, 128, 255, 0, 127);
		jdata.ly = state.ly < 128 ? map(state.ly, 0, 127, 127, 0) : map(state.ly, 128, 255, -1, -128);

		jdata.rx = state.rx < 128 ? map(state.rx, 0, 127, -128, -1) : map(state.rx, 128, 255, 0, 127);
		jdata.ry = state.ry < 128 ? map(state.ry, 0, 127, 127, 0) : map(state.ry, 128, 255, -1, -128);

		sending_bytes[0] = START_BYTE;
		memcpy(sending_bytes + 1, (uint8_t *)&jdata, 8);
		sending_bytes[9] = get_CRC_Hash(sending_bytes + 1, 8, crc_table);

		uart_write_blocking(uart0, sending_bytes, 10);

		// printf("buttons: %04x, l: %d,%d, r: %d,%d, l2,r2: %d,%d hat: %d\n",
		// 	   state.buttons, state.lx, state.ly, state.rx, state.ry,
		// 	   state.l2, state.r2, state.hat);

		print_joystick_data();

		last_sent_tick = current_tick;
	}
}

void print_joystick_data(){
	printf("%04x %04x %u %u %d %d %d %d\n",
		   jdata.button1,
		   jdata.button2,
		   jdata.l2,
		   jdata.r2,
		   jdata.lx,
		   jdata.ly,
		   jdata.rx,
		   jdata.ry);
}

uint32_t get_tick()
{
	return (uint32_t)(time_us_64() / 1000);
}

int map(int value, int inMin, int inMax, int outMin, int outMax)
{
	return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
}