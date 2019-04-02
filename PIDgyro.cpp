   
#include "BrickPi3.cpp" // for BrickPi3
#include <stdio.h>      // for printf
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals

#define MAX_MOTORPOWER 100
#define MIN_MOTORPOWER -100
#define MOTORSPEED 25

using namespace std;

BrickPi3 BP;

struct pid{
	double pBias = 1500, iBias = 2000, dBias = 2000;
	double pGain = 0.1, iGain = 0.02, dGain = 0.02;
	double iState = 0;
	double iLimit = 0.25, dLimit = 0.25;
	double iMax = 100, iMin = -100;
	double dMax = 100, dMin = -100;
	double lastError = 0;
};

void PIDconfig(pid & Pid){
	Pid.iMax = Pid.iLimit * MAX_MOTORPOWER / Pid.iGain;
	Pid.iMin = Pid.iLimit * MIN_MOTORPOWER / Pid.iGain;
}

int PIDcontrol(pid & Pid, int setting, sensor_gyro_t & Gyro4){
	//making Error
	int error = setting - Gyro4.abs ;
	
	//P part
	int pOutput = (error * Pid.pGain);
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

void exit_signal_handler(int signo);

void intersection(bool& sensorLeft, bool& sensorRight){
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	printf("Welke kant wil je op: \n");
	int choice = getchar();

	if(choice == '0'){
		BP.set_motor_power(PORT_C, 15);
		BP.set_motor_power(PORT_B, 15);
		sleep(1);
		BP.set_motor_power(PORT_C, -MOTORSPEED-10);
		BP.set_motor_power(PORT_B, MOTORSPEED+10);
		sleep(1);
	}else if(choice == '1'){
		BP.set_motor_power(PORT_C, 15);
		BP.set_motor_power(PORT_B, 15);
		sleep(1);
		BP.set_motor_power(PORT_B, -MOTORSPEED-10);
		BP.set_motor_power(PORT_C, MOTORSPEED+10);
		sleep(1);
	}else if(choice == '2'){
		BP.set_motor_power(PORT_C, MOTORSPEED);
		BP.set_motor_power(PORT_B, MOTORSPEED);
		sleep(1);
	}
}

void objects(int getal){
		BP.set_motor_power(PORT_B, ((getal - 5) * 2));
		BP.set_motor_power(PORT_C, ((getal - 5) * 2));
		if(getal <= 10){
			BP.set_motor_power(PORT_C, -MOTORSPEED-10);
			BP.set_motor_power(PORT_B, MOTORSPEED+10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(4);
			BP.set_motor_power(PORT_C, MOTORSPEED+10);
			BP.set_motor_power(PORT_B, -MOTORSPEED-10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(4);
			BP.set_motor_power(PORT_C, MOTORSPEED+10);
			BP.set_motor_power(PORT_B, -MOTORSPEED-10);
			sleep(1.5);
			BP.set_motor_power(PORT_C, MOTORSPEED);
			BP.set_motor_power(PORT_B, MOTORSPEED);
			sleep(4);
			BP.set_motor_power(PORT_C, -MOTORSPEED-10);
			BP.set_motor_power(PORT_B, MOTORSPEED+10);
			sleep(1.5);
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

	bool sensorLeft = false;
	bool sensorRight = false;
	bool sensorTouch = false;

	if(!voltageIsSafe){
		printf("Battery almost empty, exiting program...");
		BP.reset_all();
		exit(-5);
	}
	
	sleep(2);
	pid Pid;
	PIDconfig(Pid);
	while(true){
		// Read the encoders
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);

		BP.get_sensor(PORT_1, &Light1);
		BP.get_sensor(PORT_2, &Ultrasonic2);
		BP.get_sensor(PORT_3, &Light3);
		BP.get_sensor(PORT_4, &Gyro4);
		int controlValue = PIDcontrol(Pid, 0, Gyro4);
		BP.set_motor_power(PORT_C, controlValue+20);
		BP.set_motor_power(PORT_B, -controlValue+20);

		printf("Encoder C: %6d  B: %6d Left: %6d Right: %6d \n", EncoderC, EncoderB, Light3.reflected, Light1.reflected);
		printf("Ultrasonic sensor (S2): CM %5.1f ", Ultrasonic2.cm);
	}
}

// Signal handler that will be called when Ctrl+C is pressed to stop the program
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();    // Reset everything so there are no run-away motors
		exit(-2);
	}
}
