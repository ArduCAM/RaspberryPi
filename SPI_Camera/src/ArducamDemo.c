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


void *readDataThread(void *arg);
//void *sendDataThread(void *arg);
void  dataParse(char* databuf);
void  INThandler(int sig);


int sockfd, newsockfd, portno, clilen;
struct sockaddr_in serv_addr, cli_addr;
struct sigaction sa;
unsigned char start_read_data = 0;
unsigned char read_data_OK = 0;

unsigned int length_cam1;
//unsigned int length_cam2;
//unsigned int length_cam3;
//unsigned int length_cam4;





int main(int argc, char *argv[])
{
  int  n;
  int on = 1;
  pthread_t _readData;//_sendData;
  pioInit();
  ArduCAM_CS_init( CAM_CS1, -1, -1, -1 );   // init the cs
  // ArduCAM_CS_init( CAM_CS1, CAM_CS2, CAM_CS3, CAM_CS4 );   // init the cs

  sccb_bus_init();
  spiInit(4000000, 0); //8MHZ
  //Arducam_bus_detect( CAM_CS1, CAM_CS2, CAM_CS3, CAM_CS4 );   // detect the SPI bus
  Arducam_bus_detect( CAM_CS1, -1, -1, -1 );

  resetFirmware( CAM_CS1, -1, -1, -1 );  //reset the firmware
  // resetFirmware( CAM_CS1, CAM_CS2, CAM_CS3, CAM_CS4);  //reset the firmware
  ArduCAM_Init(sensor_model);
  signal(SIGINT, INThandler);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("ERROR opening socket\r\n");
    exit(1);
  }

  /* Initialize socket structure */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = 2002;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  //creat two thread
  pthread_create(&_readData, NULL, readDataThread, NULL);
 // pthread_create(&_sendData, NULL, sendDataThread, NULL);
  sleep(1);
  /*Add support port reuse*/
  if ((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
  {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }
  /* Now bind the host address using bind() call.*/
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("ERROR on binding");
    exit(1);
  }
  /* Add support reconnection */
  sa.sa_handler = SIG_IGN;
  sigaction( SIGPIPE, &sa, 0 );
  /* Now start listening for the clients, here process will
    go in sleep mode and will wait for the incoming connection
  */
  while (1) {
    read_data_OK = 0;
    start_read_data = 0;
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    printf("Waitting connection...\r\n");

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (void *) &clilen);
    if (newsockfd < 0) {
      printf("error\r\n");
    }
    printf("connect successfully\r\n");

    bzero(&revCmdData, CMD_BUF_SIZE);  // Clear the revCmdData buffer
    while ((n = read(newsockfd, &revCmdData, CMD_BUF_SIZE)) > 0) {
      // printf(revCmdData);  // Check the receive cmd data
      dataParse(revCmdData);  // parse the data received
      bzero(&revCmdData, CMD_BUF_SIZE);  // Clear the revCmdData buffer
    }

    if (n < 0) {
      printf("Connection exception!\n");
      read_data_OK = 0;
      start_read_data = 0;

    }

  }
  printf("Close the sockfd\r\n");
  close(newsockfd);
  close(sockfd);
  return 0;
}




void *readDataThread(void *arg) {
  while (1) {
    if (start_read_data == 1) {
      singleCapture(CAM_CS1);
	  
      sendbuf_cam1 = readbuf;
      length_cam1 = length;
      if (sockfd) {
        write(newsockfd, sendbuf_cam1 , length_cam1);
      }
      else {
        start_read_data = 0;
        break;
      }
/*
      singleCapture(CAM_CS2);
      sendbuf_cam2 = readbuf;
      length_cam2 = length;
      if (sockfd) {
        write(newsockfd, sendbuf_cam2 , length_cam2);
      }
      else {
        start_read_data = 0;
        break;
      }
      singleCapture(CAM_CS3);
      sendbuf_cam3 = readbuf;
      length_cam3 = length;
      if (sockfd) {
        write(newsockfd, sendbuf_cam3 , length_cam3);
      }
      else {
        start_read_data = 0;
        break;
      }
      singleCapture(CAM_CS4);
      sendbuf_cam4 = readbuf;
      length_cam4 = length;
      if (sockfd) {
        write(newsockfd, sendbuf_cam4 , length_cam4);
      }
      else {
        start_read_data = 0;
        break;
      }

*/
      // start_read_data = 0;

    }
  }
  return 0;
}


