#!/bin/sh

# Setup GPIO for Arducam Multi Adapter Board Pi zero
# Disable the camera_led
awk 'BEGIN{ count=0 }       \
{                           \
    if($1 == "disable_camera_led=1"){       \
        count++;            \
    }                       \
}END{                       \
    if(count <= 0){         \
        system("sudo sh -c '\''echo disable_camera_led=1 >> /boot/config.txt'\''"); \
    }                       \
}' /boot/config.txt 
rev=`cat /proc/cpuinfo | grep Revision | awk '{print substr($NF,length($NF)-5,6)}'`
#echo "Detected the board revsion: $rev"
case $rev in
'9000c1')
echo "The board is PiZero W."
gpio -g write 40 1 #set the gpio40 high
echo "Choose camera A"
raspistill -o camera1.jpg
gpio -g write 40 0 #set the gpio40 low
echo "Choose Camera B"
raspistill -o camera2.jpg
echo "Test OK"
;;
'a02082'|'a22082'|'a020d3')
echo "The board is Pi 3B/B+."
./rpi3-gpiovirtbuf s 134 1 #set the gpio134 high
echo "Choose camera A"
raspistill -o camera1.jpg
./rpi3-gpiovirtbuf s 134 0 #set the gpio134 low
echo "Choose Camera B"
raspistill -o camera2.jpg
echo "Test OK"
;;
'0010'|'0012'|'0013'|'1041'|'a01041'|'a21041'|'a02042'|'900093'|'920093')
echo "The board is PiZero."
gpio -g write 32 1 #set the gpio32 high
echo "Choose camera A"
raspistill -o camera1.jpg
gpio -g write 32 0 #set the gpio32 low
echo "Choose Camera B"
raspistill -o camera2.jpg
echo "Test OK"

;;
*)
esac
