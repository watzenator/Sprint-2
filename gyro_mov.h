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
#include "sounds.h"
#include "grid.h"

void turn(sensor_gyro_t & Gyro4){
	int control;
	while(true){
		usleep(BASE_SLEEP);
		control = PIDcontrol(Gyro4);
		if(control == 0) return;
		BP.set_motor_power(PORT_B, control/* + MOTORSPEED*/);
		BP.set_motor_power(PORT_C, (-1 * control)/* - MOTORSPEED*/);
	}
}

void goright(sensor_gyro_t & Gyro4){
	right_sound();
	baseline += 90;
	turn(Gyro4);
}

void goleft(sensor_gyro_t & Gyro4){
	left_sound();
	baseline -= 90;
	turn(Gyro4);
}

void turnaround(sensor_gyro_t & Gyro4){
	baseline -= 180;
	turn(Gyro4);
}

void brake(){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
}

void goforward_man(double distance, sensor_gyro_t & Gyro4){
	int controlValue;
	int32_t encoder1 = distance * 250 + BP.get_motor_encoder(PORT_C);
	int32_t encoder2 = distance * 250 + BP.get_motor_encoder(PORT_B);

	while(BP.get_motor_encoder(PORT_C) <= encoder1 && BP.get_motor_encoder(PORT_B) <= encoder2){
		BP.get_sensor(PORT_4, &Gyro4);
		controlValue = PIDcontrol(Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
}

void goforward_auto(const int32_t & distance, sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3){
	int controlValue;
	int32_t encoder1 = distance * 250 + BP.get_motor_encoder(PORT_C);
	int32_t encoder2 = distance * 250 + BP.get_motor_encoder(PORT_B);

	while(BP.get_motor_encoder(PORT_C) <= encoder1 && BP.get_motor_encoder(PORT_B) <= encoder2){
		BP.get_sensor(PORT_4, &Gyro4);
		controlValue = PIDcontrol(Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, controlValue + MOTORSPEED);

		BP.get_sensor(PORT_3, &Ultrasonic3);
		if(Ultrasonic3.cm < 10){
			int32_t encoderVerschil1 = encoder1 - BP.get_motor_encoder(PORT_C);
			int32_t encoderVerschil2 = encoder2 - BP.get_motor_encoder(PORT_B);
			object(Gyro4,Ultrasonic3, encoderVerschil1, encoderVerschil2);
		}
		usleep(BASE_SLEEP);
	}
}



#endif
