 
#include <stdio.h>
#include "E101.h"
#include <math.h>

int BWThreshold = 80;
int height=80;int dh = 130;
double ErrScale = 0.8;
int prevError = 0;
double dErrScale = 0.8;
double drevErrScale = 2.0;
int numWhitePixels = 0;
int numActWPix = 0;
int quadrant = 0;
int speed = 130;
int revSpeed = 64;
int* readings = new int[5];
int counter1 = 0;
int Q2prevErr = 0;
double Q2dErrScale = 0.4;
double Q2ErrScale = 0.1;
int dE = 0;
int Q2SPEED = 100;
int Q2ErrThreshold = 20; //TOUCH THIS AND I WILL RIP YOUR FUCKING HEAD OFF AND DROP KICK IT INTO THE NEXT FUCKING GALAXY
int Integral = 0;
int dIScale = 0.1;
int prevS2 = 0;
int prevS3 = 0;
int Q2Err = 0;
bool check = false;
bool checkRight = false;
bool checkLeft=false;

int* getCameraLine(int h) //gets single line of image taken by cam
{
    int* line = new int[320];
    for(int i = 0; i<320; i++)
    {
        line[i] = get_pixel(h,i,3);
	
    }
    return line;
    

}
int getCVertical(int pos){
  
    int line = 0;
    for(int i = 230; i<240; i++)
    {
	line =line+ get_pixel(i,pos,3);
    }
    return line/10;
  
}
int getAvgWColor(int* line)
{
    int result = 0;
    
    for(int i = 0; i<320; i++)
    {
	result= result+ line[i];
    }
    result=result/320;
    return result;
}
int getAvgRColor(int* line, int h){
    int result = 0;
    for(int i = 0; i<320; i++)
    {
	result = result+get_pixel(h,i,0);
    }
    result=result/320;
    return result;
}
int* getWhites(int* line)//Converts line of pixels to one of 1=white and 0=black based off threshold
{
      int* boolLine = new int[320];
      int numPix = 0;
      int count = 0;
      for(int i = 0; i<320; i++)
      {
            if(line[i]>=BWThreshold){boolLine[i]=1; numPix++;if(count==2){printf("▓");count=0;}else{count++;}}else
                if(line[i]<BWThreshold){boolLine[i]=0; if(count==2){printf("░");count=0;}else{count++;}}
      }
      printf("\n");
      numActWPix=numPix;
    return boolLine;
}

int* getLoc(int* line)//Gets location of white line by defining where white line begins and ends
{
      int startPos = 0;
      int endPos = 320;
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
	//printf("Motor1: %d\n", convToMot(diff));
	
       sleep1(timeS, timeMS);
}

void Q2drive(int diff, int timeS, int timeMS)//Drives
{
       set_motor(1,(convToMot(Q2SPEED-diff)));
       set_motor(2, (convToMot(Q2SPEED+diff)));
	//printf("Motor1: %d\n", convToMot(diff));
	
       sleep1(timeS, timeMS);
}bool check2 = false;
void reverse( int timeS, int timeMS)
{
       set_motor(1,(convToMot(-revSpeed-(prevError*drevErrScale))));
       set_motor(2,convToMot(-revSpeed+(prevError*drevErrScale)));
       sleep1(timeS, timeMS);
}
void Q2reverse(int timeS, int timeMS){
    
       set_motor(1,(convToMot(-revSpeed+(Q2Err*ErrScale*0.5))));
       set_motor(2,convToMot(-revSpeed-(Q2Err*ErrScale*0.5)));
       sleep1(timeS, timeMS);
  
}
void revLeft(int spd, int timeS, int timeMS)
{
    set_motor(1, (convToMot(-spd)));
    set_motor(2, convToMot((-0)));
    sleep1(timeS, timeMS);
}
void revRight(int spd, int timeS, int timeMS)
{
    set_motor(1, (convToMot(-20)));
    set_motor(2, convToMot((-spd)));
    sleep1(timeS, timeMS);
}

