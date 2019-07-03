# RaspberryPi
## SPI Camera
- https://github.com/ArduCAM/RaspberryPi/tree/master/SPI_Camera

## Multi_Camera_Adapter_V2
- This adapter bard just supports 5MP
Raspberry Pi multi camera adapter module is designed for connecting more than one camera to a single CSI camera port on Raspberry Pi board. One adapter board can connect 4 cameras and user can stack up maxim 4 adapter boards that means up to 16 cameras on a single Raspberry Pi board. Please note that Raspberry Pi multi camera adapter board is nascent product that may have some stability issues and limitations because of the cable’s signal integrity and RPi’s closed source video core libraries, so use it on your own risk.
For more information, refer to the link
- http://www.arducam.com/multi-camera-adapter-module-raspberry-pi/#more-952

## Multi_Camera_Adapter_V2.1
- This adapter bard supports 5MP and 8MP
ince the old multi-camera adapter board doesn’t work with Raspberry Pi 8MP IMX219 pi camera due to the camera driver issue. Now Arducam team designed upgrade V2.1 multi-camera adapter board for both 5MP and 8MP pi cameras. Consider that the high speed CSI camera MIPI signal integrity is sensitive to long cable connection, the new adapter board can only connect maximum 4 cameras to a single pi board. Because it covers the most of the use cases like 360 degree view photography and surveillance, and add more cameras will degrade the performance of the cameras. Like always please note that Raspberry Pi multi camera adapter board is nascent product that may have some stability issues and limitations because of the cable’s signal integrity and RPi’s closed source video core libraries, so use it on your own risk.
- http://www.arducam.com/arducam-v2-1-multi-camera-adapter-board-raspberry-pi-8mp-camera-imx219/
### Hardware Connection
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/Multi_Camera_Shield_V2.1.png)

## Motorized_Focus_Camera
- The standard pi camera is fixed focus lens, some of them are glued which can not be adjust by users, and some of them are not glued which can be adjust the lens focus manually. But in certian circumstance, you want to programmably control the focus distance remotely where the device can not be reached. The ArduCAM team now designed motorized control focus 5MP and 8MP camera module for Raspberry Pi series boards to address this problem.
http://www.arducam.com/programmable-motorized-focus-camera-raspberry-pi/
### Pi3
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/AutoFocus_diagram.png)
### Pi4
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/Motorized_Focus_Camera_Pi4.jpg)
- Demos Function
 - Motorized_Focus_Camera_Preview.py
   - This demo support focusing in preview mode, You can see the focus visually
   - Single focus by keyboard up and down
   - Run this demo is very easy , just input the "sudo python Motorized_Focus_Camera_Preview.py" in the terminal.
 - Motorized_Focus_Camera_snapshot.py
    - This demo support focusing and save the image to the local filesystem. You can save the image after each focus.
    - Single focus by keyboard up and down
    - Run this demo is very easy , just input the "sudo python Motorized_Focus_Camera_snapshot.py" in the terminal.
 - Autofocus.py 
    - Before running this demo, you have to install Python Dependency libraries.
    ```Bash
    sudo apt-get install python-opencv 
    ```
    - Enable the I2C0 adapter by running below commands
    ```Bash
    chmod +x enable_i2c_vc.sh
    ```
    ```Bash
    ./enable_i2c_vc.sh
    ```
 - Then click 'y' to allow it reboot.
 - Then run the demo by inputing the "sudo python Autofocus.py" in the terminal.
