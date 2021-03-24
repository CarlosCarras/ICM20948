/****************************************************************************
 * imu.cpp
 * 
 * Hardware   : ICM20948 Inertial Measurement Unit
 * Manual     : TDK DS-000189, Revision 1.3
 * About      : Wrapper class to interface with the ICM20948 library.
 * 
 * Author     : Carlos Carrasquillo
 * Date       : March 23, 2021
 * Modified   : March 23, 2021
 * Proprty of : ADAMUS Lab
 * 
 * NOTE       : Calibration was not deemed necessary for this IMU.
 ****************************************************************************/


#include "imu.h"


IMU::IMU() {
	debug = DEBUG;
	int status = 0;

	status += imu.setAccSens(ACCEL_SENS_4G);
	status += imu.setGyroSens(GYRO_SENS_500DPS);
	status += disableSleep();	// necessary!

	if (status < 0) printe("IMU could not be initialized.");
}

bool IMU::isActive() {
	return whoAmI();
}

void IMU::disableSleep() {
	imu.disableSleep();
}

void IMU::enableSleep() {
	imu.enableSleep();
}

float IMU::getTemperature() {
	return imu.getTemperature();
}

uint8_t* IMU::getStatus() {
	return imu.getStatus();
}

imu_t IMU::getIMUData() {
	return imu.getIMUData();
}

float* IMU::getIMUArr() {
	imu_t data = getIMUData();
	float imu_arr[7] = {data.ax, data.ay, data.az, data.gx, data.gy, data.gz, data.temperature};
	return imu_arr;
}

/****************************** Accelerometer *******************************/

acc_t IMU::getAccData() {
	return imu.getAccData();
}

float* IMU::getAccArr() {
	acc_t data = getAccData();
	float acc_arr[3] = {data.x, data.y, data.z};
	return  acc_arr;
}

int IMU::getAccSens() {
	return imu.getAccSens();
}

void IMU::setAccSens(uint8_t scale) {
	imu.setAccSens(scale);
}

/******************************** Gyroscope *********************************/

gyro_t IMU::getGyroData() {
	return imu.getGyroData();
}

float* IMU::getGyroArr() {
	gryo_t data = getGyroData();
	float gyro_arr[3] = {data.x, data.y, data.z};
	return  gyro_arr;
}

int IMU::getGyroSens() {
	return imu.getGyroSens();
}

void IMU::setGyroSens(uint8_t scale) {
	imu.setGyroSens(scale);
}