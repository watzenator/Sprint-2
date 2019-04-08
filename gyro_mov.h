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
		else if(control < 0){
			BP.set_motor_power(PORT_C, +control + MOTORSPEED);
			BP.set_motor_power(PORT_B, -control - MOTORSPEED);
		}else if(control > 0){
			BP.set_motor_power(PORT_C, -control - MOTORSPEED);
			BP.set_motor_power(PORT_B, +control + MOTORSPEED);
		}
		usleep(1);
	}
}

void goright(sensor_gyro_t & Gyro4){
// 	sensor_gyro_t Gyro4;
// 	BP.get_sensor(PORT_4, &Gyro4);
// 	usleep(1);
// 	int gyro_offset = Gyro4.abs;
// 	bool slow_down_started;
//     	BP.set_motor_power(PORT_C, 100);
//     	BP.set_motor_power(PORT_B, -100);
// 	slow_down_started = false;
// 	while(true){
//       		BP.get_sensor(PORT_4, &Gyro4);
// //       		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
//       		if(Gyro4.abs - gyro_offset >= 40 || slow_down_started){
//         		slow_down_started = true;
//         		BP.set_motor_power(PORT_C, 35);
//         		BP.set_motor_power(PORT_B, -35);
//         		if(Gyro4.abs - gyro_offset >= 90){
//           			break;
//         		}
//       		}
//       		usleep(1);
// 	}
	baseline += 90;
	// int controlValue;
	// while(PIDcontrol(Gyro4) != 0){
	// 	//cout << "difftime: " << difftime( time(0), start) << "\n: " << baseline << "\n";
	// 	controlValue = PIDcontrol(Gyro4);
	// 	BP.set_motor_power(PORT_C, +controlValue + MOTORSPEED);
	// 	BP.set_motor_power(PORT_B, -controlValue - MOTORSPEED);
	// 	usleep(1);
	// }
}

void goleft(sensor_gyro_t & Gyro4){
// 	sensor_gyro_t Gyro4;
// 	BP.get_sensor(PORT_4, &Gyro4);
// 	usleep(1);
// 	int gyro_offset = Gyro4.abs;
// 	bool slow_down_started;
//     	BP.set_motor_power(PORT_C, -100);
//     	BP.set_motor_power(PORT_B, 100);
// 	slow_down_started = false;
// 	while(true){
//       		BP.get_sensor(PORT_4, &Gyro4);
// //       		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
//       		if(Gyro4.abs - gyro_offset <= -40 || slow_down_started){
//         		slow_down_started = true;
//         		BP.set_motor_power(PORT_C, -35);
//         		BP.set_motor_power(PORT_B, 35);
//         		if(Gyro4.abs - gyro_offset <= -90){
//           			break;
//         		}
//       		}
//       		usleep(1);
// 	}
	baseline -= 90;

	// while(PIDcontrol(Gyro4) != 0){
	// 	//cout << "difftime: " << difftime( time(0), start) << "\n: " << baseline << "\n";
	// 	controlValue = PIDcontrol(Gyro4);
	// 	usleep(1);
	// }
}

void turnaround(){
	// sensor_gyro_t Gyro4;
	// BP.get_sensor(PORT_4, &Gyro4);
	// usleep(1);
	// int gyro_offset = Gyro4.abs;
	// bool slow_down_started;
  //   	BP.set_motor_power(PORT_C, -100);
  //   	BP.set_motor_power(PORT_B, 100);
	// slow_down_started = false;
	// while(true){
  //     		BP.get_sensor(PORT_4, &Gyro4);
  //     		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
  //     		if(Gyro4.abs - gyro_offset <= -160 || slow_down_started){
  //       		slow_down_started = true;
  //       		BP.set_motor_power(PORT_C, -20);
  //       		BP.set_motor_power(PORT_B, 20);
  //       		if(Gyro4.abs - gyro_offset <= -180){
  //         			break;
  //       		}
  //     		}
  //     		usleep(1);
	// }
	baseline -= 180;
}

#endif
