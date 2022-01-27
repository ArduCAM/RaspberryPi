#include "ArduCAM.h"
#include "sccb_bus.h"
#include "ov2640_regs.h"
#include "ov5642_regs.h"
#include "ov5640_regs.h"
unsigned char sensor_model = 0;
unsigned char sensor_addr = 0;
unsigned char m_fmt = JPEG;
unsigned int length = 0;
unsigned char readbuf[JPEG_BUF_SIZE];

ArduCAM::ArduCAM(){
    sensor_model = OV7670;
    sensor_addr = 0x42;
}

ArduCAM :: ArduCAM(byte mode, int CS){
    if(CS >= 0){
        B_CS = CS;
    }
    sensor_model = mode;
    switch (sensor_model)
        {
         case OV2640:
            sensor_addr =0x60; 
            break;
         case OV5642:
            sensor_addr = 0x78;
            break;        
        }
}
void  ArduCAM::Arducam_CS_Init(void){
    pinMode(B_CS, OUTPUT);
    digitalWrite(B_CS, HIGH);
}


void ArduCAM::InitCAM() 
{
  switch (sensor_model)
  {
    case OV2640:
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
}

void ArduCAM:: CS_HIGH()
{
    delay_us(1);
 	digitalWrite(B_CS, HIGH);					
}

void ArduCAM:: CS_LOW()
{
    delay_us(1);
 	digitalWrite(B_CS, LOW);						    
}

void ArduCAM:: set_format(unsigned char fmt)
{
  if (fmt == BMP)
    m_fmt = BMP;
  else if(fmt == RAW)
  	m_fmt = RAW;
  else
    m_fmt = JPEG;
}

unsigned char ArduCAM:: bus_read(int address)
{
	unsigned char value;
    CS_LOW();
	spiSendReceive(address);
	value = spiSendReceive(0x00);
	CS_HIGH();
	return value;
}

unsigned char ArduCAM:: bus_write(int address,int value)
{	
	CS_LOW();
	spiSendReceive(address);
	spiSendReceive(value);
	CS_HIGH();
	return 1;
}

unsigned char ArduCAM:: read_reg(unsigned char addr)
{
	unsigned char readData;
	readData = bus_read(addr & 0x7F);
	return readData;
}
void ArduCAM:: write_reg(unsigned char addr, unsigned char data)
{
	 bus_write(addr | 0x80, data); 
}

unsigned char ArduCAM:: read_fifo()
{
	unsigned char data;
	data = bus_read(SINGLE_FIFO_READ);
	return data;
}
void ArduCAM:: set_fifo_burst()
{
	spiSendReceive(BURST_FIFO_READ);
}


void ArduCAM:: flush_fifo()
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

void ArduCAM:: start_capture()
{
	write_reg(ARDUCHIP_FIFO, FIFO_START_MASK);
}

void ArduCAM:: clear_fifo_flag( )
{
	write_reg(ARDUCHIP_FIFO, FIFO_CLEAR_MASK);
}

unsigned int ArduCAM:: read_fifo_length()
{
	unsigned int len1,len2,len3,len=0;
	len1 = read_reg(FIFO_SIZE1);
  len2 = read_reg(FIFO_SIZE2);
  len3 = read_reg(FIFO_SIZE3) & 0x7f;
  len = ((len3 << 16) | (len2 << 8) | len1) & 0x07fffff;
	return len;	
}

//Set corresponding bit  
void ArduCAM:: set_bit(unsigned char addr, unsigned char bit)
{
	unsigned char temp;
	temp = read_reg(addr);
	write_reg(addr, temp | bit);
}
//Clear corresponding bit 
void ArduCAM:: clear_bit(unsigned char addr, unsigned char bit)
{
	unsigned char temp;
	temp = read_reg(addr);
	write_reg(addr, temp & (~bit));
}

//Get corresponding bit status
unsigned char ArduCAM:: get_bit(unsigned char addr, unsigned char bit)
{
  unsigned char temp;
  temp = read_reg(addr);
  temp = temp & bit;
  return temp;
}


void ArduCAM:: OV2640_set_JPEG_size(unsigned char size)
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



void ArduCAM:: OV5642_set_JPEG_size(unsigned char size)
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






unsigned char ArduCAM:: wrSensorReg8_8(int regID, int regDat)
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


unsigned char ArduCAM:: rdSensorReg8_8(unsigned char regID, unsigned char* regDat)
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
int ArduCAM:: wrSensorRegs8_8(const struct sensor_reg reglist[])
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

unsigned char ArduCAM:: wrSensorReg16_8(int regID, int regDat)
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

int ArduCAM:: wrSensorRegs16_8(const struct sensor_reg reglist[])
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

int ArduCAM:: rdSensorRegs16_8(const struct sensor_reg reglist[])
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

unsigned char ArduCAM:: rdSensorReg16_8(unsigned int regID, unsigned char* regDat)
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

void ArduCAM:: resetFirmware(){
    //Reset the CPLD
    write_reg(0x07, 0x80);
    delay_ms(100);
    write_reg(0x07, 0x00);
    delay_ms(100);

}

 imageData* ArduCAM:: getOneframe(imageData* dataBuf){
   int i , count;
   bool is_header=0;
   bool is_end = 0;
   bool errorFlag = 0;
   uint8_t temp_last = 0;
   uint8_t temp = 0;
   if( B_CS == CAM_CS1 )
    dataBuf->cameraID = 1;
   else if (B_CS == CAM_CS2)
     dataBuf->cameraID = 2;
   else if (B_CS == CAM_CS3)
     dataBuf->cameraID = 3;
   else if (B_CS == CAM_CS4)
     dataBuf->cameraID = 4;
   else if (B_CS == CAM_CS5)
     dataBuf->cameraID = 5;
   else if (B_CS == CAM_CS6)
     dataBuf->cameraID = 6;
   else if (B_CS == CAM_CS7)
     dataBuf->cameraID = 7;
   else if (B_CS == CAM_CS8)
     dataBuf->cameraID = 8;
   //Flush the FIFO
   flush_fifo();
   //Start capture
   start_capture(); 
  // printf("Start capture...\r\n");
   while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
   length = read_fifo_length();
   if(length == 0 || length > 0x7FFFF) //512K
    {
        printf("The %d camera length error.\r\n",dataBuf->cameraID);
	return dataBuf;
   }
   count = length;
   i = 0 ;
   CS_LOW();
   set_fifo_burst();//Set fifo burst mode
   dataBuf->dataLength = 0;
   while (count--) {
      temp_last = temp;
      temp = spiSendReceive(0x00);
    
     //Read JPEG data from FIFO
      if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
      {
        is_end = 1;
        dataBuf->pu8ImageData[i++] = temp;  //save the last  0XD9   
        dataBuf->dataLength = i-1;
#if 0
        printf("Image save OK.\r\n");
        printf("The first byte is %x\r\n", dataBuf->pu8ImageData[0]);
        printf("The end byte is %x\r\n", dataBuf->pu8ImageData[i-1]);
#endif                  
       // is_header = false;
        break;
      } 
      if (is_header == true)
      { 
        dataBuf->pu8ImageData[i++] = temp;       
      }
      
      else if ((temp == 0xD8) && (temp_last == 0xFF))
      {
        is_header = true;
        dataBuf->pu8ImageData[i++] = temp_last;
        dataBuf->pu8ImageData[i++] = temp; 
      } 
   	}
   if(is_header == 0){
     printf("The %d camera can't find the header\r\n",dataBuf->cameraID);
       errorFlag = 1;
   }else{
      is_header = 0;
   }
   if(is_end == 0){
     printf("The %d camera can't find the end\r\n",dataBuf->cameraID);
     errorFlag = 1;
   }else {
      is_end = 0;
   }   
   if(errorFlag){
     dataBuf->errCount ++;
     printf("The %d frame is error\r\n",dataBuf->errCount);
   }

	count = 0;
	CS_HIGH();
    return dataBuf;
}


void ArduCAM:: Arducam_bus_detect(){
unsigned char vid, pid,temp ;
	 while(1){	 
		 write_reg(ARDUCHIP_TEST1, 0x55  );
		 temp = read_reg(ARDUCHIP_TEST1  );
		 if (temp != 0x55){
			 printf("SPI interface Error!\n");
			 delay_ms(1000);
			 continue;
		 }
		 
		 else{
			  printf("SPIinterface OK!\r\n");
			 break;
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





