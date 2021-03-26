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


IMU::IMU(bool debug) {
	this->debug = debug;
	int status = 0;

	imu = ICM20948(debug);
	status += imu.setAccSens(ACCEL_SENS_4G);
	status += imu.setGyroSens(GYRO_SENS_500DPS);
	status += imu.disableSleep();	// necessary!

	if (status < 0) printe("IMU could not be initialized.");
}

bool IMU::isActive() {
	return imu.whoAmI();
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

uint16_t IMU::getStatus() {
	return imu.getStatus();
}

ICM20948::imu_t IMU::getIMUData() {
	return imu.getIMUData();
}

float* IMU::getIMUArr(float* arr) {
	/* requires {uint8_t arr[7];} prior to call. the values are returned in the 'arr' variable. */
	ICM20948::imu_t data = getIMUData();
	arr[0] = data.ax; 
	arr[1] = data.ay; 
	arr[2] = data.az; 
	arr[3] = data.gx; 
	arr[4] = data.gy; 
	arr[5] = data.gz; 
	arr[6] = data.temperature;

	return arr;
}

void IMU::updateIMU() {
	ICM20948::imu_t data = getIMUData();
	ax = data.ax; 
	ay = data.ay; 
	az = data.az; 
	gx = data.gx; 
	gy = data.gy; 
	gz = data.gz; 
	temperature = data.temperature;
}


/****************************** Accelerometer *******************************/

ICM20948::acc_t IMU::getAccData() {
	return imu.getAccData();
}

float* IMU::getAccArr(float* arr) {
	/* requires {uint8_t arr[3];} prior to call. the values are returned in the 'arr' variable. */
	ICM20948::acc_t data = getAccData();
	arr[0] = data.x;
	arr[1] = data.y, 
	arr[2] = data.z;
	return  arr;
}

void IMU::updateAcc() {
	ICM20948::acc_t data = getAccData();
	ax = data.x; 
	ay = data.y; 
	az = data.z; 
}


int IMU::getAccSens() {
	return imu.getAccSens();
}

void IMU::setAccSens(uint8_t scale) {
	imu.setAccSens(scale);
}

/******************************** Gyroscope *********************************/

ICM20948::gyro_t IMU::getGyroData() {
	return imu.getGyroData();
}

float* IMU::getGyroArr(float* arr) {
	/* requires {uint8_t arr[3];} prior to call. the values are returned in the 'arr' variable. */
	ICM20948::gyro_t data = getGyroData();
	arr[0] = data.x;
	arr[1] = data.y, 
	arr[2] = data.z;

	return  arr;
}

void IMU::updateGyro() {
	ICM20948::gyro_t data = getGyroData();
	gx = data.x; 
	gy = data.y; 
	gz = data.z; 
}


int IMU::getGyroSens() {
	return imu.getGyroSens();
}

void IMU::setGyroSens(uint8_t scale) {
	imu.setGyroSens(scale);
}