CC = g++
CCFLAGS = -Wall -g -std=c++11

rebuild: clean camera

camera:
	$(CC) $(CCFLAGS) -o camera arducam_multi_adapter_v2.1_c.cpp -lwiringPi

clean:
	rm camera
