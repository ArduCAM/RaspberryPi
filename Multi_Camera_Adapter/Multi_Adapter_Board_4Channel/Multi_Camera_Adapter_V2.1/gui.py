#!/usr/bin/python

import os
import sys
import time
import this
import math
#import cPickle

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5 import uic

with open('gui_ui.py', 'w') as fd:
    uic.compileUi('gui.ui', fd)
import gui_ui # pyuic gui.ui > gui_ui.py

import cv2


class PhotoGrabThread( QtCore.QThread ):
    grabbed_signal = QtCore.pyqtSignal([int,QtGui.QImage])

    def __init__(self, p):
        super(PhotoGrabThread, self).__init__()
        self.index = 0
        self.index_top = p.img_no

    def run(self):

        cap = cv2.VideoCapture(-1)
        # cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 320)
        # cap.set(cv2.CAP_PROP_FRAME_WIDTH, 240)
        cap.set(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT, 320)
        cap.set(cv2.cv.CV_CAP_PROP_FRAME_WIDTH, 240)
        #cap.set(cv2.CAP_PROP_FPS, 1)

        index = 0
        while 1:
            rev, frame = cap.read()

            h, w, c = frame.shape
            bytes_per_line = 3 * w
            # f_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            f_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            qimg = QtGui.QImage(f_rgb.data, w, h, bytes_per_line, QtGui.QImage.Format_RGB888)

            self.grabbed_signal.emit(index, qimg)

            index += 1
            index %= self.index_top


class CamGui( QtWidgets.QMainWindow ):

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
        
        d = {}
        for c in (65, 97):
            for i in range(26):
                d[chr(i+c)] = chr((i+13) % 26 + c)
        self.zen = "".join([d.get(c, c) for c in this.s])

        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.timer_update)
        self.timer.start(3142)

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

    app = QtWidgets.QApplication(sys.argv)
    gui = CamGui()
    gui.show()
    sys.exit(app.exec_())
