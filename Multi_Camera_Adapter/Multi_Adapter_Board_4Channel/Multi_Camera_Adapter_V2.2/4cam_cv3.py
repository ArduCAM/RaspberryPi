#!/usr/bin/python

import os
import sys
import time
import this
import math
#import cPickle
import RPi.GPIO as gp

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5 import uic

with open('gui_ui.py', 'w') as fd: 
    uic.compileUi('gui.ui', fd)
import gui_ui # pyuic gui.ui > gui_ui.py

import cv2
import subprocess

######################################
'''
Parameter setting
'''
width =  240 #320
height = 320 #240
fps = 30
brightness = 50               # min=0   max=100  step=1
contrast = 0                  # min=-100  max=100  step=1
saturation = 0                # min=-100  max=100  step=1
rotate = 0                    # min=0  max=360  step=90 
auto_exposure = 0             # min=0  max=3 
exposure_time_absolute = 1000 # min = 1  max=10000  step=1

#######################################

gp.setwarnings(False)
gp.setmode(gp.BOARD)
gp.setup(7,gp.OUT)
gp.setup(11,gp.OUT)
gp.setup(12,gp.OUT)

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
	
    	cap = cv2.VideoCapture(0)
    	cap.set(cv2.CAP_PROP_FRAME_HEIGHT, width)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, height)
        cap.set(cv2.CAP_PROP_FPS, fps)   
        command ="v4l2-ctl -d 0 -c brightness=%d" % (brightness)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c contrast=%d" % (contrast)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c saturation=%d" % (saturation)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c rotate=%d" % (rotate)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c auto_exposure=%d" % (auto_exposure)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c exposure_time_absolute=%d" % (exposure_time_absolute)
	output = subprocess.call(command, shell=True)
    	rev, frame = cap.read()
    	time.sleep(1)
    	i2c = "i2cset -y 1 0x70 0x00 0x05"
        os.system(i2c)
        gp.output(7, True)
    	gp.output(11, False)
    	gp.output(12, True)
    	cap.set(cv2.CAP_PROP_FRAME_HEIGHT, width)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, height)
        cap.set(cv2.CAP_PROP_FPS, fps)   
        command ="v4l2-ctl -d 0 -c brightness=%d" % (brightness)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c contrast=%d" % (contrast)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c saturation=%d" % (saturation)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c rotate=%d" % (rotate)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c auto_exposure=%d" % (auto_exposure)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c exposure_time_absolute=%d" % (exposure_time_absolute)
	output = subprocess.call(command, shell=True)
    	rev, frame = cap.read()
    	time.sleep(1)
    	i2c = "i2cset -y 1 0x70 0x00 0x06"
        os.system(i2c)
        gp.output(7, False)
    	gp.output(11, True)
    	gp.output(12, False)
    	cap.set(cv2.CAP_PROP_FRAME_HEIGHT, width)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, height)
        cap.set(cv2.CAP_PROP_FPS, fps)   
        command ="v4l2-ctl -d 0 -c brightness=%d" % (brightness)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c contrast=%d" % (contrast)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c saturation=%d" % (saturation)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c rotate=%d" % (rotate)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c auto_exposure=%d" % (auto_exposure)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c exposure_time_absolute=%d" % (exposure_time_absolute)
	output = subprocess.call(command, shell=True)
    	rev, frame = cap.read()

    	time.sleep(1)
    	i2c = "i2cset -y 1 0x70 0x00 0x07"
        os.system(i2c)
        gp.output(7, True)
    	gp.output(11, True)
    	gp.output(12, False)
   	rev, frame = cap.read()
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, width)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, height)
        cap.set(cv2.CAP_PROP_FPS, fps)   
        command ="v4l2-ctl -d 0 -c brightness=%d" % (brightness)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c contrast=%d" % (contrast)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c saturation=%d" % (saturation)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c rotate=%d" % (rotate)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c auto_exposure=%d" % (auto_exposure)
	output = subprocess.call(command, shell=True)
	command ="v4l2-ctl -d 0 -c exposure_time_absolute=%d" % (exposure_time_absolute)
	output = subprocess.call(command, shell=True)
	time.sleep(1);
	



        index = 0
        while 1:
            #rev, frame = cap.read()
            rev, frame = cap.read()
            rev, frame = cap.read()
            if index == 0:
		gp.output(7,False)
	    	gp.output(11,False)
	    	gp.output(12,True)
            if index == 3:
		gp.output(7,True)
	   	gp.output(11,False)
	    	gp.output(12,True)
            if index == 1:
		gp.output(7,False)
	    	gp.output(11,True)
	    	gp.output(12,False)
            if index == 2:
		gp.output(7,True)
	   	gp.output(11,True)
    		gp.output(12,False)

            h, w, c = frame.shape
            bytes_per_line = 3 * w
            f_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            qimg = QtGui.QImage(f_rgb.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)

            self.grabbed_signal.emit(index, qimg)

            index += 1
            index %= self.index_top


class CamGui( QtWidgets.QMainWindow):

    def __init__(self, *args):
        #super(CamGui, self).__init__(*args)
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
        return

        for i in range(self.img_no):
            if i == index:
                self.ui_label_img_list[i].setLineWidth(10)
            else:
                self.ui_label_img_list[i].setLineWidth(1)

    def on_mouse_release_label_img(self, ev):
        print 'why you click me ?!'

    def on_mouse_move(self, e):
        pass
        #x,y = e.x()/3, e.y()/3
        #print x, y

    def timer_update(self):
        print self.zen
        print '\n'*2



if __name__ == '__main__':
   # width = input("Please set the width:")
   # height = input("Please set the height:")
   # fps  = input ("Please set the fps:")
    
    app = QtWidgets.QApplication(sys.argv)
    gui = CamGui()
    gui.show()
    sys.exit(app.exec_())
