#pragma once

// pins for the camera
// got these from my kicad schematic
#define CAM_PIN_D0    4
#define CAM_PIN_D1    5
#define CAM_PIN_D2    6
#define CAM_PIN_D3    7
#define CAM_PIN_D4    8
#define CAM_PIN_D5    9
#define CAM_PIN_D6    10
#define CAM_PIN_D7    11

#define CAM_PIN_PCLK  12
#define CAM_PIN_VSYNC 13
#define CAM_PIN_HREF  14
#define CAM_PIN_XCLK  17
#define CAM_PIN_SDA   18
#define CAM_PIN_SCL   19

// no power down or reset pin on my pcb
#define CAM_PIN_PWDN  -1
#define CAM_PIN_RESET -1

// servo pins
#define PAN_PWM_PIN   20
#define TILT_PWM_PIN  21

// how far the servos can go
#define PAN_MIN_ANGLE   0
#define PAN_MAX_ANGLE   180
#define TILT_MIN_ANGLE  30
#define TILT_MAX_ANGLE  150

// start position (middle)
#define PAN_CENTER_ANGLE  90
#define TILT_CENTER_ANGLE 90

// camera resolution
// using small size bc no psram on esp32-s3fn8
#define CAM_FRAME_WIDTH  160
#define CAM_FRAME_HEIGHT 120

// pid tuning values
// p = how fast to react
// i = fix small leftover errors
// d = smooth out the movement
#define PID_KP  0.06f
#define PID_KI  0.0005f
#define PID_KD  0.015f

// max degrees servo can move per loop
#define PID_MAX_OUTPUT  5.0f

#define SERIAL_BAUD   115200
#define LOOP_DELAY_MS 20

// turn these off if serial gets too spammy
#define DEBUG_PRINT_FACE  1
#define DEBUG_PRINT_PID   1
