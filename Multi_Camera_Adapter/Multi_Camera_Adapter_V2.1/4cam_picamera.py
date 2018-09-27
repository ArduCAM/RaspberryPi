#!/usr/bin/python

import os
import sys
import time
import this
import math
#import cPickle
import RPi.GPIO as gp
try:
	import picamera
	from picamera.array import PiRGBArray
except:
	sys.exit(0)

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5 import uic

with open('gui_ui.py', 'w') as fd:
    uic.compileUi('gui.ui', fd)
import gui_ui # pyuic gui.ui > gui_ui.py

import cv2
gp.setwarnings(False)
gp.setmode(gp.BOARD)

gp.setup(7,gp.OUT)
gp.setup(11,gp.OUT)
gp.setup(12,gp.OUT)
gp.setup(13,gp.OUT)

class PhotoGrabThread( QtCore.QThread ):
    grabbed_signal = QtCore.pyqtSignal([int,QtGui.QImage])

    def __init__(self, p):
        super(PhotoGrabThread, self).__init__()
        self.index = 0
        self.index_top = p.img_no
	

    def run(self):
        i2c = "i2cset -y 1 0x70 0x00 0x04"
        os.system(i2c)
        gp.output(7, False)
    	gp.output(11, False)
    	gp.output(12, True)
    	camera = picamera.PiCamera()
   	camera.capture("dummy1.jpg")
    	time.sleep(1);

    	i2c = "i2cset -y 1 0x70 0x00 0x05"
        os.system(i2c)
        gp.output(7, True)
    	gp.output(11, False)
    	gp.output(12, True)
   	camera.capture("dummy2.jpg")
    	time.sleep(1);

    	i2c = "i2cset -y 1 0x70 0x00 0x06"
        os.system(i2c)
        gp.output(7, False)
    	gp.output(11, True)
    	gp.output(12, False)
   	camera.capture("dummy3.jpg")
    	time.sleep(1);

        i2c = "i2cset -y 1 0x70 0x00 0x07"
        os.system(i2c)
        gp.output(7, True)
    	gp.output(11, True)
    	gp.output(12, False)
   	camera.capture("dummy3.jpg") 
    	time.sleep(1);

    	

        index = 0
        camera.resolution = (320, 240)
        #camera.framerate = 24
        rawCapture = PiRGBArray(camera) 
        time.sleep(1)
        while True:
            #rev, frame = cap.read()
            camera.capture(rawCapture,format="bgr", use_video_port=True)
            image = rawCapture.array
            #rawCapture.truncate(0)
            #camera.capture(rawCapture,format="bgr", use_video_port=True)
            #image = rawCapture.array
            #rawCapture.truncate(0)
            #camera.capture(rawCapture,format="bgr", use_video_port=True)
            #image = rawCapture.array
            #image = frame.array
            if index == 0:
                #i2c = "i2cset -y 1 0x70 0x00 0x04"
                #os.system(i2c)
                gp.output(7, False)
                gp.output(11, False)
                gp.output(12, True)
            if index == 1:
                #i2c = "i2cset -y 1 0x70 0x00 0x05"
                #os.system(i2c)
                gp.output(7, True)
                gp.output(11, False)
                gp.output(12, True)
            if index == 2:
                #i2c = "i2cset -y 1 0x70 0x00 0x06"
                #os.system(i2c)
                gp.output(7, False)
                gp.output(11, True)
                gp.output(12, False)

            if index == 3:
                #i2c = "i2cset -y 1 0x70 0x00 0x07"
                #os.system(i2c)
                gp.output(7, True)
                gp.output(11, True)
                gp.output(12, False) 
            #h, w, c = frame.shape
            bytes_per_line = 3 * 320
            f_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            qimg = QtGui.QImage(f_rgb, 320, 240, bytes_per_line, QtGui.QImage.Format_RGB888)
            self.grabbed_signal.emit(index, qimg)
            rawCapture.truncate(0)
             # if the `q` key was pressed, break from the loop
           
            
            index +=1
            index %= self.index_top
            
            
class CamGui( QtWidgets.QMainWindow ):

    def __init__(self, *args):
        super(CamGui, self).__init__(*args)
        self.ui = gui_ui.Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui_label_img_list = [self.ui.label_img0, self.ui.label_img1, self.ui.label_img2, self.ui.label_img3]
        self.img_no = len(self.ui_label_img_list)

        #self.ui.label_depth.setMouseTracking(True)
        for l in self.ui_label_img_list:
            l.mouseMoveEvent = self.on_mouse_move
        self.ui.label_img0.mouseReleaseEvent = self.on_mouse_release_label_img
        


        self.grab_thread = PhotoGrabThread(self)
        self.grab_thread.grabbed_signal.connect(self.update_photo)
        self.grab_thread.start()

    def update_photo(self, index, qimg):
        self.ui_label_img_list[index].setPixmap(QtGui.QPixmap.fromImage(qimg))

        #for i in range(self.img_no):
        #    if i == index:
        #        self.ui_label_img_list[i].setLineWidth(10)
        #    else:
        #        self.ui_label_img_list[i].setLineWidth(1)

    def on_mouse_release_label_img(self, ev):
        print 'why you click me ?!'

    def on_mouse_move(self, e):
        pass
        #x,y = e.x()/3, e.y()/3
        #print x, y

    def timer_update(self):
        #print self.zen
        print '\n'*2


if __name__ == '__main__':
    gp.output(7,False)
    gp.output(11,False)
    gp.output(12,True)
    gp.output(13,False)
    gp.output(13,True)
    app = QtWidgets.QApplication(sys.argv)
    gui = CamGui()
    gui.show()
    sys.exit(app.exec_())
