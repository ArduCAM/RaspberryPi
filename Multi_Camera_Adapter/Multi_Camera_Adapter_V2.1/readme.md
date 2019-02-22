## Quickly start
  - Run the below command to install the PyQt5 module
  ```Bash
  sudo apt-get install python-pyqt5
  ```
  - Run the below command to install the cv2 module
  ```Bash
  sudo apt-get install python-opencv
  ```

## Configuration
- Configure the i2c1 
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/cfg_i2c1_1.png)

![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/data/cfg_i2c1_2.png)

- Run the below command to init the camera
```Bash
sudo ./init_camera.sh
```

- Run the below command to start the demo
```Bash
sudo python 4cam_cv2.py
```
  
