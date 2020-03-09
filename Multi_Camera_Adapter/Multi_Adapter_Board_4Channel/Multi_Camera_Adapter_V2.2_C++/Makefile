CC = g++
CCFLAGS = -Wall -g -std=c++11

rebuild: clean camera

camera:
	$(CC) $(CCFLAGS) -o camera arducam_multi_adapter_v2.2_c.cpp -lwiringPi

clean:
	rm -rf camera
