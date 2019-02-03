# Prerequisites
You have to disable the automatic management of camera led in `/boot/config.txt`.  
```bash
$ sudo echo "disable_camera_led=1" >> /boot/config.txt 
$ sudo reboot
```
If permission denied
you can do 

```bash
$ sudo nano /boot/config.txt
```
add "disable_camera_led=1" at the file end. Then save the file and reboot .
```bash
$ sudo reboot
```
# Installation and run

# Quickly start

## For the C++ code
cd /home/pi/RaspberryPi/Multi_Camera_Adapter/Multi_Camera_PI_ZERO/C++
```Bash
sudo apt-get install libopencv-dev
```
```Bash
make
```
```Bash
sudo ./pi_zero_cam
```
## For the Python code
```bash
	cd /home/pi/RaspberryPi/Multi_Camera_Adapter/Multi_Camera_PI_ZERO/python
```

### For the Pi zero
```bash 
	sudo python pi_zero_cam.py
```
### For the Pi zero w 
```bash 
	sudo python pi_zero_w_cam.py
```
