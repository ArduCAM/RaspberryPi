# Motorized_Focus_Camera
## Preparation
```bash
pip install pygame 
```
```bash
cd RaspberryPi/Motorized_Focus_Camera
```
```bash
chmod +x enable_i2c_vc.sh
```
```bash
./enable_i2c_vc.sh
```
```bash
sudo reboot
```
## Demos Function
 Arducam has release three demos Motorized_Focus_Camera_Preview.py, Motorized_Focus_Camera_Snapshot.py and Autofocus.py 
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
 -Then click 'y' to allow it reboot.
### Motorized_Focus_Camera_Preview.py
 - This demo supports focusing in preview mode, You can see the focus visually
 - Single focus by keyboard up and down
 - Run this demo is very easy , just input the "sudo python Motorized_Focus_Camera_Preview.py" in the terminal.
### Motorized_Focus_Camera_snapshot.py
 - This demo supports focusing and save the image to the local filesystem. You can save the image after each focus.
 - Single focus by keyboard up and down
 - Run this demo is very easy , just input the "sudo python Motorized_Focus_Camera_snapshot.py" in the terminal.
### Autofocus.py 
 - This demo supports auto focusing in preview mode, You can see the focus visually
 - Run the demo by inputing the "sudo python Autofocus.py" in the terminal.
