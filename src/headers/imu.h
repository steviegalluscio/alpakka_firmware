// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
// Copyright (C) 2026, Stevie Galluscio

#pragma once
#include "vector.h"

// LSM6DSR & LMS6DSV & LSM6DS3
#define IMU_WHO_AM_I 0x0f  // Identifier address.
#define IMU_CTRL1_XL 0x10  // Accelerometer config address.
#define IMU_CTRL2_G 0x11  // Gyroscope config address.
#define IMU_CTRL3_C 0x12  // IMU config address.
#define IMU_CTRL8_XL 0x17  // Accelerometer filter config address.
#define IMU_OUTX_L_G 0x22  // Gyroscope read X address.
#define IMU_OUTX_L_XL 0x28  // Accelerometer read X address.

#define IMU_READ 0b10000000  // Read byte.
#define IMU_CTRL1_XL_OFF 0b00000000  // Accelerometer value power off.
#define IMU_CTRL2_G_OFF  0b00000000  // Gyroscope value power off.

#if defined DEVICE_HAS_LSM6DSV
    // LSM6DSV
    #define IMU_CTRL6_G 0x15  // Gyroscope filter config address.
    #define IMU_CTRL9_XL 0x18  // Accelerometer filter config address 2.
    #define IMU_CTRL1_XL_2G  0b00001100  // Accelerometer value for 2G range. // [0][OP_MODE2][OP_MODE1][OP_MODE0][ODR3][ODR2][ODR1][ODR0]
    #define IMU_CTRL8_XL_LP  0b00000000  // Accelerometer value for low pass filter.
    #define IMU_CTRL9_XL_LP  0b00001000  // Accelerometer value for low pass filter. // [0][HP_MODE][FASTSETTL][HP_SLOPE][LPF2_EN][0][OFF_W][OFF_ON]
    #define IMU_CTRL2_G_ODR  0b00001100  // Gyroscope value for 7.68 kHz ODR. // [0][OP_MODE2][OP_MODE1][OP_MODE0][ODR3][ODR2][ODR1][ODR0]
    #define IMU_CTRL6_G_125  0b00000000  // Gyroscope value for 125 dps. // [0][LPF1_BW2][LPF1_BW1][LPF1_BW0][FS3][FS2][FS1][FS0]
    #define IMU_CTRL6_G_500  0b00000010  // Gyroscope value for 500 dps.
#elif defined DEVICE_HAS_LSM6DS3
    // LSM6DS3
    #define IMU_CTRL1_XL_2G 0b10100000 // Accelerometer value for 2G range at 6.66 kHz ODR
    // Enables LPF2 at ODR/9=6664/9=~740 Hz cutoff. LSM6DSR uses ODR/4=6667/4=~1666 Hz cutoff. (close enough, feels fine)
    #define IMU_CTRL8_XL_LP 0b11000000 // [LPF2_EN][HPCF1][HPCF0][0][0][HP_SL_EN][0][LP_6D]
    #define IMU_CTRL2_G_125 0b10000010 // Gyroscope value for 125 dps at 1.66 kHz ODR
    #define IMU_CTRL2_G_500 0b10000100 // Gyroscope value for 500 dps at 1.66 kHz ODR
#else
    // LSM6DSR
    #define IMU_CTRL1_XL_2G 0b10100010  // Accelerometer value for 2G range. // [ODR3][ODR2][ODR1][ODR0][FS1][FS0][LPF2_EN][0]
    #define IMU_CTRL8_XL_LP 0b00000000  // Accelerometer value for low pass filter.
    #define IMU_CTRL2_G_125 0b10100010  // Gyroscope value for 125 dps at 6.66 kHz ODR
    #define IMU_CTRL2_G_500 0b10100100  // Gyroscope value for 500 dps at 6.66 kHz ODR
#endif

#define GYRO_USER_OFFSET_FACTOR 1.5

void imu_init();
void imu_power_off();
Vector imu_read_gyro();
Vector imu_read_accel();
void imu_load_calibration();
void imu_calibrate();

