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

#define DEBUG true

int main() {
    IMU imu(DEBUG);  // only one line of initialization required

    int dur = 30;                                                                       // program loops for 30 seconds
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();     // start time for timer
    
    std::ofstream file;
    file.open("imu_test.csv");                                                          // write outputs to CSV file

    while(1) {
    	imu.updateIMU();   // update all IMU data

        file << std::to_string(imu.ax) <<  "," << std::to_string(imu.ay) << "," << std::to_string(imu.az) << ","
             << std::to_string(imu.gx) <<  "," << std::to_string(imu.gy) << "," << std::to_string(imu.gz) << ","
             << std::to_string(imu.temperature) << "\n";

        if (std::chrono::system_clock::now() - start > std::chrono::seconds(dur)) break; // timer
    }

    file.close();
    return 0;
}