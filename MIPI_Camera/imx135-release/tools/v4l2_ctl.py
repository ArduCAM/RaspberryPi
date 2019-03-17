import v4l2 #sudo pip install v4l2
import fcntl

V4L2_CID_CUSTOM_REG_ADDRESS = 1
V4L2_CID_CUSTOM_REG_VALUE   = 2

def read_red_gain():
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_RED_BALANCE
    fcntl.ioctl(vd, v4l2.VIDIOC_G_CTRL, cp)
    vd.close()
    return cp.value

def write_red_gain(val):
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_RED_BALANCE
    cp.value = int(val)
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)
    vd.close()

def read_blue_gain():
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_BLUE_BALANCE
    fcntl.ioctl(vd, v4l2.VIDIOC_G_CTRL, cp)
    vd.close()
    return cp.value

def write_blue_gain(val):
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_BLUE_BALANCE
    cp.value = int(val)
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)
    vd.close()

def write_global_gain(val):
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_GAIN
    cp.value = int(val)
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)
    vd.close()

def read_exposure():
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_EXPOSURE
    fcntl.ioctl(vd, v4l2.VIDIOC_G_CTRL, cp)
    vd.close()
    return cp.value

def write_exposure(val):
    vd = open("/dev/video0", "rw")
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_EXPOSURE
    cp.value = int(val)
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)
    vd.close()


if __name__ == "__main__":
    vd = open("/dev/video0", "rw")

    #standard control
    cp = v4l2.v4l2_control()
    cp.id = v4l2.V4L2_CID_EXPOSURE_ABSOLUTE

    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)

    cp.value = 900
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)

    #custom control
    cp = v4l2.v4l2_ext_control()
    cp.id = V4L2_CID_CUSTOM_REG_ADDRESS
    cp.reserved2[0] = 0x0202
    fcntl.ioctl(vd, v4l2.VIDIOC_S_CTRL, cp)

    fcntl.ioctl(vd, v4l2.VIDIOC_G_CTRL, cp)

    print "register addr = 0x%04X" % (cp.reserved2[0])

    cp.id = V4L2_CID_CUSTOM_REG_VALUE
    fcntl.ioctl(vd, v4l2.VIDIOC_G_CTRL, cp)

    print "register value = %d" % (cp.reserved2[0])


