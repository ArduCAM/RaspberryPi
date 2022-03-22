# Motorized_Focus_Camera
## Preparation
## Demos Function
 Arducam has release three demos Motorized_Focus_Camera_Preview.py, Motorized_Focus_Camera_Snapshot.py and Autofocus.py 
  - Before running this demo, you have to install Python Dependency libraries.
 ```Bash
 sudo apt-get install python-opencv 
 ```
```bash
sudo chmod +x enable_i2c_vc.sh
```
```bash
./enable_i2c_vc.sh
```
Press Y to reboot

```Bash
git clone https://github.com/ArduCAM/RaspberryPi.git
```
 -Then click 'y' to allow it reboot.
### Motorized_Focus_Camera_Preview.py
 - This demo supports focusing in preview mode, You can see the focus visually
 - Single focus by keyboard up and down
 - Run this demo is very easy , just input the "sudo python Motorized_Focus_Camera_Preview.py" in the terminal.
### Autofocus.py 
 - This demo supports auto focusing in preview mode, You can see the focus visually
 - Run the demo by inputing the "sudo python Autofocus.py" in the terminal.


## Install libcamera

### python 2.7

```bash
cd libcamera_python
python -m pip install ./libcamera-1.0.2-cp27-cp27mu-linux_armv7l.whl
```

### python 3.9

```bash
cd libcamera_python
python3 -m pip install ./libcamera-1.0.2-cp39-cp39-linux_armv7l.whl
```

### Usage:

## Python demo
```bash
cd RaspberryPi/Motorized_Focus_Camera/python/
```
```bash 
python xxx.py
```
## C demo

```bash
cd RaspberryPi/Motorized_Focus_Camera/C/
```
```bash
make install
```
```bash
make 
```
```bash
./manualFocus
```
