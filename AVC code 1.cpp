#include <stdio.h>
#include "E101.h"

int WBjudge = 127;// judge white or black

int* getColumnPixel(int column){// get Column white pixels 
	int VLine[240];
	for(int i = 0; i < 240; i++){
		VLine[i] = get_pixel(i, column, 3);
	}
	return VLine;
}

bool ScanVLine(int[] column){
	int startPos = 0;
	int endPos = 240;
	bool inWhite = false;
	for(int i = 0; i < 240; i++){
		if(inWhite == false && column[i] > WBjudge){
			startPos = i;
			inWhite = true;
		}
		else if(inWhite == true && column[i] < WBjudge){
			endPos = i;
			if(endPos - startPos > 20){// if there has a white line that includes over 20 white pixels, then return true
				return true;
			}
			else{inWhite = false;}
		}
	}
	return false;
}

int main(){
	init();
	while(true){
		take_picture();
		int reftColumn[240] = getColumnPixel(0);// left most column
		int rightColumn[240] = getColumnPixel(239);// right most column
		
		bool leftBranch = ScanVLine(LeftColumn);
	    bool rightBranch = ScanVline(rightColumn);
		if(leftBranch){
			//turnLeft, then use PID to run
			
		}
		else if(rightBranch){
			//turnRight, then use PID to run
		}



	}

	return 0;
}