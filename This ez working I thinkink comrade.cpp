 
#include <stdio.h>
#include "E101.h"
#include <math.h>

int BWThreshold = 50;//USING DO CARE
int height=80;int dh = 130;//USING DO CARE
double ErrScale = 0.3;//USING DO CARE
int prevError = 0;//USING DO CARE
double dErrScale = 0.9;//USING DO CARE
double drevErrScale = 2.0;//USING DO CARE
int numWhitePixels = 0;//USING DO CARE
int numActWPix = 0;//USING DO CARE
int quadrant = 0;//USING DO CARE THIS ONE ESPECIALLY DO NOT TOUCH COMRADE?
int speed = 100;//USING DO CARE
int revSpeed = 64;//USING DO CARE
int* readings = new int[5];//USING DO CARE
int counter1 = 0;//USING DO CARE
int Q2prevErr = 0;//USING DO CARE
double Q2dErrScale = 0.4;//USING DO CARE
double Q2ErrScale = 0.1;//USING DO CARE
int dE = 0;//USING DO CARE
int Q2SPEED = 100; //Not using dont care
int Q2ErrThreshold = 20; //Not using dont care
int Integral = 0; //USING DO CARE
int dIScale = 0.6;//USING DO CARE
int prevS2 = 0; //USING DO CARE
int prevS3 = 0;//USING DO CARE
int Q2Err = 0;//USING DO CARE
bool check = false;//USING DO CARE
bool check1 = false;//USING DO CARE
bool rightleft = false; //Not using dont give a shite
bool checkRight = false; //NOt using dont give a f*ck
bool checkLeft=false; //Not using dont give a shizzle mah nizzle for rizzle yo
bool justTurned = false;//USING DO CARE
int sideCheckThreshold = 90;//USING DO CARE
int numSidePix = 0;//USING DO CARE
int count3 = 0;//USING DO CARE

int* getCameraLine(int h) //gets single line of image taken by cam
{
    int* line = new int[320];
    for(int i = 0; i<320; i++)
    {
        line[i] = get_pixel(h,i,3);
	
    }
    return line;
    

}
int* getColumnPixel(int column){// get Column white pixels 
	int* VLine = new int[240];
	for(int i = 0; i < 240; i++){
		VLine[i] = get_pixel(i, column, 3);
	}
	return VLine;
}

bool ScanVLine(int* column){
	int startPos = 0;
	int endPos = 0;
	bool inWhite = false;
	int sidePix=0;
	for(int i = 0; i < 240; i++){
		//if(inWhite == false && column[i] > BWThreshold){
			//startPos = i;
			//inWhite = true;
		//}
		//else if(inWhite == true && column[i] < BWThreshold){
			//endPos = i;
			//if(endPos - startPos > sideCheckThreshold){// if there has a white line that includes over 20 white pixels, then return true
				
			  
			//  return true;
			//}
			//else{inWhite = false;}
	  if(column[i]>BWThreshold){sidePix++;}
	}
	numSidePix=sidePix;
	if(sidePix>sideCheckThreshold){return true;}else{return false;}
	
	
}
int getCVertical(int pos){
  
    int line = 0;
    for(int i = 230; i<240; i++)
    {
	line =line+ get_pixel(i,pos,3);
    }
    return line/10;
  
}
int getCalibratedBWThresh(){
    int max = 0;
    int min = 255;
    int prevVal = 0;
    for(int i = 0; i<220; i++){
	for(int j = 0; j<320; j++){
	    if(get_pixel(i,j,3)>max){
		max=get_pixel(i,j,3);
	    }
	    if(get_pixel(i,j,3)<min){
		min=get_pixel(i,j,3);
	    }
	}
      
      
    }
    if((max-min)<40){
     max=230; min=20;
    }
    return (max+min)/2;
}


