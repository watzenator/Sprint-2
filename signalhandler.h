#ifndef _SIGNALHANDLER_H
#define _SIGNALHANDLER_H

#include <signal.h>     // for catching exit signals
#include <string>
#include <iostream>
#include "gyro_mov.h"
#include "base_values.h"


// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		std::string input;
		std::cout << "Give me a choice: ";
		getline(std::cin, input);
		if(input == "q"){
			BP.reset_all();    // Reset everything so there are no run-away motors
			exit(-2);
		}else if(input == "d"){
			goright();
		}else if(input == "a"){
			goleft();
		}else if(input == "s"){
			turnaround();
		}
	}
}

bool voltageIsSafe(){
	  // printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
  	// printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
  	// printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
  	// printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());

	if(BP.get_voltage_battery() < 10.9){
		return false;
	}
	return true;
}


#endif
