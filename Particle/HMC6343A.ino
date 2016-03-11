// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// HMC6343A
// This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// HMC6343A I2C address is 19(25)
#define Addr 0x19

int xAccl = 0, yAccl =  0, zAccl = 0, xMag = 0, yMag =  0, zMag = 0;
void setup() 
{
    // Set variable
    Particle.variable("i2cdevice", "HMC6343A");
    Particle.variable("xAccl", xAccl);
    Particle.variable("yAccl", yAccl);
    Particle.variable("zAccl", zAccl);
    Particle.variable("xMag", xMag);
    Particle.variable("yMag", yMag);
    Particle.variable("zMag", zMag);
    
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
        // Start I2C Transmission
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((64+i));
        // Stop I2C Transmission
        Wire.endTransmission();
        
        // Request 1 byte of data
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
    if(xAccl > 32767)
    {
        xAccl -= 65536;
    }
    int yAccl = ((data[2] * 256) + data[3]);
    if(yAccl > 32767)
    {
        yAccl -= 65536;
    }
    int zAccl = ((data[4] * 256) + data[5]);
    if(zAccl > 32767)
    {
        zAccl -= 65536;
    }

    for(int i = 0; i < 6; i++)
    {
        // Start I2C Transmission
        Wire.beginTransmission(Addr);
        // Select data register
        Wire.write((69+i));
        // Stop I2C Transmission
        Wire.endTransmission();
        
        // Request 1 byte of data
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
    if(xMag > 32767)
    {
        xMag -= 65536;
    }
    int yMag = ((data[2] * 256) + data[3]);
    if(yMag > 32767)
    {
        yMag -= 65536;
    }
    int zMag = ((data[4] * 256) + data[5]);
    if(zMag > 32767)
    {
        zMag -= 65536;
    }

    // Output data to dashboard
    Particle.publish("Acceleration in X-Axis : ", String(xAccl));
    Particle.publish("Acceleration in Y-Axis : ", String(yAccl));
    Particle.publish("Acceleration in Z-Axis : ", String(zAccl));
    delay(1000);
    Particle.publish("Magnetic field in X-Axis : ", String(xMag));
    Particle.publish("Magnetic field in Y-Axis : ", String(yMag));
    Particle.publish("Magnetic filed in Z-Axis : ", String(zMag));
    delay(1000);
}

