#!/bin/sh
i2cset -y 1 0x70 0x00 0x04
sudo modprobe bcm2835_v4l2 
echo "camera init OK."
