#include <stdio.h>
#include "E101.h"

int BWThreshold = 127;//set this as the standard. The pixel is white when its value is bigger than 127, otherwise, it's black
int height=120;// the middle row of the picture
double ErrScale = 0.5;
int prevError = 0;
double dErrScale = 0.2;
int numWhitePixels = 0;
int quadrant = 1;
int speed = 100;
int* readings = new int[5];

int main()
{
    init();
    open_screen_stream();
    while(true){
	    if(quandrant!=3)
	    {
    take_picture();
    update_screen ();
    int* lineRaw = new int[320];
    lineRaw = getCameraLine();//get the middle row pixel(totally have 320 pixel)
    int* lineW = new int[320];
    lineW = getWhites(lineRaw);// convert the values got before into 1 or 0 (white or black)
    int* loc = new int[2];
    loc = getLoc(lineW);// get the position of the white line and pass it to get error method
    int spdDiff = getTurnDiff(loc);
    
    int dE = spdDiff-prevError;
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
	//Do something, turn back to find the white line but also need to change a few angle
	}else{

    drive(-(spdDiff*ErrScale),0,500);
	}
    }else if(quadrant == 3)
	    {
		    int sensorReading = 0;
		for(int i = 0; i<5; i++)
		{
			sensorReading = sensorReading+read_analog(0);	
			sleep1(0,100);
		}
		sensorReading = sensorReading/5;
		sensorReading = 1-(sensorReading/1024);
		driveStraight(sensorReading*255,0,10);
	    }
	    
    }
    return 0;
}






int* getCameraLine() //gets single line of image taken by cam
{
    int* line = new int[320];
    for(int i = 0; i<320; i++)
    {
        line[i] = get_pixel(height,i,3);// using number 3 is in order to return the "whiteness" of the pixel.
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
		if(line[i]==1 && inWhite==false)
		{
			inWhite=true; startPos=i;
		}
        if(line[i]==0 && inWhite==true)
		{
			endPos=i; 
            if((endPos-startPos)>20)
			{
		        numWhitePixels = endPos-startPos;
				break;
			}
            else
			{
				inWhite=false;
			}
		}
        
    }
    loc[0]= startPos;
    loc[1] = endPos;
    return loc;
}

int getTurnDiff(int* loc)//Gets error signal based off how far white line is from centre of image
{
    int diff = 0;
    diff=(loc[0]+loc[1])/2;
    diff=diff-160;// compare the distance between the middle of the white line and the middle of the picture
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
int convToMot(int spd)//Fixed our motor problem (0 is the max speed, 254 is the min speed)
{
    int result = 0;
    if(spd>=0){result=254-spd;}
    if(spd<0){result=(spd*-1)-254;}
    return result;
}

void drive(int diff, int timeS, int timeMS)//Drives
{
       set_motor(1,(convToMot(speed-diff)));
       set_motor(2, (convToMot(speed+diff)));
	printf("Motor1: %d\n", convToMot(diff));
	
       sleep1(timeS, timeMS);
	   if(numWhitePixels == 0){ // the blind alley situation, set the motor stop
	      set_motor(1, 254);
	      set_motor(2,254);
	      sleep1(0,500);
	   }
}
void reverse(int spd, int timeS, int timeMS)// what is the role of this part? and why have to set the speed in negative each time?
{
       set_motor(1,-(convToMot(-spd)));
       set_motor(2,convToMot(-spd));
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
	set_motor(1, convToMot(-spd));
	sleep1(timeS, timeMS);
}


