NAME=run_Arducam_Demo
CC=g++
CFLAGS=-Wall -pthread `pkg-config --cflags --libs opencv`
 
ODIR=obj
SDIR=src

_DEPS=ArduCAM.h bcm283x_board_driver.h sccb_bus.h ov2640_regs.h ov5640_regs.h ov5642_regs.h
DEPS=$(patsubst %,$(SDIR)/%,$(_DEPS))

_OBJ=ArducamDemo.o ArduCAM.o bcm283x_board_driver.o sccb_bus.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.cpp $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)
$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY:	run

run:	$(NAME)
	sudo ./$(NAME)

.PHONY:	clean
clean:	
	rm -f $(NAME) $(ODIR)/*.o
