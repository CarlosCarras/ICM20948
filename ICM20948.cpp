/****************************************************************************
 * ICM20948.cpp
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


#include "ICM20948.h"


ICM20948::ICM20948(bool debug, uint8_t bus, uint8_t address) {
	this->debug = debug;
	i2c = I2C_Functions(bus, address);
}

void ICM20948::selectBankReg(uint8_t bank) {
	i2c.write(REG_BANK_SEL, bank);
}

bool ICM20948::whoAmI() {
	selectBankReg(REG_BANK_0);
	uint8_t IMU_addr = i2c.read(WHO_AM_I);

	if(IMU_addr == 0xEA) {
		printi("IMU is functioning as expected.");
		return true; 
	}
	else {
 		printe("Unable to properly ready the IMU.");
		return false;
	}
}

/* necessary to change the IMU out of sleep mode by calling this function first */
int ICM20948::disableSleep() {
	selectBankReg(REG_BANK_0);
	uint8_t status = i2c.read(PWR_MGMT_1);
	status = BIT_CLEAR(status, 6);		// clears sleep bit
	status &= ~INT_OSC_BM;				// enables internal oscillator
	int result = i2c.write(PWR_MGMT_1, status);

	return result;
}

int ICM20948::enableSleep() {
	selectBankReg(REG_BANK_0);
	uint8_t status = i2c.read(PWR_MGMT_1);
	status = BIT_SET(status, 6);		// sets sleep bit
	int result = i2c.write(PWR_MGMT_1, status);

	return result;
}

uint16_t ICM20948::getStatus() {
	uint8_t status[2];
	status[0] = i2c.read(PWR_MGMT_1);
	status[1] = i2c.read(PWR_MGMT_2);

	/* PWR_MGMT_1 */
	uint8_t clksel = status[0] & INT_OSC_BM;
	switch(clksel){
		case 0:  printi("CLK: Internal 20MHz Oscillator."); break;
		case 6:  printi("CLK: Internal 20MHz Oscillator."); break;
		case 7:  printi("CLK: Stopped.");					break;
		default: printi("CLK: Auto.");						break;
	}

    if (BIT_VAL(status[0], 3)) printi("TEMPERATURE: Disabled.");
    else 					   printi("TEMPERATURE: Enabled.");

    if (BIT_VAL(status[0], 6)) printi("SLEEP: True.");
    else 					   printi("SLEEP: False.");

	if (BIT_VAL(status[0], 7)) printi("RESET: True.");
	else 					   printi("RESET: False.");


	/* PWR_MGMT_2 */
	uint8_t acc_en = status[1] & ACCEL_AXES_EN;
	if (acc_en == ACCEL_ALL_AXES_ON) printi("Accelerometer ON.");
	else 							 printi("Accelerometer OFF.");	
	
	uint8_t gyro_en = status[1] & GYRO_AXES_EN;
	if (gyro_en == GYRO_ALL_AXES_ON) printi("Gyroscope ON.");
	else 							 printi("Gyroscope OFF.");	

	uint16_t combined_status = ( ((uint16_t)status[0] << 8) | ((uint16_t)status[0] & 0xFF) );
	return combined_status;
}

float ICM20948::getTemperature() {
	selectBankReg(REG_BANK_0);
	uint16_t raw = i2c.read2(TEMP_OUT_H);
	float temperature = (float)((( (float)(raw - 21) )/333.87) + 21);

	if (debug && (temperature < 10 || temperature > 40)) printe("The temperature is out of the typical range for debugging.");

	return temperature;
}

ICM20948::imu_t ICM20948::getIMUData() {
	imu_t imu;

	acc_t acc = getAccData();
	gyro_t gyro = getGyroData();
	float temperature = getTemperature();

	imu.ax = acc.x;
	imu.ay = acc.y;
	imu.az = acc.z;
	imu.gx = gyro.x;
	imu.gy = gyro.y;
	imu.gz = gyro.z;
	imu.temperature = temperature;

	return imu;
}

/********************************* Accelerometer *********************************/

