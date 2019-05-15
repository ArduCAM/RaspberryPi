#include "ArduCAM.h"
#include "sccb_bus.h"
#include "ov2640_regs.h"
#include "ov5642_regs.h"
#include "ov5640_regs.h"
unsigned char sensor_model = 0;
unsigned char sensor_addr = 0;
unsigned char m_fmt = JPEG;
unsigned int length = 0;

void ArduCAM_Init(unsigned char model) 
{
	switch (model)
  {
    case OV2640:
    case OV9650:
    case OV9655:
		{		
		  wrSensorReg8_8(0xff, 0x01);
      wrSensorReg8_8(0x12, 0x80);
      if(m_fmt == JPEG)
      {
				wrSensorRegs8_8(OV2640_JPEG_INIT);
				wrSensorRegs8_8(OV2640_YUV422);
				wrSensorRegs8_8(OV2640_JPEG);
				wrSensorReg8_8(0xff, 0x01);
				wrSensorReg8_8(0x15, 0x00);
				wrSensorRegs8_8(OV2640_320x240_JPEG);
      }
      else
      {
        wrSensorRegs8_8(OV2640_QVGA);
      }
      break;
		}
		case OV5640:
		{
			if (m_fmt == JPEG)
			{
				wrSensorReg16_8(0x3103, 0x11);
				wrSensorReg16_8(0x3008, 0x82);
				wrSensorRegs16_8(OV5640YUV_Sensor_Dvp_Init);	
				wrSensorRegs16_8(OV5640_JPEG_QSXGA);
				wrSensorRegs16_8(OV5640_QSXGA2QVGA);
			  wrSensorReg16_8(0x4407, 0x04);

			}
			else
			{
				wrSensorReg16_8(0x3103, 0x11);
				wrSensorReg16_8(0x3008, 0x82);
				wrSensorRegs16_8(OV5640YUV_Sensor_Dvp_Init);
				wrSensorRegs16_8(OV5640_RGB_QVGA);
			} 
			 break;
		}
		case OV5642:
      {
        wrSensorReg16_8(0x3008, 0x80);
        wrSensorRegs16_8(OV5642_QVGA_Preview);
      
        if (m_fmt == JPEG)
        {
          printf("the format is JPEG\r\n");
          wrSensorRegs16_8(OV5642_JPEG_Capture_QSXGA);
          wrSensorRegs16_8(ov5642_320x240);
          wrSensorReg16_8(0x3818, 0xa8);
          wrSensorReg16_8(0x3621, 0x10);
          wrSensorReg16_8(0x3801, 0xb0);
          wrSensorReg16_8(0x4407, 0x04);

				
        }
        else
        {
         unsigned char reg_val;
          wrSensorReg16_8(0x4740, 0x21);
          wrSensorReg16_8(0x501e, 0x2a);
          wrSensorReg16_8(0x5002, 0xf8);
          wrSensorReg16_8(0x501f, 0x01);
          wrSensorReg16_8(0x4300, 0x61);
          rdSensorReg16_8(0x3818, &reg_val);
          wrSensorReg16_8(0x3818, (reg_val | 0x60) & 0xff);
          rdSensorReg16_8(0x3621, &reg_val);
          wrSensorReg16_8(0x3621, reg_val & 0xdf);
        }
			  break;
      }
     default:
     break;
  }
	if(model == OV2640 )
		OV2640_set_JPEG_size(OV2640_320x240);
	if(model == OV5640 ||model == OV5642){
    if(model == OV5640)
		 OV5640_set_JPEG_size(OV5640_1600x1200);
	
	 else{
		 OV5642_set_JPEG_size(OV5642_320x240);
	 	 printf("set resolution OK\r\n");
	 	}
	}
	
	
}

void ArduCAM_CS_init(int CS1, int CS2, int CS3, int CS4)
{
	if(CS1> -1){
		pinMode(CS1, OUTPUT);
		digitalWrite(CS1, HIGH);	
	}
	if(CS2> -1){
		pinMode(CS2, OUTPUT);
		digitalWrite(CS2, HIGH);	
	}
	if(CS3> -1){
		pinMode(CS3, OUTPUT);
		digitalWrite(CS3, HIGH);	
	}
	if(CS4> -1){
		pinMode(CS4, OUTPUT);
		digitalWrite(CS4, HIGH);	
	}
						
}

void CS_HIGH(int CS)
{
	delay_us(1);
 	digitalWrite(CS, HIGH);					
}

