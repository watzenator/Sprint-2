#ifndef _SIGNALHANDLER_H
#define _SIGNALHANDLER_H

#include <signal.h>     // for catching exit signals
#include <string>
#include <iostream>
#include "gyro_mov.h"
#include "base_values.h"
#include <cstdio>
#include <iostream>



// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}

bool voltageIsSafe(){
	std::cout << BP.get_voltage_battery() << "\n" << BP.get_voltage_9v() << "\n" << BP.get_voltage_5v() << "\n" << BP.get_voltage_3v3() << "\n";

	if(BP.get_voltage_battery() < 10.9){
		return false;
	}
	return true;
}


#endif
