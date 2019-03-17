##查看内核版本：
* 查看树梅派运行的内核版本:
    `uname -r`
* 查看驱动编译的内核版本:
    `modinfo ./arducam-ov9281-v4l2-pi3.ko | grep vermagic`

--- 

##如何使用此驱动：
###使用脚本:
* 使用前配置:
  1. `./preprocess.sh`(仅第一次使用前运行)
    **(ov7251和ov9281的设备树有冲突，使用ov9281时需要手动把/boot/config.txt中的dtoverlay=arducam-ov7251注释掉)**
* 加载驱动:
  1. `./loaddriver.sh` 
    
**如果要加载pi1的驱动使用 `./loaddriver.sh pi1`**
**如果要加载pi3的驱动使用 `./loaddriver.sh pi3`**
**(Pi 1B/Zero等使用pi1, Pi 2B/3B/3B+等使用pi3，主要区别为内核版本不同)**

###手动加载：

* 使用前配置(等价`./preprocess.sh`):

  1. 配置i2c-0:
      在/boot/config.txt中添加 dtparam=i2c_vc=on 

      执行 `sudo modprobe i2c-dev`

  2. 编译设备树:

      `sudo dtc -I dts -O dtb -o /boot/overlays/arducam-ov9281.dtbo arducam-ov9281.dts`

      在/boot/config.txt 文件最后添加一行：

      dtoverlay=arducam-ov9281
    **(ov7251和ov9281的设备树有冲突，使用ov9281时需要手动把/boot/config.txt中的dtoverlay=arducam-ov7251注释掉)**
  3. 重启树梅派:

      `sudo reboot`


* 加载内核模块(等价于`./loaddriver.sh`):

    1. 每次加载模块之前，或者在raspivid raspistill等类似命令运行之后，需要运行camera_i2c脚本：
        `sudo chmod +x camera_i2c`
        `sudo chmod +x rpi3-gpiovirtbuf`
        `./camera_i2c`

    2. 加载驱动模块到内核：
        `./loaddepends.sh`
        `sudo insmod arducam-ov9281-v4l2-pi3.ko `
        (or `sudo insmod arducam-ov9281-v4l2-pi1.ko`)

 ---

#### 播放
(如果你还有其他视屏设备则将video0中的0修改为实际的数字):

    vlc v4l2:///dev/video0 --v4l2-width 1280 --v4l2-height 720

可以使用v4l2-ctl --list-formats-ext 查看支持的分辨率。

控制选项：

    使用v4l2-ctl -l 查看支持的控制。
    
使用控制选项：

读写寄存器:

    先设置要读/写的寄存器的地址:
    
    v4l2-ctl -c register_addr=0x0202
    
    读取寄存器:
    
    v4l2-ctl -C register_val  (注意此处的-C是大写的)
    
    写寄存器:
    
    v4l2-ctl -c register_val=0x01

调整Gain：

    v4l2-ctl -c gain=0x02
    
水平反转：

    v4l2-ctl -c horizontal_flip=1
    
垂直反转:

    v4l2-ctl -c vertical_fli=1
    
曝光时间:

    v4l2-ctl -c exposure=0x50

