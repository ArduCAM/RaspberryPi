
import os
import time
import sys
import thread
from ctypes import *
keyboard = CDLL('./lib/libarducam_keyboard.so')
arducam_vcm =CDLL('./lib/libarducam_vcm.so')
UP      = 1
DOWN    = 2
SAVE    = 115
focus_val = 512;
step = 10
def run_camera(name):
    os.system("raspistill -t 0")
if __name__ == "__main__":
    thread.start_new_thread(run_camera, ("run_camera",))
    #vcm init
    arducam_vcm.vcm_init()
   # camera = picamera.PiCamera()
   # camera.preview_fullscreen=False
   # preview=camera.start_preview()
    #set windows size
   # preview.window=(0,0,800,600)
    print("Please press up and down to adjust focus.")
    while True:
        keyVal = keyboard.processKeyEvent()
        time.sleep(0.01)
        if keyVal == UP:
            print("UP")
            focus_val = focus_val+step
            if  focus_val > 1023:
                focus_val = 1023
            print(focus_val)
            arducam_vcm.vcm_write(focus_val)
        if keyVal == DOWN:
            print("DOWN")
            focus_val = focus_val-step
            if focus_val < 0:
                focus_val = 0
            print(focus_val)
            arducam_vcm.vcm_write(focus_val)


