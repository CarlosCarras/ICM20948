/****************************************************************************
 * imu.h
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


#ifndef D3_IMU_H
#define D3_IMU_H


/********************************* Includes *********************************/
#include <stdio.h>
#include <string>
#include <iostream>
#include "ICM20948.h"


/********************************* Defines **********************************/



/*********************************** IMU ************************************/

class IMU {
private:
	ICM20948 imu;

	/* Debug Functions */
	bool debug;
	void printe(std::string str) { if (debug) std::cout << "ERROR: " << str << " (imu.cpp)" << std::endl; }
	void printi(std::string str) { if (debug) std::cout << "INFO: " << str << " (imu.cpp)" << std::endl; }

public:
	float ax, ay, az, gx, gy, gz, temperature;

	explicit IMU(bool debug = false);
	bool isActive();
	void disableSleep();
	void enableSleep();
	float getTemperature();
	uint16_t getStatus();
	ICM20948::imu_t getIMUData();
	float* getIMUArr(float* arr);
	void updateIMU();

	/* accelerometer */
	ICM20948::acc_t getAccData();
	float* getAccArr(float* arr);
	void updateAcc();
	int getAccSens();
	void setAccSens(uint8_t scale);

	/* gyroscope */
	ICM20948::gyro_t getGyroData();
	float* getGyroArr(float* arr);
	void updateGyro();
	int getGyroSens();
	void setGyroSens(uint8_t scale);

};

#endif // D3_IMU_H

