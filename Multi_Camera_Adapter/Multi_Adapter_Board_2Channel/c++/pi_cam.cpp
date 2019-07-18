#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define RPI_FIRMWARE_DEV "/dev/vcio"
#define IOCTL_RPI_FIRMWARE_PROPERTY _IOWR(100, 0, char*)
#define RPI_FIRMWARE_STATUS_REQUEST 0
#define RPI_FIRMWARE_STATUS_SUCCESS 0x80000000
#define RPI_FIRMWARE_STATUS_ERROR   0x80000001
#define RPI_FIRMWARE_PROPERTY_END   0
#define RPI_FIRMWARE_SET_GPIO_STATE 0x00038041


//	define the support board version 

using namespace cv;
using namespace std;

#define PI_ZERO    0
#define PI_ZERO_W  1
#define PI_3B_3B_PLUS 2


int boardVersion = PI_ZERO_W;
typedef struct timeval TIME;
#define GPIO_NUM 3
int digitalNum[GPIO_NUM];
int status[GPIO_NUM];
int lastStatus[GPIO_NUM];
int write_virtual_gpio(int gpio,int value){
    int fd;

  fd = open(RPI_FIRMWARE_DEV, O_NONBLOCK);
  if (fd == -1) {
    //log_error("open firware dev fail.\n");
    return -1;
  }

    uint32_t buf[] = { 
        8 * sizeof(uint32_t), 
        RPI_FIRMWARE_STATUS_REQUEST, 
        RPI_FIRMWARE_SET_GPIO_STATE, 
        2 * sizeof(int), 
        0, 
        gpio,
        value ? 1 : 0,
        RPI_FIRMWARE_PROPERTY_END
    };
    int ret = ioctl(fd, IOCTL_RPI_FIRMWARE_PROPERTY, buf);
    if(ret == -1){
        //log_error("ioctl fail\n");
        return -1;
    }
    if(buf[1] != RPI_FIRMWARE_STATUS_SUCCESS){
      //  log_error("NOT RPI_FIRMWARE_STATUS_SUCCESS\n");
        return -1;
    }
    return 0;
}


//Remove the header spack
char *rtrim(char *str)
{
  if (str == NULL || *str == '\0')
  {
    return str;
  }
 
  int len = strlen(str);
  char *p = str + len - 1;
  while (p >= str  && isspace(*p))
  {
    *p = '\0';
    --p;
  }
 
  return str;
}
 
//Remove the end space
char *ltrim(char *str)
{
  if (str == NULL || *str == '\0')
  {
    return str;
  }
 
  int len = 0;
  char *p = str;
  while (*p != '\0' && isspace(*p))
  {
    ++p;
    ++len;
  }
 
  memmove(str, p, strlen(str) - len + 1);
 
  return str;
}
 
//Remove the header and end space
char *trim(char *str)
{
  str = rtrim(str);
  str = ltrim(str);
  
  return str;
}
int Judge_version(){
    FILE *fstream=NULL;     
    char buff[1024];   
    memset(buff,0,sizeof(buff));   
    if(NULL==(fstream=popen("cat /proc/cpuinfo | grep Revision | awk '{print substr($NF,length($NF)-5,6)}'","r")))     
    {    
      //  log_error("execute command failed: %s",strerror(errno));     
        return -1;     
    }
    if (NULL != fgets(buff, sizeof(buff), fstream))
    {
        if(strlen(buff) <= 0)
            return -1;
        char *temp = trim(buff);  
        cout << temp<<endl;
        if (!strcmp(temp, "9000c1") ){
            cout << "The board is Pi zero w"<< endl;
        	boardVersion =PI_ZERO_W;
        }else if(!strcmp(temp, "a02082") || !strcmp(temp, "a22082") || !strcmp(temp, "a020d3")){
            cout << "The board is Pi 3B/B+"<< endl;
            boardVersion =PI_3B_3B_PLUS; 
        }
            else{
            cout << "The board is Pi zero"<< endl;
        	boardVersion = PI_ZERO;
        }
    }
    else
    {
        pclose(fstream);
        return -1;
    }
    pclose(fstream);   

    
    return -1;    
} 
void init(){
   Judge_version();
   system("sudo modprobe bcm2835_v4l2"); 
   
   }
