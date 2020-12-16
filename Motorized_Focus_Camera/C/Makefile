Target		= manualFocus
CXX         = gcc
OLIB	:= lib
LIBS		= -Wall -larducam_vcm -lpthread
ODIR		= obj
SRC			=$(wildcard *.c)
DIR			=$(notdir $(SRC))
OBJ 		= $(patsubst %.c, %.o, $(DIR))
$(Target): $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LIBS)
.PHONY:clean
clean:
	rm -rf *.o
	rm -rf $(Target)
.PHONY:install 
install:
	sudo install -m 644 $(OLIB)/libarducam_vcm.so /usr/lib/

