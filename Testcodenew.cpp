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
    
    

}
int* getWhites(int** line)
{

}







int main()
{
    init();
    take_picture();
    lineRaw = new int[320];
    lineRaw = getCameraLine();
    return 0;
}
