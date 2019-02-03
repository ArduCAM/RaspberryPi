import RPi.GPIO as gp
import os
cmd1="echo 32 > /sys/class/gpio/export"
cmd2="echo out > /sys/class/gpio/gpio32/direction"
cmd_H="echo 1 > /sys/class/gpio/gpio32/value"
cmd_L="echo 0 > /sys/class/gpio/gpio32/value"
def main():
    os.system(cmd1) 
    os.system(cmd2)
    os.system(cmd_H)
    capture(1)
    os.system(cmd_L)
    capture(2)


def capture(cam):
    cmd = "raspistill -o capture_%d.jpg" % cam
    os.system(cmd)

if __name__ == "__main__":
    main()

 #   gp.output(7, False)
#    gp.output(11, False)
  #  gp.output(12, True)
