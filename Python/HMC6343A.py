# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# HMC6343A
# This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/content/Accelorometer?sku=HMC6343A_I2CS#tabs-0-product_tabset-2

import smbus
import time

# Get I2C bus
bus = smbus.SMBus(1)

# HMC6343A address, 0x19(25)
# Select operational mode register 1, 0x04(04)
#		0x11(17)	Run mode, Level orientation
bus.write_byte_data(0x19, 0x04, 0x11)
# HMC6343A address, 0x19(25)
# Select operational mode register 2, 0x05(05)
#		0x01(01)	Measurement rate = 5Hz
bus.write_byte_data(0x19, 0x05, 0x01)

time.sleep(0.5)

# HMC6343A address, 0x19(25)
# Read data back from 0x40(64), 6 bytes
# X-Axis Accl MSB, X-Axis Accl LSB, Y-Axis Accl MSB, Y-Axis Accl LSB
# Z-Axis Accl MSB, Z-Axis Accl LSB
data = bus.read_i2c_block_data(0x19, 0x40, 6)

# Convert the data
xAccl = data[0] * 256 + data[1]
if xAccl > 32767 :
	xAccl -= 65536

yAccl = data[2] * 256 + data[3]
if yAccl > 32767 :
	yAccl -= 65536

zAccl = data[4] * 256 + data[5]
if zAccl > 32767 :
	zAccl -= 65536

# HMC6343A address, 0x19(25)
# Read data back from 0x45(69), 6 bytes
# X-Axis Mag MSB, X-Axis Mag LSB, Y-Axis Mag MSB, Y-Axis Mag LSB
# Z-Axis Mag MSB, Z-Axis Mag LSB
data = bus.read_i2c_block_data(0x19, 0x45, 6)

# Convert the data
xMag = data[0] * 256 + data[1]
if xMag > 32767 :
    xMag -= 65536

yMag = data[2] * 256 + data[3]
if yMag > 32767 :
	yMag -= 65536

zMag = data[4] * 256 + data[5]
if zMag > 32767 :
	zMag -= 65536

# Output data to screen
print "Acceleration in X-Axis : %d" %xAccl
print "Acceleration in Y-Axis : %d" %yAccl
print "Acceleration in Z-Axis : %d" %zAccl
print "Magnetic field in X-Axis : %d" %xMag
print "Magnetic field in Y-Axis : %d" %yMag
print "Magnetic field in Z-Axis : %d" %zMag
