// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC6343A
// This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2

#include<Wire.h>

// HMC6343A I2C address is 19(25)
#define Addr 0x19

void setup() 
{
    // Initialise I2C communication as MASTER
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(9600);
    
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select Operational mode register 1
    Wire.write(0x04);
    // Set enable run mode and level orientation
    Wire.write(0x11);
    // Stop I2C Transmission
    Wire.endTransmission();
  
    // Start I2C Transmission
    Wire.beginTransmission(Addr);
    // Select Operational mode register 2
    Wire.write(0x05);
    // Set measuremnet rate 5 Hz
    Wire.write(0x01);
    // Stop I2C Transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[6];
    for(int i = 0; i < 6; i++)
    {
        // Start I2C Transmission on the device
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((64+i));
        // Stop I2C Transmission on the device
        Wire.endTransmission();

        // Request 1 byte of data from the device
        Wire.requestFrom(Addr, 1);

        // Read 6 bytes of data
        // xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
        if(Wire.available() == 1)
        {
            data[i] = Wire.read();
        }
        delay(300);
    }

    // Convert the data
    int xAccl = ((data[0] * 256) + data[1]);
    int yAccl = ((data[2] * 256) + data[3]);
    int zAccl = ((data[4] * 256) + data[5]);

    for(int i = 0; i < 6; i++)
    {
        // Start I2C Transmission on the device
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((69+i));
        // Stop I2C Transmission on the device
        Wire.endTransmission();

        // Request 1 byte of data from the device
        Wire.requestFrom(Addr, 1);

        // Read 6 bytes of data
        // xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
        if(Wire.available() == 1)
        {
            data[i] = Wire.read();
        }
        delay(300);
    }

    // Convert the data
    int xMag = ((data[0] * 256) + data[1]);
    int yMag = ((data[2] * 256) + data[3]);
    int zMag = ((data[4] * 256) + data[5]);

    // Output data to serial monitor
    Serial.print("Acceleration in X-Axis : ");
    Serial.println(xAccl);
    Serial.print("Acceleration in Y-Axis : ");
    Serial.println(yAccl);
    Serial.print("Acceleration in Z-Axis : ");
    Serial.println(zAccl);
    delay(1000);
    Serial.print("Magnetic field in X-Axis : ");
    Serial.println(xMag);
    Serial.print("Magnetic field in Y-Axis : ");
    Serial.println(yMag);
    Serial.print("Magnetic filed in Z-Axis : ");
    Serial.println(zMag);
    delay(1000);
}

