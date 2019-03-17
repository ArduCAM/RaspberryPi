#!/bin/bash
DTS_NAME=arducam-ov7251
# load i2c-dev moudle
sudo modprobe i2c-dev
# compile device tree
sudo dtc -I dts -O dtb -o /boot/overlays/${DTS_NAME}.dtbo ${DTS_NAME}.dts
# add dtparam=i2c_vc=on to /boot/config.txt
awk 'BEGIN{ count=0 }       \
{                           \
    if($1 == "dtparam=i2c_vc=on"){       \
        count++;            \
    }                       \
}END{                       \
    if(count <= 0){         \
        system("sudo sh -c '\''echo dtparam=i2c_vc=on >> /boot/config.txt'\''"); \
    }                       \
}' /boot/config.txt
# add dtoverlay='${DTS_NAME}' to /boot/config.txt
awk 'BEGIN{ count=0 }       \
{                           \
    if($1 == "dtoverlay='${DTS_NAME}'"){       \
        count++;            \
    }                       \
}END{                       \
    if(count <= 0){         \
        system("sudo sh -c '\''echo dtoverlay='${DTS_NAME}' >> /boot/config.txt'\''"); \
    }                       \
}' /boot/config.txt

echo "reboot now?(y/n):"
read USER_INPUT
case $USER_INPUT in
'y'|'Y')
    echo "reboot"
    sudo reboot
;;
*)
    echo "cancel"
;;
esac

        
