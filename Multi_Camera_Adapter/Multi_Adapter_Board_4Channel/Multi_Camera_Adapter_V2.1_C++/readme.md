# Quick start

# Installation
```Bash
sudo apt-get install wiringpi
```

# Make
```Bash
make camera
```

# Running your RPiCameras with ArduCam
Default: enables and tests all four cameras at compile time (comment out ENABLE_CAMERA_X in arducam_multi_adapter_v2.1_c.cpp to disable)
```Bash
./camera
```

## Troubleshoot
- Can't find video0 
	- check if the i2c bus is normal
		- add dtparam=i2c_vc=on to /boot/config.txt,then reboot 
		- run i2cdetect -y 0 to check the devive's address.
		- if it does not detect the device's address, please check the ribbon cable connection to ensure it is not reversed.
		
