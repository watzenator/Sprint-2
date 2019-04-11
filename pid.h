#ifndef _PID_H
#define _PID_H

// #include "BrickPi3.cpp"

#include <iostream>
#include "base_values.h"
#include <unistd.h>

int calc_degrees(int abs){
	abs %= 360;
	if(abs < -180){
		abs += 360;
	}else if(abs > 180){
		abs -= 360;
	}
	return abs;
}

int PIDcontrol(sensor_gyro_t & Gyro4){
	BP.get_sensor(PORT_4, &Gyro4);
	//std::cout << Gyro4.abs << '\n';
	int turnspeed = calc_degrees(Gyro4.abs - baseline) * 0.8;
	if(turnspeed > MAX_MOTORPOWER) return MAX_MOTORPOWER;
	if(turnspeed > -30 && turnspeed < 0) return -30;
	if(turnspeed < 30 && turnspeed > 0) return 30;
	return turnspeed;
}

#endif
