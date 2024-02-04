/**
 * @file main.h
 * @brief Header file for the main application.
 * @author Robotics Team 2024, Robotics Club Pulchowk Campus
 * @date 2023
 */

#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>

/**
 * @def START_BYTE
 * @brief Start byte used in the joystick data packet.
 */
#define START_BYTE 0xA5

/**
 * @def B_L1, B_R1, B_L2, B_R2, B_SHARE, B_OPTIONS, B_LEFT_STICK, B_RIGHT_STICK, B_PS4, B_TOUCH, B_SQUARE, B_CROSS, B_CIRCLE, B_TRIANGLE
 * @brief Bit masks for various buttons in the joystick data packet.
 */
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

/**
 * @def H_UP, H_RIGHT, H_DOWN, H_LEFT
 * @brief Macros to determine joystick hat state based on hat value.
 */
#define H_UP(__HAT__) ((__HAT__ == 7) || (__HAT__ == 0) || (__HAT__ == 1))
#define H_RIGHT(__HAT__) ((__HAT__ == 1) || (__HAT__ == 2) || (__HAT__ == 3))
#define H_DOWN(__HAT__) ((__HAT__ == 3) || (__HAT__ == 4) || (__HAT__ == 5))
#define H_LEFT(__HAT__) ((__HAT__ == 5) || (__HAT__ == 6) || (__HAT__ == 7))

typedef struct
{
    int8_t lx;
    int8_t ly;
    int8_t rx;
    int8_t ry;
    uint8_t lt;
    uint8_t rt;
    uint16_t buttons;
} JoyData;

enum JoyButtons
{
    Cross,
    Circle,
    Square,
    Triangle,
    Share,
    Power,
    Option,
    L3,
    R3,
    L1,
    R1,
    Up,
    Down,
    Left,
    Right,
    Touch
};

/**
 * @struct bt_hid_state
 * @brief Structure representing the Bluetooth HID state.
 */
typedef struct bt_hid_state bt_hid_state;

/**
 * @brief Function to start the main application.
 */
void main();

/**
 * @brief Function to get the current system tick.
 * @return The current system tick.
 */
uint32_t get_tick();

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
int map(int value, int inMin, int inMax, int outMin, int outMax);

/**
 * @brief Function to print joystick data.
 */
void print_joystick_data();

#endif
