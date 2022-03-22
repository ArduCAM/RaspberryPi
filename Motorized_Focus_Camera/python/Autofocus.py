import cv2 #sudo apt-get install python-opencv
import os
import time
from ctypes import *
import numpy as np
import libcamera

#load arducam shared object file
arducam_vcm= CDLL('./lib/libarducam_vcm.so')
    
def focusing(val):
    arducam_vcm.vcm_write(val)
    

def laplacian(img):
    img_gray = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
    img_sobel = cv2.Laplacian(img_gray,cv2.CV_16U)
    return cv2.mean(img_sobel)[0]
    
if __name__ == "__main__":
    width = 640
    height = 480
    framerate = 30
    #vcm init
    arducam_vcm.vcm_init()
    # Instantiate the libcamera class
    cam = libcamera.libcamera()

    """
    Initialize the camera

    :param1 width: Set image width
    :param2 height: Set image height
    :param3 pixelFormat: Set image format
    :param4 buffercount: Set the number of buffers
    :param5 rotation: Set the image rotation angle
    :returns ret: Whether the camera is initialized successfully
    """
    ret = cam.initCamera(width, height, libcamera.PixelFormat.RGB888, buffercount=4, rotation=0)
    if not ret:

        # Turn on the camera
        ret = cam.startCamera()

        # Set frame rate
        frame_time = 1000000 // framerate
        cam.set(libcamera.FrameDurationLimits, [frame_time, frame_time])
        # cam.set(libcamera.ExposureTime, 500)

    print("Start focusing")
    
    max_index = 10
    max_value = 0.0
    last_value = 0.0
    dec_count = 0
    focal_distance = 10
    image = None

    val = 0.0        

    while True:
        #Adjust focus
        focusing(focal_distance)
        #Take image and calculate image clarity
        ret, data = cam.readFrame()
        if not ret:
            continue
        frame = data.imageData
        cv2.imshow("image", frame)
        val = laplacian(frame)
        #Find the maximum image clarity
        if val > max_value:
            max_index = focal_distance
            max_value = val
            image = frame
            
        # #If the image clarity starts to decrease
        if val < last_value:
            dec_count += 1
        else:
            dec_count = 0
        #Image clarity is reduced by six consecutive frames
        if dec_count > 6:
            break
        last_value = val
        
        # #Increase the focal distance
        focal_distance += 15
        if focal_distance > 1000:
            break
        cv2.waitKey(1)
        cam.returnFrameBuffer(data)
    #Adjust focus to the best
    focusing(max_index)
    time.sleep(1)
    if image is not None:
        cv2.imwrite("test.jpg", image)    
    cam.stopCamera()
    cam.closeCamera()
        
    