void CS_LOW(int CS)
{
	delay_us(1);
 	digitalWrite(CS, LOW);						    
}

void set_format(unsigned char fmt)
{
  if (fmt == BMP)
    m_fmt = BMP;
  else if(fmt == RAW)
  	m_fmt = RAW;
  else
    m_fmt = JPEG;
}

unsigned char bus_read(int address,int CS)
{
	unsigned char value;
    CS_LOW(CS);
	spiSendReceive(address);
	value = spiSendReceive(0x00);
	CS_HIGH(CS);
	return value;
}

unsigned char bus_write(int address,int value,int CS)
{	
	CS_LOW(CS);
	spiSendReceive(address);
	spiSendReceive(value);
	CS_HIGH(CS);
	return 1;
}

unsigned char read_reg(unsigned char addr,int CS)
{
	unsigned char readData;
	readData = bus_read(addr & 0x7F, CS);
	return readData;
}
void write_reg(unsigned char addr, unsigned char data, int CS)
{
	 bus_write(addr | 0x80, data, CS); 
}

unsigned char read_fifo(int CS)
{
	unsigned char data;
	data = bus_read(SINGLE_FIFO_READ, CS);
	return data;
}
void set_fifo_burst()
{
	spiSendReceive(BURST_FIFO_READ);
}


void flush_fifo(int CS)
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK, CS);
}

void start_capture(int CS)
{
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK, CS);
}

void clear_fifo_flag(int CS )
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK, CS);
}

unsigned int read_fifo_length(int CS)
{
	unsigned int len1,len2,len3,len=0;
	len1 = read_reg(FIFO_SIZE1, CS);
  len2 = read_reg(FIFO_SIZE2, CS);
  len3 = read_reg(FIFO_SIZE3, CS) & 0x7f;
  len = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return len;	
}

//Set corresponding bit  
void set_bit(unsigned char addr, unsigned char bit, int CS)
{
	unsigned char temp;
	temp = read_reg(addr, CS);
	write_reg(addr, temp | bit, CS);
}
//Clear corresponding bit 
void clear_bit(unsigned char addr, unsigned char bit, int CS)
{
	unsigned char temp;
	temp = read_reg(addr, CS);
	write_reg(addr, temp & (~bit), CS);
}

//Get corresponding bit status
unsigned char get_bit(unsigned char addr, unsigned char bit, int CS)
{
  unsigned char temp;
  temp = read_reg(addr, CS);
  temp = temp & bit;
  return temp;
}

//Set ArduCAM working mode
//MCU2LCD_MODE: MCU writes the LCD screen GRAM
//CAM2LCD_MODE: Camera takes control of the LCD screen
//LCD2MCU_MODE: MCU read the LCD screen GRAM
void set_mode(unsigned char mode,int CS)
{
  switch (mode)
  {
    case MCU2LCD_MODE:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE, CS);
      break;
    case CAM2LCD_MODE:
      write_reg(ARDUCHIP_MODE, CAM2LCD_MODE, CS);
      break;
    case LCD2MCU_MODE:
      write_reg(ARDUCHIP_MODE, LCD2MCU_MODE, CS);
      break;
    default:
      write_reg(ARDUCHIP_MODE, MCU2LCD_MODE, CS);
      break;
  }
}


void OV2640_set_JPEG_size(unsigned char size)
{
	switch(size)
	{
		case OV2640_160x120:
			wrSensorRegs8_8(OV2640_160x120_JPEG);
			break;
		case OV2640_176x144:
			wrSensorRegs8_8(OV2640_176x144_JPEG);
			break;
		case OV2640_320x240:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
		case OV2640_352x288:
	  	wrSensorRegs8_8(OV2640_352x288_JPEG);
			break;
		case OV2640_640x480:
			wrSensorRegs8_8(OV2640_640x480_JPEG);
			break;
		case OV2640_800x600:
			wrSensorRegs8_8(OV2640_800x600_JPEG);
			break;
		case OV2640_1024x768:
			wrSensorRegs8_8(OV2640_1024x768_JPEG);
			break;
		case OV2640_1280x1024:
			wrSensorRegs8_8(OV2640_1280x1024_JPEG);
			break;
		case OV2640_1600x1200:
			wrSensorRegs8_8(OV2640_1600x1200_JPEG);
			break;
		default:
			wrSensorRegs8_8(OV2640_320x240_JPEG);
			break;
	}
}

