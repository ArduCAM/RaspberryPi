CXX         = g++
CXXFLAGS    = `pkg-config --cflags --libs opencv`  #sudo apt-get install libopencv-dev
LIBS		= -lwiringPi
ODIR		= obj
_OBJ        = pi_cam.o
OBJ 		= $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	

pi_zero_cam: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LIBS)

.PHONY:clean

clean:
	rm -rf $(ODIR)/*
