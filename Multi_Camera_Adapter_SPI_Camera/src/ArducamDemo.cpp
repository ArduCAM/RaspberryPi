#include <stdlib.h>
#include <stdio.h>
#include "ArduCAM.h"
#include "sccb_bus.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <netdb.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

imageData* ArducamImageData = new imageData;

#define CAMERA_1_EN  
#define CAMERA_2_EN  
#define CAMERA_3_EN  
#define CAMERA_4_EN  
//#define CAMERA_5_EN  
//#define CAMERA_6_EN 
//#define CAMERA_7_EN 
//#define CAMERA_8_EN 



#define posW            320
#define posH            240

#define SPI_CLK_SPEED   2000000

cv::Mat JPGToMat(uint8_t* bytes,int length){
	cv::Mat image = Mat(1,length,CV_8UC1,bytes);
	if(length <= 0){
		image.data = NULL;
	    return image;
	}
	image = imdecode(image,IMREAD_ANYCOLOR);
	return image;
}
cv::Mat ConvertImage(imageData* frameData){
	cv::Mat rawImage ;
       if(frameData->dataLength<=0){
        printf("The %d camera is error.\r\n",frameData->cameraID);  
        return rawImage;
    }
	rawImage = JPGToMat(frameData->pu8ImageData,frameData->dataLength);
	return rawImage;
}
#if defined (CAMERA_1_EN)
ArduCAM myCAM1( OV2640, CAM_CS1 );
#endif
#if defined (CAMERA_2_EN)
ArduCAM myCAM2( OV2640, CAM_CS2);
#endif
#if defined (CAMERA_3_EN)
ArduCAM myCAM3( OV2640, CAM_CS3 );
#endif
#if defined (CAMERA_4_EN)
ArduCAM myCAM4( OV2640, CAM_CS4);
#endif
#if defined (CAMERA_5_EN)
ArduCAM myCAM5( OV2640, CAM_CS5);
#endif
#if defined (CAMERA_6_EN)
ArduCAM myCAM6( OV2640, CAM_CS6);
#endif
#if defined (CAMERA_7_EN)
ArduCAM myCAM7( OV2640, CAM_CS7);
#endif
#if defined (CAMERA_8_EN)
ArduCAM myCAM8( OV2640, CAM_CS8);
#endif



int main(int argc, char *argv[]){
   printf("Arducam begin.\r\n");
    unsigned char vid, pid;
    unsigned char temp;
    cv::Mat rawImage ;
    pioInit();
    spiInit(SPI_CLK_SPEED,0);
    sccb_bus_init();
    pinMode(sigTrig, OUTPUT);
    digitalWrite(sigTrig, HIGH);
    #if defined (CAMERA_1_EN)
    myCAM1.Arducam_CS_Init();
    myCAM1.resetFirmware();
    #endif
    #if defined (CAMERA_2_EN)
    myCAM2.Arducam_CS_Init();
    myCAM2.resetFirmware();
    #endif
    #if defined (CAMERA_3_EN)
    myCAM3.Arducam_CS_Init();
    myCAM3.resetFirmware();
    #endif
    #if defined (CAMERA_4_EN)
    myCAM4.Arducam_CS_Init();
    myCAM4.resetFirmware();
    #endif
    #if defined (CAMERA_5_EN)
    myCAM5.Arducam_CS_Init();
    myCAM5.resetFirmware();
    #endif
 #if defined (CAMERA_6_EN)
    myCAM6.Arducam_CS_Init();
    myCAM6.resetFirmware();
    #endif
 #if defined (CAMERA_7_EN)
    myCAM7.Arducam_CS_Init();
    myCAM7.resetFirmware();
    #endif
 #if defined (CAMERA_8_EN)
    myCAM8.Arducam_CS_Init();
    myCAM8.resetFirmware();
    #endif
    delay_ms(100);
    printf("Begin to test the SPI bus\r\n");
    #if defined (CAMERA_1_EN)
    while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM1.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM1.read_reg(ARDUCHIP_TEST1);
      if (temp != 0x55){
        printf("SPI1 interface Error!\r\n");
        printf("Expect: 0x55. Read data :0x%x\r\n",temp);
        digitalWrite(sigTrig, LOW); delay_ms(5);
        digitalWrite(sigTrig, HIGH);
        delay_ms(1000);continue;
      }
      else{
        printf("SPI1 interface OK.\r\n");break;
      }
      
    }
    #endif
  #if defined (CAMERA_2_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM2.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM2.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI2 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI2 interface OK.\r\n");break;
      }
    }
  #endif
   #if defined (CAMERA_3_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM3.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM3.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI3 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI3 interface OK.\r\n");break;
      }
    }
  #endif
   #if defined (CAMERA_4_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM4.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM4.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI4 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI4 interface OK.\r\n");break;
      }
    }
  #endif
   #if defined (CAMERA_5_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM5.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM5.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI5 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI5 interface OK.\r\n");break;
      }
    }
  #endif
 #if defined (CAMERA_6_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM6.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM6.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI6 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI6 interface OK.\r\n");break;
      }
    }
  #endif
 #if defined (CAMERA_7_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM7.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM7.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI7 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI7 interface OK.\r\n");break;
      }
    }
  #endif
 #if defined (CAMERA_8_EN)
  while(1){
      //Check if the ArduCAM SPI bus is OK
      myCAM8.write_reg(ARDUCHIP_TEST1, 0x55);
      temp = myCAM8.read_reg(ARDUCHIP_TEST1);

      if (temp != 0x55){
        printf("SPI8 interface Error!\r\n");
        delay_ms(1000);continue;
      }else{
        printf("SPI8 interface OK.\r\n");break;
      }
    }
  #endif
    while(1){
        //Check if the camera module type is OV2640
        myCAM1.wrSensorReg8_8(0xff, 0x01);
        myCAM1.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
        myCAM1.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
        if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
          printf("Can't find OV2640 module!\r\n");
         delay_ms(1000);continue;
        }
        else{
          printf("OV2640 detected.\r\n");break;
        } 
    }
    myCAM1.set_format(JPEG);
    myCAM1.InitCAM();
    myCAM1.OV2640_set_JPEG_size(OV2640_320x240);
    delay_ms(1000);