void OV5640_set_JPEG_size(unsigned char size)
{
  switch (size)
  {
    case OV5640_320x240:
      wrSensorRegs16_8(OV5640_QSXGA2QVGA);
      break;
    case OV5640_352x288:
      wrSensorRegs16_8(OV5640_QSXGA2CIF);
      break;
    case OV5640_640x480:
      wrSensorRegs16_8(OV5640_QSXGA2VGA);
      break;
    case OV5640_800x480:
      wrSensorRegs16_8(OV5640_QSXGA2WVGA);
      break;
    case OV5640_1024x768:
      wrSensorRegs16_8(OV5640_QSXGA2XGA);
      break;
    case OV5640_1280x960:
      wrSensorRegs16_8(OV5640_QSXGA2SXGA);
      break;
    case OV5640_1600x1200:
      wrSensorRegs16_8(OV5640_QSXGA2UXGA);
      break;
    case OV5640_2048x1536:
      wrSensorRegs16_8(OV5640_QSXGA2QXGA);
      break;
    case OV5640_2592x1944:
      wrSensorRegs16_8(OV5640_JPEG_QSXGA);
      break;
    default:
      //320x240
      wrSensorRegs16_8(OV5640_QSXGA2QVGA);
      break;
  }
}

void OV5642_set_JPEG_size(unsigned char size)
{ 
  switch (size)
  {
    case OV5642_320x240:
      wrSensorRegs16_8(ov5642_320x240);
      break;
    case OV5642_640x480:
      wrSensorRegs16_8(ov5642_640x480);
      break;
    case OV5642_1024x768:
      wrSensorRegs16_8(ov5642_1024x768);
      break;
    case OV5642_1280x960:
      wrSensorRegs16_8(ov5642_1280x960);
      break;
    case OV5642_1600x1200:
      wrSensorRegs16_8(ov5642_1600x1200);
      break;
    case OV5642_2048x1536:
      wrSensorRegs16_8(ov5642_2048x1536);
      break;
    case OV5642_2592x1944:
      wrSensorRegs16_8(ov5642_2592x1944);
      break;
    default:
      wrSensorRegs16_8(ov5642_320x240);
      break;
  }
}




