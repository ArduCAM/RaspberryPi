# Introudction
This multi-camera adapter board is designed for Raspberry pi ZERO and let your connect two 5MP cameras to Pi Zero.
Only one of the two cameras can be activated at a time, the are working in alternate ways.

It can not run two cameras at the same time, but we build a simple software which can run two cameras and make it looks working as the same time.
But the drawback is the frame rate and resolution will be fairly low, may be useful for surveillance.

Note that this version doesn't support 8MP Pi camera borad. And it also can be used for other Raspberry pi boards with minor software modifications.

# Prerequisites
You have to disable the automatic management of camera led in `/boot/config.txt`.  
```bash
$ sudo sh -c 'echo "disable_camera_led=1" >> /boot/config.txt' 
$ sudo reboot
```
# Hardware Connection
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/data/HardwareConnection.png)
# Quickly start
## Download the Raspberrypi library
```bash
git clone https://github.com/ArduCAM/RaspberryPi.git
```
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/data/download_code.png)

## Shell version

This shell script is used to test each camera 
* Run the script
```bash
cd RaspberryPi/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/shell

sudo chmod +x pi_cam.sh

sudo ./pi_cam.sh
```
* Running Demo
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/data/shell_demo.png)

## C++ version code

This demo supports Both cameras display at the same time

* Install the opencv library
```Bash
sudo apt-get install libopencv-dev
```
* Compile and run
```Bash
cd RaspberryPi/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/c++/

sudo make

sudo ./pi_cam
```
* Runing Demo
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter/Multi_Adapter_Board_2Channel/data/demo.png)
