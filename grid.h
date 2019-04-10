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
	std::cout << "Geef de y: ";
	std::cin >> goLoc.y;
	return goLoc;
}


void object(sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3, const int32_t& encoderVerschil1, const int32_t& encoderVerschil2){
	BP.get_sensor(PORT_2, &Ultrasonic3);
	int32_t count250s = 0;
	while(Ultrasonic3.cm < 10){
		// gaat naar links
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		usleep(BASE_SLEEP);
		// rijd 250
		int32_t EncoderC = BP.get_motor_encoder(PORT_C);
		int32_t EncoderB = BP.get_motor_encoder(PORT_B);
		int32_t encoder1 = EncoderC + 250;
		int32_t encoder2 = EncoderB + 250;

		while(EncoderC <= encoder1 && EncoderB <= encoder2){
			EncoderC = BP.get_motor_encoder(PORT_C);
			EncoderB = BP.get_motor_encoder(PORT_B);
			int controlValue = PIDcontrol(Gyro4);
			BP.get_sensor(PORT_4, &Gyro4);
			BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
			BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
			usleep(BASE_SLEEP);
		}
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);

		usleep(BASE_SLEEP);

		//gaat naar rechts
		goright(Gyro4);
		count250s ++;
		BP.get_sensor(PORT_2, &Ultrasonic3);
	}

	goleft(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);

	int32_t EncoderC = BP.get_motor_encoder(PORT_C);
	int32_t EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoder1 = EncoderC + 250;
	int32_t encoder2 = EncoderB + 250;

	while(EncoderC <= encoder1 && EncoderB <= encoder2){
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
	goright(Gyro4);
	count250s ++;

	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);

	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + encoderVerschil1;
	encoder2 = EncoderB + encoderVerschil2;

	while(EncoderC <= encoder1 && EncoderB <= encoder2){
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}

	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	usleep(BASE_SLEEP);
	count250s *= 250;

	goright(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);

	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	encoder1 = EncoderC + count250s;
	encoder2 = EncoderB + count250s;

	while(EncoderC <= encoder1 && EncoderB <= encoder2){
		int controlValue = PIDcontrol(Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		usleep(BASE_SLEEP);
	}
	goleft(Gyro4);
	usleep(BASE_SLEEP);
}

void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3){
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	int32_t EncoderC = BP.get_motor_encoder(PORT_C);
	int32_t EncoderB = BP.get_motor_encoder(PORT_B);
	int controlValue;
// 	while(BP.get_sensor(PORT_2, &Ultrasonic3)){}
// 	std::cout << "start\n"; //Start draaien op basis van het grid
	if(differenceX < 0){
		turnaround(Gyro4);
		brake();
		differenceX *= -1;
		negativeX = true;
	}
	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoderX1 = differenceX * 250 + EncoderC;
	int32_t encoderX2 = differenceX * 250 + EncoderB;
	//BP.get_sensor(PORT_2, &Ultrasonic3);
	usleep(BASE_SLEEP);
// 	std::cout << "first forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderX1 && EncoderB <= encoderX2){
		BP.get_sensor(PORT_2, &Ultrasonic3);
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		std::cout << Ultrasonic3.cm << '\n';
		if(Ultrasonic3.cm < 10){
			int32_t encoderVerschil1 = encoderX1 - EncoderC;
			int32_t encoderVerschil2 = encoderX2 - EncoderB;
			object(Gyro4,Ultrasonic3, encoderVerschil1, encoderVerschil2);
		}
		usleep(BASE_SLEEP);
	}
// 	std::cout << "first break\n"; //Tweede draaien op basis van grid
	brake();
	usleep(BASE_SLEEP);
	if(negativeX == 1 && differenceY < 0){
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
		differenceY *= -1;
		negativeY = true;
	}
	else if(negativeX == 1) && differenceY > 0){
		goright(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	else if(negativeX == 0 && differenceY < 0){
		goright(Gyro4);
		brake();
		usleep(BASE_SLEEP);
		differenceY *= -1;
		negativeY = true;
	}
	else if(negativeX == 0 && differenceY > 0){
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	else{
		goleft(Gyro4);
		brake();
		usleep(BASE_SLEEP);
	}
	EncoderC = BP.get_motor_encoder(PORT_C);
	EncoderB = BP.get_motor_encoder(PORT_B);
	int32_t encoderY1 = differenceY * 250 + EncoderC;
	int32_t encoderY2 = differenceY * 250 + EncoderB;
// 	std::cout << "second forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderY1 && EncoderB <= encoderY2){
		BP.get_sensor(PORT_2, &Ultrasonic3);
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		if(Ultrasonic3.cm < 6){
			int32_t encoderVerschil1 = encoderY1 - EncoderC;
			int32_t encoderVerschil2 = encoderY2 - EncoderB;
			object(Gyro4,Ultrasonic3, encoderVerschil1, encoderVerschil2);
		}
		usleep(BASE_SLEEP);
	}
	brake();
// 	std::cout << "reset orientation\n"; //Draaien voor oriëntatie
	usleep(BASE_SLEEP);
	if(negativeY == 1){
		goleft(Gyro4);
		brake();
	}else{
		goright(Gyro4);
		brake();
	}
	usleep(500);
}
#endif
