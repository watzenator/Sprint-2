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

	signal(SIGINT, exit_signal_handler); // register the exit function for ctrl+c
 	BP.detect(); // make sure that the brickpi3 is communicating and that the firmware is compatible with the drivers.

	if(!voltageissafe){
		std::cout << "battery almost empty, exiting program...";
		bp.reset_all();
		exit(-5);
	}

	BP.set_sensor_type(PORT_4, sensor_type_ev3_gyro_abs);

	sensor_gyro_t gyro4;

	int8_t speedleft = MOTORSPEED;
	int8_t speedright = MOTORSPEED;


	while(bp.get_sensor(port_4, &gyro4)){}
	while(true){
		std::cout << "vul startlocatie in\n";
		location startlocation = asklocation();
		std::cout << "vul eindlocatie in\n";
		location endlocation = asklocation();
		grid(startlocation, endlocation, gyro4);
	}
}
