// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC6343A
// This code is designed to work with the HMC6343_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, HMC6343A I2C address is 0x19(25)
	ioctl(file, I2C_SLAVE, 0x19);

	// Select operational mode register1(0x04)
	// Set Run mode, level orientation(0x11)
	char config[2] = {0};
	config[0] = 0x04;
	config[1] = 0x11;
	write(file, config, 2);

	// Select operational mode register2(0x05)
	// Measurement rate = 5 Hz(0x01)
	config[0] = 0x05;
	config[1] = 0x01;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x40)
	// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x40};
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Error : Input/output Error \n");
		exit(1);
	}

	// Convert the data
	int xAccl = (data[0] * 256 + data[1]);
	if(xAccl > 32767)
	{
		xAccl -= 65536;
	}

	int yAccl = (data[2] * 256 + data[3]);
	if(yAccl > 32767)
	{
		yAccl -= 65536;
	}

	int zAccl = (data[4] * 256 + data[5]);
	if(zAccl > 32767)
	{
		zAccl -= 65536;
	}

	// Output data to screen
	printf("Acceleration in X-Axis : %d \n", xAccl);
	printf("Acceleration in Y-Axis : %d \n", yAccl);
	printf("Acceleration in z-Axis : %d \n", zAccl);

	// Read 6 bytes of data
	// xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
	reg[0] = 0x45;
	write(file, reg, 1);
	read(file, data, 6);

	// Convert the values
	int xMag = (data[0] * 256 + data[1]);
	if(xMag > 32767)
	{
		xMag -= 65536;
	}	
	
	int yMag = (data[2] * 256 + data[3]);
	if(yMag > 32767)
	{
		yMag -= 65536;
	}
	
	int zMag = (data[4] * 256 + data[5]);
	if(zMag > 32767)
	{
		zMag -= 65536;
	}

	// Output data to screen
	printf("Magnetic field in X-Axis : %d \n", xMag);
	printf("Magnetic field in Y-Axis : %d \n", yMag);
	printf("Magnetic field in Z-Axis : %d \n", zMag);
}
