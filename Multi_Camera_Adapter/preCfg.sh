#!/bin/sh
cd /lib/modules/$(uname -r)/kernel/drivers/staging/vc04_services/bcm2835-camera
mv bcm2835-v4l2.ko uc-bcm2835-v4l2.ko
sudo depmod
echo "preCondig OK."
sudo reboot
