#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
pthread_t manualFocusPthreadID;
unsigned char flag = 1;
int get_key_board_from_termios()
{
    int key_value;
    struct termios new_config;
    struct termios old_config;

    tcgetattr(0, &old_config);
    new_config = old_config;
    new_config.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_config);
    key_value = getchar();
    tcsetattr(0, TCSANOW, &old_config);
    return key_value;
}
void setFocus(int val){
char buffer[1024] = { 0 };
    unsigned char valMSB, valLSB;
    val = (val<<4) & 0x3ff0;
    valMSB = (val>>8)&0x3f;
    valLSB = val & 0xf0;
    sprintf(buffer,"i2cset -y 0 0x0c %d %d",valMSB,valLSB);
    system(buffer);
}
void manualFocusThread(){
   int focusVal  = 0;
   printf("Please press up and down key\r\n");
    while(flag){
    
        usleep(100);
        int KeyVal = 0;
        KeyVal = get_key_board_from_termios();
        if(KeyVal == 27){
            KeyVal = get_key_board_from_termios();
            if(KeyVal == 91){
                KeyVal = get_key_board_from_termios();      
                switch (KeyVal)
                {
                    case 65/* up */:
                    focusVal += 10;
                    if(focusVal >= 1024){
                        focusVal = 1024;
                    }
                    setFocus(focusVal);
                        printf("UP\r\n");
                    break;
                    case 66/* down */:
                    focusVal -= 10;
                    if(focusVal <= 0){
                        focusVal = 0;
                    }
                    setFocus(focusVal);
                    printf("DOWN\r\n");
                    break;
                    default:
                    break;
                }
            }
        }
    }
}

int main( void ){
    int ret = 0;
   ret = pthread_create(&manualFocusPthreadID, NULL, (void *)manualFocusThread,NULL);

   if(ret){
       printf("pthread create failed");
       return 1;
   }
    char* runCameraCmd = "raspistill -t 0";
    system(runCameraCmd);
    flag = 0;
    pthread_join(manualFocusPthreadID,NULL);
}


 
