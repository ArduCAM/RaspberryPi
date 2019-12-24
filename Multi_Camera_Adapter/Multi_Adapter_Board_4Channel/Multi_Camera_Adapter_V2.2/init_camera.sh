#!/bin/sh
echo "----Test i2c1"
i2cdetect -y 1
echo "----Configure Adapter Board V2.1"
i2cset -y 1 0x70 0x00 0x04
echo "Adapter Board V2.1 configure OK"
echo "----install v4l2 driver"
sudo modprobe bcm2835-v4l2
echo "v4l2 driver install OK" 
echo "----Detecting the /dev/video0 device"
if [ -c "/dev/video0" ]; then
echo "video0 detected!"
echo "---Start testing each camera"
echo "if all of them are normal, after it finished,you will see four image in the current path."
sleep 2
sudo python AdapterTestDemo.py 
else
echo "Can't find video0 device, please check the hardware connection! "
fi
