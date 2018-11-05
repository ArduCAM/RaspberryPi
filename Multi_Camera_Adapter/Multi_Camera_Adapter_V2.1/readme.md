## Quickly start
- Run the below command to init the camera
```Bash
sudo ./init_camera.sh
```

- Run the below command to start the demo
```Bash
sudo python 4cam_cv2.py
```

## FAQ
- from PyQt5 import QtCore, QtGui, QtWidgets
  ImportError: No module named PyQt5
![Alt text](https://github.com/ArduCAM/RaspberryPi/blob/master/Multi_Camera_Adapter/Multi_Camera_Adapter_V2.1/error1.png)

  ```Bash
  - Run the below command to install the PyQt5 module
  sudo apt-get install python-pyqt5 
  ```
  - from PyQt5 import QtCore, QtGui, QtWidgets
- ImportError: No module named cv2
  
