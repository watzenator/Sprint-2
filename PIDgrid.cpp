#include "BrickPi3.h" // for BrickPi3
#include "base_values.h"
#include "signalhandler.h"
#include "pid.h"
#include "gyro_mov.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>
using namespace std;

int main(){
	signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C
 	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	if(!voltageIsSafe){
		printf("Battery almost empty, exiting program...");
		BP.reset_all();
		exit(-5);
	}
	
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS);

	sensor_gyro_t Gyro4;

	int8_t speedLeft = MOTORSPEED;
	int8_t speedRight = MOTORSPEED;

	pid Pid;
	PIDconfig(Pid);
	while(BP.get_sensor(PORT_4, &Gyro4)){}
	while(true){
		cout << "Vul startlocatie in" << "\n";
		location startLocation = askLocation();
		cout << "Vul eindlocatie in" << "\n";
		location endLocation = askLocation();
		grid(startLocation, endLocation, Gyro4);
	}
}