int width = 320;
int height = 240;
void mergeImage(Mat &dst,Mat &src,int index){
    int offset = 10;
    int cols = width;
    int rows = height;
	
    if(dst.empty()){
        dst.create(rows * 1 + offset * 3,cols * 2 + offset * 3,CV_8UC3);
        dst.setTo(Scalar(0xE5,0xE5,0xE5));
    }
    // resize(src, src, Size(cols, rows));
#ifdef DEBUG_OUTPUT
    cout << "dst Width: " << dst.cols << endl;
    cout << "dst Height: " << dst.rows << endl;
    cout << "src Width: " << src.cols << endl;
    cout << "src Height: " << src.rows << endl;
    cout << "index: " << index << endl;
    printf("args: %d %d %d %d\n",(index % 2) * cols,(index / 2)*rows,width,height);
#endif
	src.copyTo(dst(Rect((index % 2) * cols + (index % 2 + 1) * offset, (index / 2) * rows + (index / 2 + 1) * offset, width, height)));
}

TIME currentTimeMillis(){
    struct timeval start;
    gettimeofday( &start, NULL );
    return start;
}
int brightness = 50;               // min=0   max=100  step=1
int contrast = 0;                  // min=-100  max=100  step=1
int saturation = 0;                // min=-100  max=100  step=1
int _rotate = 0;                    // min=0  max=360  step=90 
int auto_exposure = 0;             // min=0  max=3 
int exposure_time_absolute = 1000; // min = 1  max=10000  step=1

void sendCommand(){
    char command[100];
    sprintf(command,"v4l2-ctl -d 0 -c brightness=%d",brightness);
    system(command);
    sprintf(command,"v4l2-ctl -d 0 -c contrast=%d",contrast);
    system(command);
    sprintf(command,"v4l2-ctl -d 0 -c saturation=%d",saturation);
    system(command);
    sprintf(command,"v4l2-ctl -d 0 -c rotate=%d",_rotate);
    system(command);
    sprintf(command,"v4l2-ctl -d 0 -c auto_exposure=%d",auto_exposure);
    system(command);
    sprintf(command,"v4l2-ctl -d 0 -c exposure_time_absolute=%d",exposure_time_absolute);
    system(command);
}
int initCamera(VideoCapture &cap){

    sleep(1);
    if(access("/dev/video0",0)){
        printf("Please check your camera connection,then try again.\n");
        exit(0);
    }
    // open the default camera using default API
    cap.open(0);
    int deviceID = 0;        // 0 = open default camera
    int apiID = 0;

    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
   if(boardVersion == PI_ZERO_W)
    	system("gpio -g write 40 1");
   else if(boardVersion == PI_3B_3B_PLUS){
       // system("./rpi3-gpiovirtbuf s 134 1");
       write_virtual_gpio(134, 1);

   }else
    	system("gpio -g write 32 1");	
    sendCommand();
    cap.grab();
    
    if(boardVersion == PI_ZERO_W)
        system("gpio -g write 40 0");
    else if(boardVersion == PI_3B_3B_PLUS){
       //system("./rpi3-gpiovirtbuf s 134 0");
       write_virtual_gpio(134, 0);

   }
   else
        system("gpio -g write 32 0"); 
    sendCommand();
    cap.grab();
    sleep(1);
}

int main(int, char **)
{
    Mat surface;
    init();
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    initCamera(cap);

    cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    // cap.set(CV_CAP_PROP_FPS,30);
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;

    long begin_time = time(NULL);
    int totalFrame = 0;
    int flag = 2;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'

        // cap.grab();
        cap.grab();
        // cap.read(frame);
        cap.read(frame);
        TIME start,end;
        clock_t s1,e1;
        
        switch(flag){
        case 1:     //camera a
          if(boardVersion == PI_ZERO_W)
        	system("gpio -g write 40 0");
          else if(boardVersion == PI_3B_3B_PLUS)
            write_virtual_gpio(134, 0);
           // system("./rpi3-gpiovirtbuf s 134 0");
   	      else
        	system("gpio -g write 32 0"); 
           break;
        case 2:     //camera b
           if(boardVersion == PI_ZERO_W)
        	system("gpio -g write 40 1");
           else if(boardVersion == PI_3B_3B_PLUS)
            write_virtual_gpio(134, 1);
           // system("./rpi3-gpiovirtbuf s 134 1");
   	       else
        	system("gpio -g write 32 1"); 
           break;
        }
       
        ++totalFrame;
        if(time(NULL) - begin_time >= 1){
            printf("fps %d\n",totalFrame);
            totalFrame = 0;
            begin_time = time(NULL);
        }
        // check if we succeeded
        if (frame.empty())
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        char name[20];
        sprintf(name,"%c",flag + 64);
        putText(frame,name,Point(0,15),FONT_HERSHEY_SIMPLEX ,0.5,Scalar(0,0,0xFF),2);
        mergeImage(surface,frame,flag - 1);
        imshow("Arducam Multi-Camera DEMO",surface);
        if ((signed char)(waitKey(1)) >= 0)
            break;

        ++flag;
        if(flag == 3){
            flag = 1;
        }    

    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
