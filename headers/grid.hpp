#pragma once

#include "base_values.hpp"
#include "pid.hpp"
#include "gyro_mov.hpp"
#include <iostream>
#include <time.h>

struct location{
	int x = 0;
	int y = 0;

  location operator+(const location & rhs);
  location operator-(const location & rhs);
};
location askLocation();
void grid(location startLoc, location endLoc,sensor_gyro_t & Gyro4, sensor_ultrasonic_t & Ultrasonic3);