// #include "base_values.h" //BrickPi3.cpp is included here
// #include "BrickPi3.h"
// #include "signalhandler.h"
// #include "pid.h"
// #include "gyro_mov.h"
// #include "grid.h"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>

#include "BrickPi3.cpp"

#define MAX_MOTORPOWER 100
#define MIN_MOTORPOWER -100
#define MOTORSPEED 25
BrickPi3 BP;
int baseline = 0;


void goright(){
	sensor_gyro_t Gyro4;
	BP.get_sensor(PORT_4, &Gyro4);
	usleep(1);
	int gyro_offset = Gyro4.abs;
	bool slow_down_started;
    	BP.set_motor_power(PORT_C, 100);
    	BP.set_motor_power(PORT_B, -100);
	slow_down_started = false;
	while(true){
      		BP.get_sensor(PORT_4, &Gyro4);
//       		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
      		if(Gyro4.abs - gyro_offset >= 40 || slow_down_started){
        		slow_down_started = true;
        		BP.set_motor_power(PORT_C, 35);
        		BP.set_motor_power(PORT_B, -35);
        		if(Gyro4.abs - gyro_offset >= 90){
          			break;
        		}
      		}
      		usleep(1);
	}
	baseline += 90;
}

void goleft(){
	sensor_gyro_t Gyro4;
	BP.get_sensor(PORT_4, &Gyro4);
	usleep(1);
	int gyro_offset = Gyro4.abs;
	bool slow_down_started;
    	BP.set_motor_power(PORT_C, -100);
    	BP.set_motor_power(PORT_B, 100);
	slow_down_started = false;
	while(true){
      		BP.get_sensor(PORT_4, &Gyro4);
//       		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
      		if(Gyro4.abs - gyro_offset <= -40 || slow_down_started){
        		slow_down_started = true;
        		BP.set_motor_power(PORT_C, -35);
        		BP.set_motor_power(PORT_B, 35);
        		if(Gyro4.abs - gyro_offset <= -90){
          			break;
        		}
      		}
      		usleep(1);
	}
	baseline -= 90;
}

void turnaround(){
	sensor_gyro_t Gyro4;
	BP.get_sensor(PORT_4, &Gyro4);
	usleep(1);
	int gyro_offset = Gyro4.abs;
	bool slow_down_started;
    	BP.set_motor_power(PORT_C, -100);
    	BP.set_motor_power(PORT_B, 100);
	slow_down_started = false;
	while(true){
      		BP.get_sensor(PORT_4, &Gyro4);
      		std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
      		if(Gyro4.abs - gyro_offset <= -160 || slow_down_started){
        		slow_down_started = true;
        		BP.set_motor_power(PORT_C, -20);
        		BP.set_motor_power(PORT_B, 20);
        		if(Gyro4.abs - gyro_offset <= -180){
          			break;
        		}
      		}
      		usleep(1);
	}
	baseline -= 180;
}




int calc_degrees(int abs){
	abs %= 360;
	if(abs < -180){
		abs += 360;
	}else if(abs > 180){
		abs -= 360;
	}
	return abs;
}


struct pid{
	double pBias = 1500, iBias = 2000, dBias = 2000;
	double pGain = 0.5, iGain = 0.02, dGain = 0.02;
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
	int error = 0;
	int degrees = calc_degrees(Gyro4.abs);
	setting = calc_degrees(setting);
	if(degrees > setting && degrees < (setting) + 10){
		error = 20;
	}else if(degrees < setting && degrees > (setting) - 10){
		error = -20;
	}else{ error = degrees - setting;
	}
	if(error > 100 || error < -100){
		error = 100;
		std::cout << "klopt geen kut van die sensor ik ga slapen\n";
		std::cout << "Gyro: " << degrees << "    setting: " << setting << "\n";
		sleep(5);
	}

	//P part
	int pOutput = error * Pid.pGain;
	return pOutput;
}


struct location{
	int x = 0;
	int y = 0;

  location operator+(const location & rhs){
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }

  location operator-(const location & rhs){
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
  }
};



location askLocation(){
	location goLoc;
	std::cout << "Geef de x: ";
	std::cin >> goLoc.x;
	std::cout << "\nGeef de y: ";
	std::cin >> goLoc.y;
	std::cout << "\n";
	return goLoc;
}


void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4){
	pid Pid;
	time_t start;
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	std::cout << "start\n";
	if(differenceX < 0){
		turnaround();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		differenceX *= -1;
		negativeX = true;
	}
	std::cout << "first forward\n";
	start = time(0);
	while(differenceX > difftime( time(0), start)){
		BP.get_sensor(PORT_4, &Gyro4);
		//cout << "difftime: " << difftime( time(0), start) << "\nbaseline: " << baseline <<  "Gyroabs: " << Gyro4.abs << "\n";
		int controlValue = PIDcontrol(Pid, baseline, Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
	std::cout << "first break\n";
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	sleep(0.5);
	if(negativeX == 1){
		turnaround();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
	}

	if(differenceY < 0){
		goright();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
		differenceY *= -1;
		negativeY = true;
	}else{
		goleft();
		std::cout << "yes" << std::endl;
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(0.5);
	}
	std::cout << "second forward\n";
	start = time(0);
	while(differenceY > difftime( time(0), start)){
		BP.get_sensor(PORT_4, &Gyro4);
		//cout << "difftime: " << difftime( time(0), start) << "\n: " << baseline << "\n";
		int controlValue = PIDcontrol(Pid, baseline, Gyro4);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
	std::cout << "second break\n";
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	std::cout << "reset orientation\n";
	sleep(0.5);
	if(negativeY == 1){
		goleft();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}else{
		goright();
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}
}
#endif



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

	if(!voltageIsSafe){
		std::cout << "battery almost empty, exiting program...";
		BP.reset_all();
		exit(-5);
	}

	BP.set_sensor_type(PORT_4, SENSOR_TYPE_EV3_GYRO_ABS);

	sensor_gyro_t gyro4;

	int8_t speedleft = MOTORSPEED;
	int8_t speedright = MOTORSPEED;


	while(BP.get_sensor(PORT_4, &gyro4)){}
	bool firstrun = true;
	location startlocation;
	location endlocation;
	while(firstrun){
		std::cout << "vul startlocatie in\n";
		startlocation = askLocation();
		std::cout << "vul eindlocatie in\n";
		endlocation = askLocation();
		grid(startlocation, endlocation, gyro4);
		startlocation = endlocation;
		firstrun = false;
	}
	while(not firstrun){
		std::cout << "vul eindlocatie in\n";
		endlocation = askLocation();
		grid(startlocation, endlocation, gyro4);
		startlocation = endlocation;
	}

}
