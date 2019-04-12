#pragma once

#include "base_values.hpp"
#include "signalhandler.hpp"
#include "pid.hpp"
#include <unistd.h>     // for usleep
#include <signal.h>     // for catching exit signals
#include <iostream>
#include <time.h>
#include "sounds.hpp"

void turn(sensor_gyro_t & Gyro4);
void goright(sensor_gyro_t & Gyro4);
void goleft(sensor_gyro_t & Gyro4);
void turnaround(sensor_gyro_t & Gyro4);
void brake();
void object(sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3, const int32_t& encoderVerschil1, const int32_t& encoderVerschil2);