int ICM20948::setAccSens(uint8_t scale){
	if (scale != ACCEL_SENS_2G  && scale != ACCEL_SENS_4G  && scale != ACCEL_SENS_8G  && scale != ACCEL_SENS_16G) {
		printe("An appropriate accelerometer sensitivity scale was not selected.");
		return -1;
	}

	uint8_t config = i2c.read(ACCEL_CONFIG_1);
	config &= (config & ~SENSITIVITY_BM) | scale;		// all bits but the sensitivity bits remain unaltered
	i2c.write(ACCEL_CONFIG_1, config);

	return 0;
}

int ICM20948::getAccSens() {
	int sens; 		// units: LSB/g
	uint8_t raw;

	selectBankReg(REG_BANK_2);
	raw = i2c.read(ACCEL_CONFIG_1) & SENSITIVITY_BM;

	switch (raw) {
		case ACCEL_SENS_2G:  sens = 16384; break;
		case ACCEL_SENS_4G:  sens = 8192;  break;
		case ACCEL_SENS_8G:  sens = 4096;  break;
		case ACCEL_SENS_16G: sens = 2048;  break;
		default:             sens = -1;    break;
							 printe("Unknwon accelerometer sensitivity read.");
	}

	return sens;
}

ICM20948::acc_t ICM20948::getAccData() {
	int16_t rawAccX, rawAccY, rawAccZ; 
	acc_t acc;

	int sens = getAccSens();
	if (sens < 0) return {0.0, 0.0, 0.0};
	
	selectBankReg(REG_BANK_0);
	uint8_t raw[6];
	i2c.readn(ACCEL_XOUT_H, 6, raw);	// raw is returned with the desired data

	rawAccX = ((int16_t)raw[0] << 8) | raw[1];
	acc.x = (float)rawAccX / (float)sens;

	rawAccY = ((int16_t)raw[2] << 8) | raw[3];
	acc.y = (float)rawAccY / (float)sens;

	rawAccZ = ((int16_t)raw[4] << 8) | raw[5];
	acc.z = (float)rawAccZ / (float)sens;

	return acc;
}

/*********************************** Gyroscope ***********************************/

int ICM20948::setGyroSens(uint8_t scale){
	if (scale != GYRO_SENS_250DPS  && scale != GYRO_SENS_500DPS  && scale != GYRO_SENS_1000DPS  && scale != GYRO_SENS_2000DPS) {
		printe("An appropriate gyroscope sensitivity scale was not selected.");
		return -1;
	}

	uint8_t config = i2c.read(GYRO_CONFIG_1);
	config &= (config & ~SENSITIVITY_BM) | scale;		// all bits but the sensitivity bits remain unaltered
	i2c.write(GYRO_CONFIG_1, config);

	return 0;
}

float ICM20948::getGyroSens() {
	float sens; 	// units: LSB/g
	uint8_t raw;

	selectBankReg(REG_BANK_2);
	raw = i2c.read(GYRO_CONFIG_1) & SENSITIVITY_BM;

	switch (raw) {
		case GYRO_SENS_250DPS:  sens = 131.0; break;
		case GYRO_SENS_500DPS:  sens = 65.5;  break;
		case GYRO_SENS_1000DPS: sens = 32.8;  break;
		case GYRO_SENS_2000DPS: sens = 16.4;  break;
		default:   			    sens = -1;    break;
								printe("Unknwon gyroscope sensitivity read.");
	}

	return sens;
}

ICM20948::gyro_t ICM20948::getGyroData() {
	int16_t rawGyroX, rawGyroY, rawGyroZ; 
	gyro_t gyro;

	float sens = getGyroSens();
	if (sens < 0) return {0.0, 0.0, 0.0};

	selectBankReg(REG_BANK_0);
	uint8_t raw[6];
	i2c.readn(GYRO_XOUT_H, 6, raw);

	rawGyroX = ((int16_t)raw[0] << 8) | raw[1];
	gyro.x = (float)rawGyroX / sens;

	rawGyroY = ((int16_t)raw[2] << 8) | raw[3];
	gyro.y = (float)rawGyroY / sens;

	rawGyroZ = ((int16_t)raw[4] << 8) | raw[5];
	gyro.z = (float)rawGyroZ / sens;

	return gyro;
}