#if defined (CAMERA_1_EN)
    cv::namedWindow("Arducam SPI1 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI1 Camera", posW*1,posH*1);

#endif
#if defined (CAMERA_2_EN)
    cv::namedWindow("Arducam SPI2 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI2 Camera",posW*2,posH*1);

#endif
#if defined (CAMERA_3_EN)
    cv::namedWindow("Arducam SPI3 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI3 Camera", posW*3,posH*1);

#endif
#if defined (CAMERA_4_EN)
    cv::namedWindow("Arducam SPI4 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI4 Camera", posW*4,posH*1);

#endif
#if defined (CAMERA_5_EN)
    cv::namedWindow("Arducam SPI5 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI5 Camera", posW*1,posH*2);
#endif

#if defined (CAMERA_6_EN)
    cv::namedWindow("Arducam SPI6 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI6 Camera", posW*2,posH*2);
#endif
#if defined (CAMERA_7_EN)
    cv::namedWindow("Arducam SPI7 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI7 Camera", posW*3,posH*2);
#endif
#if defined (CAMERA_8_EN)
    cv::namedWindow("Arducam SPI8 Camera", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Arducam SPI8 Camera", posW*4,posH*2);
#endif





while(1){
#if defined (CAMERA_1_EN)
    myCAM1.getOneframe(ArducamImageData); 
    rawImage = ConvertImage(ArducamImageData);
    if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
    cv::imshow("Arducam SPI1 Camera", rawImage);
#endif
#if defined (CAMERA_2_EN)
    myCAM2.getOneframe(ArducamImageData); 
    rawImage = ConvertImage(ArducamImageData);
    if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
    cv::imshow("Arducam SPI2 Camera", rawImage);
#endif
#if defined (CAMERA_3_EN)
        myCAM3.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
       cv::imshow("Arducam SPI3 Camera", rawImage);
#endif
#if defined (CAMERA_4_EN)
        myCAM4.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
        cv::imshow("Arducam SPI4 Camera", rawImage);
#endif
#if defined (CAMERA_5_EN)
        myCAM5.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
        cv::imshow("Arducam SPI5 Camera", rawImage);
#endif
#if defined (CAMERA_6_EN)
        myCAM6.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
        cv::imshow("Arducam SPI6 Camera", rawImage);
#endif
#if defined (CAMERA_7_EN)
        myCAM7.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
        cv::imshow("Arducam SPI7 Camera", rawImage);
#endif
#if defined (CAMERA_8_EN)
        myCAM8.getOneframe(ArducamImageData); 
        rawImage = ConvertImage(ArducamImageData);
        if(rawImage.empty()){
        printf("Image load failed\r\n");
    }else
        cv::imshow("Arducam SPI8 Camera", rawImage);
#endif
    
    if(waitKey(20) >=0) break;       // 等待按键，跳出

} 
}





