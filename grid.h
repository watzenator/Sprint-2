#ifndef _GRID_H
#define _GRID_H

// #include "BrickPi3.cpp"

#include "base_values.h"
#include "pid.h"
#include "gyro_mov.h"
#include <iostream>
#include <time.h>

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
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	int32_t EncoderC = BP.get_motor_encoder(PORT_C);
	int32_t EncoderB = BP.get_motor_encoder(PORT_B);
	int controlValue;
// 	std::cout << "start\n"; //Start draaien op basis van het grid
	if(differenceX < 0){
		turnaround(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		differenceX *= -1;
		negativeX = true;
	}
	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoderX1 = differenceX * 250 + EncoderC;
	int32_t encoderX2 = differenceX * 250 + EncoderB;
// 	std::cout << "first forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderX1 && EncoderB <= encoderX2){
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
// 	std::cout << "first break\n"; //Tweede draaien op basis van grid
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(500);
	if(negativeX == 1){
		turnaround(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(500);
	}

	if(differenceY < 0){
		goright(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(500);
		differenceY *= -1;
		negativeY = true;
	}else{
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(500);
	}
	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoderY1 = differenceY * 250 + EncoderC;
	int32_t encoderY2 = differenceY * 250 + EncoderB;
// 	std::cout << "second forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderY1 && EncoderB <= encoderY2){
		controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(1);
	}
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
// 	std::cout << "reset orientation\n"; //Draaien voor oriëntatie
	usleep(500);
	if(negativeY == 1){
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}else{
		goright(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
	}
	usleep(500);
}
#endif
