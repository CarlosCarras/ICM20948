/****************************************************************************
 * ICM20948.h
 * 
 * Hardware   : ICM20948 Inertial Measurement Unit
 * Manual     : TDK DS-000189, Revision 1.3
 * About      : The class definition to interface with the 9-axis IMU.
 * 
 * Author     : Carlos Carrasquillo
 * Date       : March 23, 2021
 * Modified   : March 23, 2021
 * Proprty of : ADAMUS Lab
 ****************************************************************************/


#ifndef ICM20948_H
#define ICM20948_H

/********************************* Includes *********************************/
#include <stdio.h>
#include <string>
#include <iostream>
#include "I2C_Functions.h"

/********************************** Defines *********************************/
/*
 * slave address is 0b110100X. LSB bit is determined by the logic level on pin AD0.
 * slave address when pin AD0 is logic high --> 0b1101001 --> default value
 */
#define IMU_I2C_ADDR 0x69 		// slave address when pin AD0 is logic high

/* User Banks */
#define REG_BANK_0 (0 << 4)
#define REG_BANK_1 (1 << 4)
#define REG_BANK_2 (2 << 4)
#define REG_BANK_3 (3 << 4)

/* User Bank Register 0 definitions */
#define WHO_AM_I     0x00  		// 0xEA by default
#define PWR_MGMT_1   0x06
#define PWR_MGMT_2	 0x07
#define ACCEL_XOUT_H 0x2D
#define ACCEL_XOUT_L 0x2E 
#define ACCEL_YOUT_H 0x2F
#define ACCEL_YOUT_L 0x30
#define ACCEL_ZOUT_H 0x31
#define ACCEL_ZOUT_L 0x32
#define GYRO_XOUT_H  0x33
#define GYRO_XOUT_L  0x34
#define GYRO_YOUT_H  0x35
#define GYRO_YOUT_L  0x36
#define GYRO_ZOUT_H  0x37
#define GYRO_ZOUT_L  0x38
#define TEMP_OUT_H   0x39
#define TEMP_OUT_L   0x3A
#define REG_BANK_SEL 0x7F 			// write to this register to select a register bank

/* User Bank Register 2 definitions */
#define GYRO_CONFIG_1  0x01 		// used to find sensitivity of Gyroscope
#define ACCEL_CONFIG_1 0x14 		// used to find sensitivity of acceleration

/* Sensitivity Definitions */
#define ACCEL_SENS_2G  (0b00 << 1)
#define ACCEL_SENS_4G  (0b01 << 1)
#define ACCEL_SENS_8G  (0b10 << 1)
#define ACCEL_SENS_16G (0b11 << 1)

#define GYRO_SENS_250DPS  (0b00 << 1)
#define GYRO_SENS_500DPS  (0b01 << 1)
#define GYRO_SENS_1000DPS (0b10 << 1)
#define GYRO_SENS_2000DPS (0b11 << 1)

/* Bitmasks */
#define SENSITIVITY_BM (0b11 << 1) 		// both gyroscope and accelerometer use the same bitmask
#define INT_OSC_BM     (0b111 << 0) 	// use internal 20MHz oscillator (see PWR_MGMT_1, pp. 37)
#define ACCEL_AXES_EN  (0b111 << 3)		// accelerometer axes enable bits
#define GYRO_AXES_EN   (0b111 << 0)		// gyroscope axes enable bits

/* Miscellaneous */
#define ACCEL_ALL_AXES_ON (0b000 << 3)
#define GYRO_ALL_AXES_ON  (0b000 << 0)

/********************************* ICM20948 *********************************/

class ICM20948 {
private:
	I2C_Functions i2c;

	void selectBankReg(uint8_t bank);

    /* Debug Functions */
    bool debug;
    void printe(std::string str) { if (debug) std::cout << "ERROR: " << str << " (ICM20948.cpp)" << std::endl; }
	void printi(std::string str) { if (debug) std::cout << "INFO: " << str << " (ICM20948.cpp)" << std::endl; }

public:
	struct acc_t {
		float x, y, z;
	};

	struct gyro_t {
		float x, y, z;
	};

	struct imu_t {
    	float ax, ay, az;
    	float gx, gy, gz;
    	float temperature;
	};

	explicit ICM20948(bool debug = false, uint8_t bus = 2, uint8_t address = IMU_I2C_ADDR);
	int disableSleep();
	int enableSleep();
	bool whoAmI();
	uint16_t getStatus();
	float getTemperature();
	ICM20948::imu_t getIMUData();

	/* accelerometer */
	ICM20948::acc_t getAccData();
	int getAccSens();
	int setAccSens(uint8_t scale);

	/* gyroscope */
	ICM20948::gyro_t getGyroData();
	float getGyroSens();
	int setGyroSens(uint8_t scale);
};

#endif	// ICM20948_H