void OV5642_set_Color_Saturation(unsigned char Color_Saturation)
	{
		switch(Color_Saturation)
		{
			case Saturation4:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x80);
				wrSensorReg16_8(0x5584 ,0x80); //80
				wrSensorReg16_8(0x5580 ,0x02);
			break;
			case Saturation3:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x70);
				wrSensorReg16_8(0x5584 ,0x70); //70
				wrSensorReg16_8(0x5580 ,0x02);
			break;
			case Saturation2:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x60);
				wrSensorReg16_8(0x5584 ,0x60); //60
				wrSensorReg16_8(0x5580 ,0x02);
			break;
			case Saturation1:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x50);
				wrSensorReg16_8(0x5584 ,0x50); //50
				wrSensorReg16_8(0x5580 ,0x02);
			break;
			case Saturation0:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x40);
				wrSensorReg16_8(0x5584 ,0x40); //40
				wrSensorReg16_8(0x5580 ,0x02);
			break;		
			case Saturation_1:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x30);
				wrSensorReg16_8(0x5584 ,0x30); //30
				wrSensorReg16_8(0x5580 ,0x02);
			break;
				case Saturation_2:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x20);
				wrSensorReg16_8(0x5584 ,0x20);//20
				wrSensorReg16_8(0x5580 ,0x02);
			break;
				case Saturation_3:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x10);
				wrSensorReg16_8(0x5584 ,0x10); //10
				wrSensorReg16_8(0x5580 ,0x02);
			break;
				case Saturation_4:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5583 ,0x00); //0
				wrSensorReg16_8(0x5584 ,0x00); //0
				wrSensorReg16_8(0x5580 ,0x02);
			break;
		}	
	}
	
	void OV5642_set_Brightness(unsigned char Brightness)
	{

		switch(Brightness)
		{
			case Brightness4:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x40);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x00);
			break;
			case Brightness3:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x30);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x00);
			break;	
			case Brightness2:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x20);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x00);
			break;
			case Brightness1:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x10);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x00);
			break;
			case Brightness0:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x00);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x00);
			break;	
			case Brightness_1:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x10);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x08);
			break;	
			case Brightness_2:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x20);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x08);
			break;	
			case Brightness_3:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x30);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x08);
			break;	
			case Brightness_4:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5589 ,0x40);
				wrSensorReg16_8(0x5580 ,0x04);
				wrSensorReg16_8(0x558a ,0x08);
			break;	
		}
			
	}

	void OV5642_set_Special_effects(unsigned char Special_effect)
	{
		switch(Special_effect)
		{
			case Bluish:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x18);
				wrSensorReg16_8(0x5585 ,0xa0);
				wrSensorReg16_8(0x5586 ,0x40);
			break;
			case Greenish:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x18);
				wrSensorReg16_8(0x5585 ,0x60);
				wrSensorReg16_8(0x5586 ,0x60);
			break;
			case Reddish:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x18);
				wrSensorReg16_8(0x5585 ,0x80);
				wrSensorReg16_8(0x5586 ,0xc0);
			break;
			case BW:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x18);
				wrSensorReg16_8(0x5585 ,0x80);
				wrSensorReg16_8(0x5586 ,0x80);
			break;
			case Negative:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x40);
			break;
			
				case Sepia:
				wrSensorReg16_8(0x5001 ,0xff);
				wrSensorReg16_8(0x5580 ,0x18);
				wrSensorReg16_8(0x5585 ,0x40);
				wrSensorReg16_8(0x5586 ,0xa0);
			break;
			case Normal:
				wrSensorReg16_8(0x5001 ,0x7f);
				wrSensorReg16_8(0x5580 ,0x00);		
			break;		
		}
	}

	
	
	void OV5642_set_Sharpness(unsigned char Sharpness)
	{
		switch(Sharpness)
		{
			case Auto_Sharpness_default:
				wrSensorReg16_8(0x530A ,0x00);
				wrSensorReg16_8(0x530c ,0x0 );
				wrSensorReg16_8(0x530d ,0xc );
				wrSensorReg16_8(0x5312 ,0x40);
			break;
			case Auto_Sharpness1:
				wrSensorReg16_8(0x530A ,0x00);
				wrSensorReg16_8(0x530c ,0x4 );
				wrSensorReg16_8(0x530d ,0x18);
				wrSensorReg16_8(0x5312 ,0x20);
			break;
			case Auto_Sharpness2:
				wrSensorReg16_8(0x530A ,0x00);
				wrSensorReg16_8(0x530c ,0x8 );
				wrSensorReg16_8(0x530d ,0x30);
				wrSensorReg16_8(0x5312 ,0x10);
			break;
			case Manual_Sharpnessoff:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x00);
				wrSensorReg16_8(0x531f ,0x00);
			break;
			case Manual_Sharpness1:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x04);
				wrSensorReg16_8(0x531f ,0x04);
			break;
			case Manual_Sharpness2:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x08);
				wrSensorReg16_8(0x531f ,0x08);
			break;
			case Manual_Sharpness3:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x0c);
				wrSensorReg16_8(0x531f ,0x0c);
			break;
			case Manual_Sharpness4:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x0f);
				wrSensorReg16_8(0x531f ,0x0f);
			break;
			case Manual_Sharpness5:
				wrSensorReg16_8(0x530A ,0x08);
				wrSensorReg16_8(0x531e ,0x1f);
				wrSensorReg16_8(0x531f ,0x1f);
			break;
		}
	}
	
unsigned char wrSensorReg8_8(int regID, int regDat)
{
	delay_us(10);
	sccb_bus_start();                          
	if(sccb_bus_write_byte(sensor_addr) == 0)         
	{
		sccb_bus_stop();                        
		return 1;
	}
	delay_us(10);
	if(sccb_bus_write_byte(regID) == 0)
	{
		sccb_bus_stop();                              
		return 2;                                       
	}
	delay_us(10);
	if(sccb_bus_write_byte(regDat)==0)                    
	{
		sccb_bus_stop();                                 
		return 3;
	}
	sccb_bus_stop();                                    
	return 0;
}


