#include <stdio.h>
#include "E101.h"

int middleHeight = 120;
int WBjudge = 127;
int Error = 0;
int whiteNumber = 0;
int distance = 0;

int* getCameraLine(){
	int line[320];
	for(int i = 0; i < 320; i++){
		line[i] = get_pixel(middleHeight,i,3);
	}
	return line;
}

int* getWhites(int* middleLine){
	int boolLine[320];
	for(int i = 0; i < 320; i++){
		if(middleLine[i]< WBjudge){ boolLine[i] = 0;}
		else{boolLine[i] = 1;}
	}
	return boolLine;
}

int getWhiteNum(int* lineWhite){// modifying form getLoc()
	bool inWhite = false;
	int startPos;
	int endPos;
	int WhiteNum;
	for(int i = 0; i < 320; i++){
		if(lineWhite[i] == 1 && !inWhite){
			startPos = i;
			inWhite = true;
		}
		else if(lineWhite[i] == 0 && inWhite){
			endPos = i;
			if(endPos - startPos  40){
				inWhite = false;
			}
			else{
				WhiteNum = endPos - startPos;
			}
		}
	}
	return WhiteNum;
}

int getError(int*lineWhite){
	int error = 0;
	for(int i = 0; i < 320; i++){
		error = lineWhite[i]*(i-159) + error;
	}
	return error;
}

// suppose pin 1 is left, pin 2 is right, turn 90 degrees(need to test in the lab)
void turnLeft(){      
	set_motor(1,254);//left wheel stop
	set_motor(2,50);
	sleep1(0,500);
	distance = 0;
}

void turnRight(){
	set_motor(1, 50);
	set_motoe(2, 254);// right wheel stop
	sleep1(0,500);
	distance = 0;
}

void turnBack(){// turn back to the crossRoads
	int count = 0;
	while(count < distance){
		set_motor(1, -50);
		set_motoe(2, 50);
		sleep1(0,500);
		count++;
	}
}

void driveStraight(){
	set_motor(1, 50);
	set_motor(2, -50);
        sleep1(0,500);
	distance++; //suppose running with 50 speed in 500ms as one distance unit
}


int main()
{
	init();
	open._screen_stream();
	while(true)
	{
		take_picture();
		update_screen();
		//Q2
		int middleLine[320] = getCameraLine();
		int lineWhite[320] = getWhites(middleLine);
		whiteNumber = getWhiteNum(lineWhite);
		Error = getError(lineWhite);
		if(whiteNumber >= 200 && Error == 0){// The white crossRoads situation
			turnLeft();// each time when the robot go to a crossRoad, turn left first
		}
		else if(whiteNumber == 0 && Error == 0){// blind alley
			turnBack();
			turnRight();// each time after turning back, turn right 
		}
		else if(whiteNumber > 40 && Error <= 20 && Error >= -20){
			driveStraight();
		}

	}


	return 0;
}
