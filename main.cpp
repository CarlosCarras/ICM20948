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
<<<<<<< HEAD
    printf("gx: %f, gy: %f, gz: %f\n", imu->gx, imu->gy, imu->gz);
    printf("temperature: %f\n", imu->temperature);
=======
>>>>>>> 1dd15c3e444486d9f2a9d884ca7fd5200d4618e0
}


int main() {
    IMU imu(DEBUG);  		// only two lines of initialization required
    IMU* imu_ptr = &imu;

    while(1) {
        allROSGoesHere(imu_ptr);
    }

    return 0;
}