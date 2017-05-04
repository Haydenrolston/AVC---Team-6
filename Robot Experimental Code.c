#include <stdio.h>
#include "E101.h"
//APOLOGIEs FOR MY SHODDY CODE IN ADVANCE LOL
int height = 120;
int main()
{
	init();
	int[][] cameraLine = getCameraLine(height, readCamera());
	
	
	return null;
}

int[][] getCameraLine(int height, int[][][] inpImage)
{
	
	int[][][] image = new int[240][420][3];
	image = inpImage;
	int linePixels = new int[420][3];
	for(int i = 0; i<420; i++)
	{
		linePixels[i][0] = image[height][i][0];
		linePixels[i][1] = image[height][i][1];
		linePixels[i][2] = image[height][i][2];
	}
	return linePixels;
}
int[][][] readCamera()
{
	//Cant remember E101.h api for take picture
	//Assuming I can put the colour values into a 3d array 420/240 res containing 3 values for colour
	int[][][] image = new int[240][420][3];
}
int getTurnScalar(int[] loc, int scale)
{
	int avgLoc = (int)((loc[0]+loc[1])/2);	
	int turnDiff = avgLoc-209;
	return turnDiff*scale;
}
int[] getLinePosition(int colorTolerance, int[] calibratedColorBW, int[][] line)
{
	//Not sure if this way is gonna work but basically:
	//it takes the average of 5 pixel values reading from the left to the right of the image and sees whether
	//the average is within the tolerance of the calibrated white value, if it is then it will record that 
	//column in the line as the start position and wont record more start  until it encounters an average value near the calibrated
	//black value where it will record the white end point, will write a seperate method to generate commands for robot motion based
	//on the position of the white section in the image. I haven't tested this to work yet as I wrote this up on my laptop directly on github
	//if you see errors, please please please correct them, corrections are most welcome and bring us closer to our goal of getting
	//our roguebot to the end of the first quadrant
	int[] whiteEndPts = new int[2];
	bool inWhite = false;
	for(int i = 0; i<line.length;i++)
	{
		
		int avgColor = 0;
		for(int j = 0; j<5;j++)
		{
			avgColor = avgColor+getAvgColor(line[i]);
			i++;
		}
		avgColor= avgColor/5;
		if(avgColor < (calibratedColorBW[1]+colorTolerance) && avgColor > (calibratedColorBW[1]-colorTolerance))
		{
			// do you need to include "if(inWhite==false)"? it is already defined as false and won't change. 
			// correct me if im wrong but couldn't we just put " inWhite=true;
			//						     whiteEndPts[0] = i;"
			// and the same with down below
			//this will ensure that the code below will only run once, when it runs for the first time it will set inWhite to be true
			//and it won't run it again if it detects white pixels. It should only record the beginning point of the white pixels
			//along the horizontal axis of the image.
			if(inWhite==false){inWhite=true;
			whiteEndPts[0] = i;
			}
			
		}
		else if(avgColor < (calibratedColorBW[0]+colorTolerance) && avgColor>(calibratedColorBW[0]-colorTolerance))
		{
			if(inWhite==true){inWhite=false; whiteEndPts[1]=i;}
		}
	}
	return whiteEndPts;
}
int getAvgColor(int[] RGB)
{
	int avgColor = 0;
	avgColor = avgColor+RGB[0];
	avgColor = avgColor+RGB[1];
	avgColor = avgColor+RGB[2];
	avgColor = avgColor/3;
	return avgColor;
	
}
int[] calibrateCamera(int[][][] inpImage)
{
	int averageColorW = 0;
	int averageColorB = 0;
	int sumW = 0;
	int sumB = 0;
	image = inpImage;
	for(int i = 0; i<image.length; i++)
	{	
		int avgRGB = 0;
		avgRGB = avgRGB+inpImage[i][1][0];
		avgRGB = avgRGB+inpImage[i][1][1];
		avgRGB = avgRGB+inpImage[i][1][2];
		avgRGB = avgRGB/3;
		averageColorB = averageColorB+avgRGB;
	}
	for(int i = 0; i<image.length; i++)
	{
		int avgRGB = 0;
		avgRGB = avgRGB+inpImage[i][209][0];
		avgRGB = avgRGB+inpImage[i][209][1];
		avgRGB = avgRGB+inpImage[i][209][2];
		avgRGB = avgRGB/3;
		averageColorW = averageColorW+avgRGB;
	}
	averageColorW = averageColorW/240;
	averageColorB = averageColorB/240;
	int[] calibratedBW = {averageColorB, averageColorW};
	return calibratedBW;
}
void driveStraight(int speed, int timeS, int timeMS)
{
	// still need to define speed, timeS, and timeMS
	set_motor(1,speed);
	set_motor(2, speed);
	sleep1(timeS, timeMS);
	
}
void turn(int diff, int timeS, int timeMS)
{
	set_motor(1, 127+(diff/2));
	set_motor(2, 127-(diff/2));
	sleep1(timeS, timeMS);
}
