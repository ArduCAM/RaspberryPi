
import numpy as np
from PyQt5.QtWidgets import QLabel, QHBoxLayout, QVBoxLayout, QApplication, QWidget ,QFormLayout,QDoubleSpinBox,QComboBox,QSlider,QPushButton,QSpinBox,QCheckBox
from picamera2 import Picamera2
from PyQt5.QtGui import QImage,QPixmap
from PyQt5.QtCore import QThread,Qt
import RPi.GPIO as gp
import time
import os

width = 320
height = 240 

adapter_info = {  
    "A" : {   
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x01",
        "gpio_sta":[0,0],
    }, "B" : {
        "i2c_cmd":"i2cset -y 10 0x70 0x00 0x02",
        "gpio_sta":[1,0],
    }
}
picam2 = Picamera2()

class logControlSlider(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QHBoxLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(self.layout)

        self.slider = QSlider(Qt.Horizontal)
        self.box = QDoubleSpinBox()

        self.valueChanged = self.box.valueChanged
        self.valueChanged.connect(lambda: self.setValue(self.value()))
        self.slider.valueChanged.connect(self.updateValue)

        self.layout.addWidget(self.box)
        self.layout.addWidget(self.slider)

        self.precision = self.box.singleStep()
        self.slider.setSingleStep(1)
        self.minimum = 0.0
        self.maximum = 2.0

    @property
    def points(self):
        return int(1.0 / self.precision) * 2

    def boxToSlider(self, val=None):
        if val is None:
            val = self.box.value()
        if val == 0:
            return 0
        else:
            center = self.points // 2
            scaling = center / np.log2(self.maximum)
            return round(np.log2(val) * scaling) + center

    def sliderToBox(self, val=None):
        if val is None:
            val = self.slider.value()
        if val == 0:
            return 0
        else:
            center = self.points // 2
            scaling = center / np.log2(self.maximum)
            return round(2**((val - center) / scaling), int(-np.log10(self.precision)))

    def updateValue(self):
        self.blockAllSignals(True)
        if self.box.value() != self.sliderToBox():
            self.box.setValue(self.sliderToBox())
        self.blockAllSignals(False)
        self.valueChanged.emit(self.value())

    def redrawSlider(self):
        self.slider.setMinimum(0)
        self.slider.setMaximum(self.points)
        self.slider.setValue(self.boxToSlider())

    def setSingleStep(self, val):
        self.box.setSingleStep(val)
        self.precision = val

    def setValue(self, val):
        self.blockAllSignals(True)
        self.box.setValue(val)
        self.redrawSlider()
        self.blockAllSignals(False)

    def setMinimum(self, val):
        self.box.setMinimum(val)
        self.minimum = val
        self.redrawSlider()

    def setMaximum(self, val):
        self.box.setMaximum(val)
        self.maximum = val
        self.redrawSlider()

    def blockAllSignals(self, y):
        self.box.blockSignals(y)
        self.slider.blockSignals(y)

    def value(self):
        return self.box.value()

class controlSlider(QWidget):
    def __init__(self, box_type=float):
        super().__init__()
        self.layout = QHBoxLayout()
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(self.layout)

        self.slider = QSlider(Qt.Horizontal)
        if box_type == float:
            self.box = QDoubleSpinBox()
        else:
            self.box = QSpinBox()

        self.valueChanged = self.box.valueChanged
        self.valueChanged.connect(lambda: self.setValue(self.value()))
        self.slider.valueChanged.connect(self.updateValue)

        self.layout.addWidget(self.box)
        self.layout.addWidget(self.slider)

        self.precision = self.box.singleStep()
        self.slider.setSingleStep(1)

    def updateValue(self):
        self.blockAllSignals(True)
        if self.box.value() != self.slider.value() * self.precision:
            self.box.setValue(self.slider.value() * self.precision)
        self.blockAllSignals(False)
        self.valueChanged.emit(self.value())

    def setSingleStep(self, val):
        self.box.setSingleStep(val)
        self.precision = val

    def setValue(self, val):
        self.blockAllSignals(True)
        self.box.setValue(val)
        self.slider.setValue(int(val / self.precision))
        self.blockAllSignals(False)

    def setMinimum(self, val):
        self.box.setMinimum(val)
        self.slider.setMinimum(int(val / self.precision))

    def setMaximum(self, val):
        self.box.setMaximum(val)
        self.slider.setMaximum(int(val / self.precision))

    def blockAllSignals(self, y):
        self.box.blockSignals(y)
        self.slider.blockSignals(y)

    def value(self):
        return self.box.value()

class IMGTab(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QFormLayout()
        self.setLayout(self.layout)

        self.ccm = QDoubleSpinBox()
        self.ccm.valueChanged.connect(self.img_update)
        self.saturation = logControlSlider()
        self.saturation.valueChanged.connect(self.img_update)
        self.saturation.setSingleStep(0.1)
        self.contrast = logControlSlider()
        self.contrast.valueChanged.connect(self.img_update)
        self.contrast.setSingleStep(0.1)
        self.sharpness = logControlSlider()
        self.sharpness.valueChanged.connect(self.img_update)
        self.sharpness.setSingleStep(0.1)
        self.brightness = controlSlider()
        self.brightness.setSingleStep(0.1)
        self.brightness.valueChanged.connect(self.img_update)
        self.noise_reduction = QComboBox()
        self.noise_reduction.addItems(["Off", "Fast", "High Quality", "Minimal", "ZSL"])
        self.noise_reduction.currentIndexChanged.connect(self.img_update)
        self.reset_button = QPushButton("Reset")
        self.reset_button.clicked.connect(self.reset)

        self.reset()
        self.img_update()

        # self.layout.addRow("Colour Correction Matrix", self.ccm)
        self.layout.addRow("Saturation", self.saturation)
        self.layout.addRow("Contrast", self.contrast)
        self.layout.addRow("Sharpness", self.sharpness)
        self.layout.addRow("Brightness", self.brightness)
        # self.layout.addRow("Noise Reduction Mode", self.noise_reduction)
        self.layout.addRow(self.reset_button)

    @property
    def img_dict(self):
        return {
            # "ColourCorrectionMatrix": self.ccm.value(),
            "Saturation": self.saturation.value(),
            "Contrast": self.contrast.value(),
            "Sharpness": self.sharpness.value(),
            "Brightness": self.brightness.value(),
            # "NoiseReductionMode": self.noise_reduction.currentIndex()
        }

    def reset(self):
        self.ccm.setValue(picam2.camera_controls["ColourCorrectionMatrix"][2])
        self.saturation.setValue(picam2.camera_controls["Saturation"][2])
        self.contrast.setValue(picam2.camera_controls["Contrast"][2])
        self.sharpness.setValue(picam2.camera_controls["Sharpness"][2])
        self.brightness.setValue(picam2.camera_controls["Brightness"][2])

    def img_update(self):
        self.ccm.setMinimum(picam2.camera_controls["ColourCorrectionMatrix"][0])
        self.ccm.setMaximum(picam2.camera_controls["ColourCorrectionMatrix"][1])
        self.saturation.setMinimum(picam2.camera_controls["Saturation"][0])
        # self.saturation.setMaximum(picam2.camera_controls["Saturation"][1])
        self.saturation.setMaximum(6.0)
        self.contrast.setMinimum(picam2.camera_controls["Contrast"][0])
        # self.contrast.setMaximum(picam2.camera_controls["Contrast"][1])
        self.contrast.setMaximum(6.0)
        self.sharpness.setMinimum(picam2.camera_controls["Sharpness"][0])
        self.sharpness.setMaximum(picam2.camera_controls["Sharpness"][1])
        self.brightness.setMinimum(picam2.camera_controls["Brightness"][0])
        self.brightness.setMaximum(picam2.camera_controls["Brightness"][1])

        # print(self.img_dict)
        picam2.set_controls(self.img_dict)

class WorkThread(QThread):

    def __init__(self):
        super(WorkThread,self).__init__()
        gp.setwarnings(False)
        gp.setmode(gp.BOARD)
        gp.setup(7, gp.OUT)
        gp.setup(11, gp.OUT)

    def select_channel(self,index):
        channel_info = adapter_info.get(index)
        if channel_info == None:
            print("Can't get this info")
        gpio_sta = channel_info["gpio_sta"] # gpio write
        gp.output(7, gpio_sta[0])
        gp.output(11, gpio_sta[1])

    def init_i2c(self,index):
        channel_info = adapter_info.get(index)
        os.system(channel_info["i2c_cmd"]) # i2c write

    def run(self):
        global picam2
        flag = False
        for item in {"A","B"}:
            try:
                self.select_channel(item)
                self.init_i2c(item)
                time.sleep(0.5) 
                # if flag == False:
                #     flag = True
                # else :
                picam2.close()
                    # time.sleep(0.5) 
                print("init1 "+ item)
                picam2 = Picamera2()
                # print(picam2.set_controls())
                picam2.configure(picam2.create_still_configuration(main={"size": (320, 240),"format": "BGR888"},buffer_count=2)) 
                picam2.start()
                picam2.set_controls({"AeEnable":False,"ExposureTime":30000,"AnalogueGain":6})
                time.sleep(2)
                picam2.capture_array("main",wait=True)
                time.sleep(0.1)
            except Exception as e:
                print("except: "+str(e))

        while True:
            for item in {"A","B"}:
                self.select_channel(item)
                time.sleep(0.02)
                try:
                    buf = picam2.capture_array("main",wait=True)
                    buf = picam2.capture_array("main",wait=True)
                    cvimg = QImage(buf, width, height,QImage.Format_RGB888)
                    pixmap = QPixmap(cvimg)
                    if item == 'A':
                        image_label.setPixmap(pixmap)
                    elif item == 'B':
                        image_label2.setPixmap(pixmap)
                except Exception as e:
                    print("capture_buffer: "+ str(e))

class AECTab(QWidget):
    def __init__(self):
        super().__init__()
        self.layout = QFormLayout()
        self.setLayout(self.layout)

        self.aec_check = QCheckBox("AEC")
        self.aec_check.setChecked(True)
        self.aec_check.stateChanged.connect(self.aec_update)
        self.aec_meter = QComboBox()
        self.aec_meter.addItems(["Centre Weighted", "Spot", "Matrix"])
        self.aec_meter.currentIndexChanged.connect(self.aec_update)
        self.aec_constraint = QComboBox()
        self.aec_constraint.addItems(["Default", "Highlight"])
        self.aec_constraint.currentIndexChanged.connect(self.aec_update)
        self.aec_exposure = QComboBox()
        self.aec_exposure.addItems(["Normal", "Short", "Long"])
        self.aec_exposure.currentIndexChanged.connect(self.aec_update)
        self.exposure_val = controlSlider()
        self.exposure_val.valueChanged.connect(self.aec_update)
        self.exposure_val.setSingleStep(0.1)
        self.exposure_time = QSpinBox()
        self.exposure_time.setSingleStep(1000)
        self.analogue_gain = QDoubleSpinBox()
        self.analogue_label = QLabel()
        self.aec_apply = QPushButton("Apply Manual Values")
        self.aec_apply.setEnabled(False)
        self.aec_apply.clicked.connect(self.aec_manual_update)
        self.exposure_time.valueChanged.connect(lambda: self.aec_apply.setEnabled(self.exposure_time.isEnabled()))
        self.analogue_gain.valueChanged.connect(lambda: self.aec_apply.setEnabled(self.exposure_time.isEnabled()))

        self.awb_check = QCheckBox("AWB")
        self.awb_check.setChecked(True)
        self.awb_check.stateChanged.connect(self.awb_update)
        self.awb_mode = QComboBox()
        self.awb_mode.addItems([
            "Auto", "Incandescent", "Tungsten", "Fluorescent",
            "Indoor", "Daylight", "Cloudy"
        ])
        self.awb_mode.currentIndexChanged.connect(self.awb_update)
        self.colour_gain_r = QDoubleSpinBox()
        self.colour_gain_r.setSingleStep(0.1)
        self.colour_gain_r.valueChanged.connect(self.awb_update)
        self.colour_gain_b = QDoubleSpinBox()
        self.colour_gain_b.setSingleStep(0.1)
        self.colour_gain_b.valueChanged.connect(self.awb_update)

        self.reset()
        self.aec_update()
        self.awb_update()
        self.aec_apply.setEnabled(False)

        self.layout.addRow(self.aec_check)
        self.layout.addRow("AEC Metering Mode", self.aec_meter)
        self.layout.addRow("AEC Constraint Mode", self.aec_constraint)
        self.layout.addRow("AEC Exposure Mode", self.aec_exposure)
        self.layout.addRow("Exposure Value", self.exposure_val)
        self.layout.addRow("Exposure Time/\u03bcs", self.exposure_time)
        self.layout.addRow("Gain", self.analogue_gain)
        self.layout.addRow(self.analogue_label)
        self.layout.addRow(self.aec_apply)

        self.layout.addRow(self.awb_check)
        self.layout.addRow("AWB Mode", self.awb_mode)
        self.layout.addRow("Red Gain", self.colour_gain_r)
        self.layout.addRow("Blue Gain", self.colour_gain_b)

    def reset(self):
        self.aec_check.setChecked(True)
        self.awb_check.setChecked(True)
        self.exposure_time.setValue(picam2.camera_controls["ExposureTime"][2])
        self.analogue_gain.setValue(picam2.camera_controls["AnalogueGain"][2])
        self.colour_gain_r.setValue(picam2.camera_controls["ColourGains"][2])
        self.colour_gain_b.setValue(picam2.camera_controls["ColourGains"][2])

    @property
    def aec_dict(self):
        ret = {
            "AeEnable": self.aec_check.isChecked(),
            "AeMeteringMode": self.aec_meter.currentIndex(),
            "AeConstraintMode": self.aec_constraint.currentIndex(),
            "AeExposureMode": self.aec_exposure.currentIndex(),
            "ExposureValue": self.exposure_val.value(),
            "ExposureTime": self.exposure_time.value(),
            "AnalogueGain": self.analogue_gain.value()
        }
        if self.aec_check.isChecked():
            del ret["ExposureTime"]
            del ret["AnalogueGain"]
        return ret

    def aec_update(self):
        self.exposure_val.setMinimum(picam2.camera_controls["ExposureValue"][0])
        self.exposure_val.setMaximum(picam2.camera_controls["ExposureValue"][1])
        self.exposure_time.setMinimum(picam2.camera_controls["ExposureTime"][0])
        self.exposure_time.setMaximum(picam2.camera_controls["ExposureTime"][1])
        self.analogue_gain.setMinimum(picam2.camera_controls["AnalogueGain"][0])
        self.analogue_label.setText(f"Analogue up to {picam2.camera_controls['AnalogueGain'][1]:.2f}, then digital beyond")

        self.aec_meter.setEnabled(self.aec_check.isChecked())
        self.aec_constraint.setEnabled(self.aec_check.isChecked())
        self.aec_exposure.setEnabled(self.aec_check.isChecked())
        self.exposure_val.setEnabled(self.aec_check.isChecked())
        self.exposure_time.setEnabled(not self.aec_check.isChecked())
        self.analogue_gain.setEnabled(not self.aec_check.isChecked())
        if self.aec_check.isChecked():
            self.aec_apply.setEnabled(False)
        # print(self.aec_dict)
        picam2.set_controls(self.aec_dict)

    def aec_manual_update(self):
        if not self.aec_check.isChecked():
            self.aec_update()
        self.aec_apply.setEnabled(False)

    @property
    def awb_dict(self):
        ret = {
            "AwbEnable": self.awb_check.isChecked(),
            "AwbMode": self.awb_mode.currentIndex(),
            "ColourGains": [self.colour_gain_r.value(), self.colour_gain_b.value()]
        }
        if self.awb_check.isChecked():
            del ret["ColourGains"]
        return ret

    def awb_update(self):
        self.colour_gain_r.setMinimum(picam2.camera_controls["ColourGains"][0] + 0.01)
        self.colour_gain_r.setMaximum(picam2.camera_controls["ColourGains"][1])
        self.colour_gain_b.setMinimum(picam2.camera_controls["ColourGains"][0] + 0.01)
        self.colour_gain_b.setMaximum(picam2.camera_controls["ColourGains"][1])

        self.colour_gain_r.setEnabled(not self.awb_check.isChecked())
        self.colour_gain_b.setEnabled(not self.awb_check.isChecked())
        # print(self.awb_dict)
        picam2.set_controls(self.awb_dict)
      

app = QApplication([])
window = QWidget()
layout_h = QHBoxLayout()
layout_h1= QHBoxLayout()
layout_v = QVBoxLayout()
image_label = QLabel()
image_label2 = QLabel()
img_tab = IMGTab()
aec_tab = AECTab()

work = WorkThread()

if __name__ == "__main__":
    image_label.setFixedSize(320, 240)
    image_label2.setFixedSize(320, 240)
    img_tab.setFixedSize(320,200)

    window.setWindowTitle("Qt Picamera2 Arducam Multi Camera Demo")
    layout_h.addWidget(image_label)    
    layout_h.addWidget(image_label2)
    layout_h1.addWidget(img_tab)
    # layout_h1.addWidget(aec_tab)
    layout_v.addLayout(layout_h)
    layout_v.addLayout(layout_h1)
    window.setLayout(layout_v)
    window.resize(660, 350)

    work.start()
    
    window.show()
    app.exec()
    work.quit()
    picam2.close()


