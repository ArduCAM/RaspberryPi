#include "bcm283x_board_driver.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif


// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
volatile unsigned int *spi;  //pointer to base of spi registers
volatile unsigned int *pwm;

volatile unsigned int *sys_timer;
volatile unsigned int *arm_timer; // pointer to base of arm timer registers

volatile unsigned int *uart;
volatile unsigned int *cm_pwm;


/////////////////////////////////////////////////////////////////////
// General Functions
/////////////////////////////////////////////////////////////////////
// TODO: return error code instead of printing (mem_fd, reg_map)

int irq1, irq2, irqbasic;

void pioInit() {
	int  mem_fd;
	void *reg_map;

	// /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
	      printf("can't open /dev/mem \n");
	      exit(-1);
	}

	reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      GPIO_BASE);       // Offset to GPIO peripheral

	if (reg_map == MAP_FAILED) {
      printf("gpio mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

	gpio = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      SPI0_BASE);       // Offset to SPI peripheral

    if (reg_map == MAP_FAILED) {
      printf("spi mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    spi = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      PWM_BASE);       // Offset to PWM peripheral

    if (reg_map == MAP_FAILED) {
      printf("pwm mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    pwm = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      SYS_TIMER_BASE);       // Offset to Timer peripheral

    if (reg_map == MAP_FAILED) {
      printf("sys timer mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    sys_timer = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      ARM_TIMER_BASE);       // Offset to interrupts


    if (reg_map == MAP_FAILED) {
      printf("arm timer mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    arm_timer = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      UART_BASE);       // Offset to UART peripheral

    if (reg_map == MAP_FAILED) {
      printf("uart mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    uart = (volatile unsigned *)reg_map;

    reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      CM_PWM_BASE);       // Offset to ARM timer peripheral

    if (reg_map == MAP_FAILED) {
      printf("cm_pwm mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

    cm_pwm = (volatile unsigned *)reg_map;
	close(mem_fd);
}


void noInterrupts(void) {
    //save current interrupts
    irq1 = IRQ_ENABLE1;
    irq2 = IRQ_ENABLE2;
    irqbasic = IRQ_ENABLE_BASIC;

    //disable interrupts
    IRQ_DISABLE1 = irq1;
    IRQ_DISABLE2 = irq2;
    IRQ_DISABLE_BASIC = irqbasic; 
}

void interrupts(void) {
    if(IRQ_ENABLE1 == 0){ // if interrupts are disabled
    //restore interrupts
        IRQ_ENABLE1 = irq1;
        IRQ_ENABLE2 = irq2;
        IRQ_ENABLE_BASIC = irqbasic;
    }
}


void pinMode(int pin, int function) {
    int reg      =  pin/10;
    int offset   = (pin%10)*3;
    GPFSEL[reg] &= ~((0b111 & ~function) << offset);
    GPFSEL[reg] |=  ((0b111 &  function) << offset);
}

void digitalWrite(int pin, int val) {
    int reg = pin / 32;
    int offset = pin % 32;

    if (val) GPSET[reg] = 1 << offset;
    else     GPCLR[reg] = 1 << offset;
}

int digitalRead(int pin) {
    int reg = pin / 32;
    int offset = pin % 32;

    return (GPLEV[reg] >> offset) & 0x00000001;
}

void pinsMode(int pins[], int numPins, int fxn) {
    int i;
    for(i=0; i<numPins; ++i) {
        pinMode(pins[i], fxn);
    }
}

void digitalWrites(int pins[], int numPins, int val) {
    int i;
    for(i=0; i<numPins; i++) {
        digitalWrite(pins[i], (val & 0x00000001));
        val = val >> 1;
    }
}

int digitalReads(int pins[], int numPins) {
    int i, val = digitalRead(pins[0]);
    
    for(i=1; i<numPins; i++) {
        val |= (digitalRead(pins[i]) << i);
    }
    return val;
}

unsigned long  get_microsecond_timestamp()
{
    struct timespec t;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &t) != 0) {
        return 0;
    }

    return (unsigned long) t.tv_sec * 1000000 + t.tv_nsec / 1000;
}


void delay_us(unsigned int micros) {
	unsigned long nowtime = get_microsecond_timestamp();
	while((get_microsecond_timestamp() - nowtime)<micros/2){;}
}

void delay_ms(unsigned int millis) {
    delay_us(millis*1000);                // 1000 microseconds per millisecond
}

/////////////////////////////////////////////////////////////////////
// SPI Functions
/////////////////////////////////////////////////////////////////////

void spiInit(int freq, int settings) {
    //set GPIO 8 (CE), 9 (MISO), 10 (MOSI), 11 (SCLK) alt fxn 0 (SPI0)
    //pinMode(8, ALT0);
    SPI0CSbits.TA = 0;          // turn SPI on with the "transfer active" bit
    pinMode(9, ALT0);
    pinMode(10, ALT0);
    pinMode(11, ALT0);

    //Note: clock divisor will be rounded to the nearest power of 2
    SPI0CLK = 250000000/freq;   // set SPI clock to 250MHz / freq
    SPI0CS = settings;  
	SPI0CSbits.CLEAR = 3;   // this is very important 
    SPI0CSbits.TA = 1;          // turn SPI on with the "transfer active" bit
}

 char spiSendReceive(char send){	
	SPI0FIFO = send;            // send data to slave
	while(!SPI0CSbits.DONE);	// wait until SPI transmission complete
    return SPI0FIFO;            // return received data
}

short spiSendReceive16(short send) {
    short rec;
	
    SPI0CSbits.TA = 1;          // turn SPI on with the "transfer active" bit
    rec = spiSendReceive((send & 0xFF00) >> 8); // send data MSB first
    rec = (rec << 8) | spiSendReceive(send & 0xFF);
    SPI0CSbits.TA = 0;          // turn off SPI
    return rec;
}

/////////////////////////////////////////////////////////////////////
// UART Functions
/////////////////////////////////////////////////////////////////////

void uartInit(int baud) {
    uint fb = 12000000/baud; // 3 MHz UART clock
    
    pinMode(14, ALT0);
    pinMode(15, ALT0);
    UART_IBRD = fb >> 6;       // 6 Fract, 16 Int bits of BRD
    UART_FBRD = fb & 63;
    UART_LCRHbits.WLEN = 3;     // 8 Data, 1 Stop, 0 Parity, no FIFO, no Flow
    UART_CRbits.UARTEN = 1;     // Enable uart.
}

char getCharSerial(void) {
    while (UART_FRbits.RXFE);    // Wait until data is available.
    return UART_DRbits.DATA;          // Return char from serial port.
}


void putCharSerial(char c) {
    while (!UART_FRbits.TXFE);
    UART_DRbits.DATA = c;
}

/////////////////////////////////////////////////////////////////////
// Pulse Width Modulation Functions
/////////////////////////////////////////////////////////////////////

void pwmInit() {
    pinMode(18, ALT5);

    // Configure the clock manager to generate a 25 MHz PWM clock.
    // Documentation on the clock manager is missing in the datasheet
    // but found in "BCM2835 Audio and PWM Clocks" by G.J. van Loo 6 Feb 2013.
    // Maximum operating frequency of PWM clock is 25 MHz.
    // Writes to the clock manager registers require simultaneous writing
    // a "password" of 5A to the top bits to reduce the risk of accidental writes.

    CM_PWMCTL = 0; // Turn off PWM before changing
    CM_PWMCTL =  PWM_CLK_PASSWORD|0x20; // Turn off clock generator
    while(CM_PWMCTLbits.BUSY); // Wait for generator to stop
    CM_PWMCTL = PWM_CLK_PASSWORD|0x206; // Src = unfiltered 500 MHz CLKD
    CM_PWMDIV = PWM_CLK_PASSWORD|(PLL_CLOCK_DIVISOR << 12); // PWM Freq = 25 MHz
    CM_PWMCTL = CM_PWMCTL|PWM_CLK_PASSWORD|0x10;    // Enable PWM clock
    while (!CM_PWMCTLbits.BUSY);    // Wait for generator to start    
    PWM_CTLbits.MSEN1 = 1;  // Channel 1 in mark/space mode
    PWM_CTLbits.PWEN1 = 1;  // Enable pwm
}

/**
 * dut is a value between 0 and 1 
 * freq is pwm frequency in Hz
 */
void setPWM(float freq, float dut) {
    PWM_RNG1 = (int)(CM_FREQUENCY / freq);
    PWM_DAT1 = (int)(dut * (CM_FREQUENCY / freq));
}

void analogWrite(int val) {
	setPWM(78125, val/255.0);
}


#ifdef __cplusplus
}
#endif


