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

	// Reset the encoders
	int32_t EncoderC = BP.offset_motor_encoder(PORT_C, BP.get_motor_encoder(PORT_C));
	int32_t EncoderB = BP.offset_motor_encoder(PORT_B, BP.get_motor_encoder(PORT_B));

	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS_DPS);

	sensor_light_t Light1;
	sensor_ultrasonic_t Ultrasonic2;
	sensor_light_t Light3;
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
// 	while(true){
// 		// Read the encoders
// 		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
// 		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

// 		BP.get_sensor(PORT_1, &Light1);
// 		BP.get_sensor(PORT_2, &Ultrasonic2);
// 		BP.get_sensor(PORT_3, &Light3);
// 		BP.get_sensor(PORT_4, &Gyro4);

// 		int controlValue = PIDcontrol(Pid, baseline, Gyro4);
// 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
// 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
// 		printf("Gyro abs: %4d \n", Gyro4.abs);
// 		usleep(1);
// 		if(Ultrasonic2.cm <= 20){
// 			objects(Ultrasonic2.cm);
// 		}
// 	}
}
