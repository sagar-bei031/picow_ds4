#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

#define START_BYTE 0xA5

// AND MASK for state from library
#define B_L1 0x0001U
#define B_R1 0x0002U
#define B_L2 0x0004U
#define B_R2 0x0008U
#define B_SHARE 0x0010U
#define B_OPTIONS 0x0020U
#define B_LEFT_STICK 0x0040U
#define B_RIGHT_STICK 0x0080U
#define B_PS4 0x0100U
#define B_TOUCH 0x0200U
#define B_SQUARE 0x1000U
#define B_CROSS 0x2000U
#define B_CIRCLE 0X4000U
#define B_TRIANGLE 0X8000U

// Macro to get HAT STATE from library
#define H_UP(__HAT__) ((__HAT__ == 7) || (__HAT__ == 0) || (__HAT__ == 1))
#define H_RIGHT(__HAT__) ((__HAT__ == 1) || (__HAT__ == 2) || (__HAT__ == 3))
#define H_DOWN(__HAT__) ((__HAT__ == 3) || (__HAT__ == 4) || (__HAT__ == 5))
#define H_LEFT(__HAT__) ((__HAT__ == 5) || (__HAT__ == 6) || (__HAT__ == 7))

/******************************************************************************
 * Joystick data structure
 *
 * There are ten bytes in a packet of control data.
 * First byte is start byte.
 * Last byte is crc8 hash.
 * Between eigth bytes include followings:
 *
 * Button byte-1: Square Triangle Cross Circle Up Down L1 R1
 * Button byte-2: Share Options PS4 Left right Left-stick-press Right-stick-press Touch_Pad_Click
 * L2
 * R2
 * Left-stick-horizontal(x)
 * Left-stick-vertical(y)
 * Right-stick-horizontal(x)
 * Right-stick-vertical(y)
 *******************************************************************************/
typedef struct
{
	uint8_t button1;
	uint8_t button2;
	uint8_t l2;
	uint8_t r2;
	int8_t lx;
	int8_t ly;
	int8_t rx;
	int8_t ry;
} JoystickData;

typedef struct bt_hid_state bt_hid_state;

void main();

uint32_t get_tick();

int map(int value, int inMin, int inMax, int outMin, int outMax);

void print_joystick_data();

#endif