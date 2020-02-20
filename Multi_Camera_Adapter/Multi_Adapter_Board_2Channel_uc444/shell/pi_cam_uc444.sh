#!/bin/sh
i2cset -y 1 0x70 0x00 0x01
gpio -g mode 17 out
gpio -g mode 4  out
gpio -g write 17 0 #set the gpio17 low
gpio -g write 4 0 #set the gpio4   low
echo "Choose camera A"
raspistill -o camera1.jpg
i2cset -y 1 0x70 0x00 0x02
gpio -g write 4 1 #set the gpio4 high
echo "Choose Camera B"
raspistill -o camera2.jpg
echo "Test OK"
