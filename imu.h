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


#ifndef IMU_H
#define IMU_H


/********************************* Includes *********************************/
#include "ICM20948.h"


/********************************* Defines **********************************/

#define DEBUG true


/*********************************** IMU ************************************/

class IMU {
private:
	ICM20948 imu(DEBUG);

	/* Debug Functions */
	bool debug;
	void printe(std::string str) { if (debug) std::cout << "ERROR: " << str << " (imu.cpp)" << std::endl; }
	void printi(std::string str) { if (debug) std::cout << "INFO: " << str << " (imu.cpp)" << std::endl; }

public:
	explicit IMU(bool debug = false);
	bool isActive();
	void disableSleep();
	void enableSleep();
	float getTemperature();
	uint8_t* getStatus();
	imu_t getIMUData();
	float* getIMUArr();

	/* accelerometer */
	acc_t getAccData();
	float* getAccArr();
	int getAccSens();
	void setAccSens(uint8_t scale);

	/* gyroscope */
	gyro_t getGyroData();
	float* getGyroArr();
	int getGyroSens();
	void setGyroSens(uint8_t scale);

};

#endif // IMU_H

