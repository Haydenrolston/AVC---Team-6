#include <stdio.h>
#include "E101.h"

int BWThreshold = 80;
int height=120;
int ErrScale = 0.5;
int prevError = 0;
int dErrScale = 0.2;
int numWhitePixels = 0;

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
            if(line[i]=>BWThreshold){boolLine[i]=1;}else
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
    for(int i = 0; i<320; i++)
    {
           if(line[i]==1 && inWhite==false){inWhite=true; startPos=i;}
           if(line[i]==0 && inWhite=true){endPos=i; if((endPos-startPos)>20){numWhitePixels = endPos-startPos;break;}else{inWhite=false;}}
        
    }
    loc[0]=startPos;
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
}

void drive(int diff, int timeS, int timeMS)//Drives
{
       set_motor(1,127-convToMot(diff));
       set_motor(2, 127+convToMot(diff));
       sleep1(timeS, timeMS);
}
int convToMot(int spd)//Fixed our motor problem
{
    int result = 0;
    if(spd>=0){result=254-spd;}
    if(spd<0){result=(spd*-1)-254;}
    return result;
}




int main()
{
    init();
    open_screen_stream ():
    while(true){
    take_picture();
    update_screen ();
    lineRaw = new int[320];
    lineRaw = getCameraLine();
    lineW = new int[320];
    lineW = getWhites(lineRaw);
    int* loc = new int[2];
    loc = getLoc(lineW);
    int spdDiff = getTurnDiff(loc);
    prevError = spdDiff;
    drive((spdDiff*ErrScale),0,5000);
    }
    return 0;
}
