#ifndef _GYRO_MOV_H
#define _GYRO_MOV_H

// #include "BrickPi3.cpp"

#include "base_values.h"
#include "signalhandler.h"
#include "pid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>

void turn(sensor_gyro_t & Gyro4){
	int control;
	while(true){
		control = PIDcontrol(Gyro4);
		if(control == 0) return;
		BP.set_motor_power(PORT_B, control/* + MOTORSPEED*/);
		BP.set_motor_power(PORT_C, (-1 * control)/* - MOTORSPEED*/);
		usleep(1);
	}
}

void goright(sensor_gyro_t & Gyro4){
	baseline += 90;
	turn(Gyro4);
}

void goleft(sensor_gyro_t & Gyro4){
	baseline -= 90;
	turn(Gyro4);
}

void turnaround(){
	baseline -= 180;
	turn(Gyro4);
}

#endif
