# OV7251 13MP MIPI Camera User Guide

## 1. Driver Installation 

###	Important Note

The camera driver should match with your current Raspbian OS kernel version.
Something like:

`4.14.71-v7+`

`4.14.79-v7+`

Older Kernel version might not be supported.

For Raspberry Pi A/A+, B/B+, Pi Zero/W, Compute module V1, you need pi1 suffix version driver, for Raspberry pi 2B/3B/3B+, Compute module V3, you need pi3 suffix version driver.

###	Auto Install

Run the bash script `./preprocess.sh` and choose yes to reboot, this step is only need once.
After reboot, run the bash script `./loaddriver.sh` with the RPI board version parameter, this step should be done every time reboot the Raspbian OS. 

For example:
`./loaddriver.sh pi1`
Or
`./loaddriver.sh pi3`

###	Manual Install

Manual install the driver is explained here if you want to investigate the details or fix the problem when the driver installation.

Add the following code to the last line of /boot/config.txt file

`dtparam=i2c_vc=on`

Save the file and probe the i2c driver using the command below:

`sudo modprobe i2c-dev`

Compile the device tree for the ov7251 camera

`sudo dtc -I dts -O dtb -o /boot/overlays/arducam-ov7251.dtbo arducam-ov7251.dts`

Add the following code to the last line of /boot/config.txt file, only one Arducam camera driver allowed here.

`dtoverlay=arducam-ov7251`

Save the /boot/config.txt then reboot, and the above steps only need to be done for once.

Every time reboot the board or run after raspivid/raspistill command, and you have to run the following commands

`sudo chmod +x camera_i2c`

`sudo chmod +x rpi3-gpiovirtbuf`

`./camera_i2c`

Install the kernel driver using the following command

`./loaddepends.sh`

`sudo insmod arducam-ov7251-v4l2-pi3.ko`  (or `sudo insmod arducam-ov7251-v4l2-pi1.ko`)

After this step, you should find a new video device like /dev/video0.

## 2.	VLC Player Demo

###	Download the VLC player

You can download the vlc player (sudo `apt-get install vlc`) to open the camera for fast evaluation easily.

###	Run the VLC player

Run the VLC player by the following command

`vlc v4l2:///dev/video0`

###	V4L2 Control Functions

Arducam ov7251 camera driver supports a list of V4L2 control functions, and you can check the supported functions using the following commands:

#### 1. Check supported resolution

`v4l2-ctl --list-formats-ext`

#### 2. Check the supported controls

`v4l2-ctl -l`

#### 3. Manual Exposure Setting

`v4l2-ctl -c exposure=0x50`

#### 4. Manual Global Gain Setting

`v4l2-ctl -c gain=0x02`

#### 5. Manual Red Channel Gain Setting

`v4l2-ctl -c red_balance=0x02`

#### 6. Manual Blue Channel Gain Setting

`v4l2-ctl -c blue_balance=0x02`

#### 7. Mirror

`v4l2-ctl -c horizontal_flip=1`

#### 8. Flip

`v4l2-ctl -c vertical_flip=1`

#### 9. Manual Focus Setting

`v4l2-ctl -c focus_absolute=0x02`

#### 10. Sensor Register Access

First, you have to set the address of the register to be accessed, for example, 0x0202 in our case

`v4l2-ctl -c register_addr=0x0202`

Read the value from 0x0202 register

`v4l2-ctl -C register_val`

Write the value to 0x0202 register

`v4l2-ctl -c register_val=0x01`

## 3. Note

The VLC player uses Raspberry Pi's CPU do the video rendering, which limits the real time preview performance.
So do not maximize the VLC player window, it might be stucked.