unsigned char rdSensorReg8_8(unsigned char regID, unsigned char* regDat)
{
	delay_us(10);
	
	sccb_bus_start();
	if(sccb_bus_write_byte(sensor_addr) == 0)                 
	{
		sccb_bus_stop();                                
		//goto start;
		return 1;                                        
	}
	delay_us(10);
	if(sccb_bus_write_byte(regID)==0)//ID
	{
		sccb_bus_stop();                                  
		//goto start;
		return 2;                                       
	}
	sccb_bus_stop();                                   
	delay_us(10);	
	sccb_bus_start();
	if(sccb_bus_write_byte(sensor_addr|0x01)==0)                    
	{
		sccb_bus_stop();                                   
		//goto start;
		return 3;                                          
	}
	delay_us(10);
	*regDat = sccb_bus_read_byte();                    
	sccb_bus_send_noack();                                
	sccb_bus_stop();                                      
	return 0;                
}

//I2C Array Write 8bit address, 8bit data
int wrSensorRegs8_8(const struct sensor_reg reglist[])
{
  int err = 0;
  unsigned int reg_addr = 0;
  unsigned int reg_val = 0;
  const struct sensor_reg *next = reglist;
  while ((reg_addr != 0xff) | (reg_val != 0xff))
  {
    reg_addr =next->reg;
    reg_val = next->val;
    err = wrSensorReg8_8(reg_addr, reg_val);
    delay_ms(10);
    next++;
  }

  return err;
}

unsigned char wrSensorReg16_8(int regID, int regDat)
{
	sccb_bus_start();
	if(0==sccb_bus_write_byte(sensor_addr))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(10);
  if(0==sccb_bus_write_byte(regID>>8))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(10);
  if(0==sccb_bus_write_byte(regID))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(10);
  if(0==sccb_bus_write_byte(regDat))
	{
		sccb_bus_stop();
		return(0);
	}
  sccb_bus_stop();
	
  return(1);
}

int wrSensorRegs16_8(const struct sensor_reg reglist[])
{
  int err = 0;
  unsigned int reg_addr;
  unsigned char reg_val;
  const struct sensor_reg *next = reglist;

  while ((reg_addr != 0xffff) | (reg_val != 0xff))
  {
    reg_addr =next->reg;
    reg_val = next->val;
    err = wrSensorReg16_8(reg_addr, reg_val);
    next++;
	delay_ms(1);
  }
  return err;
}


int rdSensorRegs16_8(const struct sensor_reg reglist[])
{
  int err = 0;
  unsigned char testVal =0;
  unsigned int reg_addr;
  unsigned char reg_val;
  const struct sensor_reg *next = reglist;

  while ((reg_addr != 0xffff) | (reg_val != 0xff))
  {
    reg_addr =next->reg;
    reg_val = next->val;
   // err = wrSensorReg16_8(reg_addr, reg_val);
		//printf("Write register %04x value %02x\r\n",reg_addr,reg_val);
		rdSensorReg16_8(reg_addr,&testVal);
		printf("Read  register %04x value %02x  ",reg_addr,testVal);
		if(testVal != reg_val){
			printf("(error) \r\n");
		}else{
		printf("\r\n");
		}
    next++;
  }
  return err;
}

unsigned char rdSensorReg16_8(unsigned int regID, unsigned char* regDat)
{
	sccb_bus_start();                  
	if(0==sccb_bus_write_byte(0x78))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
	delay_us(20);
  if(0==sccb_bus_write_byte(regID>>8))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
  if(0==sccb_bus_write_byte(regID))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
	sccb_bus_stop();
	
	delay_us(20);
	
	
	sccb_bus_start();                 
	if(0==sccb_bus_write_byte(0x79))
	{
		sccb_bus_stop();
		return(0);
	}
	delay_us(20);
  *regDat=sccb_bus_read_byte();
  sccb_bus_send_noack();
  sccb_bus_stop();
  return(1);
}

