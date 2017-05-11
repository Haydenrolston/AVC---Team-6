#include <stdio.h>
#include "E101.h"

int BWThreshold = 80;

int* getCameraLine()
{
    int* line = new int[320];
    for(int i = 0; i<320; i++)
    {
        line[i] = get_pixel(i,j,3);
    }
    return line;
    

}
int* getWhites(int* line)
{
      int* boolLine = new int[320];
      for(int i = 0; i<320; i++)
      {
            if(line[i]=>BWThreshold){boolLine[i]=1;}else
                if(line[i]<BWThreshold){boolLine[i]=0;}
      }
    return boolLine;
}

int* getLoc(int* line)
{
      int startPos;
      int endPos;
    bool inWhite = false;
    int* loc = new int[2];
    for(int i = 0; i<320; i++)
    {
           if(line[i]==1 && inWhite==false){inWhite=true; startPos=i;}
           if(line[i]==0 && inWhite=true){endPos=i; break;}
        
    }
    loc[0]=startPos;
    loc[1] = endPos;
    return loc;
}

int getTurnDiff(int* loc)
{
    int diff = 0;
    diff=(loc[0]+loc[1])/2;
    diff=diff-160;
    return diff;
}

void drive(int diff, int timeS, int timeMS)
{
       set_motor(1,127-convToMot(diff)*ErrScale);
       set_motor(2, 127+convToMot(diff)*ErrScale);
       sleep1(timeS, timeMS);
}
int convToMot(int spd)
{
    int result = 0;
    if(spd>=0){result=255-spd;}
    if(spd<0){result=(spd*-1)-255;}
    return result;
}




int main()
{
    init();
    while(true){
    take_picture();
    lineRaw = new int[320];
    lineRaw = getCameraLine();
    lineW = new int[320];
    lineW = getWhites(lineRaw);
    int* loc = new int[2];
    loc = getLoc(lineW);
    int spdDiff = getTurnDiff(loc);
    drive(spdDiff,0,5000);
    }
    return 0;
}
