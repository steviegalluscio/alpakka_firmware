// SPDX-License-Identifier: GPL-2.0-only
// Copyright (C) 2022, Input Labs Oy.
// Copyright (C) 2026, Stevie Galluscio

#pragma once
#include "vector.h"

// LMS6DSV

#define IMU_WHO_AM_I 0x0f  // Identifier address.
#define IMU_CTRL1_XL 0x10  // Accelerometer config address.
#define IMU_CTRL2_G 0x11  // Gyroscope config address.
#define IMU_CTRL3_C 0x12  // IMU config address.
#define IMU_CTRL7_G 0x16 // Gyroscope filter enable address.
#define IMU_CTRL8_XL 0x17  // Accelerometer filter config address.
#define IMU_OUTX_L_G 0x22  // Gyroscope read X address.
#define IMU_OUTX_L_XL 0x28  // Accelerometer read X address.

#define IMU_READ 0b10000000

#define IMU_CTRL1_XL_OFF 0b00000000  // Accelerometer value power off.
#define IMU_CTRL2_G_OFF  0b00000000  // Gyroscope value power off.


#if defined DEVICE_HAS_LSM6DSV
    // LSM6DSV
    #define IMU_CTRL6_G 0x15  // Gyroscope filter config address.
    #define IMU_CTRL9_XL 0x18  // Accelerometer filter config address 2.
    #define IMU_CTRL1_XL_2G  0b00001100  // Accelerometer value for 2G range. // [0][OP_MODE2][OP_MODE1][OP_MODE0][ODR3][ODR2][ODR1][ODR0]
    #define IMU_CTRL9_XL_LP  0b00001000  // Accelerometer value for low pass filter. // [0][HP_MODE][FASTSETTL][HP_SLOPE][LPF2_EN][0][OFF_W][OFF_ON]
    #define IMU_CTRL2_G_ODR  0b00001100  // Gyroscope value for 7.68 kHz ODR. // [0][OP_MODE2][OP_MODE1][OP_MODE0][ODR3][ODR2][ODR1][ODR0]
    #define IMU_CTRL6_G_125  0b00010000  // Gyroscope value for 125 dps and 213Hz cutoff.
    #define IMU_CTRL6_G_500  0b00010010  // Gyroscope value for 500 dps and 213Hz cutoff.
    #define IMU_CTRL7_G_LP   0b00000001  // LP enable.
#endif

#define IMU_CTRL3_C_BOOT 0b00000001  // Reboot the IMU.

/* XL = 100b that is ODR/100 bandwidth, 66Hz for 6.6kHz ODR */
#define IMU_CTRL8_XL_LP  0b10000000  // Accelerometer value for low pass filter.


/* Gyro int-to-float conversion constants (rad/s)*/
#define GYRO_SENS_RADPS_125  7.63581547747519E-05f
#define GYRO_SENS_RADPS_250  0.000152716309549504f
#define GYRO_SENS_RADPS_500  0.000305432619099008f
#define GYRO_SENS_RADPS_1000 0.000610865238198015f
#define GYRO_SENS_RADPS_2000 0.00122173047639603f
#define GYRO_SENS_RADPS_4000 0.00244346095279206f

#define GYRO_SENS_DEGPS_125  4.375E-03f
#define GYRO_SENS_DEGPS_250  8.75E-03f
#define GYRO_SENS_DEGPS_500  1.75E-2f
#define GYRO_SENS_DEGPS_1000 3.5E-2f
#define GYRO_SENS_DEGPS_2000 7E-2f
#define GYRO_SENS_DEGPS_4000 14E-2f

#define RAD_2_DEG 57.2957795130823f
#define DEG_2_RAD 0.0174532925199433f
#define ACCEL_G 9.80665f  // Standard gravity in m/s^2.



#define ACCEL_SENS_2G 0.00059841f
#define ACCEL_SENS_4G 0.00119682f
#define ACCEL_SENS_8G 0.00239364f
#define ACCEL_SENS_16G 0.00478728f

// Highest value in integer, needed to reach the respective dps range value.
// E.g. GYRO_SENS_DEGPS_500 * 28571 ~= 500
#define GYRO_MAXVAL_INT 28571

#define GYRO_USER_OFFSET_FACTOR 1.5

void imu_init();
void imu_power_off();
Vector imu_read_gyro();
Vector imu_read_accel();
void imu_load_calibration();
void imu_calibrate();

typedef struct calib_data_struct {
    float offset;
    float variance;
    float stddev;
} CalibData;

typedef struct imu_calib_struct {
    CalibData gyro[3];
    CalibData accel[3];
} ImuCalib;