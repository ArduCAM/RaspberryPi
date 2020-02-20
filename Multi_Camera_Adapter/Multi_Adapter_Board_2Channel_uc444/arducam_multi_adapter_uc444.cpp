// #include <opencv2/core.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
//#include <sys/time.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
using namespace cv;
using namespace std;

typedef struct timeval TIME;
#define GPIO_NUM 2
int digitalNum[GPIO_NUM];
int status[GPIO_NUM];
int lastStatus[GPIO_NUM];
void init(){
	digitalNum[0] = 7;
	digitalNum[1] = 11;
    wiringPiSetup() ;
	for(int i = 0 ; i < GPIO_NUM ; ++i){
		pinMode (digitalNum[i], OUTPUT) ;
	}
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
    // src.copyTo(dst(Rect((index % 2) * cols + (index % 2) * offset, (index / 2) * rows + (index / 2) * offset, width, height)));
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
    if(access("/dev/video0",0)){
        printf("Please check your camera connection,then try again.\n");
        exit(0);
    }
    char *i2c = "i2cset -y 1 0x70 0x00 0x01";
    system(i2c);
    digitalWrite(7,0);
    digitalWrite(11,0);

    // open the default camera using default API
    cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = 0;
    // int apiID = cv::CAP_ANY; // 0 = autodetect default API
                             // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    sendCommand();
    cap.grab();
    i2c = "i2cset -y 1 0x70 0x00 0x02";
    system(i2c);
    digitalWrite(7,1);
    digitalWrite(11,0);
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
        cap.grab();
        cap.read(frame);
        TIME start,end;
        clock_t s1,e1;
        
        switch(flag){
        case 1:     //camera b
            digitalWrite(7,1) ;
            break;
        case 2:     //camera a
            digitalWrite(7,0) ;
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
