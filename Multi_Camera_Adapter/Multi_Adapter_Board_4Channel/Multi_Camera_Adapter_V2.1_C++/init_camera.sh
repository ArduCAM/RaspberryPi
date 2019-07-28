\#!/bin/sh
echo "----Test i2c1"
i2cdetect -y 1
echo "----Configure Adapter Board V2.1"
i2cset -y 1 0x70 0x00 0x04
echo "Adapter Board V2.1 configure OK"
