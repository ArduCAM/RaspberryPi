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
