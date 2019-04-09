#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>
using namespace std;

#define MAX_MOTORPOWER 100
#define MIN_MOTORPOWER -100
#define MOTORSPEED 25

int baseline = 0;
void exit_signal_handler(int signo);
BrickPi3 BP;

struct pid{
	double pBias = 1500, iBias = 2000, dBias = 2000;
	double pGain = 0.5, iGain = 0.02, dGain = 0.02;
	double iState = 0;
	double iLimit = 0.25, dLimit = 0.25;
	double iMax = 100, iMin = -100;
	double dMax = 100, dMin = -100;
	double lastError = 0;
};

int calc_degrees(int abs){
	abs %= 360;
	if(abs < -180){
		abs += 360;
	}else if(abs > 180){
		abs -= 360;
	}
	return abs;
}

void PIDconfig(pid & Pid){
	Pid.iMax = Pid.iLimit * MAX_MOTORPOWER / Pid.iGain;
	Pid.iMin = Pid.iLimit * MIN_MOTORPOWER / Pid.iGain;
}
int PIDcontrol(pid & Pid, int setting, sensor_gyro_t & Gyro4){
	int error = 0;
	BP.get_sensor(PORT_4,&Gyro4);
	int degrees = calc_degrees(Gyro4.abs);
	setting = calc_degrees(setting);
	//std::cout << "Gyro: " << degrees << "    setting: " << setting << "\n";

	//std::cout << "degrees :";
	if(degrees > setting && degrees < (setting) + 10){
		error = 20;
	}else if(degrees < setting && degrees > (setting) - 10){
		error = -20;
	}else{ error = degrees - setting;
	}
	if((error > 100 || error < -100)){
		error = 100;
		std::cout << "klopt geen kut van die sensor ik ga slapen\n";
		//std::cout << "Gyro: " << Gyro4.abs << "    setting: " << setting << "\n";
		sleep(5);
	}

	//P part
	int pOutput = error * Pid.pGain;
	return pOutput;
}

bool voltageIsSafe(){
	printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
  	printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
  	printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
  	printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());
	
	if(BP.get_voltage_battery() < 10.9){
		return false;
	}
	return true;
}
void goright(){
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
//        		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
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
}

void goleft(){
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
//        		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
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
}

void turnaround(){
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
//       		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
//       		if(Gyro4.abs - gyro_offset <= -160 || slow_down_started){
//         		slow_down_started = true;
//         		BP.set_motor_power(PORT_C, -20);
//         		BP.set_motor_power(PORT_B, 20);
//         		if(Gyro4.abs - gyro_offset <= -180){
//           			break;
//         		}
//       		}
//       		usleep(1);
// 	}
	baseline -= 180;
}

void objects(int getal){
		BP.set_motor_power(PORT_B, ((getal - 5) * 2));
		BP.set_motor_power(PORT_C, ((getal - 5) * 2));
		if(getal <= 5){
			goleft();
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(2);
			goright();
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(3);
			goright();
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(2);
			goleft();
		}
}


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
	
	sleep(2);	
	pid Pid;
	PIDconfig(Pid);
	BP.get_sensor(PORT_4, &Gyro4);
	sleep(2);
	int32_t afstand = 750;
	while(true){
		// Read the encoders
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);
		BP.get_sensor(PORT_1, &Light1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Gyro4);
		time_t start;
		start = time(0);
		while((EncoderC <= afstand) && (EncoderB <= afstand)){
			int32_t EncoderC = BP.get_motor_encoder(PORT_C);
			int32_t EncoderB = BP.get_motor_encoder(PORT_B);
			int controlValue = PIDcontrol(Pid, baseline, Gyro4);
			BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
			BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
			usleep(1);
			cout << "afstand is: " << afstand << "\n";
			printf("start EncoderC: %4d,  EncoderB: %5d \n", EncoderC, EncoderB);
		}
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		printf("eind EncoderC: %4d,  EncoderB: %5d \n", EncoderC, EncoderB);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(2);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
		printf("start EncoderC: %4d,  EncoderB: %5d \n", EncoderC, EncoderB);
		usleep(1);
		afstand += 250;
		printf("tussenstop\n");
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		string input;
		cout << "Give me a choice: ";
		getline(cin, input);
		if(input == "q"){	
			BP.reset_all();    // Reset everything so there are no run-away motors
			exit(-2);
		}else if(input == "d"){
			goright();
			baseline += 90;
		}else if(input == "a"){
			goleft();
			baseline -= 90;
		}else if(input == "s"){
			turnaround();
			baseline -= 180;
		}
	}
}
