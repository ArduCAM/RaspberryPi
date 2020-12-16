#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
int arducam_get_key()
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
int processKeyEvent(void){
	int keyVal = 0;
		keyVal = arducam_get_key();
		if(keyVal == 27){
			keyVal = arducam_get_key();
			if(keyVal ==  91){
				keyVal = arducam_get_key();
				if(keyVal == 65){
					return 1;
				}
				if(keyVal == 66){
					return 2;
				}
			}
		}
		else{
			return keyVal;
		}
	
}
//test
int main(int argc, char** argv){
	int val = processKeyEvent();
	printf("val = %d\r\n",val);
}
