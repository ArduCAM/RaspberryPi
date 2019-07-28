#include <iostream>
#include <unistd.h>

#include <string>
#include <string.h>

#include <wiringPi.h>
#include <wiringPiI2C.h>

// Comment out any cameras that are not available to your device.
// This will cause set_camera to return a negative number and for them
// not to be tested by main.
// This must be done before compiling.
#define ENABLE_CAMERA_1
#define ENABLE_CAMERA_1
#define ENABLE_CAMERA_2
#define ENABLE_CAMERA_4

int setup() {
    wiringPiSetup();
    pinMode(7, OUTPUT);
    pinMode(0, OUTPUT);

    pinMode(1, OUTPUT);
    
    digitalWrite(1, HIGH);
    digitalWrite(0, HIGH);

    int extraPins[4] = {3, 4, 13, 6};
    for (int i = 0 ; i < 4 ; i++ ) {
        pinMode(extraPins[i], OUTPUT);
        digitalWrite(extraPins[i], HIGH);
    }

    return 0;
}

// Set the camera utilizing i2c and GPIO interfaces
// 1-4 corresponds to Cameras A-D as labelled on multiplexer
int set_camera(int cam) {
    if (cam == 1) {
        #ifdef ENABLE_CAMERA_1
        system("i2cset -y 1 0x70 0x00 0x04");
        digitalWrite(7, LOW);
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        #else
        return -1;
        #endif
    } else if (cam == 2) {
        #ifdef ENABLE_CAMERA_2
        system("i2cset -y 1 0x70 0x00 0x05");
        digitalWrite(7, HIGH);
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        #else
        return -2;
        #endif
    } else if (cam == 3) {
        #ifdef ENABLE_CAMERA_3
        system("i2cset -y 1 0x70 0x00 0x05");
        digitalWrite(7, HIGH);
        digitalWrite(0, LOW);
        digitalWrite(1, HIGH);
        #else
        return -3;
        #endif
    } else if (cam == 4) {
        #ifdef ENABLE_CAMERA_4
        system("i2cset -y 1 0x70 0x00 0x07");
        digitalWrite(7, HIGH);
        digitalWrite(0, HIGH);
        digitalWrite(1, LOW);
        #else
        return -4; // Indicates incorrect parameter 4
        #endif
    } else {
        return -5; // -5 indicates incorrect parameter, none of 1-4
    }

    return 0;
}

void capture(std::string name) {
    std::string command ("raspistill -t 100 -o ");
    std::string cmd = command + name + ".jpg";
    system(cmd.c_str());
}

int main() {
    setup();
    #ifdef ENABLE_CAMERA_1
    std::cout << "Testing Camera A..." << std::endl;
    set_camera(1);
    capture("CAMERA_A");
    #endif
    #ifdef ENABLE_CAMERA_2
    std::cout << "Testing Camera B..." << std::endl;
    set_camera(2);
    capture("CAMERA_B");
    #endif
    #ifdef ENABLE_CAMERA_3
    std::cout << "Testing Camera C..." << std::endl;
    set_camera(3);
    capture("CAMERA_C");
    #endif
    #ifdef ENABLE_CAMERA_4
    std::cout << "Testing Camera D..." << std::endl;
    set_camera(4);
    capture("CAMERA_D");
    #endif
}
