#ifndef _GRID_H
#define _GRID_H

// #include "BrickPi3.cpp"

#include "base_values.h"
#include "pid.h"
#include "gyro_mov.h"
#include <iostream>
#include <time.h>

struct location{
	int x = 0;
	int y = 0;

  location operator+(const location & rhs){
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }

  location operator-(const location & rhs){
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
  }
};

location askLocation(){
	location goLoc;
	std::cout << "Geef de x: ";
	std::cin >> goLoc.x;
	std::cout << "Geef de y: ";
	std::cin >> goLoc.y;
	return goLoc;
}




void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3){
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	if(differenceX < 0){
		turnaround(Gyro4);
		brake();
		differenceX *= -1;
		negativeX = true;
	}
	goforward_auto(differenceX, Gyro4, Ultrasonic3);
	brake();
	usleep(BASE_SLEEP);
	if(negativeX == 1 && differenceY < 0){
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
		differenceY *= -1;
		negativeY = true;
	}
	else if(negativeX == 1 && differenceY > 0){
		goright(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	else if(negativeX == 1 && differenceY == 0){
		goright(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	else if(negativeX == 0 && differenceY < 0){
		goright(Gyro4);
		brake();
		usleep(BASE_SLEEP);
		differenceY *= -1;
		negativeY = true;
	}
	else if(negativeX == 0 && differenceY > 0){
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	else{
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	goforward_auto(differenceY, Gyro4, Ultrasonic3);
	brake();
	usleep(BASE_SLEEP);
	if(negativeY == 1){
		goleft(Gyro4);
		brake();
	}else{
		goright(Gyro4);
		brake();
	}
	usleep(500);
}
#endif
