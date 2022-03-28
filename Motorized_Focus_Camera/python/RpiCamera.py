# MIT License
# Copyright (c) 2019 JetsonHacks
# See license
# Using a CSI camera (such as the Raspberry Pi Version 2) connected to a
# NVIDIA Jetson Nano Developer Kit using OpenCV
# Drivers for the camera and OpenCV are included in the base image

import cv2
import time
try:
    from  Queue import  Queue
except ModuleNotFoundError:
    from  queue import  Queue

import  threading
import signal
import sys

import libcamera

# def signal_handler(sig, frame):
#     print('You pressed Ctrl+C!')
#     sys.exit(0)
# signal.signal(signal.SIGINT, signal_handler)


def gstreamer_pipeline(
    capture_width=1280,
    capture_height=720,
    display_width=640,
    display_height=360,
    framerate=60,
    flip_method=0,
):
    return (
        "nvarguscamerasrc ! "
        "video/x-raw(memory:NVMM), "
        "width=(int)%d, height=(int)%d, "
        "format=(string)NV12, framerate=(fraction)%d/1 ! "
        "nvvidconv flip-method=%d ! "
        "video/x-raw, width=(int)%d, height=(int)%d, format=(string)BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=(string)BGR ! appsink"
        % (
            capture_width,
            capture_height,
            framerate,
            flip_method,
            display_width,
            display_height,
        )
    )

class FrameReader(threading.Thread):
    queues = []
    _running = True
    camera = None
    def __init__(self, camera, name):
        threading.Thread.__init__(self)
        self.name = name
        self.camera = camera
 
    def run(self):
        while self._running:
            # _, frame = self.camera.read()
            ret, data = self.camera.readFrame()
            if not ret:
                continue
            while self.queues:
                queue = self.queues.pop()
                queue.put(data.imageData)
            self.camera.returnFrameBuffer(data)
    
    def addQueue(self, queue):
        self.queues.append(queue)

    def getFrame(self, timeout = None):
        queue = Queue(1)
        self.addQueue(queue)
        return queue.get(timeout = timeout)

    def stop(self):
        self._running = False

class Previewer(threading.Thread):
    window_name = "Arducam"
    _running = True
    camera = None
    def __init__(self, camera, name):
        threading.Thread.__init__(self)
        self.name = name
        self.camera = camera
    
    def run(self):
        self._running = True
        while self._running:
            cv2.imshow(self.window_name, self.camera.getFrame(2000))
            keyCode = cv2.waitKey(16) & 0xFF
        cv2.destroyWindow(self.window_name)

    def start_preview(self):
        self.start()
    def stop_preview(self):
        self._running = False

class Camera(object):
    frame_reader = None
    cam = None
    previewer = None

    def __init__(self, width=640, height=360):
        self.open_camera(width, height)

    def open_camera(self, width=640, height=360, framerate=30):
        # self.cap = cv2.VideoCapture(gstreamer_pipeline(flip_method=0, display_width=width, display_height=height), cv2.CAP_GSTREAMER)\
        self.cam = libcamera.libcamera()
        ret = self.cam.initCamera(width, height, libcamera.PixelFormat.RGB888, buffercount=4, rotation=0)
        # if not self.cap.isOpened():
        #     raise RuntimeError("Failed to open camera!")
        if not ret:
            # Turn on the camera
            ret = self.cam.startCamera()
            # Set frame rate
            frame_time = 1000000 // framerate
            self.cam.set(libcamera.FrameDurationLimits, [frame_time, frame_time])
        else:
            raise RuntimeError("Failed to open camera!")

        if self.frame_reader == None:
            self.frame_reader = FrameReader(self.cam, "")
            self.frame_reader.daemon = True
            self.frame_reader.start()
        self.previewer = Previewer(self.frame_reader, "")

    def getFrame(self, timeout = None):
        return self.frame_reader.getFrame(timeout)

    def start_preview(self):
        self.previewer.daemon = True
        self.previewer.start_preview()

    def stop_preview(self):
        self.previewer.stop_preview()
        self.previewer.join()
        self.cam.stopCamera()
    
    def close(self):
        self.frame_reader.stop()
        # self.cap.release()
        self.cam.closeCamera()

if __name__ == "__main__":
    camera = Camera()
    camera.start_preview()
    time.sleep(10)
    camera.stop_preview()
    camera.close()
