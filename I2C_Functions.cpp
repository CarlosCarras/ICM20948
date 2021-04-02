 /****************************************************************************
 * I2C_Functions.cpp
 *
 * @about      : A class to simplify the use of the I2C bus on Linux.
 * @author     : Carlos Carrasquillo
 * @contact    : c.carrasquillo@ufl.edu
 * @date       : July 16, 2020
 * @modified   : March 23, 2021
 *
 * Property of ADAMUS lab, University of Florida.
 ****************************************************************************/

#include "I2C_Functions.h"

I2C_Functions::I2C_Functions() {
	I2CBus = 0;
	set_address(0);
}

I2C_Functions::I2C_Functions(uint8_t bus, uint8_t device_addr, bool endianness) {
	I2CBus = bus;
	std::cout << "New Device Created! Device Address: " << std::hex << static_cast<int>(device_addr) << "\n" << std::endl;
	set_address(device_addr);
	this->endianness = endianness;
}

void I2C_Functions::set_address(uint8_t new_addr) {
	I2CAddr_Write = (new_addr << 1) | 0;
	I2CAddr_Read = (new_addr << 1) | 1;

	if (new_addr != 0) {
		std::cout << "Device Write Address: " << std::hex << static_cast<int>(I2CAddr_Write) << std::endl;
		std::cout << "Device Read Address: " << std::hex << static_cast<int>(I2CAddr_Read) << std::endl;

	}
}

uint8_t I2C_Functions::get_address() {
	return (I2CAddr_Write >> 1) & 0x7F;
}

int I2C_Functions::write(uint8_t reg, uint8_t data) {
	int status; 

	uint16_t write_sequence[] = {I2CAddr_Write, reg, data};

	int handle = i2c_open(I2CBus);
	status = i2c_send_sequence(handle, write_sequence, 3, 0);
	i2c_close(handle);

	return status;
}

int I2C_Functions::write2(uint8_t reg, uint16_t data) {
	int status; 

	uint8_t low = (data >> 0) & 0xFF;
	uint8_t high = (data >> 8) & 0xFF;

	if (endianness == BIG_ENDIAN) {
		write(reg, high);
		status = write(reg+1, low);
	} else {
		write(reg, low);
		status = write(reg+1, high);
	}

	return status;
}

int I2C_Functions::writen(uint8_t reg, uint8_t* data, int n) {
	int status; 

	int m = 2;									// initial write sequence length
	int write_seq_len = n+m;
	uint16_t write_sequence[write_seq_len] = {I2CAddr_Write, reg};
	int j = 0;									// data byte counter
	for (int i = m; i < write_seq_len; i++) {
		write_sequence[i] = data[j++];
	}

	int handle = i2c_open(I2CBus);
	status = i2c_send_sequence(handle, write_sequence, write_seq_len, 0);
	i2c_close(handle);

	return status;
}

uint8_t I2C_Functions::read(uint8_t reg) {
	uint16_t read_sequence[] = {I2CAddr_Write, reg, I2C_RESTART, I2CAddr_Read, I2C_READ};
	uint8_t data_received[1] = {0};

	int handle = i2c_open(I2CBus);
	i2c_send_sequence(handle, read_sequence, 5, &data_received[0]);
	i2c_close(handle);

	return data_received[0];
}

uint16_t I2C_Functions::read2(uint8_t reg) {
	uint16_t read_sequence[] = {I2CAddr_Write, reg, I2C_RESTART, I2CAddr_Read, I2C_READ, I2C_READ};
	uint8_t data_received[2] = {0};

	int handle = i2c_open(I2CBus);
	i2c_send_sequence(handle, read_sequence, 6, &data_received[0]);
	i2c_close(handle);

	uint16_t data_read;
	if (endianness == BIG_ENDIAN) data_read = (((uint16_t)data_received[0])<<8) | ((uint16_t)data_received[1]);
	else 						  data_read = (((uint16_t)data_received[1])<<8) | ((uint16_t)data_received[0]);
	
	return data_read;
}

uint8_t* I2C_Functions::readn(uint8_t reg, int n) {
	int m = 4;					// initial read sequence length
	int read_seq_len = m+n;
	uint16_t read_sequence[read_seq_len] = {I2CAddr_Write, reg, I2C_RESTART, I2CAddr_Read};
	for (int i = m; i < read_seq_len; i++) {
		read_sequence[i] = I2C_READ;
	}
	uint8_t data_received[n] = {0};

	int handle = i2c_open(I2CBus);
	i2c_send_sequence(handle, read_sequence, read_seq_len, &data_received[0]);
	i2c_close(handle);

	return &data_received[0];
}

/********************* Visualize *******************/

void I2C_Functions::print_uint8(std::string descriptor, uint8_t data) {
	std::cout << descriptor << ": " << std::hex << static_cast<int>(data) << std::endl;
}

void I2C_Functions::print_uint16(std::string descriptor, uint16_t data) {
	std::cout << descriptor << ": " << std::hex << static_cast<int>(data) << std::endl;
}