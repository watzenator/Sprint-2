//#include "BrickPi3.cpp" // for BrickPi3
#include "movement.hpp"
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>
#include <iostream>// for catching exit signals
//#include <math>
#include <cstdlib>

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

	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS_DPS);
	if(!voltageIsSafe){
		printf("Battery almost empty, exiting program...");
		BP.reset_all();
		exit(-5);
    }

  sensor_gyro_t Gyro4;

	const int normal_speed = 25;
	int lspeed = normal_speed, rspeed = normal_speed;
	const int speedoffset = 1;
	const int min_speed = 10;
  int gyro_offset = 0;
  bool slow_down_started;

	while(BP.get_sensor(PORT_4, &Gyro4)){}
  while(true){
    BP.set_motor_power(PORT_C, normal_speed);
    BP.set_motor_power(PORT_B, normal_speed);
    sleep(2);
    BP.set_motor_power(PORT_C, 100);
    BP.set_motor_power(PORT_B, -100);
    slow_down_started = false;
    while(true){
      BP.get_sensor(PORT_4, &Gyro4);
      std::cout << Gyro4.abs%360 << '\n';
      if(Gyro4.abs - gyro_offset >= 10 || Gyro4.abs + gyro_offset <= 10 || slow_down_started){
        slow_down_started = true;
        BP.set_motor_power(PORT_C, 20);
        BP.set_motor_power(PORT_B, -20);
        if(Gyro4.abs - gyro_offset >= 90 || Gyro4.abs + gyro_offset <= -90){
          gyro_offset = Gyro4.abs;
          break;
        }
      }
      usleep(1);

    }
    //sleep(1);
  }
}