bool getReds(){
	int* lineR = new int[320];
	int* lineG = new int[320];
	int* lineB = new int[320];
	int redAvg = 0;
	int greenAvg = 0;
	int blueAvg = 0;
	take_picture();
	for(int i = 0; i < 320; i++){
		lineR[i] = get_pixel(height, i, 0);
		lineG[i] = get_pixel(height, i, 1);
		lineB[i] = get_pixel(height, i, 2);
	}
	for(int j = 0; j < 320; j++){
		redAvg = redAvg + lineR[j];
		greenAvg = greenAvg + lineG[j];
		blueAvg = blueAvg + lineB[j];
	}
	redAvg = redAvg/320;
	greenAvg = greenAvg/320;
	blueAvg = blueAvg/320;
	printf("RED:%d\n", redAvg);
	printf("BLUE:%d\n", blueAvg);
	printf("GREEN:%d\n", greenAvg);
	if(redAvg >= 150 && greenAvg <= 80 && blueAvg <= 80){
		  printf("ITS RED CHANGE TO Q4");
	  return true;
	}
	return false;
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
            if(line[i]>=BWThreshold){boolLine[i]=1; numPix++;if(count==2){count=0;}else{count++;}}else
                if(line[i]<BWThreshold){boolLine[i]=0; if(count==2){count=0;}else{count++;}}
      }
      
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
       set_motor(1,(convToMot(-revSpeed+(prevError*drevErrScale))));
       set_motor(2,convToMot(-revSpeed-(prevError*drevErrScale)));
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
  printf("TURNING RIGHT");
    set_motor(1, (convToMot(spd)));
    set_motor(2, convToMot((-spd)));
    sleep1(timeS, timeMS);
}
void turnLeft(int spd, int timeS, int timeMS)
{
    printf("TURNING LEFT");
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
	char* messageFromServer = new char[24];
	send_to_server("Please");
	receive_from_server(messageFromServer);
	if(messageFromServer!=NULL){printf(messageFromServer);
	send_to_server(messageFromServer); break;}
	
	
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
    BWThreshold=getCalibratedBWThresh();
    if(numActWPix<100&&check1==true && quadrant==1){quadrant=2;}
     if(getReds()==true){
       //if(quadrant==2){
      quadrant=3;
      printf("it is Q4.\n");
    }
    if(quadrant==0){
	
	int IRSensorReading = frontalSensorReading();
	if(IRSensorReading<150){quadrant=1;}else{sleep1(1,50000); }
	
	//printf("Test: %d\n",0);
	
    }
    if(numActWPix>300){// reach to the crossRoads 
      if(quadrant==1){
	
	check1=true;
	speed=50;
	driveStraight(50,0,500000);
	printf("Transitioning: Q2-Q3");
	//sleep1(2,0);
	//turnRight(80,0,50000);
	
	
	dE=0; Integral=0;
	speed=50; height=80; dh=100;
	ErrScale=0.6; dErrScale=0.4; dIScale=0.4;
	
      }
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
	   int* leftColumn = new int[240];
		leftColumn= getColumnPixel(0);// left most column
		
		int* rightColumn = new int[240];
		rightColumn= getColumnPixel(319);// right most column
		rightleft=false;
		bool leftBranch = ScanVLine(leftColumn);
		printf("LEFT: %d\n",numSidePix); numSidePix=0;
		rightleft=true;
	    bool rightBranch = ScanVLine(rightColumn);
	    printf("RIGHT: %d\n",numSidePix); numSidePix=0;
	    printf("NUMBER OF WHITE PIXELS ####: %d\n",numActWPix);
	 if(numActWPix>319)// have two junction at two sides
	 {
	   printf("Junction found: TUrning left");
	    driveStraight(80,0,500000);
	    turnLeft(150,0,225000);
	    driveStraight(-80,0,200000);
	    justTurned=true;
	    
	 }else if((numActWPix>100 && numActWPix<160) && rightBranch && counter1>20){
	   counter1=0;
	   printf("RIGHT TURN\n");
	    driveStraight(80,0,500000);
	    turnRight(150,0,225000);
	    driveStraight(-80,0,200000);
	 }
	 
	 
	 
	 
	 else if(numActWPix<10){
	   printf("End point found, about turning");
	   driveStraight(80,0,300000);
	    turnAround(180,0,500000);
	    driveStraight(-80,0,200000);
	 }
	 
	   
	 
	 
	 
	 else{
	    //printf("ERRVALUE: %d\n",spdDiff);
	    if(abs(-((spdDiff*ErrScale)-(dE*dErrScale)+(Integral*dIScale)))<30){
	    drive(-((spdDiff*ErrScale)-(dE*dErrScale)+(Integral*dIScale)),0,50);
		 dE = postErrDiff-spdDiff;
		 Integral=Integral+spdDiff;
	      
	    }else{dE=0;Integral=0;drive(-((spdDiff*ErrScale)-(dE*dErrScale)+(Integral*dIScale)),0,50);}
	    }
	    if(justTurned==true){counter1++;}
	    if(counter1>40){justTurned=false;counter1=0;}
	  
	 //}
    }
    }
    if(quadrant == 3)///Q3 code begins here (a-mazing maze)
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
			 turnRight(50,0,1000); 
			}else if(leftSensor<150){
			 turnLeft(50,0,1000); 
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
			 if(rightSensor<250){rightSensor=prevS2; turnRight(80,0,100);}
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