void turnRight(int spd, int timeS, int timeMS)
{
    set_motor(1, (convToMot(spd)));
    set_motor(2, convToMot((-spd)));
    sleep1(timeS, timeMS);
}
void turnLeft(int spd, int timeS, int timeMS)
{
    set_motor(1, (convToMot(-spd)));
    set_motor(2, convToMot((spd)));
    sleep1(timeS, timeMS);
}
void driveStraight(int spd, int timeS, int timeMS)
{
	set_motor(1, convToMot(spd));
	set_motor(2, convToMot(spd));
	sleep1(timeS, timeMS);
}
void stop(int timeS, int timeMS){
    set_motor(1, convToMot(0));
	set_motor(2, convToMot(0));
	sleep1(timeS, timeMS);
}
int frontalSensorReading()
{
    int result = 0;
    for(int i = 0; i<5; i++)
    {
	result = result+read_analog(0);
	sleep1(0,10);
      
    }
    return result/5;
  
}
void turnAround(int spd, int timeS, int timeMS){
  
  
  set_motor(1, (convToMot(spd)));
    set_motor(2, convToMot((-spd)));
    sleep1(timeS, timeMS);
  
  
  
  
}
double abs(double input)
{
    if(input>=0){
     return input; 
      
    }else if(input<0){
     return input*-1; 
    }
  
  
}
void openGate()
{
	while(true){
	connect_to_server("130.195.006.196",1024);
	char* gitgud1 = new char[24];
	send_to_server("Please");
	receive_from_server(gitgud1);
	if(gitgud1!=NULL){printf(gitgud1);
	send_to_server(gitgud1); break;}
	
	
  }
	
  
}

int getSpdDiff(){
  
  take_picture();
  int* lineRaw = new int[320];
  lineRaw = getCameraLine(height);
   int* lineW = new int[320];
   lineW = getWhites(lineRaw);
  int* loc = new int[2];
  loc = getLoc(lineW);
  int spdDiff = getTurnDiff(loc);
  return spdDiff;
}

int checkLine(int h, int* line){
  
    int* line1 = new int[320];
    line1=line;
    int result = 0;
    for(int i = h;i<h+10;i++){
      result = line1[i]+result;
      
    }
    return result/10;
  
}







