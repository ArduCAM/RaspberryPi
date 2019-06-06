# RaspberryPi
## Arducam releases new Arducam Mini cameras drivers for Raspberry Pi platform.
- Optimized the speed of SPI and Improved image transmission speed
- Support for Arducam Mini 2MP/2MP Plus 5MP plus （the default is 2mp/2mp plus）
- Support for 4 cameras work together
- Support image preview through hdmi monitor.
## Hardware Setup
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter_SPI_Camera/data/RPI_SPI_1.jpg)

![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter_SPI_Camera/data/RPI_SPI_2.jpg)
## Pins definition (BCM)
```Bash
//ArduCAM CS pins
#define     CAM_CS1         17
#define     CAM_CS2         23
#define     CAM_CS3         22
#define     CAM_CS4         24
```
```Bash
//I2c pins
#define sda_port        2 
#define scl_port        3
```
```Bash 
//SPI pins 
//set 9 (MISO), 10 (MOSI), 11 (SCLK) alt fxn 0 (SPI0)
```

## Platform configuration (bcm283x_board_driver.h)
- These #define values are specific to the BCM2835, taken from "BCM2835 ARM Peripherals"
```Bash
#define BCM2835_PERI_BASE        0x20000000
```
- Updated to BCM2836 for Raspberry Pi 2.0 
```Bash
#define BCM2835_PERI_BASE        0x3F000000
```

## Quickly start
- Install the open cv library 
```bash
sudo apt-get install libopencv-dev
```
- Install the RaspberryPi library
```bash
sudo git clone https://github.com/ArduCAM/RaspberryPi.git
```
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter_SPI_Camera/data/git_clone.png)

- Compile and run the test demo
- [Notice: Before running the test demo, you should connect your hdmi monitor, if not, the opencv will can't find the display device.]
```bash 
cd RaspberryPi/Multi_Camera_Adapter_SPI_Camera
```
```bash
sudo make 
```
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter_SPI_Camera/data/Compile.png)

```bash
sudo ./run_Arducam_Demo 
```
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter_SPI_Camera/data/rundemo.png)










