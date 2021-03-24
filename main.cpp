#include <iostream>
#include <fstream>
#include <chrono>
#include "imu.h"

void allROSGoesHere(IMU imu) {
    imu_t data = imu.getIMUData(); 
    // float* data = imu.getIMUArr();  // alternatively, if you prefer arrays: 
    
    /* do something with data */
}


int main() {
    IMU imu();  // only one line of initialization required

    while(1) {
        allROSGoesHere(imu);
    }

    return 0;
}