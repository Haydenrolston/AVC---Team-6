#include <stdio.h>
#include "E101.h"
//APOLOGIEs FOR MY SHODDY CODE IN ADVANCE LOL
int height = 120;
int tolerance = 5;
int ERRScale = 0.5;


int** getCameraLine(int height, int*** inpImage)
{
	
	//int image[240][320][3];
	int*** image = inpImage;
	int** linePixels = new int*[320];
	for(int i = 0; i<320; i++)
	{
		linePixels[i] = new int[3];
	}
	for(int i = 0; i<320; i++)
	{
		linePixels[i][0] = image[height][i][0];
		linePixels[i][1] = image[height][i][1];
		linePixels[i][2] = image[height][i][2];
	}
	//printf("TEST\n");
	return linePixels;
}
int*** readCamera()
{
	//Cant remember E101.h api for take picture
	//Assuming I can put the colour values into a 3d array 420/240 res containing 3 values for colour
	int*** image = new int**[240];
	for(int i = 0; i<240; i++)
	{
		image[i] = new int*[320];
		for(int j = 0; j<320; j++)
		{
			image[i][j] = new int[3];
			
		}
	}
	for(int i = 0; i<240; i++)
	{
		for(int j = 0; j<320; j++)
		{
			image[i][j][0] = get_pixel(i,j,0);
			image[i][j][1] = get_pixel(i,j,1);
			image[i][j][2] = get_pixel(i,j,2);
			
		}
	}
	return image;
	
}
int getTurnScalar(int* loc, int scale)
{
	int avgLoc = (int)((loc[0]+loc[1])/2);	
	int turnDiff = avgLoc-160;
	return turnDiff*scale;
}
int getAvgColor(int* RGB)
{
	int avgColor = 0;
	avgColor = avgColor+RGB[0];
	avgColor = avgColor+RGB[1];
	avgColor = avgColor+RGB[2];
	avgColor = avgColor/3;
	return avgColor;
	
}
int* getLinePosition(int colorTolerance, int* calibratedColorBW, int** line)
{
	//Not sure if this way is gonna work but basically:
	//it takes the average of 5 pixel values reading from the left to the right of the image and sees whether
	//the average is within the tolerance of the calibrated white value, if it is then it will record that 
	//column in the line as the start position and wont record more start  until it encounters an average value near the calibrated
	//black value where it will record the white end point, will write a seperate method to generate commands for robot motion based
	//on the position of the white section in the image. I haven't tested this to work yet as I wrote this up on my laptop directly on github
	//if you see errors, please please please correct them, corrections are most welcome and bring us closer to our goal of getting
	//our roguebot to the end of the first quadrant
	int* whiteEndPts = new int[2];
	bool inWhite = false;
	int count = 0;
	for(int i = 0; i<320;i++)
	{
		
		int avgColor = 0;
		for(int j = 0; j<5;j++)
		{
			if(i<319)
			{
			avgColor = avgColor+getAvgColor(line[i]);
			
			i++;
		}
			
		}
		
		avgColor= avgColor/5;
		printf("TEST! %d ", count);
		printf("lum: %d\n", avgColor);
		count++;
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
			printf("St: %d\n",whiteEndPts[0]);
			printf("End: %d\n", whiteEndPts[1]);
			return whiteEndPts;
		}
	}
	return whiteEndPts;
}

int* calibrateCamera(int*** inpImage)
{
	int averageColorW = 0;
	int averageColorB = 0;
	//int sumW = 0;
	//int sumB = 0;
	int*** image = new int**[240];
	for(int i = 0; i<240; i++)
	{
		image[i] = new int*[320];
		for(int j = 0; j<320; j++)
		{
			image[i][j] = new int[3];
		}
		
	
		
	}
	image = inpImage;
	for(int i = 0; i<240; i++)
	{	
		int avgRGB = 0;
		avgRGB = avgRGB+inpImage[i][1][0];
		avgRGB = avgRGB+inpImage[i][1][1];
		avgRGB = avgRGB+inpImage[i][1][2];
		avgRGB = avgRGB/3;
		averageColorB = averageColorB+avgRGB;
	}
	for(int i = 0; i<240; i++)
	{
		int avgRGB = 0;
		avgRGB = avgRGB+inpImage[i][160][0];
		avgRGB = avgRGB+inpImage[i][160][1];
		avgRGB = avgRGB+inpImage[i][160][2];
		avgRGB = avgRGB/3;
		averageColorW = averageColorW+avgRGB;
	}
	averageColorW = averageColorW/240;
	averageColorB = averageColorB/240;
	int* calibratedBW = new int[2];
	calibratedBW[0] = averageColorB;
	calibratedBW[1] = averageColorW;
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
int main()
{
	init();
	//int cameraLine[320][3];
	open_screen_stream();
	take_picture();
	update_screen();
	int* calibrated;
	int* location;
	printf("test\n");
	 calibrated = calibrateCamera(readCamera());//Calibrate method returns average black coolor and the average white color
	printf("%d\n", calibrated[0]);
	printf("%d\n", calibrated[1]);
	while(true)
	{
	take_picture();
	update_screen();
	int turnDif;
	int** cameraLine = new int*[320];
	for(int i = 0; i<320; i++)
	{
		cameraLine[i] = new int[3];
	}
	cameraLine = getCameraLine(height, readCamera());
	//printf("TEST %d\n", cameraLine[53][1]);
	turnDif = getTurnScalar(getLinePosition(tolerance,calibrated, cameraLine), ERRScale);
	
	printf("TURNFDIF: %d\n",turnDif);
	sleep1(1,250);
	}
	
	
	
	
	return 0;
}
