#!/bin/sh
raspi-gpio set 4 op
raspi-gpio set 17 op
i2cset -y 1 0x70 0x00 0x01
raspi-gpio set 17 dl #set the gpio17 low
raspi-gpio set  4 dl #set the gpio4 low
echo "Choose camera A"
libcamera-still -o camera1.jpg
i2cset -y 1 0x70 0x00 0x02
raspi-gpio set 17 dl #set the gpio17 low
raspi-gpio set  4 dh #set the gpio4 high
echo "Choose Camera B"
libcamera-still -o camera2.jpg
echo "Test OK"
