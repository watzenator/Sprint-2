#include "base_values.h" //BrickPi3.cpp is included here
// #include "BrickPi3.h"
#include "signalhandler.h"
#include "pid.h"
#include "gyro_mov.h"
#include "grid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>
//#include "sounds.h"

// #include "BrickPi3.cpp"






int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for ctrl+c
	signal(SIGCHLD, SIG_IGN); //needed to prevent zombie processes?
 	BP.detect(); // make sure that the brickpi3 is communicating and that the firmware is compatible with the drivers.

	if(!voltageIsSafe()){
		std::cout << "battery almost empty, exiting program...";
		BP.reset_all();
		exit(-5);
	}

	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS);

	sensor_ultrasonic_t Ultrasonic3;
	sensor_gyro_t Gyro4;

	int8_t speedleft = MOTORSPEED;
	int8_t speedright = MOTORSPEED;

// 	forward_sound();

	while(BP.get_sensor(PORT_4, &Gyro4)){}
	bool firstrun = true;
	location startlocation;
	location endlocation;
	while(firstrun){
		std::cout << "vul startlocatie in\n";
		startlocation = askLocation();
		std::cout << "vul eindlocatie in\n";
		endlocation = askLocation();
		grid(startlocation, endlocation, Gyro4, Ultrasonic3);
		startlocation = endlocation;
		firstrun = false;
	}
	while(not firstrun){
		std::cout << "vul eindlocatie in\n";
		endlocation = askLocation();
		grid(startlocation, endlocation, Gyro4, Ultrasonic3);
		startlocation = endlocation;
	}

}
