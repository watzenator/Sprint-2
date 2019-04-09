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


void object(sensor_gyro_t & Gyro4, const int32_t& encoderVerschil1, const int32_t& encoderVerschil2){
	sensor_ultrasonic_t Ultrasonic3;
	BP.get_sensor(PORT_2, &Ultrasonic3);
	int32_t count250s = 0;
	while(Ultrasonic3.cm < 10){
		// gaat naar links
		goleft(Gyro4);
		BP.set_motor_power(PORT_C, 0);
		BP.set_motor_power(PORT_B, 0);
		sleep(1);
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
			usleep(1);
		}

		sleep(1);
		
		//gaat naar rechts
		goright(Gyro4);
		count250s ++;
		BP.get_sensor(PORT_2, &Ultrasonic3);
	}

	goleft(Gyro4);
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	sleep(1);
		
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
		usleep(1);
	}
	goright(Gyro4);
	
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	sleep(1);
	printf("ff sleep");
	sleep(1);
	
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
		usleep(1);
	}
	
	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	sleep(1);
	printf("Ver genoeg alleen nog terug naar lijn....");
	sleep(1);	
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
		usleep(1);
	}
	goleft(Gyro4);
}

void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4){
	double differenceX = endLoc.x - startLoc.x;
	double differenceY = endLoc.y - startLoc.y;
	bool negativeX = false;
	bool negativeY = false;
	int32_t EncoderC = BP.get_motor_encoder(PORT_C);
	int32_t EncoderB = BP.get_motor_encoder(PORT_B);
	int controlValue;
	sensor_ultrasonic_t Ultrasonic3;
// 	while(BP.get_sensor(PORT_2, &Ultrasonic3)){}
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
	//BP.get_sensor(PORT_2, &Ultrasonic3);
	usleep(1);
// 	std::cout << "first forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderX1 && EncoderB <= encoderX2){
		BP.get_sensor(PORT_2, &Ultrasonic3);
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		std::cout << Ultrasonic3.cm << "\n";
		if(Ultrasonic3.cm < 6){
			int32_t encoderVerschil1 = encoderX1 - EncoderC;
			int32_t encoderVerschil2 = encoderX2 - EncoderB;
			object(Gyro4, encoderVerschil1, encoderVerschil2);
		}
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
		BP.get_sensor(PORT_2, &Ultrasonic3);
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		if(Ultrasonic3.cm < 6){
			int32_t encoderVerschil1 = encoderX1 - EncoderC;
			int32_t encoderVerschil2 = encoderX2 - EncoderB;
			object(Gyro4, encoderVerschil1, encoderVerschil2);
		}
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
