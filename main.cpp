#include <iostream>
#include <fstream>
#include <chrono>
#include "imu.h"
#include "ICM20948.h"	// only necessary if you want return values to be structs

#define DEBUG true

void allROSGoesHere(IMU* imu) {
    // ICM20948::imu_t data = imu->getIMUData(); 	// alternatively, if you prefer data.ax, data.gx, etc. notation
    // float data[7]; imu.getIMUArr(data);  		// alternatively, if you prefer arrays
    imu->updateIMU();   							// alternatively, if you prefer the imu->ax, imu->gx, etc. notation

    /* do something with data */
    printf("ax: %f, ay: %f, az: %f\n", imu->ax, imu->ay, imu->az);
    printf("gx: %f, gy: %f, gz: %f\n", imu->gx, imu->gy, imu->gz);
    printf("temperature: %f\n", imu->temperature);
}


int main() {
    IMU imu(DEBUG);  		// only two lines of initialization required
    IMU* imu_ptr = &imu;

    imu_ptr->getStatus();
    for(int i = 0; i < 5; i++) {
        allROSGoesHere(imu_ptr);
    }
    imu_ptr->getStatus();

    return 0;
}
