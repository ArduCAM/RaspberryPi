# Quick start

# Installation
```Bash
sudo apt-get install wiringpi
```
```Bash
sudo apt-get upgrade
sudo apt install -y python3-kms++ python3-libcamera
sudo apt install -y python3-pyqt5 python3-prctl libatlas-base-dev ffmpeg
sudo pip3 install numpy --upgrade
sudo pip3 install picamera2
```
## Add support for Pi4 B
- Just a quick post to let you know that you’ll need a new wiringPi for the Raspberry Pi 4B.
- To upgrade:
```bash
cd /tmp
```
```bash
wget https://project-downloads.drogon.net/wiringpi-latest.deb
```
```bash
sudo dpkg -i wiringpi-latest.deb
```
# test

- Download the RaspberryPi library 

```Bash
git clone https://github.com/ArduCAM/RaspberryPi.git
```
```Bash
cd RaspberryPi/Multi_Camera_Adapter/Multi_Adapter_Board_4Channel/Multi_Camera_Adapter_V2.2_python/ 
```
```Bash
sudo chmod +x init_camera.sh
```
```Bash
sudo ./init_camera.sh
```

# Running your RPiCameras with ArduCam
```Bash
cd RaspberryPi/Multi_Camera_Adapter/Multi_Adapter_Board_4Channel/Multi_Camera_Adapter_V2.2_python/ 
```
* For the use of remote control terminals, first specify the location of the image display
```Bash
export DISPLAY=:0
```
```Bash
 python previewOpencv.py
```
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/Multi_Camera_Adapter_Board_openCV3.0.png)


## Troubleshoot
- Can't find video0 
	- check if the i2c bus is normal
		- add dtparam=i2c_vc=on to /boot/config.txt,then reboot 
		- run i2cdetect -y 0 to check the devive's address.
		- if it does not detect the device's address, please check the ribbon cable connection to ensure it is not reversed.
		
