//#include "BrickPi3.cpp" // for BrickPi3
#include "movement.hpp"
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h> // for catching exit signals

//BrickPi3 BP;

//void exit_signal_handler(int signo);


// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
if(signo == SIGINT){
	BP.reset_all();    // Reset everything so there are no run-away motors
	exit(-2);
}
}
// //Checks the Batttery.
// bool voltageIsSafe(){
// 	printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
//   	printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
//   	printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
//   	printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());
//
// 	if(BP.get_voltage_battery() < 10.9){
// 		return false;
// 	}
// 	return true;
// }

int main(){
  signal(SIGINT, exit_signal_handler); // register the exit function for Ctrl+C

  BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

  int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
  int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));


	if(!voltageIsSafe){
		printf("Battery almost empty, exiting program...");
		BP.reset_all();
		exit(-5);
    }

  sensor_gyro_t Gyro4;

  while(true){
    BP.get_sensor(PORT_4, &Gyro4);
    printf("Gyro sensor: degrees per second %4d  \n", Gyro4.abs);

    BP.set_motor_power(PORT_C, 25);
    BP.set_motor_power(PORT_B, 25);
		BP.get_sensor(PORT_4, &Gyro4);

		printf("Gyro sensor: degrees per second %4d  \n", Gyro4.abs);
		sleep(3);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
		BP.set_motor_power(PORT_C, -25);
		BP.set_motor_power(PORT_B, -25);
		BP.get_sensor(PORT_4, &Gyro4);
		
		printf("Gyro sensor: degrees per second %4d  \n", Gyro4.abs);
		sleep(3);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
  }
}