int main()
{
    init();
    //open_screen_stream();
    openGate();
    
    while(true){
      //if(check==false){continue;}else{check=true;}
      take_picture();
      numActWPix=0;
    //update_screen ();
    int* lineRaw = new int[320];
    int* linedRaw = new int[320];
    linedRaw = getCameraLine(dh);
    lineRaw = getCameraLine(height);
    int* lineW = new int[320];
    int* linedW = new int[320];
    linedW = getWhites(linedRaw);
    lineW = getWhites(lineRaw);
    int* loc = new int[2];
    int* locd = new int[2];
    locd = getLoc(linedW);
    loc = getLoc(lineW);
    //printf("ACTUAL PIXELS: %d\n",numActWPix);
    if(quadrant==0){
	
	int IRSensorReading = frontalSensorReading();
	if(IRSensorReading<150){quadrant=1;}else{sleep1(1,50000); }
	
	//printf("Test: %d\n",0);
	
    }
    if(numActWPix>300){
      if(counter1 >5 && quadrant!=2){
	
	quadrant=2;
	speed=50;
	stop(2,0);
	printf("Transitioning: Q2-Q3");
	sleep1(2,0);
	turnRight(80,0,50000);
	driveStraight(80,1,0);
	speed=50; height=120; dh=140;
      }else{counter1++;}
    }
    
      
	    if(quadrant==2||quadrant==1)
	    {
    
    int spdDiff = getTurnDiff(loc);
    int postErrDiff = getTurnDiff(locd);
    
    if(quadrant==1){
    prevError = postErrDiff;
    
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
    //printf("Num white pixels:%d\n",numWhitePixels);
    //printf("Error: %d\n",spdDiff);
	if(numActWPix==0){
	//Do something
		reverse(0,100);
	}else{
    
    drive(-((spdDiff*ErrScale)-(dE*dErrScale)+(Integral*dIScale)),0,50);
		Integral=Integral+spdDiff;
		 dE = postErrDiff-spdDiff;
	}
    }else if (quadrant==2){
         int spdDiff = getTurnDiff(loc);
	 int postErrDiff = getTurnDiff(locd);
	 //speed = (int)(80*abs((1/spdDiff)/200));
	 
	 int leftv = getCVertical(0);
	 int rightv = getCVertical(240);
	 //printf("LEFT: %d\n",leftv);
	 //printf("RIGHT: %d\n",rightv);
	 prevError = postErrDiff;
	 if(numWhitePixels>200){speed=30;}else{speed=50;}
	   
	 if(numActWPix>250)
	 {
	   printf("Junction found: TUrning left");
	    driveStraight(80,0,500000);
	    turnLeft(180,0,275000);
	    driveStraight(-80,0,250000);
	 }else if(numWhitePixels>160 && spdDiff<-20){
	    driveStraight(80,0,500000);
	    turnLeft(180,0,275000);
	    driveStraight(-80,0,250000);
	 }else if(numWhitePixels>160 && spdDiff<20){
	    driveStraight(80,0,500000);
	    turnRight(180,0,275000);
	    driveStraight(-80,0,250000);
	 }
	 
	 
	 
	 else if(numActWPix<10){
	   printf("End point found, about turning");
	   driveStraight(80,0,500000);
	    turnAround(180,0,550000);
	    driveStraight(-80,0,250000);
	 }
	 
	   
	 
	 
	 
	 else{
	    //printf("ERRVALUE: %d\n",spdDiff);
	    
	    drive(-((spdDiff*ErrScale)-(dE*dErrScale)+(Integral*dIScale)),0,50);
		 dE = postErrDiff-spdDiff;
		 Integral=Integral+spdDiff;
	  
	 }
    }
    }else if(quadrant == 3)///Q3 code begins here (a-mazing maze)
	    {
		    //Sensor1 FrontFacing handler
		    int frontalSensor = 0;
		    int rightSensor = 0;
		    int leftSensor = 0;
		for(int i = 0; i<5; i++)
		{
			frontalSensor = frontalSensor+read_analog(0);
			rightSensor = rightSensor+read_analog(1);
			leftSensor = leftSensor+read_analog(2);
			sleep1(0,40);
		}
		frontalSensor = frontalSensor/5;
		rightSensor = rightSensor/5;
		leftSensor = leftSensor/5;
		printf("Front: %d\n",frontalSensor);
		printf("Right: %d\n",rightSensor);
		printf("Left: %d\n", leftSensor);    
		//Checks both sides and sees which side is clear to turn into, if both are clear, turn left
		//if both are blocked about turn
		if(frontalSensor>250){
			if(rightSensor<150){
			 turnLeft(50,0,1000); 
			}else if(leftSensor<150){
			 turnRight(50,0,1000); 
			}
			
			
			
		}else{//Drive straight through corridor command
			
			//if(rightSensor>400){
			  //revLeft(40,0,1000);
			  //turnRight(100,0,2000);
			//}
			//if(leftSensor>400){
			  //revRight(40,0,1000);
			  //turnLeft(80,0,2000);
			//}
			
			if(rightSensor<250||leftSensor<250){
			 if(rightSensor<250){rightSensor=prevS2; turnLeft(80,0,100);}
			 if(leftSensor<250){leftSensor=prevS3;}
			 if(rightSensor<250&&leftSensor<250){rightSensor=prevS2;leftSensor=prevS3;}
			  
			}
			 Q2Err = rightSensor-leftSensor;
			int Q2dErr = (Q2Err-Q2prevErr);
			if(Q2dErr>Q2ErrThreshold){driveStraight(20,0,10);}else{
			Q2drive(((Q2Err*Q2ErrScale)+(Q2dErr*Q2dErrScale)),0,50);}
			
			
			
			if(!rightSensor<250){prevS2=rightSensor+20;}
			if(!leftSensor<250){prevS3=leftSensor+20;}
			Q2prevErr = Q2Err;
			
			
			
		}
		
	    }else if(quadrant==-1){
	     if(numActWPix!=0){quadrant=0;}
	     
	     
	      
	    }
	    
	check=false;    
    }
    return 0;
}