void resetFirmware(int CS1, int CS2, int CS3, int CS4){

	if (CS1 > -1 ){
		write_reg(0x07, 0x80,CS1);
		delay_ms(100);
		write_reg(0x07, 0x00,CS1);
		delay_ms(100);
	if(sensor_model == OV5640 || sensor_model == OV5642){
		  write_reg(ARDUCHIP_FRAMES, 0x00,CAM_CS1);
		  set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK, CAM_CS1);
	}
		
	}
	if (CS2 > -1 ){
		write_reg(0x07, 0x80,CS2);
		delay_ms(100);
		write_reg(0x07, 0x00,CS2);
		delay_ms(100);
		if(sensor_model==OV5640 || sensor_model == OV5642){
		  write_reg(ARDUCHIP_FRAMES, 0x00,CAM_CS2);
		  set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK, CAM_CS2);
	}
	}
	if (CS3 > -1 ){
		write_reg(0x07, 0x80,CS3);
		delay_ms(100);
		write_reg(0x07, 0x00,CS3);
		delay_ms(100);
		if(sensor_model==OV5640 || sensor_model == OV5642){
		  write_reg(ARDUCHIP_FRAMES, 0x00,CAM_CS3);
		  set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK, CAM_CS3);
	}
	}
	if (CS4 > -1 ){
		write_reg(0x07, 0x80,CS4);
		delay_ms(100);
		write_reg(0x07, 0x00,CS4);
		delay_ms(100);
		if(sensor_model==OV5640 || sensor_model == OV5642){
		  write_reg(ARDUCHIP_FRAMES, 0x00,CAM_CS4);
		  set_bit(ARDUCHIP_TIM, VSYNC_LEVEL_MASK, CAM_CS4);
	}
	}

}

 void singleCapture(int CS){
   int i , count;
   //Flush the FIFO
   flush_fifo(CS);
   //Start capture
   start_capture(CS); 
   while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK, CS)){;}
   length = read_fifo_length(CS);
   count = length;
   i = 0 ;
   CS_LOW(CS);
   set_fifo_burst();//Set fifo burst mode
   while (count--) {
	  readbuf[i++] = spiSendReceive(0x00);
   	}
	count = 0;
	CS_HIGH(CS);
}





void Arducam_bus_detect(int CS1,int CS2,int CS3,int CS4){
unsigned char vid, pid,temp ;

if(CS1> -1){
	while(1){  	
  		write_reg(ARDUCHIP_TEST1, 0x55 ,CS1 );
  		temp = read_reg(ARDUCHIP_TEST1 ,CS1 );
 		if (temp != 0x55){
    		printf("SPI1 interface Error!\n");
			delay_ms(1000);
    		continue;
  		}
		
  		else{
    		 printf("SPI1 interface OK!\r\n");
	 		break;
    	}
	}	
}


 if(CS2> -1){
	 while(1){	 
		 write_reg(ARDUCHIP_TEST1, 0x55 ,CS2 );
		 temp = read_reg(ARDUCHIP_TEST1 ,CS2 );
		 if (temp != 0x55){
			 printf("SPI2 interface Error!\n");
			 delay_ms(1000);
			 continue;
		 }
		 
		 else{
			  printf("SPI2 interface OK!\r\n");
			 break;
		 }
	 }	 
 }

 if(CS3> -1){
	while(1){  	
  		write_reg(ARDUCHIP_TEST1, 0x55 ,CS3 );
  		temp = read_reg(ARDUCHIP_TEST1 ,CS3 );
 		if (temp != 0x55){
    		printf("SPI3 interface Error!\n");
			delay_ms(1000);
    		continue;
  		}
		
  		else{
    		 printf("SPI3 interface OK!\r\n");
	 		break;
    	}
	}	
}
 if(CS4> -1){
	 while(1){	 
		 write_reg(ARDUCHIP_TEST1, 0x55 ,CS4 );
		 temp = read_reg(ARDUCHIP_TEST1 ,CS4 );
		 if (temp != 0x55){
			 printf("SPI4 interface Error!\n");
			 delay_ms(1000);
			 continue;
		 }
		 
		 else{
			  printf("SPI4 interface OK!\r\n");
			 break;
		 }
	 }	 
 }


 while(1){
   sensor_addr = 0x60;
   wrSensorReg8_8(0xff, 0x01);
   rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
   rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
   if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
   printf("Can't find OV2640 module!\r\n");
   else{
		  sensor_model =  OV2640 ;
		  printf("OV2640 detected.\r\n");   
		  break;
	 }
	 sensor_addr = 0x78;
	 rdSensorReg16_8(OV5640_CHIPID_HIGH, &vid);
   rdSensorReg16_8(OV5640_CHIPID_LOW, &pid);
   if ((vid != 0x56) || (pid != 0x40))
   printf("Can't find OV5640 module!\r\n");
   else{
			sensor_model =  OV5640 ;
			printf("OV5640 detected.\r\n");
		  break;
	 }
	 
	 sensor_addr = 0x78;
	 rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
   rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
   if ((vid != 0x56) || (pid != 0x42)){
		printf("Can't find OV5642 module!\r\n");
		 continue;
	 }
   else{
		 sensor_model =  OV5642 ;
		 printf("OV5642 detected.\r\n"); 
		 break;		 
	 }
  }
}





