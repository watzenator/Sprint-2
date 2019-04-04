#ifndef _GRID_H
#define _GRID_H

#include "base_values.h"
#include "BrickPi3.h"
#include "pid.h"
#include "gyro_mov.h"
#include <iostream>

struct location{
	int x = 0;
	int y = 0;
};

location askLocation(){
	location goLoc;
	std::cout << "Geef de x: ";
	std::cin >> goLoc.x;
	std::cout << "\n";
	std::cout << "Geef de y: ";
	std::cin >> goLoc.y;
	std::cout << "\n";
	return goLoc;
}


void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4){
	pid Pid;
	time_t start;
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	std::cout << "start\n";
	if(differenceX < 0){
		turnaround();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		differenceX *= -1;
		negativeX = true;
	}
	std::cout << "first forward\n";
	start = time(0);
	while(differenceX > difftime( time(0), start)){
		BP.get_sensor(PORT_4, &Gyro4);
		//cout << "difftime: " << difftime( time(0), start) << "\nbaseline: " << baseline <<  "Gyroabs: " << Gyro4.abs << "\n";
		int controlValue = PIDcontrol(Pid, baseline, Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
	cout << "first break\n";
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	sleep(0.5);
	if(negativeX == 1){
		turnaround();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
	}

	if(differenceY < 0){
		goright();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
		differenceY *= -1;
		negativeY = true;
	}else{
		goleft();
		std::cout << "yes" << endl;
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
	}
	std::cout << "second forward\n";
	start = time(0);
	while(differenceY > difftime( time(0), start)){
		BP.get_sensor(PORT_4, &Gyro4);
		//cout << "difftime: " << difftime( time(0), start) << "\n: " << baseline << "\n";
		int controlValue = PIDcontrol(Pid, baseline, Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
	std::cout << "second break\n";
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	std::cout << "reset orientation\n";
	sleep(0.5);
	if(negativeY == 1){
		goleft();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}else{
		goright();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}
}
#endif
