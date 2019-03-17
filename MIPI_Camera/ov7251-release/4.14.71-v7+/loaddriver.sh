#!/bin/bash
MODULE_NAME_SPECIFIC=ov7251
MODULE_NAME=arducam-${MODULE_NAME_SPECIFIC}-v4l2-pi3.ko
MODULE_PREFIX=arducam_
load_module(){
    check_kernel_version
    sudo chmod +x camera_i2c
    sudo chmod +x rpi3-gpiovirtbuf
    ./camera_i2c
    #load depends
    modinfo ${MODULE_NAME} | grep depends | awk '{print $NF}' | \
        sed 's/,/ /g' | xargs sudo modprobe -a
    #remove other arducam module
    lsmod | grep ${MODULE_PREFIX} | awk 'NR==1{if($1){system("sudo rmmod "$1)}}'
    #load module 
    sudo insmod ${MODULE_NAME}
}
check_kernel_version(){
    MODULE_KERNEL_VERSION=$(modinfo ${MODULE_NAME} | grep vermagic | awk '{print $2}')
    CURRENT_KERNEL_VERSION=$(uname -r)
    if [ "$MODULE_KERNEL_VERSION" != "$CURRENT_KERNEL_VERSION" ]; then
        echo "The current kernel version does not match the kernel version of the module."
        echo "Try selecting other parameters or re-downloading the correct driver."
        echo ""
    fi
}
case $@ in
"pi0" | "pi1")
    MODULE_NAME=arducam-${MODULE_NAME_SPECIFIC}-v4l2-pi1.ko
    load_module
;;
"pi2" | "pi3")
    MODULE_NAME=arducam-${MODULE_NAME_SPECIFIC}-v4l2-pi3.ko
    load_module
;;
*) 
    echo "Illegal parameter, please enter the version number."
    echo "  Optional: pi0 pi1 pi2 pi3"
    echo "  example: ./loaddriver.sh pi3"
    exit
;;
esac
