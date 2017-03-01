#include <ctime>
#include <fstream>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <iostream>
#include <string>
#include <raspicam/raspicam_cv.h>
#include <raspicam/raspicam.h>
// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define	LEFT	13
#define	RIGHT	19

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

int input;

int main (void)
{



int counter=0;
raspicam::RaspiCam_Cv Camera; //Cmaera object
cv::Mat image;
Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
Camera.open();

initscr();
    initscr();

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    scrollok(stdscr, TRUE);
  printf ("Raspberry Pi blink\n") ;

  wiringPiSetupGpio () ;
  pinMode (LEFT, OUTPUT) ;
 pinMode (RIGHT, OUTPUT) ;
digitalWrite(LEFT, HIGH);//backwards??? if L&R=HI, car goes straight
digitalWrite(RIGHT, HIGH);//low 4 right, hi 4 left
  int x;
  while (input!=32)
  {
	input=-1;
	input=getch();
if(input==97)//left
{
 Camera.grab();
Camera.retrieve(image);
std::stringstream ss;
ss<<counter;
std::string temp=ss.str();
std::string lpath="/home/pi/Downloads/raspicam-0.1.4/build/utils/trainingData/L/img"+temp+".png";
cv::imwrite(lpath,image);
++counter;

digitalWrite(RIGHT, LOW);
digitalWrite (LEFT, HIGH) ;	 
}
else if(input==115)//fwd
{
 Camera.grab();
Camera.retrieve(image);
std::stringstream ss;
ss<<counter;
std::string temp=ss.str();
std::string fpath="/home/pi/Downloads/raspicam-0.1.4/build/utils/trainingData/FWD/img"+temp+".png";
cv::imwrite(fpath,image);
++counter;

digitalWrite(RIGHT, LOW);
digitalWrite (LEFT, LOW) ;	 
}
else if(input==100)//right
{
 Camera.grab();
Camera.retrieve(image);
std::stringstream ss;
ss<<counter;
std::string temp=ss.str();
std::string rpath="/home/pi/Downloads/raspicam-0.1.4/build/utils/trainingData/R/img"+temp+".png";
cv::imwrite(rpath,image);
++counter;
digitalWrite(RIGHT, HIGH);
digitalWrite (LEFT, LOW) ;	 
}
else if(input==98)
{
digitalWrite(RIGHT, HIGH);
digitalWrite (LEFT, HIGH) ;
}
    
}
  Camera.release();

  return 0 ;
}
