
import os
import time
import sys
import picamera
from ctypes import *
keyboard = CDLL('./lib/libarducam_keyboard.so')
arducam_vcm =CDLL('./lib/libarducam_vcm.so')
#camera.capture('Image%d.jpg' %(time.time()))
UP      = 1
DOWN    = 2
SAVE    = 115
focus_val = 512;
step = 10
if __name__ == "__main__":
    #vcm init
    arducam_vcm.vcm_init()
    camera = picamera.PiCamera()
    camera.preview_fullscreen=False
    preview=camera.start_preview()
    #set windows size
    preview.window=(0,0,800,600)
    print("Please press up and down to adjust focus. Press 's' to save image.")
    while True:
        keyVal = keyboard.processKeyEvent()
        if keyVal == UP:
            print("UP")
            focus_val = focus_val+step
            if  focus_val > 1023:
                focus_val = 1023
            arducam_vcm.vcm_write(focus_val)
        if keyVal == DOWN:
            print("DOWN")
            focus_val = focus_val-step
            if focus_val < 0:
                focus_val = 0
            arducam_vcm.vcm_write(focus_val)
        if keyVal == SAVE:
            camera.capture('Image%d.jpg' %(time.time()))
            print("Image save OK.")
            camera.stop_preview()
            break


