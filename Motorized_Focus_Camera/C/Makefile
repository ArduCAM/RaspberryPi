Target		= manualFocus
CXX         = gcc
LIBS		= -Wall -lpthread
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
