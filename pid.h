#ifndef _PID_H
#define _PID_H

#include <iostream>
#include "base_values.h"
#include <unistd.h>

struct pid{
	double pBias = 1500, iBias = 2000, dBias = 2000;
	double pGain = 0.5, iGain = 0.02, dGain = 0.02;
	double iState = 0;
	double iLimit = 0.25, dLimit = 0.25;
	double iMax = 100, iMin = -100;
	double dMax = 100, dMin = -100;
	double lastError = 0;
};

void PIDconfig(pid & Pid){
	Pid.iMax = Pid.iLimit * MAX_MOTORPOWER / Pid.iGain;
	Pid.iMin = Pid.iLimit * MIN_MOTORPOWER / Pid.iGain;
}

int PIDcontrol(pid & Pid, int setting, sensor_gyro_t & Gyro4){
	int error = 0;
	if(Gyro4.abs %360 > setting %360 && Gyro4.abs %360 < (setting %360) + 10){
		error = 20;
	}else if(Gyro4.abs %360 < setting %360 && Gyro4.abs %360 > (setting %360) - 10){
		error = -20;
	}else{ error = Gyro4.abs %360 - setting %360;
	}
	if(error > 100 || error < -100){
		error = 100;
		std::cout << "klopt geen kut van die sensor ik ga slapen\n";
		std::cout << "Gyro: " << Gyro4.abs << "    setting: " << setting << "\n";
		sleep(5);
	}

	//P part
	int pOutput = error * Pid.pGain;
	return pOutput;
}

#endif
