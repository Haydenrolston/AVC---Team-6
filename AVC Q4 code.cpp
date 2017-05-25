#include <stdio.h>
#include "E101.h"
#include <math.h>

int middleRow = 120;
int Rdistance = 150;
int Ldistance = 150;
int ScaleQ3 = 0.2;
int speedQ3 = 120;



bool getReds(){
	int lineR[320];
	int lineG[320];
	int lineB[320];
	int redAvg = 0;
	int greenAvg = 0;
	int blueAvg = 0;
	for(int i = 0; i < 320; i++){
		lineR[i] = get_pixel(middleRow, i, 0);
		lineG[i] = get_pixel(middleRow, i, 1);
		lineB[i] = get_pixel(middleRow, i, 2);
	}
	for(int j = 0; j < 320; j++){
		redAvg = redAvg + lineR[i];
		greenAvg = greenAvg + lineG[i];
		blueAvg = blueAvg + lineB[i];
	}
	redAvg = redAvg/320;
	greenAvg = greenAvg/320;
	blueAvg = blueAvg/320;

	if(redAvg >= 200 && greenAvg <= 50 && blueAvg <= 50){
		return true;
	}
	return false;
}

void driveQ3(int Error, int timeS, int timeMS){
	set_motor(1, (convToMot(speed+Error)*ScaleQ3));
	set_motoe(2, (convToMot(speed-Error)*ScaleQ3));
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


int main(){

	init();
	while(true){
		take_picture();
		if(getReds()){quadrant == 3; stop(2,0);}
		if(quardrant == 3){
			int FSensor;
			int LSensor;
			int RSensor;
		    for(int i = 0; i<5; i++){
			    FSensor = frontalSensor+read_analog(0);
			    RSensor = rightSensor+read_analog(1);
			    LSensor = leftSensor+read_analog(2);
			    sleep1(0,40);
		    }
		    frontalSensor = frontalSensor/5;
		    rightSensor = rightSensor/5;
		    leftSensor = leftSensor/5;
		    printf("Front: %d\n",frontalSensor);
		    printf("Right: %d\n",rightSensor);
		    printf("Left: %d\n", leftSensor);
			int Error = RSensor - LSensor;// Error is nagetive means the robot is closing to the left wall
			UI.println("Error: " + Error);
			}
			if(FSensor >= 250){// there has road in front of the robot
				driveQ3(Error, 0, 500000);
			}
			// I think that is hard for the robto to turn around in the maze, so i choose to use turn back
			else if(FSensor < 250 && Rsensor < 150 && LSensor < 150){// reach to a terminal,
				driveStraight(-80, 2, 0);
				turnRightt(180, 0, 250000);
			}
			else{// reach to turn corner 
				if(Error > 250){//there has road at right
					driveStraight(80,0,500000);
	                turnRight(180,0,250000);
	                driveStraight(-80,0,250000);
				}
				else if(Error < -250){//there has road at left
					driveStraight(80,0,500000);
	                turnLeft(180,0,250000);
	                driveStraight(-80,0,250000);
				}
			}

		}
	}

	return 0;
}
