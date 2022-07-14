
from ast import Try
from PyQt5.QtWidgets import QLabel, QHBoxLayout, QVBoxLayout, QApplication, QWidget
from picamera2 import Picamera2
from PyQt5.QtGui import QImage,QPixmap
from PyQt5.QtCore import QThread
import RPi.GPIO as gp
import time
import os

width = 320
height = 240 

adapter_info = {  
    "A" : {   
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x04",
        "gpio_sta":[0,0,1],
    }, "B" : {
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x05",
        "gpio_sta":[1,0,1],
    }, "C" : {
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x06",
        "gpio_sta":[0,1,0],
    },"D" : {
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x07",
        "gpio_sta":[1,1,0],
    }
}

class WorkThread(QThread):

    def __init__(self):
        super(WorkThread,self).__init__()
        gp.setwarnings(False)
        gp.setmode(gp.BOARD)
        gp.setup(7, gp.OUT)
        gp.setup(11, gp.OUT)
        gp.setup(12, gp.OUT)


    def select_channel(self,index):
        channel_info = adapter_info.get(index)
        if channel_info == None:
            print("Can't get this info")
        gpio_sta = channel_info["gpio_sta"] # gpio write
        gp.output(7, gpio_sta[0])
        gp.output(11, gpio_sta[1])
        gp.output(12, gpio_sta[2])

    def init_i2c(self,index):
        channel_info = adapter_info.get(index)
        os.system(channel_info["i2c_cmd"]) # i2c write

    def run(self):
        global picam2
        # picam2 = Picamera2()
        # picam2.configure( picam2.still_configuration(main={"size": (320, 240),"format": "BGR888"},buffer_count=1))

        flag = False

        for item in {"A","B","C","D"}:
            try:
                self.select_channel(item)
                self.init_i2c(item)
                time.sleep(0.5) 
                if flag == False:
                    flag = True
                else :
                    picam2.close()
                    # time.sleep(0.5) 
                print("init1 "+ item)
                picam2 = Picamera2()
                picam2.configure(picam2.create_still_configuration(main={"size": (320, 240),"format": "BGR888"},buffer_count=2)) 
                picam2.start()
                time.sleep(2)
                picam2.capture_array(wait=False)
                time.sleep(0.1)
            except Exception as e:
                print("except: "+str(e))

        while True:
            for item in {"A","B","C","D"}:
                self.select_channel(item)
                time.sleep(0.02)
                try:
                    buf = picam2.capture_array()
                    buf = picam2.capture_array()
                    cvimg = QImage(buf, width, height,QImage.Format_RGB888)
                    pixmap = QPixmap(cvimg)
                    if item == 'A':
                        image_label.setPixmap(pixmap)
                    elif item == 'B':
                        image_label2.setPixmap(pixmap)
                    elif item == 'C':
                        image_label3.setPixmap(pixmap)
                    elif item == 'D':
                        image_label4.setPixmap(pixmap)
                except Exception as e:
                    print("capture_buffer: "+ str(e))

app = QApplication([])
window = QWidget()
layout_h = QHBoxLayout()
layout_h2 = QHBoxLayout()
layout_v = QVBoxLayout()
image_label = QLabel()
image_label2 = QLabel()
image_label3 = QLabel()
image_label4 = QLabel()

# picam2 = Picamera2()

work = WorkThread()

if __name__ == "__main__":
    image_label.setFixedSize(320, 240)
    image_label2.setFixedSize(320, 240)
    image_label2.setFixedSize(320, 240)
    image_label2.setFixedSize(320, 240)
    window.setWindowTitle("Qt Picamera2 Arducam Multi Camera Demo")
    layout_h.addWidget(image_label)    
    layout_h.addWidget(image_label2)
    layout_h2.addWidget(image_label3)
    layout_h2.addWidget(image_label4)
    layout_v.addLayout(layout_h,20)
    layout_v.addLayout(layout_h2,20)
    window.setLayout(layout_v)
    window.resize(660, 500)

    work.start()
    
    window.show()
    app.exec()
    work.quit()
    picam2.close()


