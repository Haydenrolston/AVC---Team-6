#include <stdio.h>
#include "E101.h"

int BWThreshold = 127;
int height=120;
double ErrScale = 0.5;
int prevError = 0;
double dErrScale = 0.2;
double drevErrScale = 1.0;
int numWhitePixels = 0;
int quadrant = 3;
int speed = 140;
int revSpeed = 64;
int* readings = new int[5];

int Q2prevErr = 0;
double Q2dErrScale = 0.3;
double Q2ErrScale = 0.5;
int dE = 0;

int* getCameraLine() //gets single line of image taken by cam
{
    int* line = new int[320];
    for(int i = 0; i<320; i++)
    {
        line[i] = get_pixel(height,i,3);
    }
    return line;
    

}
int* getWhites(int* line)//Converts line of pixels to one of 1=white and 0=black based off threshold
{
      int* boolLine = new int[320];
      for(int i = 0; i<320; i++)
      {
            if(line[i]>=BWThreshold){boolLine[i]=1;}else
                if(line[i]<BWThreshold){boolLine[i]=0;}
      }
    return boolLine;
}

int* getLoc(int* line)//Gets location of white line by defining where white line begins and ends
{
      int startPos;
      int endPos;
    bool inWhite = false;
    int* loc = new int[2];
	numWhitePixels=0;
    for(int i = 0; i<320; i++)
    {
           if(line[i]==1 && inWhite==false){inWhite=true; startPos=i;}
           if(line[i]==0 && inWhite==true){endPos=i; 
           if((endPos-startPos)>20){numWhitePixels = endPos-startPos;break;}
           else{inWhite=false;}}
        
    }
    loc[0]= startPos;
    loc[1] = endPos;
    return loc;
}

int getTurnDiff(int* loc)//Gets error signal based off how far white line is from centre of image
{
    int diff = 0;
    diff=(loc[0]+loc[1])/2;
    diff=diff-160;
    return diff;
}
int getError(int* line)//Testcode ignore
{
    int err = 0;
    for(int i = 0; i<320; i++)
    {
          err = err+(i-160)*ErrScale;
    }
    return err;
}
int convToMot(int spd)//Fixed our motor problem
{
    int result = 0;
    if(spd>=0){result=254-spd;}
    if(spd<0){result=(spd*-1)-254;}
    return result;
}

void drive(int diff, int timeS, int timeMS)//Drives
{
       set_motor(1,(convToMot(speed-diff)));
       set_motor(2, (convToMot(speed+diff)));
	printf("Motor1: %d\n", convToMot(diff));
	
       sleep1(timeS, timeMS);
}
void reverse(int spd, int timeS, int timeMS)
{
       set_motor(1,(convToMot(-revSpeed-(prevError*drevErrScale))));
       set_motor(2,convToMot(-revSpeed+(prevError*drevErrScale)));
       sleep1(timeS, timeMS);
}
void turnLeft(int spd, int timeS, int timeMS)
{
    set_motor(1, -(convToMot(-spd)));
    set_motor(2, convToMot(spd));
    sleep1(timeS, timeMS);
}
void driveStraight(int spd, int timeS, int timeMS)
{
	set_motor(1, convToMot(-spd));
	set_motor(1, convToMot(spd));
	sleep1(timeS, timeMS);
}






int main()
{
    init();
    //open_screen_stream();
    while(true){
	    if(quadrant!=3)
	    {
    take_picture();
    //update_screen ();
    int* lineRaw = new int[320];
    lineRaw = getCameraLine();
    int* lineW = new int[320];
    lineW = getWhites(lineRaw);
    int* loc = new int[2];
    loc = getLoc(lineW);
    int spdDiff = getTurnDiff(loc);
    
    
    prevError = spdDiff;
   // if(numWhitePixels<10 && quadrant == 1){
   //        reverse(-40,0,5000);
   // }else
     //   if(numWhitePixels<10 && quadrant == 2){
      //     reverse(-40,0,5000);
   // }
    //    else if(numWhitePixels>180)
    //{
     //      quadrant = 2;
      //     turnLeft(-254,0,50000);
    //}
    printf("Num white pixels:%d\n",numWhitePixels);
    printf("Error: %d\n",spdDiff);
	if(numWhitePixels==0){
	//Do something
		reverse(0,0,100);
	}else{

    drive(-((spdDiff*ErrScale)+(dE*dErrScale)),0,50);
		 dE = spdDiff-prevError;
	}
    }else if(quadrant == 3)///Q3 code begins here (a-mazing maze)
	    {
		    //Sensor1 FrontFacing handler
		    int sensorReading = 0;
		    int sensorReading2 = 0;
		    int sensorReading3 = 0;
		for(int i = 0; i<5; i++)
		{
			sensorReading = sensorReading+read_analog(0);
			sensorReading2 = sensorReading2+read_analog(1);
			sensorReading3 = sensorReading3+read_analog(2);
			sleep1(0,50);
		}
		sensorReading = sensorReading/5;
		sensorReading2 = sensorReading2/5;
		sensorReading3 = sensorReading3/5;
		printf("Front: %d\n",sensorReading);
		printf("Right: %d\n",sensorReading2);
		printf("Left: %d\n", sensorReading3);    
		//Checks both sides and sees which side is clear to turn into, if both are clear, turn left
		//if both are blocked about turn
		if(sensorReading>400){
			if(sensorReading2>400 && sensorReading3>400){
			turnLeft(254,0,500000);}
			else if(sensorReading2>400){turnLeft(254,0,500000);}
			else if(sensorReading3>400){turnLeft(-254,0,500000);}
			else{turnLeft(254,1,0);}
			
		}else{//Drive straight through corridor command
			int Q2Err = sensorReading2-sensorReading3;
			Q2Err = Q2Err/1024;
			printf("%d\n", Q2Err);
			int Q2dErr = Q2Err-Q2prevErr;
			drive(-((Q2Err*Q2ErrScale)+(Q2dErr*Q2dErrScale)),0,250);
			Q2prevErr = Q2Err;
			
			
		}
		
	    }
	    
    }
    return 0;
}
