/****************************************************************************
 * main_plotter.cpp
 * 
 * Hardware   : ICM20948 Inertial Measurement Unit
 * Manual     : TDK DS-000189, Revision 1.3
 * About      : Creates a .csv file with accelerometer and gyroscope data.
 * 
 * Author     : Carlos Carrasquillo
 * Date       : March 23, 2021
 * Modified   : March 23, 2021
 * Proprty of : ADAMUS Lab
 ****************************************************************************/

#include <iostream>
#include <fstream>
#include <chrono>
#include "imu.h"


int main() {
    IMU imu();  // only one line of initialization required
    imu_t data; // struct to hold all of the IMU data

    int dur = 30;                                                                       // program loops for 30 seconds
    std::chrono::steady_clock::timepoint start = std::chrono::steady_clock::now();      // start time for timer
    std::ofstream file.open("imu_test.csv");                                            // write outputs to CSV file

    while(1) {
    	data = imu.getIMUData();   // get all IMU data

        file << std::to_string(data.ax) <<  "," << std::to_string(data.ay) << "," << std::to_string(data.az) << ","
             << std::to_string(data.gx) <<  "," << std::to_string(data.gy) << "," << std::to_string(data.gz) << ","
             << std::to_string(data.temperature) << "\n";

        if (std::chrono::steady_clock::now() - start > std::chrono::seconds(dur)) break; // timer
    }

    file.close();
    return 0;
}