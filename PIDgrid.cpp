#include "base_values.h" //BrickPi3.cpp is included here
#include "signalhandler.h"
#include "pid.h"
#include "gyro_mov.h"
#include "grid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>

int main(){
	location one;
	location two;
	one.x = 1;
	two.x = 5;
	one.y = 2;
	two.y = 6;

	location three = one + two;
	std::cout << "one " << one.x << ", " << one.y << "\n";
	std::cout << "two " << two.x << ", " << two.y << "\n";
	std::cout << "three " << three.x << ", " << three.y << "\n";  

	// signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 	// BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.
	//
	// if(!voltageIsSafe){
	// 	std::cout << "Battery almost empty, exiting program...";
	// 	BP.reset_all();
	// 	exit(-5);
	// }
	//
	// BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS);
	//
	// sensor_gyro_t Gyro4;
	//
	// int8_t speedLeft = MOTORSPEED;
	// int8_t speedRight = MOTORSPEED;
	//
	//
	// while(BP.get_sensor(PORT_4, &Gyro4)){}
	// while(true){
	// 	std::cout << "Vul startlocatie in\n";
	// 	location startLocation = askLocation();
	// 	std::cout << "Vul eindlocatie in\n";
	// 	location endLocation = askLocation();
	// 	grid(startLocation, endLocation, Gyro4);
	// }
}
