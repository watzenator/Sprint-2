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


void object(){
	sensor_ultrasonic_t Ultrasonic3;
	BP.get_sensor(PORT_3, &Ultrasonic3);
	while(Ultrasonic3.cm < 10){
		// gaat naar links
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

		//gaat naar rechts
		BP.get_sensor(PORT_4, &Gyro4);
		usleep(1);
		gyro_offset = Gyro4.abs;
		slow_down_started;
			BP.set_motor_power(PORT_C, 100);
			BP.set_motor_power(PORT_B, -100);
		slow_down_started = false;
		while(true){
				BP.get_sensor(PORT_4, &Gyro4);
					std::cout << Gyro4.abs%360 << "        " << gyro_offset << '\n';
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
		BP.get_sensor(PORT_3, &Ultrasonic3);
	}

	BP.set_motor_power(PORT_C, 0);
	BP.set_motor_power(PORT_B, 0);
	printf("hetwerktahnifffauw");
	sleep(2);
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
	while(BP.get_sensor(PORT_3, &Ultrasonic3)){}
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
	//BP.get_sensor(PORT_3, &Ultrasonic3);
	usleep(1);
// 	std::cout << "first forward\n"; //Naar voren met PID systeem op basis van encoder afstanden
	while(EncoderC <= encoderX1 && EncoderB <= encoderX2){
		BP.get_sensor(PORT_3, &Ultrasonic3);
		controlValue = PIDcontrol(Gyro4);
		BP.get_sensor(PORT_4, &Gyro4);
		EncoderC = BP.get_motor_encoder(PORT_C);
		EncoderB = BP.get_motor_encoder(PORT_B);
 		BP.set_motor_power(PORT_C, -controlValue + MOTORSPEED);
 		BP.set_motor_power(PORT_B, +controlValue + MOTORSPEED);
		if(Ultrasonic3.cm < 6){
			object();
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