//void *sendDataThread(void *arg) {
 // ;
//}


void  INThandler(int sig)
{
  char  c;
  signal(sig, SIG_IGN);
  printf("You hit Ctrl-C\n"
         "Do you really want to quit? [y/n] ");
  c = getchar();
  if (c == 'y' || c == 'Y') {
  	
    if (newsockfd < 0) {
      close(newsockfd);
      close(sockfd);
    }
	start_read_data = 0;
    printf("Bye Arducam.\n");
    printf("If you want to resart Arducam, just run \"sudo ./run_Arducam_Demo\"\r\n");
    exit(0);
  }
  else
    signal(SIGINT, INThandler);
  getchar(); // Get new line character
}



void  dataParse(char* databuf) {
  char* response = "HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
  char* net_check = "HTTP/1.1 200 OK\r\n\r\n 123";

  if (strchr(databuf, 0)) {
    write(newsockfd, net_check , strlen(net_check));
  }
  if (strstr(databuf, "stream") ) {
    write(newsockfd, response , strlen(response));
    start_read_data = 1;
  }
  if (strstr(databuf, "ql=0")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_160x120);
      printf("Set the resolution to OV2640_160x120 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_320x240);
      printf("Set the resolution to OV5640_320x240 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_320x240);
      printf("Set the resolution to OV5642_320x240 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=1")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_176x144);
      printf("Set the resolution to OV2640_176x144 successfully\r\n");

    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_352x288);
      printf("Set the resolution to OV5640_352x288 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_640x480);
      printf("Set the resolution to OV5642_640x480 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=2")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_320x240);
      printf("Set the resolution to OV2640_320x240 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_640x480);
      printf("Set the resolution to OV5640_640x480 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_1024x768);
      printf("Set the resolution to OV5642_1024x768 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=3")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_352x288);
      printf("Set the resolution to OV2640_352x288 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_800x480);
      printf("Set the resolution to OV5640_800x480 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_1280x960);
      printf("Set the resolution to OV5642_1280x960 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=4") ) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_640x480);
      printf("Set the resolution to OV2640_640x480 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_1024x768);
      printf("Set the resolution to OV5640_1024x768 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_1600x1200);
      printf("Set the resolution to OV5642_1600x1200 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=5")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_800x600);
      printf("Set the resolution to OV2640_800x600 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_1280x960);
      printf("Set the resolution to OV5640_1280x960 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_2048x1536);
      printf("Set the resolution to OV5642_2048x1536 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=6")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_1024x768);
      printf("Set the resolution to OV2640_1024x768 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_1600x1200);
      printf("Set the resolution to OV5640_1600x1200 successfully\r\n");
    }
    else if (sensor_model == OV5642) {
      OV5642_set_JPEG_size(OV5642_2592x1944);
      printf("Set the resolution to OV5642_2592x1944 successfully\r\n");

    }
  }
  else if (strstr(databuf, "ql=7")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_1280x1024);
      printf("Set the resolution to OV2640_1280x1024 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_2048x1536);
      printf("Set the resolution to OV5640_2048x1536 successfully\r\n");
    }
  }
  else if (strstr(databuf, "ql=8")) {
    if (sensor_model == OV2640) {
      OV2640_set_JPEG_size(OV2640_1600x1200);
      printf("Set the resolution to OV2640_1600x1200 successfully\r\n");
    }
    else if (sensor_model == OV5640) {
      OV5640_set_JPEG_size(OV5640_2592x1944);
      printf("Set the resolution to OV5640_2592x1944 successfully\r\n");
    }
  }
}



