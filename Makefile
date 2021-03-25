CC= gcc
CCC= g++

CFLAGS= -Wall
CPPFLAGS= $(CFLAGS)
# BINS= imu_test i2clib.a


all: testros

main.o: main.cpp
	$(CCC) $(CPPFLAGS) -c main.cpp -o main.o

imu.o: imu.h imu.cpp
	$(CCC) $(CPPFLAGS) -c imu.cpp -o imu.o

ICM20948.o: ICM20948.h ICM20948.cpp
	$(CCC) $(CPPFLAGS) -c ICM20948.cpp -o ICM20948.o

I2C_Functions.o: I2C_Functions.h I2C_Functions.cpp
	$(CCC) $(CPPFLAGS) -c I2C_Functions.cpp -o I2C_Functions.o

lsquaredc.o: lsquaredc.h lsquaredc.c
	$(CC) $(CFLAGS) -c lsquaredc.c -o lsquaredc.o

testros: lsquaredc.o I2C_Functions.o ICM20948.o imu.o main.o
	$(CCC) $(CPPFLAGS) -o testros main.o imu.o ICM20948.o I2C_Functions.o

testplot: lsquaredc.o I2C_Functions.o ICM20948.o imu.o main_plotter.o
	$(CCC) $(CPPFLAGS) -o testplot main_plotter.o imu.o ICM20948.o I2C_Functions.o


# i2clib.a: libi2c.o
#	 ar rcs i2clib.a libi2c.o lsquaredc.o

clean:
	rm -rf *.o testros testplot