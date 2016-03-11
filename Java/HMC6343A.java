// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC6343A
// This code is designed to work with the HMC6343_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class HMC6343A
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, HMC6343A I2C address is 0x19(25)
		I2CDevice device = bus.getDevice(0x19);
		Thread.sleep(500);

		// Select operational mode register1
		// Set Run mode, level orientation
		device.write(0x04, (byte)0x11);
		// Select operational mode register2
		// Measurement rate = 5 Hz
		device.write(0x05, (byte)0x01); 
		Thread.sleep(500);

		// Read 6 bytes of data
		// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
		byte[] data = new byte[6];
		device.read(0x40, data, 0, 6);

		// Convert the data
		int xAccl = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
		if(xAccl > 32767)
		{
			xAccl -= 65536;
		}	

		int yAccl = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));
		if(yAccl > 32767)
		{
			yAccl -= 65536;
		}

		int zAccl = ((data[4] & 0xFF) * 256 + (data[5] & 0xFF));
		if(zAccl > 32767)
		{
			zAccl -= 65536;
		}

		// Read 6 bytes of data
		// xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
		device.read(0x45, data, 0, 6);
		int xMag = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
		if(xMag > 32767)
		{
			xMag -= 65536;
		}	
		
		int yMag = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));
		if(yMag > 32767)
		{
			yMag -= 65536;
		}
		
		int zMag = ((data[4] & 0xFF) * 256 + (data[5] & 0xFF));
		if(zMag > 32767)
		{
			zMag -= 65536;
		}

		// Output data to screen
		System.out.printf("Acceleration in X-Axis : %d %n", xAccl);
		System.out.printf("Acceleration in Y-Axis : %d %n", yAccl);
		System.out.printf("Acceleration in Z-Axis : %d %n", zAccl);
		System.out.printf("Magnetic field in X-Axis : %d %n", xMag);
		System.out.printf("Magnetic field in Y-Axis : %d %n", yMag);
		System.out.printf("Magnetic field in Z-Axis : %d %n", zMag);
	}	
}
